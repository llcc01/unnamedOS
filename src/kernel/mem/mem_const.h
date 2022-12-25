#ifndef __MEM_CONST_H__
#define __MEM_CONST_H__

#include "types.h"

/*
 * Following global vars are defined in mem.S
 */
extern const uword_t TEXT_START;
extern const uword_t TEXT_END;
extern const uword_t DATA_START;
extern const uword_t DATA_END;
extern const uword_t RODATA_START;
extern const uword_t RODATA_END;
extern const uword_t BSS_START;
extern const uword_t BSS_END;
extern const uword_t HEAP_START;
extern const uword_t HEAP_SIZE;

#endif