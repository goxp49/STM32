#ifndef __IIC_EEPROM_H
#define __IIC_EEPROM_H


#include "stm32f10x.h"


#define I2C_SPEED				400000
#define I2C_PageSize           	8			/* AT24C02每页有8个字节 */
#define EEPROM_ADDRESS			0xA0



void I2C_EEPROM_Config(void);
void I2C_GPIO_Config(void);
void I2C_MODE_Config(void);
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);










#endif


