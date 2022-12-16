#include "shm_msg_queue.h"

#include "buddy_memory_allocator_base_object_builder.h"
#include "glog/logging.h"
#include "pointer.h"

#include <sys/mman.h>
// #include <sys/shm.h>
#include <thread>

namespace examples {

namespace iner {
struct ChunkRegionList {
  ShmMsgChunkList::Node* head;
  ShmMsgChunkList::Node* tail;
  ShmMsgChunkList::Node* free_list_head;
  alignas(64) ShmMsgChunkList::Node nodes[];
};

struct ShmMsgChunkList::Data {
  size_t shm_size = 0;
  ChunkRegionList* chunk_list = nullptr;
};

ShmMsgChunkList::ShmMsgChunkList() : data_(std::make_unique<Data>()) {}

ShmMsgChunkList::~ShmMsgChunkList() {
  if (data_->chunk_list) {
    CHECK(munmap(data_->chunk_list, data_->shm_size) == 0)
        << "failed to munmap, err_msg: " << strerror(errno);
  }
}

void ShmMsgChunkList::init(int32_t capacity) {
  const size_t size = sizeof(ChunkRegionList) + sizeof(Node) * capacity;

  data_->shm_size = (size + 4095) & ~size_t(4095);  // 对齐内存
  data_->chunk_list = reinterpret_cast<ChunkRegionList*>(
      mmap(nullptr, data_->shm_size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
  CHECK(data_->chunk_list != MAP_FAILED) << "failed to mmap, err_msg: " << strerror(errno);

  data_->chunk_list->head = nullptr;
  data_->chunk_list->tail = nullptr;

  data_->chunk_list->free_list_head = data_->chunk_list->nodes;
  for (int32_t i = 0; i < capacity - 1; ++i) {
    data_->chunk_list->nodes[i] = {
        .size = 0, .data = nullptr, .next = data_->chunk_list->nodes + i + 1};
  }
  data_->chunk_list->nodes[capacity - 1] = {.size = 0, .data = nullptr, .next = nullptr};
}

ShmMsgChunkList::Node* ShmMsgChunkList::allocate_node() {
  auto list = data_->chunk_list;
  Node* node = nullptr;
  if (list->free_list_head) {
    node = list->free_list_head;
    list->free_list_head = node->next;
  }
  return node;
}

void ShmMsgChunkList::deallocate_node(Node* node) {
  auto list = data_->chunk_list;
  node->next = list->free_list_head;
  list->free_list_head = node;
}

void ShmMsgChunkList::put(Node* node) {
  auto list = data_->chunk_list;

  node->next = nullptr;
  if (list->tail) {
    list->tail->next = node;
  }
  list->tail = node;

  if (!list->head) {
    list->head = node;
  }
}

ShmMsgChunkList::Node* ShmMsgChunkList::pop() {
  auto list = data_->chunk_list;

  if (!list->head) return nullptr;

  Node* node = list->head;
  list->head = node->next;
  if (!node->next) {
    list->tail = nullptr;
  }

  return node;
}

ShmMsgChunkList::Node* ShmMsgChunkList::nodes() { return data_->chunk_list->nodes; }

}  // namespace iner

namespace {
// 伙伴内存池最小的块大小
constexpr size_t kMinBlockSize = 512;
// 计算层数
uint32_t compute_num_order(size_t pool_size, size_t min_block_size) {
  uint32_t num_order = 0;
  while (pool_size >= min_block_size) {
    num_order++;
    pool_size /= 2;
  }
  return num_order;
}
// 对齐内存
size_t compute_aligned_size(size_t size, size_t align) {
  return (size + align - 1) & (~(align - 1));
}
struct MutexLocker {
  explicit MutexLocker(pthread_mutex_t* mutex) : mutex_(mutex) {
    CHECK(pthread_mutex_lock(mutex_) == 0) << " err_msg: " << strerror(errno);
  }
  ~MutexLocker() { CHECK(pthread_mutex_unlock(mutex_) == 0) << " err_msg: " << strerror(errno); }
  pthread_mutex_t* mutex_;
};
}  // namespace

// 控制块结构定义
struct SharedMemoryMessageQueue::MmapedControlBlock {
  // 活跃的生产者数量
  int32_t active_preducer_count{-1};

  // 内存分配器, 基于伙伴算法
  Pointer<BuddyMemoryAllocatorBaseObjectBuilder> memory_allocator;

  // 互斥量
  pthread_mutex_t mutex_;
};

// 在 buffer_ 地址上构造 MmapedControlBlock
std::vector<uint8_t> SharedMemoryMessageQueue::build_control_block(const Options& options,
                                                                   size_t pool_size,
                                                                   void* pool_addr) {
  ObjectBuilder builder;
  int32_t object = builder.allocate<MmapedControlBlock>();
  const uint32_t num_order = compute_num_order(pool_size, kMinBlockSize);
  const uint32_t allocator_offset =
      BuddyMemoryAllocatorBaseObjectBuilder::create(builder, pool_addr, pool_size, num_order);
  builder.update_object_pointer_member(object, &MmapedControlBlock::memory_allocator,
                                       allocator_offset);
  return builder.buffer();
}

SharedMemoryMessageQueue::SharedMemoryMessageQueue(const std::string& name) : name_(name) {}

SharedMemoryMessageQueue::~SharedMemoryMessageQueue() {
  if (buffer_addr_) {
    munmap(buffer_addr_, buffer_mmap_size_);
  }
  if (control_block_) {
    munmap(control_block_, control_block_mmap_size_);
  }
}

bool SharedMemoryMessageQueue::init(const Options* options) {
  SharedMemoryMessageQueue::Options init_options;
  if (options) init_options = *options;

  chunk_list_ = std::make_unique<iner::ShmMsgChunkList>();
  chunk_list_->init(init_options.capacity);

  // 为 buffer 分配内存
  buffer_mmap_size_ = init_options.buffer_size * 1024UL * 1024UL;
  buffer_addr_ =
      mmap(nullptr, buffer_mmap_size_, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (buffer_addr_ == MAP_FAILED) {
    LOG(ERROR) << "mmap failed, err_msg: " << strerror(errno)
               << ", mmap_size: " << buffer_mmap_size_;
    return false;
  }

  // 构造 mmap_data
  const auto control_block_buffer =
      build_control_block(init_options, buffer_mmap_size_, buffer_addr_);
  control_block_mmap_size_ = compute_aligned_size(control_block_buffer.size(),
                                                  4096);  // 虚拟页大小为 4k 字节，按虚拟页对齐
  void* addr = mmap(nullptr, control_block_mmap_size_, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (addr == MAP_FAILED) {
    LOG(ERROR) << "mmap failed, err_msg: " << strerror(errno);
    return false;
  }
  memcpy(addr, control_block_buffer.data(), control_block_buffer.size());
  control_block_ = reinterpret_cast<MmapedControlBlock*>(addr);
  control_block_->active_preducer_count = -1;

  // init mutex
  pthread_mutexattr_t mutex_attr = {};
  CHECK(pthread_mutexattr_init(&mutex_attr) == 0) << " err_msg: " << strerror(errno);
  CHECK(pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED) == 0)
      << " err_msg: " << strerror(errno);
  CHECK(pthread_mutex_init(&control_block_->mutex_, &mutex_attr) == 0)
      << " err_msg: " << strerror(errno);
  CHECK(pthread_mutexattr_destroy(&mutex_attr) == 0) << " err_msg: " << strerror(errno);

  return true;
}

bool SharedMemoryMessageQueue::allocate(size_t size, bool block, uint32_t* out_chunk_id,
                                        void** out_chunk_addr) {
  CHECK_LE(size, buffer_mmap_size_);

  const auto try_allocate = [this, size, out_chunk_id, out_chunk_addr]() {
    MutexLocker lck(&control_block_->mutex_);

    auto node = chunk_list_->allocate_node();
    if (!node) {
      return false;
    }

    auto addr = control_block_->memory_allocator->allocate(size);
    if (addr) {
      node->data = addr;
      node->size = size;

      *out_chunk_id = node - chunk_list_->nodes();
      *out_chunk_addr = addr;
      return true;
    }

    chunk_list_->deallocate_node(node);
    return false;
  };

  while (true) {
    if (try_allocate()) {
      return true;
    }
    if (block) {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
      continue;
    }
    break;
  }
  return false;
}

void SharedMemoryMessageQueue::enqueue(uint32_t chunk_id) {
  MutexLocker lock(&control_block_->mutex_);
  chunk_list_->put(chunk_list_->nodes() + chunk_id);
}

bool SharedMemoryMessageQueue::enqueue(const void* msg, size_t msg_size) {
  uint32_t chunk_id;
  void* chunk_addr = nullptr;
  if (!allocate(msg_size, true, &chunk_id, &chunk_addr)) {
    return false;
  }

  memcpy(chunk_addr, msg, msg_size);

  enqueue(chunk_id);
  return true;
}

SharedMemoryMessageQueue::Status SharedMemoryMessageQueue::dequeue(uint32_t* out_chunk_id,
                                                                   const void** out_addr,
                                                                   size_t* out_chunk_size,
                                                                   int64_t timeout_us) {
  const auto t = std::chrono::steady_clock::now();
  while ((timeout_us == INT64_MAX) ||
         ((std::chrono::steady_clock::now() - t) < std::chrono::microseconds(timeout_us))) {
    MutexLocker lock(&control_block_->mutex_);
    auto node = chunk_list_->pop();
    if (node) {
      *out_chunk_id = node - chunk_list_->nodes();

      *out_addr = node->data;
      *out_chunk_size = node->size;

      return Status::ok;
    }

    if (control_block_->active_preducer_count == 0) {
      return Status::closed;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(500));
  }

  return Status::timeout;
}

void SharedMemoryMessageQueue::release_chunk(uint32_t chunk_id) {
  MutexLocker lock(&control_block_->mutex_);
  auto node = chunk_list_->nodes() + chunk_id;
  control_block_->memory_allocator->deallocate(node->data, node->size);

  chunk_list_->deallocate_node(node);
}

void SharedMemoryMessageQueue::close() { signal_preducing_complete(); }

void SharedMemoryMessageQueue::set_preducer_count(int32_t count) {
  MutexLocker lock(&control_block_->mutex_);
  control_block_->active_preducer_count = count;
}

void SharedMemoryMessageQueue::signal_preducing_complete() {
  MutexLocker lock(&control_block_->mutex_);
  control_block_->active_preducer_count--;
}

}  // namespace examples