#ifndef __UTILS_MALLOC_H__
#define __UTILS_MALLOC_H__

#include "inc/types.h"
#include <stddef.h>

struct malloc_frame
{
    uint16_t mark;  // 0x1c1c
    uint16_t flags; // reserved
    uint32_t size;
    struct malloc_frame *prev;
    struct malloc_frame *next;
    uint8_t data[0];
};

void malloc_init();
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif /* __UTILS_MALLOC_H__ */