
#ifndef __APP_FLASH_H__
#define __APP_FLASH_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    
    //  USB-KVM分析，多种模式
    struct{
        uint8_t left_count;     //  左按钮计数
        uint8_t right_count;    //  右按钮计数
        uint8_t select_mode;    //  选择左按钮还是右按钮,0-选择左边
    }USB_KVM;
    
    //  USB-切换，一从多主，从站可以选择依附哪一个主站
    struct{
        uint8_t select_usb;     //  USB选择
    }USB_SELECT;
    
    //  拓展模块是否起作用
    struct{
        bool ext_enable;    //  false:禁用拓展模块，启用USB从站的指示灯，，，，true:启用拓展模块，禁用USB从站的指示灯
    }EXT_MODE;
} APP_FLASH_DATA_Typedefstruct;


typedef struct {
    void ( * init ) ( void ) ;
    void ( * loop ) ( void ) ;
    void ( * read ) ( APP_FLASH_DATA_Typedefstruct * da ) ;
}APP_FLASH_FUN_Typestruct;
extern const APP_FLASH_FUN_Typestruct APP_FLASH_FUN;


#endif
