#include <stddef.h>
#include "types.h"
#include "mem_const.h"
#include "utils/printf.h"
#include "page.h"

static uword_t _page_num = 0;
static struct page_frame *_page_start = 0;
static struct page_frame *_page_end = 0;
static struct page_descriptor *_pd_start = 0;
static struct page_descriptor *_pd_end = 0;

static inline void _clear(struct page_descriptor *pd_d)
{
    pd_d->flags = 0;
}

static inline void _set_flag(struct page_descriptor *pd_d, uint8_t flags)
{
    pd_d->flags |= flags;
}

static inline int _is_free(struct page_descriptor *pd_p)
{
    return !(pd_p->flags & PAGE_USED);
}

static inline int _is_rear(struct page_descriptor *pd_p)
{
    return !(pd_p->flags & PAGE_IS_REAR);
}

/*
 * align the address to the border of page(4K)
 */
static inline struct page_frame *_align_page(struct page_frame *address)
{
    uword_t order = (1 << PAGE_ORDER) - 1;
    return (struct page_frame *)(((uword_t)address + order) & (~order));
}

void page_init()
{
    _page_num = (HEAP_SIZE / PAGE_FRAME_SIZE) - PAGE_FRAME_RESERVED_NUM;
    printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n", HEAP_START, HEAP_SIZE, _page_num);

    struct page_descriptor *pd_p = (struct page_descriptor *)HEAP_START;
    for (uword_t i = 0; i < _page_num; i++)
    {
        // printf("clean %x\n", pd_p);
        _clear(pd_p);
        pd_p++;
    }

    _page_start = _align_page((struct page_frame *)(HEAP_START + PAGE_FRAME_RESERVED_NUM * PAGE_FRAME_SIZE));
    _page_end = _page_start + (PAGE_FRAME_SIZE * _page_num);
    _pd_start = (struct page_descriptor *)HEAP_START;
    _pd_end = _pd_start + _page_num;

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("HEAP:   0x%x -> 0x%x\n", _page_start, _page_end);
}

/*
 * Allocate a memory block which is composed of contiguous physical pages
 * - npages: the number of PAGE_SIZE pages to allocate
 * 采用2^n对齐 align the addr with 2^n
 */
struct page_frame *page_alloc(uword_t npages)
{
    /* Note we are searching the page descriptor bitmaps. */
    int found = 0;
    uword_t align = 1;
    while (align < npages)
    {
        align <<= 1;
    }
    // printf("page_alloc: n=%d, align=%d\n", npages, align);

    for (struct page_descriptor *pd_i = _pd_start; pd_i <= (_pd_end - npages); pd_i += align)
    {
        if (_is_free(pd_i))
        {
            found = 1;
            /*
             * meet a free page, continue to check if following
             * (npages - 1) pages are also unallocated.
             */

            for (struct page_descriptor *pd_j = pd_i + 1; pd_j < (pd_i + npages); pd_j++)
            {
                if (!_is_free(pd_j))
                {
                    found = 0;
                    break;
                }
            }
            if (found)
            {
                struct page_descriptor *pd_op = pd_i;
                for (uword_t i = 0; i < npages; i++, pd_op++)
                {
                    _set_flag(pd_op, PAGE_USED);
                }
                _set_flag(pd_i, PAGE_IS_HEAD);
                _set_flag(pd_i + (npages - 1), PAGE_IS_REAR);
                return _page_start + (pd_i - _pd_start);
            }
        }
    }
    return NULL;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(struct page_frame *p)
{
    if (p == NULL || p < _page_start || p >= _page_end)
        return;

    struct page_descriptor *pd_op = _pd_start + (p - _page_start);
    if (_is_free(pd_op))
        return;

    for (; !_is_rear(pd_op); pd_op++)
    {
        pd_op->flags = 0;
    }
}

void page_test()
{
    struct page_frame *p = page_alloc(2);
    printf("p = 0x%x\n", p);
    // page_free(p);

    struct page_frame *p2 = page_alloc(7);
    printf("p2 = 0x%x\n", p2);
    page_free(p2);

    struct page_frame *p3 = page_alloc(4);
    printf("p3 = 0x%x\n", p3);
}
