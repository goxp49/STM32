/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��systick.c
 * ����    ���δ�ʱ��Ӧ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/

#include "systick.h"

void Systick_Config(uint32_t ticks)
{
	
	//��鶨ʱ�������Ƿ�����
	while(SysTick_Config(ticks))
	{
		LED3(ON);
	}

	//�رն�ʱ������ʼ��ʱ��Ĭ�ϴ�
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}



