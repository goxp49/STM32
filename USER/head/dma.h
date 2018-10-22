#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"
#include <stdio.h>


#define SEND_BUFF_LENGTH 		5000
#define USART1_DR_BASE	 		0x40013804


void NVIC_Config(void);
void DMA_Config(void);


#endif