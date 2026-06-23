
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct{
    bool state[3];
}BSP_KEY_DATA_Typedefstruct;

typedef struct{
    void (*init)(void);     //  初始化
    void (*read)(BSP_KEY_DATA_Typedefstruct * key);  //  当前的按键状态
}BSP_KEY_FUN_Typedefstruct;
extern const BSP_KEY_FUN_Typedefstruct BSP_KEY_FUN;

#endif
