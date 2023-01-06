#include "inc/types.h"
#include <stddef.h>
#include "mem/page.h"
#include "utils/string.h"
#include "utils/malloc.h"

struct page_frame *_used = NULL;

void *malloc(size_t size)
{
    if (_used == NULL)
    {
        _used = page_alloc(1);
        if (_used == NULL)
        {
            return NULL;
        }
    }

    size += sizeof(struct malloc_frame);
    reg_t align = 1;
    while (align < size)
    {
        align <<= 1;
    }

    struct malloc_frame *next = (struct malloc_frame *)_used->data;
    uint8_t *end = _used->data + PAGE_FRAME_SIZE;
    for (uint8_t *p = _used->data; p < end; p += align)
    {
        struct malloc_frame *mf = (struct malloc_frame *)p;

        if (mf < next)
        {
            continue;
        }
        if (mf->mark == 0x1c1c)
        {
            next = mf->next;
            continue;
        }

        mf->mark = 0x1c1c;
        mf->flags = 0;
        mf->size = align - sizeof(struct malloc_frame);
        mf->next = (struct malloc_frame *)(p + align);
        return mf->data;
    }

    return NULL;
}

void free(void *ptr)
{
    struct malloc_frame *mf = (struct malloc_frame *)((uint8_t *)ptr - sizeof(struct malloc_frame));
    mf->mark = 0;
    mf->size = 0;
    mf->next = NULL;
}

void *realloc(void *ptr, size_t size)
{
    struct malloc_frame *mf = (struct malloc_frame *)((uint8_t *)ptr - sizeof(struct malloc_frame));
    if (mf->size >= size)
    {
        return ptr;
    }

    void *new = malloc(size);
    if (new == NULL)
    {
        return NULL;
    }

    memcpy(new, ptr, mf->size);
    free(ptr);
    return new;
}