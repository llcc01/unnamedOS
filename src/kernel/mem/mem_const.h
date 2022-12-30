#ifndef __MEM_CONST_H__
#define __MEM_CONST_H__

#include "inc/types.h"

/*
 * Following global vars are defined in mem.S
 */
extern const reg_t TEXT_START;
extern const reg_t TEXT_END;
extern const reg_t DATA_START;
extern const reg_t DATA_END;
extern const reg_t RODATA_START;
extern const reg_t RODATA_END;
extern const reg_t BSS_START;
extern const reg_t BSS_END;
extern const reg_t HEAP_START;
extern const reg_t HEAP_SIZE;

#endif