
#include "BSP_LED.h"
#include "BSP.h"

static const BSP_GPIO_INFO_typedefstruct led_1_2={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_1,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct led_3_4={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_0,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct usb_1_2={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_2,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct usb_3_4={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_INPUT,    //  输入模式
    .num            =   GPIO_PIN_3,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};


//  1号指示灯点亮
static void led_1_on(void)
{
    PA01_SETLOW();      //  低电平 点亮led1，熄灭led2
    PA01_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  1号指示灯熄灭
static void led_1_off(void)
{
    PA01_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  2号指示灯点亮
static void led_2_on(void)
{
    PA01_SETHIGH();     //  高电平 点亮led2，熄灭led1
    PA01_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  2号指示灯熄灭
static void led_2_off(void)
{
    PA01_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  3号指示灯点亮
static void led_3_on(void)
{
    PA00_SETLOW();      //  低电平 点亮led3，熄灭led4
    PA00_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  3号指示灯熄灭
static void led_3_off(void)
{
    PA00_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  4号指示灯点亮
static void led_4_on(void)
{
    PA00_SETHIGH();     //  高电平 点亮led4，熄灭led3
    PA00_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  4号指示灯熄灭
static void led_4_off(void)
{
    PA00_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}

//  USB-1号指示灯点亮
static void led_usb_1_on(void)
{
    PA02_SETLOW();      //  低电平 点亮led1，熄灭led2
    PA02_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  USB-1号指示灯熄灭
static void led_usb_1_off(void)
{
    PA02_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  USB-2号指示灯点亮
static void led_usb_2_on(void)
{
    PA02_SETHIGH();     //  高电平 点亮led2，熄灭led1
    PA02_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  USB-2号指示灯熄灭
static void led_usb_2_off(void)
{
    PA02_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  USB-3号指示灯点亮
static void led_usb_3_on(void)
{
    PA03_SETLOW();      //  低电平 点亮led3，熄灭led4
    PA03_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  USB-3号指示灯熄灭
static void led_usb_3_off(void)
{
    PA03_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}
//  USB-4号指示灯点亮
static void led_usb_4_on(void)
{
    PA03_SETHIGH();     //  高电平 点亮led4，熄灭led3
    PA03_DIR_OUTPUT();  //  输出模式，必定点亮1颗led
}
//  USB-4号指示灯熄灭
static void led_usb_4_off(void)
{
    PA03_DIR_INPUT();   //  输入模式，两颗led都会熄灭
}

//  指示灯动作分组
static void (*led_state[][2])(void)={
    {led_1_off, led_1_on},
    {led_2_off, led_2_on},
    {led_3_off, led_3_on},
    {led_4_off, led_4_on},
    {led_usb_1_off, led_usb_1_on},
    {led_usb_2_off, led_usb_2_on},
    {led_usb_3_off, led_usb_3_on},
    {led_usb_4_off, led_usb_4_on},
};
//  指示灯状态
static uint8_t state[sizeof(led_state)/sizeof(led_state[0])];

static bool USB_LED_ENABLE = false;

//  指示灯初始化
static void led_io_init(bool usb_led)
{
    BSP_FUN_GPIO.Init(&led_1_2);
    BSP_FUN_GPIO.Init(&led_3_4);
    BSP_FUN_GPIO.Init(&usb_1_2);
    BSP_FUN_GPIO.Init(&usb_3_4);
    memset(state, 0, sizeof(state));
    USB_LED_ENABLE = usb_led;
}

//  刷新指示灯状态
static void led_io_refresh(void)
{
    static uint8_t set_point=0; //  组别，

    //  指示灯编号分组
    static const uint8_t point[][2]={
        {1-1,   2-1},
        {3-1,   4-1},
        {5-1,   6-1},
        {7-1,   8-1},
    };
    
    set_point=(set_point+1)%(sizeof(point[0])/sizeof(point[0][0]));  //  同一组内的编号切换
    
    led_state[point[0][set_point]][state[point[0][set_point]]]();   //  第0组别指示灯的状态
    led_state[point[1][set_point]][state[point[1][set_point]]]();   //  第1组别指示灯的状态
    if(USB_LED_ENABLE){
        led_state[point[2][set_point]][state[point[2][set_point]]]();   //  第2组别指示灯的状态
        led_state[point[3][set_point]][state[point[3][set_point]]]();   //  第3组别指示灯的状态
    }
}

static void led_io_on(BSP_LED_NUM_Typedefenum num)
{
    state[num]=1;
}

static void led_io_off(BSP_LED_NUM_Typedefenum num)
{
    state[num]=0;
}

static void led_io_toggle(BSP_LED_NUM_Typedefenum num)
{
    state[num]=!state[num];
}

static char led_io_read(BSP_LED_NUM_Typedefenum num)
{
    return state[num];
}


const BSP_LED_FUN_Typedefstruct BSP_LED_FUN={
    .init   =   led_io_init,
    .refresh=   led_io_refresh,
    .on     =   led_io_on,
    .off    =   led_io_off,
    .toggle =   led_io_toggle,
    .read   =   led_io_read,
};


