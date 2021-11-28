#ifndef __BSP_DEBUG_USART_H
#define __BSP_DEBUG_USART_H

#include "stm32f4xx.h"
#include "stdio.h"


//引脚定义
/*******************************************************/
#define DEBUG_USART                       USART2
#define USART_CLK                         RCC_APB1Periph_USART2
#define USART_BAUDRATE                    115200  //串口波特率

#define USART_RX_GPIO_PORT                GPIOA
#define USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USART_RX_PIN                      GPIO_Pin_3
#define USART_RX_AF                       GPIO_AF_USART2
#define USART_RX_SOURCE                   GPIO_PinSource3

#define USART_TX_GPIO_PORT                GPIOA
#define USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define USART_TX_PIN                      GPIO_Pin_2
#define USART_TX_AF                       GPIO_AF_USART2
#define USART_TX_SOURCE                   GPIO_PinSource2

#define USART_IRQHandler                  USART1_IRQHandler
#define USART_IRQ                         USART1_IRQn
/************************************************************/


void Debug_USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif /*__BSP_DEBUG_USART_H*/

