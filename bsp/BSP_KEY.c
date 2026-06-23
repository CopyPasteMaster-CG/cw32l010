
#include "BSP_KEY.h"
#include "BSP.h"

static const BSP_GPIO_INFO_typedefstruct key_1={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_4,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_UP,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct key_2={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_5,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_UP,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct key_3={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_4,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOB,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_UP,       //  无上下拉
};

//  初始化
static void fun_init(void)
{
    BSP_FUN_GPIO.Init(&key_1);
    BSP_FUN_GPIO.Init(&key_2);
    BSP_FUN_GPIO.Init(&key_3);
}

static void fun_read(BSP_KEY_DATA_Typedefstruct * key)
{
    key->state[0]   =   (BSP_FUN_GPIO.Read(&key_1)==0);
    key->state[1]   =   (BSP_FUN_GPIO.Read(&key_2)==0);
    key->state[2]   =   (BSP_FUN_GPIO.Read(&key_3)==0);
}

const BSP_KEY_FUN_Typedefstruct BSP_KEY_FUN={
    .init   =   fun_init,
    .read   =   fun_read,
};


