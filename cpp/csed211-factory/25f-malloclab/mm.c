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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Imported from CSAPP:3E */
/* Basic constants and macros */
#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(size_t *)(p))
#define PUT(p, val) (*(size_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PREV_ALLOC(p) ((GET(p) >> 1) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((void *)(bp) - WSIZE)
#define FTRP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((void *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((void *)(bp) - GET_SIZE(HDRP(bp) - WSIZE))

#define NEXT_FREE_BLOCK(bp) (*(void**)bp)
#define PREV_FREE_BLOCK(bp) (*(void**)(bp + WSIZE))

static void *heap = NULL;
static void *free_list = NULL;

static int mm_check(void);

static void *extend_heap(size_t words);
static void *find_best_fit(size_t size);
static void place(void *bp, size_t size);
static void *coalesce(void *bp);

static void list_insert(void *bp);
static void list_remove(void *bp);

// #define DEBUG

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap = mem_sbrk(8 * WSIZE)) == (void*)-1)
        return -1;
    
    PUT(heap + (0 * WSIZE), PACK(0, 1));
    PUT(heap + (1 * WSIZE), PACK(6 * WSIZE, 0));
    PUT(heap + (2 * WSIZE), PACK(0, 0));
    PUT(heap + (3 * WSIZE), PACK(0, 0));
    PUT(heap + (4 * WSIZE), PACK(0, 0));
    PUT(heap + (5 * WSIZE), PACK(0, 0));
    PUT(heap + (6 * WSIZE), PACK(6 * WSIZE, 0));
    PUT(heap + (7 * WSIZE), PACK(0, 1));

    free_list = heap + WSIZE;
    heap += (2 * WSIZE);
#ifdef DEBUG
    printf("MM_INIT::INFO: heap initialized at %p\n", heap);
    if(mm_check() == 0)
        printf("MM_CHECK::ERROR: mm_check failed in mm_init. Heap is currently inconsistent.\n");
#endif
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize = MAX(ALIGN(size + 2 * WSIZE), 4 * WSIZE);
    void *bp = find_best_fit(asize);
    if (bp == NULL)
    {
        size_t extended_size = MAX(asize, CHUNKSIZE);
        if (extend_heap(extended_size / WSIZE) == NULL)
            return NULL;
        bp = find_best_fit(asize);
    }
    place(bp, asize);
#ifdef DEBUG
    if(mm_check() == 0)
        printf("MM_CHECK::ERROR: mm_check failed in mm_malloc. Heap is currently inconsistent.\n");
#endif
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if (ptr == NULL)
        return;
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
#ifdef DEBUG
    if(mm_check() == 0)
        printf("MM_CHECK::ERROR: mm_check failed in mm_free. Heap is currently inconsistent.\n");
#endif
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return mm_malloc(size);
    if (size == 0)
    {
        mm_free(ptr);
        return NULL;
    }
    
    void *bp, *next_bp;
    size_t current_size = GET_SIZE(HDRP(ptr)), requested_size = MAX(ALIGN(size) + DSIZE, 4 * WSIZE);
    if (requested_size >= current_size)
    {
        next_bp = HDRP(NEXT_BLKP(ptr));
        size_t new_size = current_size + GET_SIZE(next_bp);
        if (!GET_ALLOC(next_bp) && new_size >= requested_size)
        {
            list_remove(NEXT_BLKP(ptr));
            PUT(HDRP(ptr), PACK(requested_size, 1));
            PUT(FTRP(ptr), PACK(requested_size, 1));
            bp = NEXT_BLKP(ptr);
            PUT(HDRP(bp), PACK(new_size - requested_size, 1));
            PUT(FTRP(bp), PACK(new_size - requested_size, 1));
            mm_free(bp);
            return ptr;
        }
        else
        {
            bp = mm_malloc(requested_size);
            if (bp == NULL)
                return NULL;
            memcpy(bp, ptr, current_size);
            mm_free(ptr);
            return bp;
        }
    }
    else
    {
        if (size > 4 * WSIZE && current_size - requested_size > 4 * WSIZE)
        {
            PUT(HDRP(ptr), PACK(requested_size, 1));
            PUT(FTRP(ptr), PACK(requested_size, 1));
            bp = NEXT_BLKP(ptr);
            PUT(HDRP(bp), PACK(current_size - requested_size, 1));
            PUT(FTRP(bp), PACK(current_size - requested_size, 1));
            mm_free(bp);
        }
        return ptr;
    }
}

