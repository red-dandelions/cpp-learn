/**
 *#include <sys/mman.h>
 * void* mmap(void* start, size_t length, int prot, int flags, int fd, off_t offset);
 * int munmap(void* start, size_t length);
 */
#include <sys/mman.h>

int main() {
  size_t size = 1 << 10;
  void* addr = mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  munmap(addr, size);

  return 0;
}