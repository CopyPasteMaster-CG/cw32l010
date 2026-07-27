#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "BSP.h"
#include "cw32l052_uart.h"


/*============================================================
 * UART”≤º˛≈‰÷√
 *===========================================================*/

/*  π”√UART2 */
#define BSP_UART_AX6800                   CW_UART1
#define BSP_UART_AX6800_CLK               RCC_APB2_PERIPH_UART1

#define BSP_UART_N32                      CW_UART2
#define BSP_UART_N32_CLK                  RCC_APB1_PERIPH_UART2


#define BSP_AX6800_UART_BAUDRATE          115200U

#define BSP_UART_UCLK_FREQ                48000000U

#define BSP_N32_UART_BAUDRATE             4800U


typedef struct
{
    void    (*init)(void);

    void    (*UART_send_buffer)(UART_TypeDef* UARTx,const uint8_t *buffer,uint16_t length);

    void    (*UART_send_byte)(UART_TypeDef* UARTx, uint8_t Data);

} BSP_UART_FUN_Typedefstruct;

extern const BSP_UART_FUN_Typedefstruct BSP_UART_FUN;

#endif