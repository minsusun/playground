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
#define MIN_BLOCK_SIZE 4 * WSIZE   // minimum block size
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

static int mm_check();

static void *extend_heap(size_t words);
static void *find_best_fit(size_t size);
static void place(void *bp, size_t size);
static void *coalesce(void *bp);

static void list_insert(void *bp);
static void list_remove(void *bp);

#define DEBUG

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
        printf("MM_CHECK::ERROR: mm_check failed. Heap is currently inconsistent.\n");
#endif
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize = MAX(ALIGN(size + SIZE_T_SIZE), MIN_BLOCK_SIZE);
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
        printf("MM_CHECK::ERROR: mm_check failed. Heap is currently inconsistent.\n");
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
        printf("MM_CHECK::ERROR: mm_check failed. Heap is currently inconsistent.\n");
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
    
    size = MAX(ALIGN(size) + DSIZE, MIN_BLOCK_SIZE);
    size_t current_size = GET_SIZE(HDRP(ptr));
    void* bp;
    if (size == current_size)
        return ptr;
    else if (size < current_size)
    {
        if (size > MIN_BLOCK_SIZE && current_size - size > MIN_BLOCK_SIZE)
        {
            PUT(HDRP(ptr), PACK(size, 1));
            PUT(FTRP(ptr), PACK(size, 1));
            bp = NEXT_BLKP(ptr);
            PUT(HDRP(bp), PACK(size - current_size, 1));
            PUT(FTRP(bp), PACK(size - current_size, 1));
            mm_free(bp);
            return ptr;
        }
        else
            return ptr;
    }
    else
    {
        void *next_bp = HDRP(NEXT_BLKP(ptr));
        size_t newsize = current_size + GET_SIZE(next_bp);
        if (!GET_ALLOC(next_bp) && GET_SIZE(next_bp) + current_size >= size)
        {
            list_remove(NEXT_BLKP(ptr));
            PUT(HDRP(ptr), PACK(size, 1));
            PUT(FTRP(ptr), PACK(size, 1));
            bp = NEXT_BLKP(ptr);
            PUT(HDRP(bp), PACK(newsize - size, 1));
            PUT(FTRP(bp), PACK(newsize - size, 1));
            mm_free(bp);
            return ptr;
        }
        else
        {
            bp = mm_malloc(size);
            if (bp == NULL)
                return NULL;
            memcpy(bp, ptr, current_size);
            mm_free(ptr);
            return bp;
        }
    }
}

static int mm_check() {
    void *cursor;
    // Is every block in the free list marked as free?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        if (GET_ALLOC(HDRP(cursor))) {
            printf("MM_CHECK::ERROR: block %p in free list but marked allocated!\n", cursor);
            return;
        }
    }
    // Are there any contiguous free blocks that somehow escaped coalescing?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        // Check the header and footer of each free block whether it is contiguous with its previous block
        char *prev = PREV_FREE_BLOCK(HDRP(cursor));
        if (prev != NULL && HDRP(cursor) - FTRP(prev) == DSIZE) {
            printf("MM_CHECK::ERROR: block %p missed coalescing!\n", cursor);
            return;
        }
    }
    // Is every free block actually in the free list?
    for (cursor = heap; GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        // Check the header and footer of each free block whether it is in the free list
        if (GET_ALLOC(HDRP(cursor)) == 0) {
            char *bp;
            for (bp = free_list; bp != NULL && GET_ALLOC(HDRP(bp)) == 0; bp = NEXT_FREE_BLOCK(bp)) {
                if (bp == cursor)
                    break;
            }
            // if bp is NULL, free block is not in the free list
            if (bp == NULL) {
                printf("MM_CHECK::ERROR: free block %p not in free list!\n", cursor);
                return;
            }
        }
    }
    // Do the pointers in the free list point to valid free blocks?
    for (cursor = free_list; cursor != NULL && GET_ALLOC(HDRP(cursor)) == 0; cursor = NEXT_FREE_BLOCK(cursor)) {
        // Check the header and footer of each free block whether it is valid
        if (cursor < mem_heap_lo() || cursor > mem_heap_hi()) {
            // if address of free block is smaller than mem_heap_lo or bigger than mem_heap_hi, it is invalid
            printf("MM_CHECK::ERROR: free block %p invalid", cursor);
            return;
        }
    }
    // Do any allocated blocks overlap?
    for (cursor = NEXT_BLKP(heap); GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        // Check the header and footer of each allocated block whether it is overlapped with its previous block
        if (GET_ALLOC(HDRP(cursor))) {
            // if prev block is allocated and the size difference is smaller than WSIZE(4 bytes), it is overlapped
            // char *prev = PREV_FREE_BLOCK(HDRP(cursor));
            char *prev = PREV_BLKP(cursor);
            if (prev != NULL && GET_ALLOC(HDRP(prev)) && HDRP(cursor) - FTRP(prev) < WSIZE) {
                printf("MM_CHECK::ERROR: allocated block %p overlaps with allocated block %p!\n", cursor, prev);
                return;
            }
        }
    }
    // Do the pointers in a heap block point to valid heap addresses?
    for (cursor = heap; GET_SIZE(HDRP(cursor)) > 0; cursor = NEXT_BLKP(cursor)) {
        if (cursor < mem_heap_lo() || cursor > mem_heap_hi()) {
            printf("MM_CHECK::ERROR: block %p outside designated heap space!\n", cursor);
            return;
        }
    }
}

static void* extend_heap(size_t words)
{
    void *bp;
    size_t size = ((words % 2) ? words + 1 : words) * WSIZE;
    if (size < MIN_BLOCK_SIZE)
        size = MIN_BLOCK_SIZE;
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
        if (current == NULL)
            break;
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
    size_t block_size = GET_SIZE(HDRP(bp));
    size_t left_size = block_size - size;
    if (left_size < MIN_BLOCK_SIZE)
    {
        PUT(HDRP(bp), PACK(block_size, 1));
        PUT(FTRP(bp), PACK(block_size, 1));
        list_remove(bp);
    }
    else
    {
        PUT(HDRP(bp), PACK(size, 1));
        PUT(FTRP(bp), PACK(size, 1));
        list_remove(bp);
        void *next_bp = NEXT_BLKP(bp);
        PUT(HDRP(next_bp), PACK(left_size, 0));
        PUT(FTRP(next_bp), PACK(left_size, 0));
        coalesce(next_bp);
    }
}

static void *coalesce(void *bp)
{
    // Determine the current allocation state of the previous and next blocks
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));

    // Get the size of the current free block
    size_t size = GET_SIZE(HDRP(bp));
    // if next block is free, coalese with next block
    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        // pop next block from free list
        list_remove(NEXT_BLKP(bp));
        // modify current block's header and footer
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    // If the previous block is free, coalesce with the previous block
    else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        // list_remove previous block from free list
        bp = PREV_BLKP(bp);
        list_remove(bp);
        // modify previous block's header and footer
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    // If both the previous and next blocks are free, coalesce with both
    else if (!prev_alloc && !next_alloc) { // Case 4 (in text)
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        // list_remove previous and next block from free list
        list_remove(PREV_BLKP(bp));
        list_remove(NEXT_BLKP(bp));
        // modify previous block's header and footer
        bp = PREV_BLKP(bp);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    // push coalesced block to free list
    list_insert(bp);

#ifdef DEBUG
    if(mm_check() == 0)
        printf("MM_CHECK::ERROR: mm_check failed. Heap is currently inconsistent.\n");
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