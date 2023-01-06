#ifndef __UTILS_STRING_H__
#define __UTILS_STRING_H__

#include "inc/types.h"
#include <stddef.h>

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
size_t strlen(const char *s);
void *memmove(void *dest, const void *src, size_t n);

#endif /* __UTILS_STRING_H__ */