
#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"

#define SYSTICK_DELAY_1MS			(SystemCoreClock/1000)

#define SYSTICK_DELAY_10US			(SystemCoreClock/100000)

#define SYSTICK_DELAY_1US			(SystemCoreClock/1000000)




void Systick_Config(uint32_t ticks);
void Delay_SysTick(uint32_t ntime);








#endif
