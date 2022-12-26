#ifndef __MEM_CONST_H__
#define __MEM_CONST_H__

#include "types.h"

/*
 * Following global vars are defined in mem.S
 */
extern const ssize_t TEXT_START;
extern const ssize_t TEXT_END;
extern const ssize_t DATA_START;
extern const ssize_t DATA_END;
extern const ssize_t RODATA_START;
extern const ssize_t RODATA_END;
extern const ssize_t BSS_START;
extern const ssize_t BSS_END;
extern const ssize_t HEAP_START;
extern const ssize_t HEAP_SIZE;

#endif