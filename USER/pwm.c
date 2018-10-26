/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��pwm.c
 * ����    ���δ�ʱ��Ӧ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/

#include "pwm.h"


void PWM_Config(void)
{
	PWM_GPIO_Config();

	/*  ����PWM�趨 */
	PWM_Mode_Config();
}


void PWM_GPIO_Config(void)
{
	GPIO_InitTypeDef gpio_initSturcture;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_PP;

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;

	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &gpio_initSturcture);
}


void PWM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef tim_baseInitSturcture;
	TIM_OCInitTypeDef tim_ocInitSturcture;

	//�趨������ֵ
	u16 CCR1_Val = 900;
	u16 CCR2_Val = 500;
	u16 CCR3_Val = 300;
	u16 CCR4_Val = 100;

	/* TIME BASE �趨 */

	//�趨�������ֵ
	tim_baseInitSturcture.TIM_Period = 999;
	//����Ԥ��ƵΪ0
	tim_baseInitSturcture.TIM_Prescaler = 0;
	//�趨����Ƶ(72 mhz)
	tim_baseInitSturcture.TIM_ClockDivision = TIM_CKD_DIV1;
	//���ϼ�����ʽ
	tim_baseInitSturcture.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim_baseInitSturcture);

	/* Chanel1 �趨 */
	tim_ocInitSturcture.TIM_OCMode = TIM_OCMode_PWM1;
	//����ʹ�����
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//����������ֵ
	tim_ocInitSturcture.TIM_Pulse = CCR1_Val;
	//������С����ֵʱΪ�ߵ�ƽ
	tim_ocInitSturcture.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &tim_ocInitSturcture);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel2 �趨 */
	//����ʹ�����
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//����������ֵ
	tim_ocInitSturcture.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM2, &tim_ocInitSturcture);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel3 �趨 */
	//����ʹ�����
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//����������ֵ
	tim_ocInitSturcture.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM2, &tim_ocInitSturcture);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel4 �趨 */
	//����ʹ�����
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//����������ֵ
	tim_ocInitSturcture.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(TIM2, &tim_ocInitSturcture);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	//ʹ��TIM2���ؼĴ���
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//ʹ��TIM2
	TIM_Cmd(TIM2, ENABLE);

}

