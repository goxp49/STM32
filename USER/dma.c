/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：dma.c
 * 描述    ：MDA应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
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

	// 用默认值重置DMA1
	DMA_DeInit(DMA1_Channel4);

	/* 设置缓存器长度 */
	dma_initStructure.DMA_BufferSize = SEND_BUFF_LENGTH;

	/*  设置传输数据大小 */
	dma_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*  设置数据传输方向: 内存 -> 外设  */
	dma_initStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/* 禁止内存 -->  内存传输  */
	dma_initStructure.DMA_M2M = DMA_M2M_Disable;

	/* 设置内存地址与外设地址  */
	dma_initStructure.DMA_MemoryBaseAddr = (u32)SendBuff;
	dma_initStructure.DMA_PeripheralBaseAddr = USART1_DR_BASE;

	/*  内存地址每次读取时自动加一,外设不变 */
	dma_initStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*  普通模式，只执行一次  */
	dma_initStructure.DMA_Mode = DMA_Mode_Normal;

	/*  设置优先级 */
	dma_initStructure.DMA_Priority = DMA_Priority_Medium;

	DMA_Init(DMA1_Channel4, &dma_initStructure);

	DMA_Cmd(DMA1_Channel4, ENABLE);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

}
