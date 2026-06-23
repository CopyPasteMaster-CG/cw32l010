
#ifndef __BSP_RCC_H__
#define __BSP_RCC_H__

#include "stdint.h"

typedef struct{
    void (*init)(void);             //  初始化
    uint32_t (*get_core_fre)(void); //  获取内核时钟频率
}BSP_RCC_FUN_Typedefstruct;
extern const BSP_RCC_FUN_Typedefstruct BSP_RCC_FUN;

#endif
