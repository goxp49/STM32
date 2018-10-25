/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：systick.c
 * 描述    ：滴答定时器应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "systick.h"

void Systick_Config(uint32_t ticks)
{
	
	//检查定时器启动是否正常
	while(SysTick_Config(ticks))
	{
		LED3(ON);
	}

	//关闭定时器，初始化时被默认打开
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}



