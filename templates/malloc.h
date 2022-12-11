#ifndef __TEMPLATES_MALLOC_H__
#define __TEMPLATES_MALLOC_H__

#include <cinttypes>
#include <cstdlib>
#include <errno.h>

namespace templates {

/**
 * TODO: fix bug
 * [mem_heap, mem_brk] 为已分配内存
 * mem_brk 之后为未分配内存
 * mem_sbrk 分配堆内存，不能收缩
 */
const int64_t MAX_HEAP = 1 << 24;  // 16 M 内存
static uint8_t* mem_heap;          // 指向
static uint8_t* mem_brk;           // mem_brk 之后的都是未分配的
static uint8_t* mem_max_addr;      // 堆最大值地址
static uint8_t* heap_listp;        // 指向隐式空闲链表的恒定形式的头部

// 申请堆内存进行初始化，堆大小为 MAX_HEAP
void mem_init() {
  mem_heap = reinterpret_cast<uint8_t*>(
      malloc(MAX_HEAP));  // 可以用系统调用 sbrk，c 库里的 malloc 也是对 sbrk 进行封装
  mem_brk = mem_heap;
  mem_max_addr = mem_heap + MAX_HEAP;
}

// 将 mem_brk 增加 incr 个字节，不够则返回 void*(-1)
void* mem_sbrk(size_t incr) {
  uint8_t* old_brk = mem_brk;

  if (incr < 0 || (mem_brk + incr) > mem_max_addr) {
    return (void*)-1;
  }
  mem_brk += incr;
  return reinterpret_cast<void*>(old_brk);
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define WSIZE 4  // 1 字 4 字节
#define DSIZE 8  // 双字 8 字节
#define CHUNKSIZE 1 << 12
#define PACK(size, alloc) ((size) | (alloc))                   // 将块大小和标志合并
#define GET(p) (*reinterpret_cast<uint64_t*>(bp))               // 获取 p 的值
#define PUT(p, val) (*reinterpret_cast<uint32_t*>(p) = (val))  // 存 val 到头部
#define GET_SIZE(p) (GET(p) & ~0x7)                            // 获取头部存储的块大小
#define GET_ALLOC(p) (GET(p) & 0x1)                            // 获取是否分配标志
#define HDRP(bp) (reinterpret_cast<uint8_t*>(bp) - WSIZE)      // 头部
#define FTRP(bp) (reinterpret_cast<uint8_t*>(bp) + GET_SIZE(HDRP(bp)) - DSIZE)  // 脚部
#define NEXT_BLKP(bp) \
  (reinterpret_cast<uint8_t*>(bp) + GET_SIZE(reinterpret_cast<uint8_t*>(bp) - WSIZE))  // 后一个块
#define PREV_BLKP(bp) \
  (reinterpret_cast<uint8_t*>(bp) - GET_SIZE(reinterpret_cast<uint8_t*>(bp) - DSIZE))  // 前一个块

static void* find_fit(size_t asize) {
  uint8_t* bp;
  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
      // 未分配且空间足够
      return bp;
    }
  }
  return nullptr;
}

static void place(void* bp, size_t asize) {
  size_t csize = GET_SIZE(HDRP(bp));

  if ((csize - asize) >= 2 * DSIZE) {
    // 如果分配 asize 后剩下的内存超过两个字（这里要考虑头部和脚部的开销，所以是 2 * DSIZE），则分割
    PUT(HDRP(bp), PACK(asize, 1));          // 设置头部
    PUT(FTRP(bp), PACK(asize, 1));          // 设置脚部
    bp = NEXT_BLKP(bp);                     // 移动 bp 到下一个块
    PUT(HDRP(bp), PACK(csize - asize, 0));  // 设置空闲块头部
    PUT(FTRP(bp), PACK(csize - asize, 0));  // 设置空闲块脚部
  } else {
    // 不用分割
    PUT(HDRP(bp), PACK(csize, 1));  // 设置头部
    PUT(FTRP(bp), PACK(csize, 1));  // 设置脚部
  }
}

