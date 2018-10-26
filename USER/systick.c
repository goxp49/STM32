/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��systick.c
 * ����    ���δ�ʱ��Ӧ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/

#include "systick.h"
#include "led.h"

u32 TimingDelay = 0;

void Systick_Config(uint32_t ticks)
{
	LED_GPIO_Config();

	//��鶨ʱ�������Ƿ�����
	while(SysTick_Config(ticks))
	{
		LED3(ON);
	}

	//�رն�ʱ������ʼ��ʱ��Ĭ�ϴ�
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void Delay_SysTick(uint32_t ntime)
{
	//���ж�ʱ�����Ƿ����0
	if(ntime != 0)
	{
		TimingDelay = ntime;

		//����������
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

		while(TimingDelay);
	}
}

