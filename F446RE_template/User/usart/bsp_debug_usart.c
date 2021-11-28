#include "bsp_debug_usart.h"

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)

//static void NVIC_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStruct;
//
//    /*嵌套向量中断控制器组选择*/
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//
//    /*配置USART2为中断源*/
//    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
//    /*配置主优先级为1*/
//    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//    /*配置子优先级为1*/
//    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//    /*使能中断*/
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//    /*初始化配置NVIC*/
//    NVIC_Init(&NVIC_InitStruct);
//
//}


void Debug_USART_Config(void)
{
    /* Disable I/O buffering for STDOUT stream, so that
    * chars are sent out as soon as they are printed.
    * ������Ϊ0��һ�����ݾͷ���
    */
    setvbuf(stdout, NULL, _IONBF, 0);

    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

/*----------------------1_������ص�GPIO��ʼ��-------------------------*/
    /*ʹ��GPIOAʱ��*/
    RCC_AHB1PeriphClockCmd(USART_RX_GPIO_CLK|USART_TX_GPIO_CLK, ENABLE);

    /*ʹ��USARTʱ��*/
    RCC_APB1PeriphClockCmd(USART_CLK, ENABLE);

    /*GPIO��ʼ��*/
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;

    /*����TX����Ϊ���ù���*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
    GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);

    /*����RX����Ϊ���ù���*/
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = USART_RX_PIN  ;
    GPIO_Init(USART_RX_GPIO_PORT , &GPIO_InitStruct);

    /*����PXx��USARTx_Tx*/
    GPIO_PinAFConfig(USART_TX_GPIO_PORT, USART_TX_SOURCE, USART_TX_AF);
    /*����PXx��USARTx_Rx*/
    GPIO_PinAFConfig(USART_RX_GPIO_PORT, USART_RX_SOURCE, USART_RX_AF);

/*----------------------2_�������ó�ʼ��-------------------------*/
    /* ���������ã�115200 */
    USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
    /* �ֳ�(����λ+У��λ)��8 */
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    /* ֹͣλ��1��ֹͣλ */
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    /* У��λѡ�񣺲�ʹ��У�� */
    USART_InitStruct.USART_Parity = USART_Parity_No;
    /* Ӳ�������ƣ���ʹ��Ӳ���� */
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* ���USART��ʼ������ */
    USART_Init(DEBUG_USART, &USART_InitStruct);

    /*????NVIC?????????ж?*/
    //NVIC_Config();
    /* ??????????ж? */
    //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    /*ʹ�ܴ���*/
    USART_Cmd(DEBUG_USART, ENABLE);

}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* ����һ���ֽ����ݵ�USART */
    USART_SendData(pUSARTx,ch);

    /* �ȴ��������ݼĴ���Ϊ�� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* ȡ���߰�λ */
    temp_h = (ch&0XFF00)>>8;
    /* ȡ���Ͱ�λ */
    temp_l = ch&0XFF;

    /* ���͸߰�λ */
    USART_SendData(pUSARTx,temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* ���͵Ͱ�λ */
    USART_SendData(pUSARTx,temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

    /* �ȴ�������� */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
    {}
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
PUTCHAR_PROTOTYPE {
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(DEBUG_USART, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC) == RESET) {
    }

    return ch;
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���

GETCHAR_PROTOTYPE{
    /* �ȴ������������� */
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET){
    }

    return (int)USART_ReceiveData(DEBUG_USART);
};
