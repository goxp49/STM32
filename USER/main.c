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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#include "dma.h"
#include "led.h"
#include "usart.h"


extern u8 SendBuff[SEND_BUFF_LENGTH];

void dma2usart1(void);

int main(void)
{
	dma2usart1();

}


void dma2usart1(void)
{
	u32 i;

	USART1_Config();
	DMA_Config();
	LED_GPIO_Config();

	for(i=0;i<SEND_BUFF_LENGTH;i++)
	{
		SendBuff[i] = 0xff;
	}

	/*  DMA开始向USART发送数据 */
	USART_DMACmd( USART1, USART_DMAReq_Tx, ENABLE);

	LED1(ON);

	while(1);
}
