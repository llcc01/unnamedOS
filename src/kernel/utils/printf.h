#ifndef __PRINTF_H__
#define __PRINTF_H__

extern int printf(const char *s, ...);
extern void panic(char *s);

#define mark()                                         \
    {                                                  \
        printf("[mark] %s:%d\n", __FILE__, __LINE__); \
    }

#endif