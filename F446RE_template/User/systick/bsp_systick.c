#include "bsp_systick.h"

__IO u32 TimingDelay;

/*启动系统滴答定时器 SysTick*/
void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000	 10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}

/*ms延时程序,*/
void Delay_ms(__IO u32 nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

//标志位实现延时函数
void SysTick_Delay_Ms(__IO uint32_t ms)
{
    uint32_t i;
    SysTick_Config(SystemCoreClock / 1000);

    for(i=0;i<ms;i++)
    {
        //当计数器的值减小到0的时候，CRTL寄存器的位16会置1
        //当置1时，读取该位会清0
        while(!((SysTick->CTRL) & (1<<16)));
    }
    //关闭Systick定时器
    SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
