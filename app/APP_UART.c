#include "APP_UART.h"
#include "APP_KVM.h"
#include <stdint.h>

#define KVM_FRAME_HEAD1  0x55
#define KVM_FRAME_HEAD2  0xAA

static KVM_RX_DATA kvm_rx_n32;
static KVM_RX_DATA kvm_rx_ax6800;


uint8_t KVM_CRC8_Calculate(const uint8_t *data, uint8_t length)
{
    uint8_t crc = 0x00;
    uint8_t i;

    while (length--)
    {
        crc ^= *data++;

        for (i = 0; i < 8; i++)
        {
            if (crc & 0x80)
            {
                crc = (uint8_t)((crc << 1) ^ 0x07);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

void KVM_UART_SendFrame(UART_TypeDef* UARTx,uint8_t cmd, uint8_t mode)
{
    uint8_t crc_data[2];
    uint8_t frame[5];

    crc_data[0] = cmd;
    crc_data[1] = mode;

    frame[0] = KVM_FRAME_HEAD1;
    frame[1] = KVM_FRAME_HEAD2;
    frame[2] = cmd;
    frame[3] = mode;
    frame[4] = KVM_CRC8_Calculate(crc_data, 2);

    BSP_UART_FUN.UART_send_buffer(UARTx, frame, sizeof(frame));

}


void KVM_UART_HandleFrame(UART_TypeDef* forward_uart, uint8_t cmd, uint8_t mode)
{
    switch (cmd)
    {
        case KVM_CMD_SET_MODE:
        {
            if ((mode >= 0x01) && (mode <= 0x0F) )
            {
                //接收并设置模式
                /* 返回实际设置成功的模式 */
                APP_KVM_FUN.set_mode(mode);
                KVM_UART_SendFrame(forward_uart,KVM_CMD_SET_MODE, mode);
            }
            break;
        }

        case KVM_CMD_GET_MODE:
        {
            //读取模式
            KVM_UART_SendFrame(forward_uart,KVM_CMD_REPORT_MODE, mode);
            break;
        }

        default:
        {
            break;
        }
    }
}


static void KVM_UART_ReceiveByteWithState(KVM_RX_DATA *rx, UART_TypeDef* forward_uart, uint8_t byte)
{
    uint8_t crc_data[2];
    uint8_t crc;

    switch (rx->state)
    {
        case KVM_RX_HEAD1:
        {
            if (byte == 0x55)
            {
                rx->state = KVM_RX_HEAD2;
            }
            break;
        }

        case KVM_RX_HEAD2:
        {
            if (byte == 0xAA)
            {
                rx->state = KVM_RX_CMD;
            }
            else if (byte == 0x55)
            {
                rx->state = KVM_RX_HEAD2;
            }
            else
            {
                rx->state = KVM_RX_HEAD1;
            }
            break;
        }

        case KVM_RX_CMD:
        {
            rx->cmd = byte;
            rx->state = KVM_RX_MODE;
            break;
        }

        case KVM_RX_MODE:
        {
            rx->mode = byte;
            rx->state = KVM_RX_CRC;
            break;
        }

        case KVM_RX_CRC:
        {
            crc_data[0] = rx->cmd;
            crc_data[1] = rx->mode;

            crc = KVM_CRC8_Calculate(crc_data, 2);

            if (crc == byte)
            {
                KVM_UART_HandleFrame(forward_uart, rx->cmd, rx->mode);
            }

            rx->state = KVM_RX_HEAD1;
            break;
        }

        default:
        {
            rx->state = KVM_RX_HEAD1;
            break;
        }
    }
}

void KVM_UART_ReceiveByte(uint8_t byte)
{
    KVM_UART_ReceiveByteWithState(&kvm_rx_n32, BSP_UART_AX6800, byte);
}

void KVM_UART_AX6800_ReceiveByte(uint8_t byte)
{
    KVM_UART_ReceiveByteWithState(&kvm_rx_ax6800, BSP_UART_N32, byte);
}
