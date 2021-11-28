#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f4xx.h"

void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);

extern __IO u32 TimingDelay;

#endif /*__BSP_SYSTICK_H*/

