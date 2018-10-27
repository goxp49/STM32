/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：iic_eeprom.c
 * 描述    ：滴答定时器应用函数库

 * 库版本  ：ST3.5.0
 *
 * 作者    ：goxp49
**********************************************************************************/

#include "iic_eeprom.h"


void I2C_EEPROM_Config(void)
{
	I2C_GPIO_Config();

	I2C_MODE_Config();
	
}


void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef gpio_initSturcture;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_OD;	//注意IIC一般为开漏输出

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &gpio_initSturcture);
	
}


void I2C_MODE_Config(void)
{
	I2C_InitTypeDef i2c_initStructure;

	i2c_initStructure.I2C_Ack = I2C_Ack_Enable;
	//24c02 EEPROM只有高七位是地址，最低位是数据流向位
	i2c_initStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//自定义传输速率
	i2c_initStructure.I2C_ClockSpeed = I2C_SPEED;
	//设置SCL线的占空比，高占空比可以提高容错率
	i2c_initStructure.I2C_DutyCycle = I2C_DutyCycle_2;

	i2c_initStructure.I2C_Mode = I2C_Mode_I2C;
	//可以自定义STM32的IIC地址
	i2c_initStructure.I2C_OwnAddress1 = 0x0B;

	I2C_Init(I2C1, &i2c_initStructure);

	I2C_Cmd(I2C1, ENABLE);
	
}


/*
 * 函数名：I2C_EE_BufferWrite
 * 描述  ：将缓冲区中的数据写到I2C EEPROM中
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址
 *         -NumByteToWrite 要写入EEPROM的字节数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/*
 * 函数名：I2C_EE_ByteWrite
 * 描述  ：写一个字节到I2C EEPROM中
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
	//开始IIC传输
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	
	//仿真发现在这会卡主!!!!!!!!!!网上说这是bug，需要+DMA+最高优先级中断解决!
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	//先发送要写的EEPROM地址
	I2C_SendData(I2C1, WriteAddr);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);

	//发送要传输的数据
	I2C_SendData(I2C1, *pBuffer);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);

	I2C_GenerateSTOP(I2C1, ENABLE);
	
}


/*
 * 函数名：I2C_EE_PageWrite
 * 描述  ：在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
 *         不能超过EEPROM页的大小。AT24C02每页有8个字节。
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址 
 *         -NumByteToWrite 要写入EEPROM的字节数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
 
 void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	//开始IIC传输
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	//先发送要写的EEPROM地址
	I2C_SendData(I2C1, WriteAddr);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);

	/* While there is data to be written */
  	while(NumByteToWrite--)  
  	{
    	/* Send the current byte */
    	I2C_SendData(I2C1, *pBuffer); 

    	/* Point to the next byte to be written */
    	pBuffer++; 
  
    	/* Test on EV8 and clear it */
    	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  	}

  	/* Send STOP condition */
  	I2C_GenerateSTOP(I2C1, ENABLE);
	
}


/*
 * 函数名：I2C_EE_BufferRead
 * 描述  ：从EEPROM里面读取一块数据。 
 * 输入  ：-pBuffer 存放从EEPROM读取的数据的缓冲区指针。
 *         -WriteAddr 接收数据的EEPROM的地址。 
 *         -NumByteToWrite 要从EEPROM读取的字节数。
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	//开始IIC传输
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);

	//用于清除EV6标志位
	I2C_Cmd(I2C1, ENABLE);

  	I2C_SendData(I2C1, ReadAddr);  

  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2C1, ENABLE);

	 /* Test on EV5 and clear it */
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  	/* Send EEPROM address for read */
  	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  	/* Test on EV6 and clear it */
  	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  	/* While there is data to be read */
  	while(NumByteToRead)  
  	{
    	if(NumByteToRead == 1)
    	{
	      	/* Disable Acknowledgement */
	      	I2C_AcknowledgeConfig(I2C1, DISABLE);
	      
	      	/* Send STOP Condition */
	      	I2C_GenerateSTOP(I2C1, ENABLE);
    	}

    	/* Test on EV7 and clear it */
    	if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    	{      
      		/* Read a byte from the EEPROM */
      		*pBuffer = I2C_ReceiveData(I2C1);

      		/* Point to the next location where the byte read will be saved */
      		pBuffer++; 
      
      		/* Decrement the read bytes counter */
      		NumByteToRead--;        
    	}   
  	}

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}


/*
 * 函数名：I2C_EE_WaitEepromStandbyState
 * 描述  ：Wait for EEPROM Standby state 
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 * 调用  ： 
 */

 void I2C_EE_WaitEepromStandbyState(void)
{
	vu16 SR1_Tmp = 0;
	
	do
	{
	  /* Send START condition */
	  I2C_GenerateSTART(I2C1, ENABLE);
	  /* Read I2C1 SR1 register */
	  SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
	  /* Send EEPROM address for write */
	  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	}while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
	
	/* Clear AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	/* STOP condition */	  
	I2C_GenerateSTOP(I2C1, ENABLE); 

}










