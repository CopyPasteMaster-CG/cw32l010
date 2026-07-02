#include "APP_UART.h"
#include "APP_KVM.h"
#include <stdint.h>

#define KVM_FRAME_HEAD1  0x55
#define KVM_FRAME_HEAD2  0xAA

static KVM_RX_DATA kvm_rx;


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


void KVM_UART_HandleFrame(uint8_t cmd, uint8_t mode)
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
                KVM_UART_SendFrame(BSP_UART_AX6800,KVM_CMD_SET_MODE, mode);
                //KVM_UART_SendFrame(BSP_UART_N32,KVM_CMD_ACK, mode);
            }
            break;
        }

        case KVM_CMD_GET_MODE:
        {
            //读取模式
            KVM_UART_SendFrame(BSP_UART_N32,KVM_CMD_REPORT_MODE, mode);
            break;
        }

        default:
        {
            break;
        }
    }
}


void KVM_UART_ReceiveByte(uint8_t byte)
{
    uint8_t crc_data[2];
    uint8_t crc;

    switch (kvm_rx.state)
    {
        case KVM_RX_HEAD1:
        {
            if (byte == 0x55)
            {
                kvm_rx.state = KVM_RX_HEAD2;
            }
            break;
        }

        case KVM_RX_HEAD2:
        {
            if (byte == 0xAA)
            {
                kvm_rx.state = KVM_RX_CMD;
            }
            else if (byte == 0x55)
            {
                kvm_rx.state = KVM_RX_HEAD2;
            }
            else
            {
                kvm_rx.state = KVM_RX_HEAD1;
            }
            break;
        }

        case KVM_RX_CMD:
        {
            kvm_rx.cmd = byte;
            kvm_rx.state = KVM_RX_MODE;
            break;
        }

        case KVM_RX_MODE:
        {
            kvm_rx.mode = byte;
            kvm_rx.state = KVM_RX_CRC;
            break;
        }

        case KVM_RX_CRC:
        {
            crc_data[0] = kvm_rx.cmd;
            crc_data[1] = kvm_rx.mode;

            crc = KVM_CRC8_Calculate(crc_data, 2);

            if (crc == byte)
            {
                KVM_UART_HandleFrame(kvm_rx.cmd, kvm_rx.mode);
            }

            kvm_rx.state = KVM_RX_HEAD1;
            break;
        }

        default:
        {
            kvm_rx.state = KVM_RX_HEAD1;
            break;
        }
    }
}