
#ifndef __BSP_KVM_H__
#define __BSP_KVM_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct{
    bool state[4];
}BSP_KVM_DATA_Typedefstruct;

typedef struct{
    void (*init)(void);             //  初始化
    void (*out)(BSP_KVM_DATA_Typedefstruct * set);     //  获取时间戳
}BSP_KVM_FUN_Typedefstruct;
extern const BSP_KVM_FUN_Typedefstruct BSP_KVM_FUN;

#endif
