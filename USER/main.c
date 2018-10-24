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
#include "delay.h"
#include <stdio.h>

#include "adc.h"
#include "led.h"

extern __IO u16 ADC_ConvertedValue;
float ADC_Result;



void ADCAndDMA(void);

int main(void)
{
	ADCAndDMA();

}


#if(0)

#include "dma.h"
#include "led.h"
#include "usart.h"
extern u8 SendBuff[SEND_BUFF_LENGTH];

void DMA2Usart1(void)
{
	u32 i;

	USART1_Config();
	DMA_Config();
	LED_GPIO_Config();

	for(i=0;i<SEND_BUFF_LENGTH;i++)
	{
		SendBuff[i] = 0xff;
	}

	/*  DMA��ʼ��USART�������� */
	USART_DMACmd( USART1, USART_DMAReq_Tx, ENABLE);

	LED1(ON);

	while(1);
}
#endif

/*  ͨ��DMA��ADC�ɼ�������͵�USART1�� */
void ADCAndDMA(void)
{
	ADC1_IT_Config();
	LED_GPIO_Config();
	LED3(ON);

	while(1)
	{
		ADC_Result = (float)ADC_ConvertedValue/4096*3.3;
		if(ADC_Result > 1.65)
		{
			LED1(ON);
		}else{
			LED1(OFF);
		}
	}
}
