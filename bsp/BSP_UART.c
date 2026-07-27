#include "BSP_UART.h"



static void uart_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();
		
		PB00_AFx_UART2RXD();
		PB01_AFx_UART2TXD();
    

		GPIO_InitStructure.Pins = 1;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = 0;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

     
}

static void fun_init(void)
{
    UART_InitTypeDef uart_init = {0};

    uart_gpio_init();
		
    RCC_APBPeriphClk_Enable1(BSP_UART_N32_CLK,ENABLE);
    RCC_APBPeriphClk_Enable2(BSP_UART_AX6800_CLK,ENABLE);

   

    /* 16倍过采样 */
    uart_init.UART_Over = UART_Over_16;

    /* UART时钟源选择PCLK */
    uart_init.UART_Source = UART_Source_PCLK;

    /*
     * 告诉UART库函数：
     * 当前UART输入时钟为8MHz。
     */
    uart_init.UART_UclkFreq = BSP_UART_UCLK_FREQ;

    /* 起始位检测方式 */
    uart_init.UART_StartBit = UART_StartBit_FE;

    /* 1个停止位 */
    uart_init.UART_StopBits = UART_StopBits_1;

    /* 无校验 */
    uart_init.UART_Parity = UART_Parity_No;

    /* 不使用RTS、CTS硬件流控 */
    uart_init.UART_HardwareFlowControl = UART_HardwareFlowControl_None;

   
	uart_init.UART_BaudRate = BSP_AX6800_UART_BAUDRATE;
    uart_init.UART_Mode = UART_Mode_Rx|UART_Mode_Tx;
    UART_Init(BSP_UART_AX6800, &uart_init);
	 
	uart_init.UART_BaudRate = BSP_N32_UART_BAUDRATE;
    uart_init.UART_Mode =UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(BSP_UART_N32, &uart_init);
		
		 /* 开启发送 */


    UART_ClearITPendingBit(BSP_UART_N32, UART_IT_RC);
    UART_ITConfig(BSP_UART_N32, UART_IT_RC, ENABLE);

    NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_SetPriority(UART1_IRQn, 1);
    NVIC_EnableIRQ(UART1_IRQn);


    UART_ClearITPendingBit(BSP_UART_AX6800, UART_IT_RC);
    UART_ITConfig(BSP_UART_AX6800, UART_IT_RC, ENABLE);
    
    NVIC_ClearPendingIRQ(UART2_IRQn);
    NVIC_SetPriority(UART2_IRQn, 1);
    NVIC_EnableIRQ(UART2_IRQn);
}

/**
 * @brief 通过UART发送一段Buffer
 *
 * @param buffer 待发送数据地址
 * @param length 待发送数据长度
 */
static void send_buffer(UART_TypeDef* UARTx,const uint8_t *buffer,uint16_t length)
{
    uint16_t i;

    if ((buffer == NULL) || (length == 0U))
    {
        return;
    }

    for (i = 0U; i < length; i++)
    {
        while (UART_GetFlagStatus(UARTx,UART_FLAG_TXE) == RESET)
        {
        }
       UART_SendData_8bit(UARTx,buffer[i]);
    }

    /*
     * 这里最后等待TXE，只表示发送数据寄存器已经空。
     *
     * 如果库中存在UART_FLAG_TC，可以进一步等待TC，
     * 表示最后一个字节已经完全从TX引脚发送出去。
     */
   while (UART_GetFlagStatus(UARTx,UART_FLAG_TC) == RESET)
    {
    }
}

static void send_byte(UART_TypeDef* UARTx, uint8_t Data)
{
    while (UART_GetFlagStatus(UARTx, UART_FLAG_TXE) == RESET)
    {
    }

    UART_SendData_8bit(UARTx, Data);

    while (UART_GetFlagStatus(UARTx, UART_FLAG_TC) == RESET)
    {
    }
}

const BSP_UART_FUN_Typedefstruct BSP_UART_FUN =
{
    .init                = fun_init,
    .UART_send_byte      = send_byte,
    .UART_send_buffer    = send_buffer,
};