// 合并空闲块
static void* coalesce(void* bp) {
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  // 前后都不是空闲块
  if (prev_alloc && next_alloc) {  // 前后都不是空闲块
    return bp;
  } else if (prev_alloc && !next_alloc) {     // 只有后面的块是空闲块
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));    // 重新计算 size
    PUT(HDRP(bp), PACK(size, 0));             // 设置头部
    PUT(FTRP(bp), PACK(size, 0));             // 设置脚部
  } else if (!prev_alloc && next_alloc) {     // 只有前面的是空闲块
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));    // 重新计算 size
    PUT(FTRP(bp), PACK(size, 0));             // 设置头部
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));  // 设置脚部
    bp = PREV_BLKP(bp);                       // 移动指针
  } else {                                    // 前后都是空闲块
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));  // 重新计算 size
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));                                // 设置头部
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));                                // 设置脚部
    bp = PREV_BLKP(bp);                                                     // 移动指针
  }

  return bp;
}

// 拓展隐式空闲链表
static void* extend_heap(size_t words) {
  uint8_t* bp;
  size_t size;
  size = (words & 1) ? (words + 1) * WSIZE : words * WSIZE;  // 按两个字，八字节对齐

  if ((long)(bp = reinterpret_cast<uint8_t*>(mem_sbrk(size))) == -1) {
    // 申请内存失败，返回 nullptr
    return nullptr;
  }

  PUT(HDRP(bp), PACK(size, 0));          // 空闲块头部，在结尾块写入
  PUT(FTRP(bp), PACK(size, 0));          // 空闲块脚部
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));  // 重新标记结尾块

  return coalesce(reinterpret_cast<void*>(bp));
}

// 成功返回 0，否则 -1
int mm_init() {
  if ((heap_listp = reinterpret_cast<uint8_t*>(mem_sbrk(4 * WSIZE))) ==
      reinterpret_cast<uint8_t*>(-1)) {
    // 申请了 16 个字节，4 个字。如果内存不够，初始化失败，返回 -1
    return -1;
  }

  PUT(heap_listp, 0);                             // 第一个字使用 0 做为填充
  PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  // 第二个字为序言块头部， 8 | 1，八字节，已分配
  PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  // 第三个字为序言块脚部， 8 | 1，八字节，已分配
  PUT(heap_listp + (3 * WSIZE), PACK(0, 1));  // 第四个字为结尾块,     0 | 1，0字节，已分配

  heap_listp += 2 * WSIZE;  // 总是指向序言块

  if (extend_heap(CHUNKSIZE / WSIZE) == nullptr) {
    return -1;
  }
  return 0;
}

// 释放 bp 指向的内存
void mm_free(void* bp) {
  size_t size = GET_SIZE(HDRP(bp));  // 从头部获得 size

  PUT(HDRP(bp), PACK(size, 0));  // 在头部标记为未分配
  PUT(FTRP(bp), PACK(size, 0));  // 在脚部标记为未分配

  coalesce(bp);
}

// 申请内存
void* mm_malloc(size_t size) {
  size_t asize;
  size_t extend_size;
  uint8_t* bp;

  if (size == 0) {
    return nullptr;
  }

  if (size <= DSIZE) {
    asize = 2 * DSIZE;  // 对齐要求， 头部和脚部占了两个字，申请的内存占两个字
  } else {
    asize = DSIZE * ((size + (DSIZE) + (DSIZE - 1)) /
                     DSIZE);  // 对齐要求，将申请的内存按两个字对齐，加上头部和脚部占的两个字
  }

  if ((bp = reinterpret_cast<uint8_t*>(find_fit(asize))) != nullptr) {
    // 找到合适的块
    place(bp, asize);
    return reinterpret_cast<void*>(bp);
  }

  // 内存不够，申请更大的
  extend_size = MAX(asize, CHUNKSIZE);
  if ((bp = reinterpret_cast<uint8_t*>(extend_heap(extend_size / WSIZE))) == nullptr) {
    // 申请失败
    return nullptr;
  }

  place(bp, asize);
  return reinterpret_cast<void*>(bp);
}

}  // namespace templates

#endif