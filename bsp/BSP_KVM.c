
#include "BSP_KVM.h"
#include "BSP.h"

static const BSP_GPIO_INFO_typedefstruct kvm_io_1={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_3,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct kvm_io_2={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_1,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOB,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct kvm_io_3={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_0,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOB,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

static const BSP_GPIO_INFO_typedefstruct kvm_io_4={
    .irq_callback   =   NULL,                   //  无中断回调
    .mode           =   BSP_GPIO_MODE_OUTPUT_PP,    //  输出模式
    .num            =   GPIO_PIN_2,             //  引脚标号
    .option         =   0,                      //  无自定义
    .port           =   CW_GPIOA,               //  引脚端口
    .pull           =   BSP_GPIO_PULL_NO,       //  无上下拉
};

//  初始化
static void fun_init(void)
{
    BSP_FUN_GPIO.Init(&kvm_io_1);
    BSP_FUN_GPIO.Init(&kvm_io_2);
    BSP_FUN_GPIO.Init(&kvm_io_3);
    BSP_FUN_GPIO.Init(&kvm_io_4);
    
    BSP_FUN_GPIO.Reset(&kvm_io_1);
    BSP_FUN_GPIO.Reset(&kvm_io_2);
    BSP_FUN_GPIO.Reset(&kvm_io_3);
    BSP_FUN_GPIO.Reset(&kvm_io_4);
}

static void fun_out(BSP_KVM_DATA_Typedefstruct * set)
 {
    if(set->state[0]){
        BSP_FUN_GPIO.Set(&kvm_io_1);
    }else{
        BSP_FUN_GPIO.Reset(&kvm_io_1);
    }
    
    if(set->state[1]){
        BSP_FUN_GPIO.Set(&kvm_io_2);
    }else{
        BSP_FUN_GPIO.Reset(&kvm_io_2);
    }
    
    if(set->state[2]){
        BSP_FUN_GPIO.Set(&kvm_io_3);
    }else{
        BSP_FUN_GPIO.Reset(&kvm_io_3);
    }
    
    if(set->state[3]){
        BSP_FUN_GPIO.Set(&kvm_io_4);
    }else{
        BSP_FUN_GPIO.Reset(&kvm_io_4);
    }
}


const BSP_KVM_FUN_Typedefstruct BSP_KVM_FUN={
    .init   =   fun_init,
    .out    =   fun_out,
};


