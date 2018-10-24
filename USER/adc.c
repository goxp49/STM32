/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��adc.c
 * ����    ��ADCӦ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/
#include "stm32f10x.h"
#include "adc.h"

__IO u16 ADC_ConvertedValue=0;

void ADC1_DMA_Config(void)
{
	DMA_InitTypeDef dma_initStructure;
	ADC_InitTypeDef adc_initStructure;
	GPIO_InitTypeDef gpio_initStructure;

	/*  ������GPIO�趨 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	gpio_initStructure.GPIO_Mode = GPIO_Mode_AIN;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_0;

	GPIO_Init(GPIOA, &gpio_initStructure);

	/*  ����ADC����ģʽ  */

	/*  DMA�趨 */
	DMA_DeInit(DMA1_Channel1);

	/*  ����Ϊ����Դ */
	dma_initStructure.DMA_PeripheralBaseAddr = ADC1_DR_BASE;
	dma_initStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_initStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

	dma_initStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
	dma_initStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_initStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

	dma_initStructure.DMA_M2M = DMA_M2M_Disable;
	dma_initStructure.DMA_Mode = DMA_Mode_Circular;
	dma_initStructure.DMA_Priority = DMA_Priority_High;
	dma_initStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma_initStructure.DMA_BufferSize = 1;

	DMA_Init(DMA1_Channel1, &dma_initStructure);

	/*  ADC���� */
	//��������ת��
	adc_initStructure.ADC_ContinuousConvMode = ENABLE;
	adc_initStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// ��ʹ���ⲿ����ģʽ
	adc_initStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//����ADCģʽ��ÿ��ADC��������
	adc_initStructure.ADC_Mode = ADC_Mode_Independent;
	//ת����ͨ������
	adc_initStructure.ADC_NbrOfChannel = 1;
	//  ��ֹɨ��ģʽ�����ڶ�ͨ�� ����ʹ���ⲿ����ɨ��
	adc_initStructure.ADC_ScanConvMode = DISABLE;

	ADC_Init(ADC1,&adc_initStructure);

	//����ADCʱ��,ADCLCK����С��14MHz������72/8=9Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	//����ADC����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	/*  У׼ADC */
	//��λ
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));//�����崻�

	//У׼
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	/*  ͨ�������������ADC */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

void ADC1_IT_Config(void)
{
	ADC_InitTypeDef adc_initStructure;
	GPIO_InitTypeDef gpio_initStructure;
	NVIC_InitTypeDef nvic_initStructure;

	/* �������ȼ����� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* �������ȼ� */
	nvic_initStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic_initStructure.NVIC_IRQChannelCmd = ENABLE;
	nvic_initStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initStructure.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&nvic_initStructure);

	/*  ������GPIO�趨 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	gpio_initStructure.GPIO_Mode = GPIO_Mode_AIN;

	gpio_initStructure.GPIO_Pin = GPIO_Pin_1;

	GPIO_Init(GPIOA, &gpio_initStructure);

	/*  ����ADC����ģʽ  */

	/*  ADC���� */
	//��������ת��
	adc_initStructure.ADC_ContinuousConvMode = ENABLE;
	adc_initStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// ��ʹ���ⲿ����ģʽ
	adc_initStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//����ADCģʽ��ÿ��ADC��������
	adc_initStructure.ADC_Mode = ADC_Mode_Independent;
	//ת����ͨ������
	adc_initStructure.ADC_NbrOfChannel = 1;
	//  ��ֹɨ��ģʽ�����ڶ�ͨ�� ����ʹ���ⲿ����ɨ��
	adc_initStructure.ADC_ScanConvMode = DISABLE;

	ADC_Init(ADC1,&adc_initStructure);

	//����ADCʱ��,ADCLCK����С��14MHz������72/8=9Mhz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//����ADC����ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);

	// �ж��趨
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	ADC_Cmd(ADC1, ENABLE);

	/*  У׼ADC */
	//��λ
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));//protues�����崻�

	//У׼
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));//protues�����崻�

	/*  ͨ�������������ADC */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

