
#ifndef __CAN_H
#define __CAN_H


#include "stm32f10x.h"



void CAN_Config(void);
void CAN_GPIO_INIT(void);
void CAN_NVIC_INIT(void);
void CAN_MODE_INIT(void);
void CAN_FILTER_INIT(void);
void CAN_SendMessage(void);












#endif
