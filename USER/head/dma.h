#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"


#define SEND_BUFF_LENGTH 		5000
#define USART1_DR_BASE	 		(USART1_BASE + 0x04)


void DMA_Config(void);
void DMA_NVIC_Config(void);


#endif
