#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_systick.h"
#include "bsp_debug_usart.h"

int main(void)
{
    //初始化LED
    LED_GPIO_Config();
    //初始化滴答定时器
    SysTick_Init();
    //初始化串口
    Debug_USART_Config();

    while(1)
    {
        LD2_ON;
        Delay_ms(1000);
        LD2_OFF;
        Delay_ms(1000);
        printf("\r\nnucleoF4_clion_test\r\n");
    }
}





