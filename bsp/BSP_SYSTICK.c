
#include "BSP_SYSTICK.h"
#include "BSP.h"
#include "cw32l010_systick.h"


//  初始化
static void fun_init(void)
{
    InitTick(SYSCTRL_GetHClkFreq());   //  系统定时器，中断频率1KHz
}

static uint32_t fun_get_tick(void)
{
    return uwTick;
}

static void fun_delay_us(uint32_t us)
{
    volatile uint32_t delay_ms = us/1000;
    volatile uint32_t delay_us = us%1000;
    volatile uint32_t in_tick = fun_get_tick();
    volatile uint32_t in_count = SysTick->VAL;
    volatile uint32_t in_reload = SysTick->LOAD;
    volatile const uint32_t systick_div_us = SYSCTRL_GetHClkFreq()/(1000*1000);
    volatile const uint32_t out_count = (in_count+in_reload-((us*systick_div_us)%in_reload))%in_reload;
    
    if(in_tick<(delay_us*systick_div_us)){
        //  需要新的周期才能满足延时需求
        delay_ms++;
    }
    
    while(1){
        if((fun_get_tick()-in_tick)==delay_ms){
            //  定时器的复位次数到达目标次数，判断计数值决定是否到达目标计数值
            if(SysTick->VAL<out_count){
                return;
            }
        }else if((fun_get_tick()-in_tick)>=delay_ms){
            //  定时器的复位次数超过目标次数，已经超出目标时间，立即退出
            return;
        }
    }
}

static void fun_delay_ms(uint32_t ms)
{
    SysTickDelay(ms);
}


const BSP_SYSTICK_FUN_Typedefstruct BSP_SYSTICK_FUN={
    .init       =   fun_init,
    .get_tick   =   fun_get_tick,
    .delay_us   =   fun_delay_us,
    .delay_ms   =   fun_delay_ms,
};


