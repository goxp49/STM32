/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��spi.c
 * ����    ��SPIӦ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
**********************************************************************************/

#include "spi.h"


void SPI_Config(void)
{
	GPIO_InitTypeDef gpio_initSturcture;
	SPI_InitTypeDef spi_initStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);

	/*   ������GPIO�趨 */
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_PP;

	// 5 --> CLK, 6 --> MISO, 7 --> MOSI
	gpio_initSturcture.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;

	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_6;

	gpio_initSturcture.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	// ����CS Pin
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_Out_PP;

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_4;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	/*  ��ʼ���ر�SPIƬѡ  */
	FLASH_CSS_Select(HIGH);

	/*   ����SPI�趨 */
	spi_initStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;

	spi_initStructure.SPI_CPHA = SPI_CPHA_2Edge;

	spi_initStructure.SPI_CPOL = SPI_CPOL_High;

	spi_initStructure.SPI_CRCPolynomial = 7;

	spi_initStructure.SPI_DataSize = SPI_DataSize_8b;

	spi_initStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	spi_initStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	spi_initStructure.SPI_Mode = SPI_Mode_Master;

	spi_initStructure.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1, &spi_initStructure);
	SPI_Cmd(SPI1, ENABLE);

}


u8 FLASH_SendByte(u8 data)
{
	//���DR�Ĵ����Ƿ�Ϊ�գ��Ƿ��ѷ������
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	//��������
	SPI_I2S_SendData(SPI1, data);

	//�ȴ������ܴӻ�����
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	return SPI_I2S_ReceiveData(SPI1);
}

u32 FLASH_ReadDriverID(void)
{
	u32 temp = 0;
	//ʹ��FLASHƬѡ
	FLASH_CSS_Select(LOW);
	//���Ͷ�ȡID CMD
	FLASH_SendByte(W25X_DEVICE_ID);
	FLASH_SendByte(DUMMY_BYTE);
	FLASH_SendByte(DUMMY_BYTE);
	FLASH_SendByte(DUMMY_BYTE);
	//��FLASH��ȡ����
	temp = FLASH_SendByte(DUMMY_BYTE);
	FLASH_CSS_Select(HIGH);
	return temp;
}

void FLASH_CSS_Select(u8 Status)
{
	if(Status)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
}


void FLASH_WriteEnable(void)
{
	FLASH_CSS_Select(LOW);

	FLASH_SendByte(W25X_WRITE_ENABLE);

	FLASH_CSS_Select(HIGH);
}

void FLASH_WaitWriteEnd(void)
{
	u8 FLASH_Status=0;

	FLASH_CSS_Select(LOW);

	FLASH_SendByte(W25X_READ_STATUS);

	do
	{
		FLASH_Status = FLASH_SendByte(DUMMY_BYTE);
	}
	while((FLASH_Status & WIP_FLAG) == SET);

	FLASH_CSS_Select(HIGH);

}











































