/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��iic_eeprom.c
 * ����    ���δ�ʱ��Ӧ�ú�����

 * ��汾  ��ST3.5.0
 *
 * ����    ��goxp49
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

	gpio_initSturcture.GPIO_Mode = GPIO_Mode_AF_OD;	//ע��IICһ��Ϊ��©���

	gpio_initSturcture.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

	gpio_initSturcture.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &gpio_initSturcture);
	
}


void I2C_MODE_Config(void)
{
	I2C_InitTypeDef i2c_initStructure;

	i2c_initStructure.I2C_Ack = I2C_Ack_Enable;
	//24c02 EEPROMֻ�и���λ�ǵ�ַ�����λ����������λ
	i2c_initStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//�Զ��崫������
	i2c_initStructure.I2C_ClockSpeed = I2C_SPEED;
	//����SCL�ߵ�ռ�ձȣ���ռ�ձȿ�������ݴ���
	i2c_initStructure.I2C_DutyCycle = I2C_DutyCycle_2;

	i2c_initStructure.I2C_Mode = I2C_Mode_I2C;
	//�����Զ���STM32��IIC��ַ
	i2c_initStructure.I2C_OwnAddress1 = 0x0B;

	I2C_Init(I2C1, &i2c_initStructure);

	I2C_Cmd(I2C1, ENABLE);
	
}


/*
 * ��������I2C_EE_BufferWrite
 * ����  �����������е�����д��I2C EEPROM��
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ
 *         -NumByteToWrite Ҫд��EEPROM���ֽ���
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
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
 * ��������I2C_EE_ByteWrite
 * ����  ��дһ���ֽڵ�I2C EEPROM��
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ 
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
	//��ʼIIC����
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	
	//���淢������Ῠ��!!!!!!!!!!����˵����bug����Ҫ+DMA+������ȼ��жϽ��!
	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	//�ȷ���Ҫд��EEPROM��ַ
	I2C_SendData(I2C1, WriteAddr);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);

	//����Ҫ���������
	I2C_SendData(I2C1, *pBuffer);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);

	I2C_GenerateSTOP(I2C1, ENABLE);
	
}


/*
 * ��������I2C_EE_PageWrite
 * ����  ����EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
 *         ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽڡ�
 * ����  ��-pBuffer ������ָ��
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ 
 *         -NumByteToWrite Ҫд��EEPROM���ֽ���
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
 
 void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	//��ʼIIC����
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	//�ȷ���Ҫд��EEPROM��ַ
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
 * ��������I2C_EE_BufferRead
 * ����  ����EEPROM�����ȡһ�����ݡ� 
 * ����  ��-pBuffer ��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ�롣
 *         -WriteAddr �������ݵ�EEPROM�ĵ�ַ�� 
 *         -NumByteToWrite Ҫ��EEPROM��ȡ���ֽ�����
 * ���  ����
 * ����  ����
 * ����  ���ⲿ����
 */
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	//��ʼIIC����
	I2C_GenerateSTART(I2C1, ENABLE);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);

	I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);

	//�������EV6��־λ
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
 * ��������I2C_EE_WaitEepromStandbyState
 * ����  ��Wait for EEPROM Standby state 
 * ����  ����
 * ���  ����
 * ����  ����
 * ����  �� 
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










