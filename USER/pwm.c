/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：pwm.c
 * 描述    ：滴答定时器应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "pwm.h"


void PWM_Config(void)
{
	PWM_GPIO_Config();

	/*  配置PWM设定 */
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

	//设定跳变阈值
	u16 CCR1_Val = 900;
	u16 CCR2_Val = 500;
	u16 CCR3_Val = 300;
	u16 CCR4_Val = 100;

	/* TIME BASE 设定 */

	//设定计数最大值
	tim_baseInitSturcture.TIM_Period = 999;
	//设置预分频为0
	tim_baseInitSturcture.TIM_Prescaler = 0;
	//设定不分频(72 mhz)
	tim_baseInitSturcture.TIM_ClockDivision = TIM_CKD_DIV1;
	//向上计数方式
	tim_baseInitSturcture.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim_baseInitSturcture);

	/* Chanel1 设定 */
	tim_ocInitSturcture.TIM_OCMode = TIM_OCMode_PWM1;
	//设置使能输出
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//设置跳变阈值
	tim_ocInitSturcture.TIM_Pulse = CCR1_Val;
	//当计数小于阈值时为高电平
	tim_ocInitSturcture.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &tim_ocInitSturcture);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel2 设定 */
	//设置使能输出
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//设置跳变阈值
	tim_ocInitSturcture.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM2, &tim_ocInitSturcture);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel3 设定 */
	//设置使能输出
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//设置跳变阈值
	tim_ocInitSturcture.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM2, &tim_ocInitSturcture);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* Chanel4 设定 */
	//设置使能输出
	tim_ocInitSturcture.TIM_OutputState = TIM_OutputState_Enable;
	//设置跳变阈值
	tim_ocInitSturcture.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(TIM2, &tim_ocInitSturcture);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	//使能TIM2重载寄存器
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//使能TIM2
	TIM_Cmd(TIM2, ENABLE);

}

