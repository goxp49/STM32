#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define ADC1_DR_BASE	(ADC1_BASE + 0x4c)

void ADC1_IT_Config(void);
void ADC1_DMA_Config(void);


#endif
