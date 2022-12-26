#ifndef __PAGE_H__
#define __PAGE_H__

#include "types.h"

#define PAGE_FRAME_SIZE 4096
#define PAGE_ORDER 8
/*
 * We reserved 8 Page (1 x 8 x 4096) to hold the Page structures.
 * It should be enough to manage at most 128 MB (8 x 4096 x 4096)
 */
#define PAGE_FRAME_RESERVED_NUM 32

#define PAGE_USED (uint8_t)(1 << 0)
#define PAGE_IS_HEAD (uint8_t)(1 << 1)
#define PAGE_IS_REAR (uint8_t)(1 << 2)

struct page_frame
{
    uint8_t data[PAGE_FRAME_SIZE];
};

struct page_descriptor
{
    uint8_t flags;
};

void page_init();
struct page_frame *page_alloc(ssize_t npages);
void page_free(struct page_frame *p);
void page_test();

#endif