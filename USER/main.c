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

#include "iic_eeprom.h"

#define  EEP_Firstpage      0x00
u8 I2c_Buf_Write[256];
u8 I2c_Buf_Read[256];


void I2C_EEPROM(void);

int main(void)
{
	I2C_EEPROM();

}

#if(0)

#include "led.h"
#include "usart.h"
/*  ͨ�����ڽ����ݷ��ͳ�ȥ */
void Usart1(void)
{

	USART1_Config();
	LED_GPIO_Config();

	LED2(ON);

	while(1)
	{
		USART_SendMessage('w');
		delay_ms(0xff);
	};
}
#endif

#if(0)

#include "dma.h"
#include "usart.h"
extern u8 SendBuff[SEND_BUFF_LENGTH];

/*  ͨ��DMA�������е����ݷ��͵�USART�з��� */
void DMA2Usart1(void)
{
	u32 i;

	for(i=0;i<SEND_BUFF_LENGTH;i++)
	{
		SendBuff[i] = 'w';
	}

	USART1_Config();
	DMA_Config();

	/*  DMA��ʼ��USART�������� */
	USART_DMACmd( USART1, USART_DMAReq_Tx, ENABLE);

	while(1);
}
#endif

/*****************************************************************  
*
*			ͨ��DMA��ADC�ɼ�������͵�USART1�� 
*
*****************************************************************/
#if(0)
#include "adc.h"
#include "led.h"

extern __IO u16 ADC_ConvertedValue;
float ADC_Result;

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
#endif

/*****************************************************************  
*
*			��֤systick��ʱ�Ƿ�ɹ�
*
*****************************************************************/

#if(0)
#include "systick.h"
#include "led.h"

void Systick(void)
{
	//���ö�ʱ��ÿ���ж�ʱ��Ϊ1ms
	Systick_Config(SYSTICK_DELAY_1MS);

	LED2(ON);

	while(1)
	{
		LED1_Switch();
		Delay_SysTick(500);
	}
}

#endif


/*****************************************************************  
*
*			��֤timer��ΪPWM����Ƿ�ɹ�
*
*****************************************************************/

#if(0)

#include "pwm.h"

void timer_pwm(void)
{
	PWM_Config();
	while(1);
}

#endif

/*****************************************************************  
*
*			��֤I2Cͨ���Ƿ�ɹ�
*
*****************************************************************/

void I2C_EEPROM(void)
{
	u16 i;

	I2C_EEPROM_Config();

    
	for ( i=0; i<=255; i++ ) //��仺��
  	{   
    	I2c_Buf_Write[i] = i;

   	}

  	//��I2c_Buf_Write��˳�����������д��EERPOM�� 
	//I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);	 
  
  	//��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read�� 
	//I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 

	

	for ( i=0; i<=255; i++ )
	{
		I2C_EE_ByteWrite(&I2c_Buf_Write[i], EEPROM_ADDRESS);

	};
	
}
