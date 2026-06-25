#include "main.h"
#include "APP_UART.h"

void UART1_IRQHandler(void)
{
    uint8_t rx_byte;

    if (UART_GetITStatus(CW_UART1, UART_IT_RC) != RESET)
    {
        rx_byte = UART_ReceiveData_8bit(CW_UART1);
        UART_ClearITPendingBit(CW_UART1, UART_IT_RC);
        KVM_UART_ReceiveByte(rx_byte);
    }
}
