/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：usart.c
 * 描述    ：串口应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/
#include "stm32f10x.h"
#include <stdio.h>


void USART1_Config(void)
{
	GPIO_InitTypeDef gpio_initSturcture;
	USART_InitTypeDef usart_initSturcture;

	/* 使能时钟,USART1复用在PA8 ~ PA12 ,TX ->9, RX->10*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);

	/* 配置TX设定 */
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initSturcture.GPIO_Pin = GPIO_Pin_9;
	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initSturcture);

	/* 配置RX设定 */
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initSturcture.GPIO_Pin = GPIO_Pin_10;
	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initSturcture);

	/* USART1设定 */
	usart_initSturcture.USART_BaudRate = 9600;
	usart_initSturcture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_initSturcture.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	usart_initSturcture.USART_Parity = USART_Parity_No;
	usart_initSturcture.USART_StopBits = USART_StopBits_1;
	usart_initSturcture.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart_initSturcture);

	/* 使能USART */
	USART_Cmd(USART1, ENABLE);

}


int fputc(int ch, FILE *f)
{
	USART_SendData(USART1,(unsigned char)ch);
	/* 检查是否发送完毕 */
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	return ch;
}
