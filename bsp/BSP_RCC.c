
#include "BSP_RCC.h"
#include "BSP.h"
#include "cw32l052_flash.h"
//  初始化
static void fun_init(void)
{
    FLASH_SetLatency(FLASH_Latency_2);

    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

    if (RCC_HSI_Enable(RCC_HSIOSC_DIV1) != 0)
    {
        /* 启动失败处理 */
    }

    if (RCC_SysClk_Switch(RCC_SYSCLKSRC_HSI) != 0)
    {
        /* 切换失败处理 */
    }

    RCC_SystemCoreClockUpdate(RCC_Sysctrl_GetHClkFreq());

}

static uint32_t fun_get_core_fre(void)
{
    return RCC_Sysctrl_GetHClkFreq();
}

const BSP_RCC_FUN_Typedefstruct BSP_RCC_FUN={
    .init           =   fun_init,
    .get_core_fre   =   fun_get_core_fre,
};


