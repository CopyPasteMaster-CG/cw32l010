#ifndef __APP_UART_H__
#define __APP_UART_H__

#include "BSP_UART.h"


#define KVM_MODE_PORT1_ONLY              0x01
#define KVM_MODE_PORT2_ONLY              0x02
#define KVM_MODE_PORT3_ONLY              0x03
#define KVM_MODE_PORT4_ONLY              0x04

#define KVM_MODE_TWO_SMALL_MAIN_PORT1    0x05
#define KVM_MODE_TWO_SMALL_MAIN_PORT2    0x06
#define KVM_MODE_TWO_SMALL_MAIN_PORT3    0x07

#define KVM_MODE_PORT12_ONLY             0x08
#define KVM_MODE_PORT34_ONLY             0x09
#define KVM_MODE_PORT_ALL                0x0A
#define KVM_MODE_PORT_ALL_SYNC           0x0B

#define KVM_MODE_THREE_SMALL_MAIN_PORT4  0x0C
#define KVM_MODE_THREE_SMALL_MAIN_PORT3  0x0D
#define KVM_MODE_THREE_SMALL_MAIN_PORT2  0x0E
#define KVM_MODE_THREE_SMALL_MAIN_PORT1  0x0F

#define KVM_CMD_SET_MODE     0x01  /* 设置KVM模式 */
#define KVM_CMD_GET_MODE     0x02  /* 查询当前模式 */
#define KVM_CMD_REPORT_MODE  0x82  /* 返回当前模式 */
#define KVM_CMD_ACK          0x80  /* 设置结果应答 */


typedef enum
{
    KVM_RX_HEAD1 = 0,
    KVM_RX_HEAD2,
    KVM_RX_CMD,
    KVM_RX_MODE,
    KVM_RX_CRC
} KVM_RX_STATE;

typedef struct
{
    KVM_RX_STATE state;
    uint8_t cmd;
    uint8_t mode;
} KVM_RX_DATA;




uint8_t KVM_CRC8_Calculate(const uint8_t *data, uint8_t length);
void KVM_UART_SendFrame(UART_TypeDef* UARTx,uint8_t cmd, uint8_t mode);
void KVM_UART_HandleFrame(uint8_t cmd, uint8_t mode);
void KVM_UART_ReceiveByte(uint8_t byte);


#endif
