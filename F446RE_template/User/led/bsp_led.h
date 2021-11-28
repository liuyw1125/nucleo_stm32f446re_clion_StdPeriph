#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"

#define LD2_Pin 		GPIO_Pin_5
#define LD2_GPIO_Port 	GPIOA
#define LD2_CLK	   	    RCC_AHB1Periph_GPIOA

#define LD2_OFF			GPIO_ResetBits(LD2_GPIO_Port, LD2_Pin)
#define LD2_ON			GPIO_SetBits(LD2_GPIO_Port, LD2_Pin)

void LED_GPIO_Config(void);

#endif /*__BSP_LED_H*/


