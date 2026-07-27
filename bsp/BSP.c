
#include "BSP.h"

#include "stdbool.h"
#include "string.h"



static inline bool ___FUN_GPIO_CLK_ENABLE(BSP_GPIO_INFO_typedefstruct const * const info)
{
    switch ((uint32_t)(info->port))
    {
#if defined ( CW_GPIOA )
    case ((uint32_t)(GPIOA_BASE)) :
        __RCC_GPIOA_CLK_ENABLE();
        break ;
#endif

#if defined ( CW_GPIOB )
    case ((uint32_t)(GPIOB_BASE)) :
        __RCC_GPIOB_CLK_ENABLE();  
        break ;
#endif
#if defined ( CW_GPIOC )
    case ((uint32_t)(GPIOC_BASE)) :
        __RCC_GPIOC_CLK_ENABLE();  
        break ;
#endif
    default :
        return false ;
    }
    return true ;
}
static BSP_GPIO_IRQ_Callback_typedef gpio_irq_callback [ 16 ] ;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    for (uint8_t i = 0 ; i < (sizeof(gpio_irq_callback) / sizeof(gpio_irq_callback [ 0 ])) ; i++)
    {
        if ((GPIO_Pin & (0x01 << i)) && (gpio_irq_callback [ i ] != NULL))
        {
            gpio_irq_callback [ i ]() ;
        }
    }
}
static void FUN_GPIO_Init(BSP_GPIO_INFO_typedefstruct const * const info)
{
    GPIO_InitTypeDef InitStruct = { 0 } ;
    uint8_t irq_flag  = 0 ;
    uint32_t    pin_num;
    for(pin_num=0;pin_num<16;pin_num++){
        if(info->num&(0x01<<pin_num)){
            break;
        }
    }
    if(pin_num>=16){
        return;
    }

    if (___FUN_GPIO_CLK_ENABLE(info) == false)
    {
        return ;
    }
    
    switch (info->mode)
    {
    case BSP_GPIO_MODE_INPUT :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_SET(info->port->DIR,        info->num); //  1：将端口配置成输入
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
        }
        
        break ;

    case BSP_GPIO_MODE_OUTPUT_PP :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_CLR(info->port->DIR,        info->num); //  0：将端口配置成输出
        REGBITS_CLR(info->port->OPENDRAIN,  info->num); //  0：推挽输出
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
        }
        break ;

    case BSP_GPIO_MODE_OUTPUT_OD :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_CLR(info->port->DIR,        info->num); //  0：将端口配置成输出
        REGBITS_SET(info->port->OPENDRAIN,  info->num); //  1：开漏输出
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
            info->port->AFRL |= ((info->option&0x07)<<(pin_num));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
            info->port->AFRH |= ((info->option&0x07)<<(pin_num-8));
        }
        break ;

    case BSP_GPIO_MODE_AF_PP :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_CLR(info->port->DIR,        info->num); //  0：将端口配置成输出
        REGBITS_CLR(info->port->OPENDRAIN,  info->num); //  0：推挽输出
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
            info->port->AFRL |= ((info->option&0x07)<<(pin_num));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
            info->port->AFRH |= ((info->option&0x07)<<(pin_num-8));
        }
        break ;

    case BSP_GPIO_MODE_AF_OD :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_CLR(info->port->DIR,        info->num); //  0：将端口配置成输出
        REGBITS_SET(info->port->OPENDRAIN,  info->num); //  1：开漏输出
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
            info->port->AFRL |= ((info->option&0x07)<<(pin_num));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
            info->port->AFRH |= ((info->option&0x07)<<(pin_num-8));
        }
        break ;

    case BSP_GPIO_MODE_AF_INPUT :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_SET(info->port->DIR,        info->num); //  1：将端口配置成输入
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
            info->port->AFRL |= ((info->option&0x07)<<(pin_num));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
            info->port->AFRH |= ((info->option&0x07)<<(pin_num-8));
        }
        break ;

    case BSP_GPIO_MODE_ANALOG :
        REGBITS_SET(info->port->ANALOG,     info->num); //  1：将端口配置为模拟功能
        break ;

    case BSP_GPIO_MODE_IT_RISING :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_SET(info->port->DIR,        info->num); //  1：将端口配置成输入
        REGBITS_SET(info->port->RISEIE,     info->num); //  1：使能相应端口的上升沿中断
        REGBITS_CLR(info->port->FALLIE,     info->num); //  0：禁止相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
        }
        irq_flag = 1 ;
        break ;

    case BSP_GPIO_MODE_IT_FALLING :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_SET(info->port->DIR,        info->num); //  1：将端口配置成输入
        REGBITS_CLR(info->port->RISEIE,     info->num); //  0：禁止相应端口的上升沿中断
        REGBITS_SET(info->port->FALLIE,     info->num); //  1：使能相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
        }
        irq_flag = 1 ;
        break ;

    case BSP_GPIO_MODE_IT_RISING_FALLING :
        REGBITS_CLR(info->port->ANALOG,     info->num); //  0：将端口配置为数字功能
        REGBITS_SET(info->port->DIR,        info->num); //  1：将端口配置成输入
        REGBITS_SET(info->port->RISEIE,     info->num); //  1：使能相应端口的上升沿中断
        REGBITS_SET(info->port->FALLIE,     info->num); //  1：使能相应端口的下降沿中断
        if(pin_num<8){
            info->port->AFRL &= (~(0xF<<(pin_num)));
        }else{
            info->port->AFRH &= (~(0xF<<(pin_num-8)));
        }
        irq_flag = 1 ;
        break ;

