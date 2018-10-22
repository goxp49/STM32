/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
 */
#include "exti.h"

void NVIC_Config(void)
{
	NVIC_InitTypeDef nvic_initStructure;

	/* 设置优先级分组 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 设置优先级 */
	nvic_initStructure.NVIC_IRQChannel = EXTI0_IRQn;
	nvic_initStructure.NVIC_IRQChannelCmd = ENABLE;
	nvic_initStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&nvic_initStructure);
}


void EXTI_Config(void)
{
	GPIO_InitTypeDef gpio_initStructure;
	EXTI_InitTypeDef exti_initStructure;

	/* 使能外部时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	/* 设置 NVIC */
	NVIC_Config();

	/* 设置GPIO设定 */
	gpio_initStructure.GPIO_Pin = GPIO_Pin_0;
	gpio_initStructure.GPIO_Mode = GPIO_Mode_IPU;
	gpio_initStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initStructure);

	/* 中断设定 */
	exti_initStructure.EXTI_Line = EXTI_Line0;
	exti_initStructure.EXTI_LineCmd = ENABLE;
	exti_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&exti_initStructure);
	
}
