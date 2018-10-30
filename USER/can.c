/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：can.c
 * 描述    ：CAN应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "can.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;


void CAN_Config(void)
{
	CAN_GPIO_INIT();
	CAN_NVIC_INIT();
	CAN_MODE_INIT();
	CAN_FILTER_INIT();
}


void CAN_GPIO_INIT(void)
{
	GPIO_InitTypeDef gpio_initStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// RX --> 11 , TX --> 12;
	gpio_initStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_12;

	gpio_initStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &gpio_initStructure);

	gpio_initStructure.GPIO_Mode = GPIO_Mode_IPU;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_11;

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

	/*  初始化寄存器 */
	CAN_DeInit(CAN1);
	CAN_StructInit(&can_initStructure);

	//自动退出离线状态功能
	can_initStructure.CAN_ABOM = ENABLE;

	//自动唤醒状态功能
	can_initStructure.CAN_AWUM = ENABLE;

	can_initStructure.CAN_BS1 = CAN_BS1_6tq;

	can_initStructure.CAN_BS2 = CAN_BS1_3tq;

	//can_initStructure.CAN_Mode = CAN_Mode_Normal;
	can_initStructure.CAN_Mode = CAN_Mode_LoopBack;

	can_initStructure.CAN_NART = DISABLE;

	can_initStructure.CAN_Prescaler = 4;

	can_initStructure.CAN_RFLM = DISABLE;

	can_initStructure.CAN_SJW = CAN_SJW_2tq;

	can_initStructure.CAN_TTCM = DISABLE;

	can_initStructure.CAN_TXFP = DISABLE;

	CAN_Init(CAN1, &can_initStructure);

}


void CAN_FILTER_INIT(void)
{

	CAN_FilterInitTypeDef filter_initStruture;

	filter_initStruture.CAN_FilterActivation = ENABLE;

	filter_initStruture.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;

	//要过滤的ID
	filter_initStruture.CAN_FilterIdHigh = 0xFFFF;
	filter_initStruture.CAN_FilterIdLow = 0xFFFF;

	//选择哪些ID位必须匹配(1为必须)
	filter_initStruture.CAN_FilterMaskIdHigh = 0x0000;
	filter_initStruture.CAN_FilterMaskIdLow= 0x0000;

	filter_initStruture.CAN_FilterMode = CAN_FilterMode_IdMask;

	filter_initStruture.CAN_FilterNumber = 0;

	filter_initStruture.CAN_FilterScale = CAN_FilterScale_32bit;

	CAN_FilterInit(&filter_initStruture);

	//使能过滤器中断
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}


void CAN_SendMessage(void)
{
	u8 TransmitMailbox=0;

	TxMessage.StdId=0x11;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.DLC=2;
	TxMessage.Data[0]=0xFF;
	TxMessage.Data[1]=0x00;

	TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);

	while(CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK);
}






































