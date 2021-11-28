#include "bsp_led.h"

//0.首先要开GPIO端口的时钟
//1.要先确定引脚好
//2.要确定是输入还是输出MODER
//3.如果是输出，那么是推挽还是开漏输出OTYPER
//4.是上拉还是下拉
//5.那么输出的速度是多少呢？

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(LD2_CLK, ENABLE);

    GPIO_InitStruct.GPIO_Pin = LD2_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;

    GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}


