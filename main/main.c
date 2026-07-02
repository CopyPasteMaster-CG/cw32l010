
#include "main.h"

#include "BSP_RCC.h"
#include "BSP_SYSTICK.h"
#include "BSP_UART.h"

#include "APP_FLASH.h"
#include "APP_LED.h"
#include "APP_KEY.h"
#include "APP_KVM.h"
#include "APP_SWITCH.h"

int main(void)
{
    BSP_RCC_FUN.init();
    BSP_SYSTICK_FUN.init();
    

    APP_FLASH_FUN.init();
    APP_LED_FUN.init();
    APP_KEY_FUN.init();
    APP_KVM_FUN.init();
    APP_SWITCH_FUN.init();
    BSP_UART_FUN.init();
	
    while(1){


        APP_FLASH_FUN.loop();
        APP_KEY_FUN.loop();
        APP_LED_FUN.loop();
        APP_KVM_FUN.loop();
        APP_SWITCH_FUN.loop();
    }
}


void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

