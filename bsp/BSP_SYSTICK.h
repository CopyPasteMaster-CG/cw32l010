
#ifndef __BSP_SYSTICK_H__
#define __BSP_SYSTICK_H__

#include "stdint.h"

typedef struct{
    void (*init)(void);             //  初始化
    uint32_t (*get_tick)(void);     //  获取时间戳
    void (*delay_us)(uint32_t us);  //  微秒级延时，阻塞式
    void (*delay_ms)(uint32_t ms);  //  毫秒级延时，阻塞式
}BSP_SYSTICK_FUN_Typedefstruct;
extern const BSP_SYSTICK_FUN_Typedefstruct BSP_SYSTICK_FUN;

#endif