static int mm_check(void) {
    void *cursor;
    // Is every block in the free list marked as free?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        if (GET_ALLOC(HDRP(cursor))) {
            printf("MM_CHECK::ERROR: block %p in free list but marked allocated!\n", cursor);
            return 0;
        }
    }
    // Are there any contiguous free blocks that somehow escaped coalescing?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        // Check the header and footer of each free block whether it is contiguous with its previous block
        char *prev = PREV_FREE_BLOCK(HDRP(cursor));
        if (prev != NULL && HDRP(cursor) - FTRP(prev) == DSIZE) {
            printf("MM_CHECK::ERROR: block %p missed coalescing!\n", cursor);
            return 0;
        }
    }
    // Is every free block actually in the free list?
    for (cursor = heap; GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        if (GET_ALLOC(HDRP(cursor)) == 0) {
            void *bp;
            for (bp = free_list; bp != NULL && GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREE_BLOCK(bp)) {
                if (bp == cursor)
                    break;
            }
            if (bp == NULL) {
                printf("MM_CHECK::ERROR: free block %p not in free list!\n", cursor);
                return 0;
            }
        }
    }
    // Do the pointers in the free list point to valid free blocks?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        if (cursor < mem_heap_lo() || cursor > mem_heap_hi()) {
            printf("MM_CHECK::ERROR: free block %p invalid", cursor);
            return 0;
        }
    }
    // Do any allocated blocks overlap?
    for (cursor = NEXT_BLKP(heap); GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        if (GET_ALLOC(HDRP(cursor))) {
            void *prev = PREV_BLKP(cursor);
            if (prev != NULL && GET_ALLOC(HDRP(prev)) && HDRP(cursor) - FTRP(prev) < WSIZE) {
                printf("MM_CHECK::ERROR: allocated block %p overlaps with allocated block %p!\n", cursor, prev);
                return 0;
            }
        }
    }
    // Do the pointers in a heap block point to valid heap addresses?
    for (cursor = heap; GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        if (cursor < mem_heap_lo() || cursor > mem_heap_hi()) {
            printf("MM_CHECK::ERROR: block %p outside designated heap space!\n", cursor);
            return 0;
        }
    }

    return 1;
}

static void* extend_heap(size_t words)
{
    void *bp;
    size_t size = ((words % 2) ? words + 1 : words) * WSIZE;
    if((bp = mem_sbrk(size)) == (void*)-1)
        return NULL;

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

static void *find_best_fit(size_t size)
{
    void *current, *candidate;
    if (free_list == NULL)
        return NULL;
    for (current = candidate = free_list; current != NULL && GET_ALLOC(HDRP(current)) == 0; current = NEXT_FREE_BLOCK(current))
    {
        if (GET_SIZE(HDRP(current)) < size)
            continue;
        if (GET_SIZE(HDRP(candidate)) < size || GET_SIZE(HDRP(current)) < GET_SIZE(HDRP(candidate)))
            candidate = current;
    }
    if (GET_SIZE(HDRP(candidate)) < size)
        return NULL;
    else
        return candidate;
}

static void place(void *bp, size_t size)
{
    size_t dst_block_size = GET_SIZE(HDRP(bp)), size_diff = dst_block_size - size;
    if (size_diff < 4 * WSIZE)
    {
        PUT(HDRP(bp), PACK(dst_block_size, 1));
        PUT(FTRP(bp), PACK(dst_block_size, 1));
        list_remove(bp);
    }
    else
    {
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
        list_remove(bp);
        void *next_bp = NEXT_BLKP(bp);
        PUT(HDRP(next_bp), PACK(size_diff, 0));
        PUT(FTRP(next_bp), PACK(size_diff, 0));
        coalesce(next_bp);
    }
}

static void *coalesce(void *bp)
{
    void *prev_bp = PREV_BLKP(bp), *next_bp = NEXT_BLKP(bp);
    size_t prev_alloc = GET_ALLOC(FTRP(prev_bp)) || prev_bp == bp,
        next_alloc = GET_ALLOC(HDRP(next_bp)),
        new_size = GET_SIZE(HDRP(bp));

    if (!prev_alloc && !next_alloc) {
        new_size += GET_SIZE(HDRP(prev_bp)) + GET_SIZE(HDRP(next_bp));
        list_remove(prev_bp);
        list_remove(next_bp);
        bp = prev_bp;
    }
    else if (!prev_alloc && next_alloc) {
        new_size += GET_SIZE(HDRP(prev_bp));
        list_remove(prev_bp);
        bp = prev_bp;
    }
    else if (prev_alloc && !next_alloc) {
        new_size += GET_SIZE(HDRP(next_bp));
        list_remove(next_bp);
    }

    PUT(HDRP(bp), PACK(new_size, 0));
    PUT(FTRP(bp), PACK(new_size, 0));

    list_insert(bp);

#ifdef DEBUG
    if(mm_check() == 0)
        printf("MM_CHECK::ERROR: mm_check failed in coalesce. Heap is currently inconsistent.\n");
#endif
    return bp;
}

static void list_insert(void *bp)
{
    if (free_list == NULL)
    {
        NEXT_FREE_BLOCK(bp) = NULL;
        PREV_FREE_BLOCK(bp) = NULL;
        free_list = bp;
        return;
    }
    else {
        NEXT_FREE_BLOCK(bp) = free_list;
        PREV_FREE_BLOCK(bp) = NULL;
        PREV_FREE_BLOCK(free_list) = bp;
        free_list = bp;
    }
}

static void list_remove(void *bp)
{
    if (bp == NULL)
        return;
    if (PREV_FREE_BLOCK(bp) == NULL)
        free_list = NEXT_FREE_BLOCK(bp);
    else
        NEXT_FREE_BLOCK(PREV_FREE_BLOCK(bp)) = NEXT_FREE_BLOCK(bp);
    if (NEXT_FREE_BLOCK(bp) != NULL)
        PREV_FREE_BLOCK(NEXT_FREE_BLOCK(bp)) = PREV_FREE_BLOCK(bp);
}