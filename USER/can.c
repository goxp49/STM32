/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：can.c
 * 描述    ：CAN应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "can.h"



void CAN_Config(void)
{
	CAN_GPIO_INIT();
	CAN_NVIC_INIT();
	CAN_MODE_INIT();
}


void CAN_GPIO_INIT(void)
{
	GPIO_InitTypeDef gpio_initStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// RX --> 11 , TX --> 12;
	gpio_initStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_12

	gpio_initStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &gpio_initStructure);

	gpio_initStructure.GPIO_Mode = GPIO_Mode_IPU;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_11

	GPIO_Init(GPIOA, &gpio_initStructure);

}


void CAN_NVIC_INIT(void)
{
	NVIC_InitTypeDef nvic_initStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	//可以从启动文件中找到
	nvic_initStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;

	nvic_initStructure.NVIC_IRQChannelCmd = ENABLE;

	nvic_initStructure.NVIC_IRQChannelPreemptionPriority = 0;

	nvic_initStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&nvic_initStructure);

}


void CAN_MODE_INIT(void)
{
	CAN_InitTypeDef can_initStructure;

	//CAN_DeInit(CAN1);
}











































