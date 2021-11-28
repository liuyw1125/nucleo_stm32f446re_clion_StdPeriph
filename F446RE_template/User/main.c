#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_systick.h"
#include "bsp_debug_usart.h"

int main(void)
{
    //定义变量，测试能否打印浮点数
    double test_nember = 3.14;
    char ch;

    //初始化LED
    LED_GPIO_Config();
    //初始化滴答定时器
    SysTick_Init();
    //初始化串口
    Debug_USART_Config();

    printf("\r\nnucleoF4驱动支持\r\n");

    //测试浮点数能否正常打印
    printf("%0.2f\r\n",test_nember);
    //确认没有换行符也可以输出
    printf("%0.2f", test_nember);

    /*下面注释的代码，是测试键盘输入，当不在串口使用getchar()函数时，
     * 把usart文件夹下面的bsp_debug_usart.c中中断相关代码注释掉，默认已经注释，位置在第6行和81行。
     * 把stm32f4xx_it.c中的串口中断处理函数相关代码注释掉，默认已经注释，位置在156行。
     * 如需使用，把相关注释打开就可以了。
    */

    //ch = getchar();
    //printf("%c\r\n",ch);
    //需要屏蔽上一个程序中的串口中断接收，串口中断优先级更高
    //不然就一直接收，做不了其他事情。

    printf("\r\nnucleoF4_clion_test\r\n");

    while(1)
    {
        LD2_ON;
        Delay_ms(1000);
        LD2_OFF;
        Delay_ms(1000);
    }
}





