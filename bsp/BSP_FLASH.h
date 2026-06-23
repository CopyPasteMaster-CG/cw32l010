
#ifndef __BSP_FLASH_H__
#define __BSP_FLASH_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct{
    uint8_t left_count;     //  左按钮计数
    uint8_t right_count;    //  右按钮计数
    uint8_t select_mode;    //  选择左按钮还是右按钮
    uint8_t select_usb;     //  USB选择
    uint8_t UART_ENABLE;    //  串口是否使能
}BSP_FLASH_DATA_Typedefstruct;

typedef void(*BSP_FLASH_DATA_DEFAULT_Callback_typedef)(BSP_FLASH_DATA_Typedefstruct * data);

typedef struct{
    void (*init)(BSP_FLASH_DATA_DEFAULT_Callback_typedef callback); //  初始化
    void (*read)(BSP_FLASH_DATA_Typedefstruct * data);              //  读取数据
    void (*write)(BSP_FLASH_DATA_Typedefstruct * data);             //  写入数据
}BSP_FLASH_FUN_Typedefstruct;
extern const BSP_FLASH_FUN_Typedefstruct BSP_FLASH_FUN;

#endif
