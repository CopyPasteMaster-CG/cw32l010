
#include "BSP_RCC.h"
#include "BSP.h"

//  初始化
static void fun_init(void)
{
    while(SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1));         //  HSI时钟由HSISOC经过1分频得来，死等稳定，最总HSI是48MHz
    SYSCTRL_SysClk_Switch(SYSCTRL_SYSCLKSRC_HSI);           //  SYSCLK时钟的来源，设置为HSI
    SYSCTRL_HCLKPRS_Config(SYSCTRL_HCLK_DIV1);              //  HCLK由SYSCLK，经过1分频得来
    SYSCTRL_PCLKPRS_Config(SYSCTRL_PCLK_DIV1);              //  PCLK由HCLK，经过1分频得来
    SYSCTRL_SystemCoreClockUpdate(SYSCTRL_GetHClkFreq());   //  配置系统时钟的变量
}

static uint32_t fun_get_core_fre(void)
{
    return SYSCTRL_GetHClkFreq();
}

const BSP_RCC_FUN_Typedefstruct BSP_RCC_FUN={
    .init           =   fun_init,
    .get_core_fre   =   fun_get_core_fre,
};


