#ifndef __PRINTF_H__
#define __PRINTF_H__

extern int printf(const char *s, ...);
extern void panic(char *s);

#define mark()                                         \
    {                                                  \
        printf("\33[34m[mark]\33[0m %s:%d\n", __FILE__, __LINE__); \
    }

#endif