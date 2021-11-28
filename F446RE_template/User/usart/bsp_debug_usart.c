#include "bsp_debug_usart.h"

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)

void Debug_USART_Config(void)
{
    /* Disable I/O buffering for STDOUT stream, so that
    * chars are sent out as soon as they are printed.
    * 缓冲区为0，一有数据就发。
    */
    setvbuf(stdout, NULL, _IONBF, 0);

    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

/*----------------------11_串口相关的GPIO初始化-------------------------*/
    /*使能GPIOA时钟*/
    RCC_AHB1PeriphClockCmd(USART_RX_GPIO_CLK|USART_TX_GPIO_CLK, ENABLE);

    /*使能USART时钟*/
    RCC_APB1PeriphClockCmd(USART_CLK, ENABLE);

    /*GPIO初始化*/
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;

    /*配置TX引脚为复用功能*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
    GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);

    /*配置RX引脚为复用功能*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = USART_RX_PIN  ;
    GPIO_Init(USART_RX_GPIO_PORT , &GPIO_InitStruct);

    /*链接PXx到USARTx_Tx*/
    GPIO_PinAFConfig(USART_TX_GPIO_PORT, USART_TX_SOURCE, USART_TX_AF);
    /*链接PXx到USARTx_Rx*/
    GPIO_PinAFConfig(USART_RX_GPIO_PORT, USART_RX_SOURCE, USART_RX_AF);

/*----------------------2_串口配置初始化--------------------------*/
    /* 波特率设置：115200 */
    USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
    /* 字长(数据位+校验位)：8 */
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    /* 停止位：1个停止位 */
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    /* 校验位选择：不使用校验 */
    USART_InitStruct.USART_Parity = USART_Parity_No;
    /* 硬件流控制：不使用硬件流 */
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* USART模式控制：同时使能接收和发送 */
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* 完成USART初始化配置 */
    USART_Init(DEBUG_USART, &USART_InitStruct);

//    /*配置NVIC串口的接受中断*/
//    NVIC_Config();
//    /* 使能串口接收中断 */
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /*使能串口*/
    USART_Cmd(DEBUG_USART, ENABLE);

}

/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* 发送一个字节数据到USART */
    USART_SendData(pUSARTx,ch);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************   发送一个16位数  **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* 取出高八位 */
    temp_h = (ch&0XFF00)>>8;
    /* 取出低八位 */
    temp_l = ch&0XFF;

    /* 发送高八位 */
    USART_SendData(pUSARTx,temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* 发送低八位 */
    USART_SendData(pUSARTx,temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
    {}
}

//重定向c库函数printf到串口，重定向后可使用printf函数
PUTCHAR_PROTOTYPE {
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(DEBUG_USART, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC) == RESET) {
    }

    return ch;
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
GETCHAR_PROTOTYPE{
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET){
    }

    return (int)USART_ReceiveData(DEBUG_USART);
};
