#include "../csapp.c"

/* Basic constants and macros */
#define WSIZE       4           /* Word and header/footer size (bytes) */
#define DSIZE       8           /* Double word size (bytes) */
#define CHUNKSIZE   (1<<12)     /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(unsigned int *)(p))
#define PUT(p, val)     (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~0x7)
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)   ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)   ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))



static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);



#define MAX_HEAP (20*(1<<20))  /* 20 MB */

static char *mem_heap;
static char *mem_brk;
static char *mem_max_addr;

static void mem_init()
{
    mem_heap = (char *)Malloc(MAX_HEAP);
    mem_brk = (char *)mem_heap;
    mem_max_addr = (char *)(mem_heap + MAX_HEAP);
}

static void *mem_sbrk(int incr)
{
    char *old_brk = mem_brk;

    if ((incr < 0) || ((mem_brk + incr) > mem_max_addr)) {
        errno = ENOMEM;
        fprintf(stderr, "ERROR: mem_sbrk failed. Ran out of memory...\n");
        return (void *)-1;
    }

    mem_brk += incr;
    return (void *)old_brk;
}



static char *heap_listp;
static void *last_searched_bp;

int mm_init()
{
    mem_init();

    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) {
        return -1;
    }

    PUT(heap_listp, 0);                             /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));    /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));    /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 1));        /* Epilogue header */
    heap_listp += (2*WSIZE);

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) {
        return -1;
    }

    last_searched_bp = heap_listp;

    return 0;
}

static void *extend_heap(size_t words)
{
    char *bp;

    /* Allocate an even number of words to maintain alignment */
    size_t size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1) {
        return NULL;
    }

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));           /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));           /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));   /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    coalesce(bp);
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {                 /* Case 1: prev and next allocated */
        return bp;
    }

    else if (prev_alloc && !next_alloc) {           /* Case 2: prev allocated, next free */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc) {           /* Case 3: prev free, next allocated */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    else {                                          /* Case 4: prev and next free */
        size += (GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp))));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    /* Make sure that last_searched_bp is not pointing to a block that we just coalesce */
    if (last_searched_bp > bp && last_searched_bp < (void *)NEXT_BLKP(bp)) {
        last_searched_bp = bp;
    }

    return bp;
}

void *mm_malloc(size_t size)
{
    size_t asize;       /* Adjusted block size */
    size_t extendsize;  /* Amount to extend heap if no fit */
    char *bp;

    /* Ignore spurious request */
    if (size == 0) {
        return NULL;
    }

    /* Adjust block size to include overhead and alignment requests. */
    if (size <= DSIZE) {
        asize = (2*(DSIZE));
    } else {
        asize = (DSIZE) * ((size + (DSIZE) + ((DSIZE) - 1)) / (DSIZE));
    }

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL) {
        return NULL;
    }

    place(bp, asize);
    return bp;
}



static void *find_fit(size_t asize)
{
    for (void *bp = NEXT_BLKP(last_searched_bp); bp != last_searched_bp; bp = NEXT_BLKP(bp)) {
        // Epilogue block
        if (GET_SIZE(HDRP(bp)) == 0) {
            bp = heap_listp; // wrap-around
        }

        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            last_searched_bp = bp;
            return bp;
        }
    }

    return NULL;
}

static void place(void *bp, size_t asize)
{
    size_t min_block_size = 2*(DSIZE);
    size_t block_size = GET_SIZE(HDRP(bp));
    size_t remainder_size = (block_size - asize);

    if (remainder_size >= min_block_size) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(remainder_size, 0));
        PUT(FTRP(bp), PACK(remainder_size, 0));
    } else {
        /* The remainder is not large enough to be used as another block, so use the whole block. */
        PUT(HDRP(bp), PACK(block_size, 1));
        PUT(FTRP(bp), PACK(block_size, 1));
    }
}
