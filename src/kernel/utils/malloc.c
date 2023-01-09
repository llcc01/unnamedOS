#include "inc/types.h"
#include <stddef.h>
#include "mem/page.h"
#include "utils/string.h"
#include "utils/printf.h"
#include "utils/malloc.h"

struct page_frame *_used = NULL;

void malloc_init()
{
    _used = page_alloc(1);
    if (_used == NULL)
    {
        panic("malloc_init failed");
    }

    struct malloc_frame *mf = (struct malloc_frame *)_used->data;
    mf->mark = 0x1c1c;
    mf->flags = 0;
    mf->size = 0;
    mf->prev = NULL;
    mf->next = (struct malloc_frame *)(_used->data + PAGE_FRAME_SIZE);
    printf("[malloc init] ptr:%p mf:%p end:%p\n", mf->data, mf, mf->next);
}

void *malloc(size_t size)
{

    size += sizeof(struct malloc_frame);
    reg_t align = 1;
    while (align < size)
    {
        align <<= 1;
    }

    struct malloc_frame *cur_try = (struct malloc_frame *)_used->data;
    uint8_t *end = _used->data + PAGE_FRAME_SIZE;
    for (uint8_t *p = _used->data; p < end; p += align)
    {
        struct malloc_frame *mf = (struct malloc_frame *)p;

        // printf("[malloc info] cur_try:%p cur_try->next:%p mf:%p\n", cur_try, cur_try->next, mf);

        // move to the current frame
        if (mf < cur_try)
        {
            // printf("[malloc info] mf < cur_try\n");
            continue;
        }

        // check if the current frame is used
        if (cur_try->size != 0)
        {
            // printf("[malloc info] cur_try->size != 0\n");
            cur_try = cur_try->next;
            continue;
        }

        // check if the current frame is big enough
        if ((uint8_t *)cur_try->next < p + size)
        {
            // printf("[malloc info] (uint8_t *)cur_try->next < p + size\n");
            cur_try = cur_try->next;
            continue;
        }

        struct malloc_frame *next_ori = cur_try->next;
        struct malloc_frame *next = (struct malloc_frame *)(p + align);

        mf->mark = 0x1c1c;
        mf->flags = 0;
        mf->size = align - sizeof(struct malloc_frame);
        mf->next = next;

        if (cur_try != mf)
        {
            cur_try->next = mf;
            mf->prev = cur_try;
        }

        if (next != next_ori)
        {
            next->mark = 0x1c1c;
            next->flags = 0;
            next->size = 0;
            next->prev = mf;
            next->next = next_ori;
            next_ori->prev = next;
        }

        printf("[malloc] ptr:%p mf:%p next:%p\n", mf->data, mf, mf->next);
        return mf->data;
    }

    return NULL;
}

void free(void *ptr)
{
    struct malloc_frame *mf = (struct malloc_frame *)((uint8_t *)ptr - sizeof(struct malloc_frame));
    printf("[free] ptr:%p mf:%p end:%p\n", ptr, mf, mf->next);
    mf->size = 0;
    struct malloc_frame *prev = mf->prev;
    struct malloc_frame *next = mf->next;

    if (next->mark == 0x1c1c)
    {
        mf->next = next->next;
        next->next->prev = mf;
    }

    if (prev != NULL && prev->size == 0)
    {
        mf->mark = 0;
        prev->next = mf->next;
        mf->next->prev = prev;
        mf->next = NULL;
        mf->prev = NULL;
    }
}

void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        return malloc(size);
    }
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
    printf("[realloc] ptr:%p mf:%p end:%p\n", ptr, mf, mf->next);
    return new;
}