//    case BSP_GPIO_MODE_EVT_RISING :
//        InitStruct.GPIO_Mode = GPIO_MODE_EVT_RISING ;
//        break ;

//    case BSP_GPIO_MODE_EVT_FALLING :
//        InitStruct.GPIO_Mode = GPIO_MODE_EVT_FALLING ;
//        break ;

//    case BSP_GPIO_MODE_EVT_RISING_FALLING :
//        InitStruct.GPIO_Mode = GPIO_MODE_EVT_RISING_FALLING ;
//        break ;

    default :
        return ;
    }

    switch (info->pull)
    {
    case BSP_GPIO_PULL_UP :
        REGBITS_SET(info->port->PUR,        info->num); //  1：使能上拉电阻
        break ;

//    case BSP_GPIO_PULL_DOWN :
//        InitStruct.Pull = GPIO_PULLDOWN ;
//        break ;

    case BSP_GPIO_PULL_NO :
        REGBITS_CLR(info->port->PUR,        info->num); //  0：禁止上拉电阻
        break ;
    default :
        return ;
    }

    if (irq_flag != 0)
    {
        if (gpio_irq_callback [ pin_num ] != NULL)
        {
            return ;
        }
        else
        {
            gpio_irq_callback [ pin_num ] = info->irq_callback ;
        }
    }
    if((info->mode==BSP_GPIO_MODE_OUTPUT_PP) || (info->mode==BSP_GPIO_MODE_OUTPUT_OD)){
        if(info->option){
            BSP_FUN_GPIO.Set(info);
        }else{
            BSP_FUN_GPIO.Reset(info);
        }
    }
//    GPIO_Init(info->port, &InitStruct) ;

}

static void FUN_GPIO_DeInit(BSP_GPIO_INFO_typedefstruct const * const info)
{
    GPIO_DeInit(info->port, info->num);
}

static void FUN_GPIO_Set(BSP_GPIO_INFO_typedefstruct const * const info)
{
    GPIO_WritePin(info->port, info->num, GPIO_Pin_SET);
}

static void FUN_GPIO_Reset(BSP_GPIO_INFO_typedefstruct const * const info)
{
    GPIO_WritePin(info->port, info->num, GPIO_Pin_RESET);
}

static void FUN_GPIO_Toggle(BSP_GPIO_INFO_typedefstruct const * const info)
{
    GPIO_TogglePin(info->port, info->num) ;
}

static char FUN_GPIO_Read(BSP_GPIO_INFO_typedefstruct const * const info)
{
    return GPIO_ReadPin(info->port, info->num) != GPIO_Pin_RESET ;
}

BSP_FUN_GPIO_typestruct BSP_FUN_GPIO =
{
    .Init = FUN_GPIO_Init,
    .DeInit = FUN_GPIO_DeInit,
    .Set = FUN_GPIO_Set,
    .Reset = FUN_GPIO_Reset,
    .Toggle = FUN_GPIO_Toggle,
    .Read = FUN_GPIO_Read,
} ;

