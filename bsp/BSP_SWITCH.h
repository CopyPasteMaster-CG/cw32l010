
#ifndef __BSP_SWITCH_H__
#define __BSP_SWITCH_H__

#include "stdint.h"
#include "stdbool.h"

typedef enum{
    BSP_SWITCH_MODE_1 = 0,
    BSP_SWITCH_MODE_2,
    BSP_SWITCH_MODE_3,
    BSP_SWITCH_MODE_4,
}BSP_SWITCH_MODE_Typedefstruct;

typedef struct{
    void (*init)(void);             //  初始化
    void (*out)(BSP_SWITCH_MODE_Typedefstruct mode);     //  获取时间戳
}BSP_SWITCH_FUN_Typedefstruct;
extern const BSP_SWITCH_FUN_Typedefstruct BSP_SWITCH_FUN;

#endif
