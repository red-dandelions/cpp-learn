/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
// 不改变接口
// 不使用跟内存管理相关的库函数和系统调用
// 只能定义单变量和指针，不能定义数据结构
// 总是要求 8 字节对齐
#include "mm.h"

#include "memlib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "dandelion",
    /* First member's full name */
    "moshouhua",
    /* First member's email address */
    "740139206@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define INT_MAX (20 * (1 << 20))

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define PACK(size, val) ((size) | (val))

#define GET(p) (*(size_t*)(p))
#define PUT(p, val) (*(size_t*)(p) = (val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define DATA_SIZE(p) (GET_SIZE((p)) - 2 * SIZE_T_SIZE)
#define IS_END(p) ((GET(p) & 0x7) == 0x7)
#define IS_BEGIN(p) ((void*)(p) == (void*)(STEP(mem_heap_lo())))

#define STEP(p) (void*)((char*)(p) + SIZE_T_SIZE)

#define HDRP(p) ((void*)((char*)(p)-SIZE_T_SIZE))
#define FTRP(p) ((void*)((char*)(p) + GET_SIZE(HDRP(p)) - 2 * SIZE_T_SIZE))

#define NEXT_BLK(p) ((void*)((char*)(p) + GET_SIZE(HDRP(p))))
#define PREV_BLK(p) ((void*)((char*)(p)-GET_SIZE(HDRP(HDRP(p)))))

static size_t total_free_size = 0;

static size_t final_free_size() {
  if (mem_heapsize() == SIZE_T_SIZE) {
    return 0;
  }
  void* ht = (char*)mem_heap_hi() + 1 - 2 * SIZE_T_SIZE;
  if (GET_ALLOC(ht)) {
    return 0;
  }
  size_t free_size = GET_SIZE(ht);
  return free_size;
}

static void coalesce(void* p) {
  size_t prev_alloc = 1;
  if (!IS_BEGIN(p)) {
    prev_alloc = GET_ALLOC(HDRP(PREV_BLK(p)));
  }
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLK(p)));
  size_t free_size = GET_SIZE(HDRP(p));
  if (prev_alloc && next_alloc) {
    PUT(HDRP(p), PACK(free_size, 0));
    PUT(FTRP(p), PACK(free_size, 0));
  } else if (prev_alloc && !next_alloc) {
    // 下一块空闲
    free_size += GET_SIZE(HDRP(NEXT_BLK(p)));
    PUT(HDRP(p), PACK(free_size, 0));
    PUT(FTRP(p), PACK(free_size, 0));
  } else if (!prev_alloc && next_alloc) {
    // 前一块空闲
    free_size += GET_SIZE(HDRP(PREV_BLK(p)));
    PUT(FTRP(p), PACK(free_size, 0));
    PUT(HDRP(PREV_BLK(p)), PACK(free_size, 0));
  } else {
    //  前后都空闲
    free_size += GET_SIZE(HDRP(PREV_BLK(p))) + GET_SIZE(HDRP(NEXT_BLK(p)));
    PUT(HDRP(PREV_BLK(p)), PACK(free_size, 0));
    PUT(FTRP(NEXT_BLK(p)), PACK(free_size, 0));
  }
}

static void coalesces() {
  void* p = STEP(mem_heap_lo());
  while (!IS_END(HDRP(p))) {
    if (!GET_ALLOC(HDRP(p))) {
      coalesce(p);
    }
    p = NEXT_BLK(p);
  }
}
static void* find_fit_block(size_t new_size) {
  // 首次匹配
  void* p = STEP(mem_heap_lo());
  while (!IS_END(HDRP(p))) {
    if (!GET_ALLOC(HDRP(p)) && GET_SIZE(HDRP(p)) >= new_size) {
      return p;
    }
    p = NEXT_BLK(p);
  }
  return NULL;
}

static void* place(void* p, size_t new_size) {
  size_t free_size = GET_SIZE(HDRP(p));
  void* res = p;
  if ((free_size - new_size) > 2 * SIZE_T_SIZE) {
    PUT(HDRP(p), PACK(new_size, 1));
    PUT(FTRP(p), PACK(new_size, 1));
    p = NEXT_BLK(p);
    PUT(HDRP(p), PACK(free_size - new_size, 0));
    PUT(FTRP(p), PACK(free_size - new_size, 0));
  } else {
    PUT(HDRP(p), PACK(free_size, 1));
    PUT(FTRP(p), PACK(free_size, 1));
  }
  return res;
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  // 用作标记结束块
  void* end = mem_sbrk(SIZE_T_SIZE);
  PUT(end, PACK(0, 0x7));
  return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void* mm_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }
  size_t new_size = ALIGN(size) + 2 * SIZE_T_SIZE;
  void* p = find_fit_block(new_size);
  if (p != NULL) {
    place(p, new_size);
    return p;
  }
  if (total_free_size >= new_size) {
    coalesces();
  }
  p = find_fit_block(new_size);
  if (p != NULL) {
    place(p, new_size);
    return p;
  }

  size_t alloc_size = new_size;
  size_t free_size = 0;  // final_free_size();
  if (final_free_size() != 0) {
    alloc_size = new_size - free_size;
  }
  p = mem_sbrk(alloc_size);
  if (p == NULL) {
    return NULL;
  }
  if (free_size != 0) {
    p = (char*)p - free_size;
  }
  PUT(HDRP(p), PACK(new_size, 1));
  PUT(FTRP(p), PACK(new_size, 1));
  void* end = NEXT_BLK(p);
  PUT(HDRP(end), PACK(0, 0x7));
  return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void* ptr) {
  if (ptr == NULL) return;

  size_t free_size = GET_SIZE(HDRP(ptr));
  total_free_size += free_size;
  PUT(HDRP(ptr), PACK(free_size, 0));
  PUT(FTRP(ptr), PACK(free_size, 0));
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void* mm_realloc(void* p, size_t size) {
  if (size == 0) {
      mm_free(p);
  }
  if (p == NULL) {
      return mm_malloc(size);
  }
  size_t new_size = ALIGN(size) + 2 * SIZE_T_SIZE;
  size_t old_size = GET_SIZE(HDRP(p));
  size_t data_size = DATA_SIZE(HDRP(p));

  if (old_size == new_size) {
      return p;
  }

  if (old_size > new_size) {
      return place(p, new_size);
  }
  void* new_p = find_fit_block(new_size);
  if (new_p != NULL) {
      place(new_p, new_size);
      memcpy(new_p, p, data_size);
      return p;
  }
  if (total_free_size > new_size) {
    coalesces();
  }
  new_p = find_fit_block(new_size);
  if (new_p != NULL) {
      place(new_p, new_size);
      memcpy(new_p, p, data_size);
      return p;
  }

  new_p = mem_sbrk(new_size);
  if (new_p == NULL)
    return NULL;
  PUT(HDRP(new_p), PACK(new_size, 1));
  PUT(FTRP(new_p), PACK(new_size, 1));
  void* end = NEXT_BLK(new_p);
  PUT(HDRP(end), PACK(0, 0x7));

  memcpy(new_p, p, data_size);
  mm_free(p);

  return new_p;
}
