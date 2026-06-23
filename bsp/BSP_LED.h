
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stdbool.h"

typedef enum{
    BSP_LED_NUM_1,
    BSP_LED_NUM_2,
    BSP_LED_NUM_3,
    BSP_LED_NUM_4,
    BSP_LED_USB_1,
    BSP_LED_USB_2,
    BSP_LED_USB_3,
    BSP_LED_USB_4,
}BSP_LED_NUM_Typedefenum;

typedef struct{
    void (*init)(bool usb_led_enable);              //  初始化指示灯,usb_led_enable:USB指示灯是否使能
    void (*refresh)(void);                      //  刷新指示灯的状态，需要定时调用，建议5ms以内
    void (*on)(BSP_LED_NUM_Typedefenum);        //  将一颗指示灯的状态，标记为点亮（不会立即生效，需要调用刷新函数后才会改变状态）
    void (*off)(BSP_LED_NUM_Typedefenum);       //  将一颗指示灯的状态，标记为熄灭（不会立即生效，需要调用刷新函数后才会改变状态）
    void (*toggle)(BSP_LED_NUM_Typedefenum);    //  将一颗指示灯的状态，标记为翻转（不会立即生效，需要调用刷新函数后才会改变状态）
    char (*read)(BSP_LED_NUM_Typedefenum);      //  读取一颗指示灯的标记状态
}BSP_LED_FUN_Typedefstruct;
extern const BSP_LED_FUN_Typedefstruct BSP_LED_FUN;

#endif
