/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：spi.c
 * 描述    ：SPI应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "spi.h"


void SPI_Config(void)
{
	GPIO_InitTypeDef gpio_initSturcture;
	SPI_InitTypeDef spi_initStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE);

	/*   先设置GPIO设定 */
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_PP;

	// 5 --> CLK, 6 --> MISO, 7 --> MOSI
	gpio_initSturcture.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;

	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_6;

	gpio_initSturcture.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	// 设置CS Pin
	gpio_initSturcture.GPIO_Mode = GPIO_Mode_Out_PP;

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_4;

	GPIO_Init(GPIOA,&gpio_initSturcture);

	/*  初始化关闭SPI片选  */
	FLASH_CSS_Select(HIGH);

	/*   设置SPI设定 */
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
	//检查DR寄存器是否为空，是否已发送完毕
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	//发送数据
	SPI_I2S_SendData(SPI1, data);

	//等待并接受从机数据
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	return SPI_I2S_ReceiveData(SPI1);
}

u32 FLASH_ReadDriverID(void)
{
	u32 temp = 0;
	//使能FLASH片选
	FLASH_CSS_Select(LOW);
	//发送读取ID CMD
	FLASH_SendByte(W25X_DEVICE_ID);
	FLASH_SendByte(DUMMY_BYTE);
	FLASH_SendByte(DUMMY_BYTE);
	FLASH_SendByte(DUMMY_BYTE);
	//从FLASH读取数据
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











































