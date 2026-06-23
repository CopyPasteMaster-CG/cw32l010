
#include "BSP_SWITCH.h"
#include "BSP.h"

static const BSP_GPIO_INFO_typedefstruct SWITCH_io_12_34={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_2,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOB,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct SWITCH_1_2={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_6,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct SWITCH_3_4={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_3,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOB,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

//  初始化
static void fun_init(void)
{
    BSP_FUN_GPIO.Init(&SWITCH_io_12_34);
    BSP_FUN_GPIO.Init(&SWITCH_1_2);
    BSP_FUN_GPIO.Init(&SWITCH_3_4);
    
    BSP_FUN_GPIO.Reset(&SWITCH_io_12_34);
    BSP_FUN_GPIO.Reset(&SWITCH_1_2);
    BSP_FUN_GPIO.Reset(&SWITCH_3_4);
}

static void fun_out(BSP_SWITCH_MODE_Typedefstruct mode)
{
    switch(mode){
        case    BSP_SWITCH_MODE_1:
            BSP_FUN_GPIO.Reset(&SWITCH_io_12_34);
            BSP_FUN_GPIO.Reset(&SWITCH_1_2);
            BSP_FUN_GPIO.Set(&SWITCH_3_4);
        break;
        
        case    BSP_SWITCH_MODE_2:
            BSP_FUN_GPIO.Reset(&SWITCH_io_12_34);
            BSP_FUN_GPIO.Set(&SWITCH_1_2);
            BSP_FUN_GPIO.Reset(&SWITCH_3_4);
        break;
        
        case    BSP_SWITCH_MODE_3:
            BSP_FUN_GPIO.Set(&SWITCH_io_12_34);
            BSP_FUN_GPIO.Set(&SWITCH_1_2);
            BSP_FUN_GPIO.Reset(&SWITCH_3_4);
        break;
        
        case    BSP_SWITCH_MODE_4:
            BSP_FUN_GPIO.Set(&SWITCH_io_12_34);
            BSP_FUN_GPIO.Reset(&SWITCH_1_2);
            BSP_FUN_GPIO.Set(&SWITCH_3_4);
        break;
    }
}


const BSP_SWITCH_FUN_Typedefstruct BSP_SWITCH_FUN={
    .init   =   fun_init,
    .out    =   fun_out,
};


