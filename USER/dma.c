/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��dma.c
 * ����    ��MDAӦ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/
#include "dma.h"

u8 SendBuff[SEND_BUFF_LENGTH];

void DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef nvic_initStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	nvic_initStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	nvic_initStructure.NVIC_IRQChannelCmd = ENABLE;
	nvic_initStructure.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_initStructure);
}

void DMA_Config(void)
{
	DMA_InitTypeDef dma_initStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_NVIC_Config();

	// ��Ĭ��ֵ����DMA1
	DMA_DeInit(DMA1_Channel4);

	/* ���û��������� */
	dma_initStructure.DMA_BufferSize = SEND_BUFF_LENGTH;

	/*  ���ô������ݴ�С */
	dma_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*  �������ݴ��䷽��: �ڴ� -> ����  */
	dma_initStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/* ��ֹ�ڴ� -->  �ڴ洫��  */
	dma_initStructure.DMA_M2M = DMA_M2M_Disable;

	/* �����ڴ��ַ�������ַ  */
	dma_initStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
	dma_initStructure.DMA_PeripheralBaseAddr = USART1_DR_BASE;

	/*  �ڴ��ַÿ�ζ�ȡʱ�Զ���һ,���費�� */
	dma_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*  ��ͨģʽ��ִֻ��һ��  */
	dma_initStructure.DMA_Mode = DMA_Mode_Normal;

	/*  �������ȼ� */
	dma_initStructure.DMA_Priority = DMA_Priority_Medium;

	DMA_Init(DMA1_Channel4, &dma_initStructure);

	DMA_Cmd(DMA1_Channel4, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

}
