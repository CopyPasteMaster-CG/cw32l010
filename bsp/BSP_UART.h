#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "BSP.h"
#include "cw32l010_uart.h"


/*============================================================
 * UART”≤º˛≈‰÷√
 *===========================================================*/

/*  π”√UART2 */
#define BSP_UART_AX6800                   CW_UART2
#define BSP_UART_AX6800_CLK               SYSCTRL_APB1_PERIPH_UART2

#define BSP_UART_N32                      CW_UART1
#define BSP_UART_N32_CLK                  SYSCTRL_APB1_PERIPH_UART1

#define BSP_UART_BAUDRATE                 115200U
#define BSP_UART_UCLK_FREQ                48000000U


typedef struct
{
    void    (*init)(void);

    void    (*UART_send_buffer)(UART_TypeDef* UARTx,const uint8_t *buffer,uint16_t length);

    void    (*UART_send_byte)(UART_TypeDef* UARTx, uint8_t Data);

} BSP_UART_FUN_Typedefstruct;

extern const BSP_UART_FUN_Typedefstruct BSP_UART_FUN;

#endif