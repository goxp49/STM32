#ifndef __SPI_H
#define __SPI_H


#include "stm32f10x.h"

#define LOW								(0)
#define HIGH							(1)

/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      		256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WRITE_ENABLE		      	0x06
#define W25X_WRITE_DISABLE		      	0x04
#define W25X_READ_STATUS		    	0x05
#define W25X_WRITE_STATUS		    	0x01
#define W25X_Read_DATA			        0x03
#define W25X_FAST_READ_DATA		      	0x0B
#define W25X_FAST_READ_DUAL		      	0x3B
#define W25X_PAGE_PROGRAM		      	0x02
#define W25X_BLOCK_ERASE		      	0xD8
#define W25X_SECTOR_ERASE		      	0x20
#define W25X_CHIP_ERASE			      	0xC7
#define W25X_POWERDOWN			      	0xB9
#define W25X_RELEASE_POWERDOWN	    	0xAB
#define W25X_DEVICE_ID			        0xAB
#define W25X_M_DEVICE_ID   				0x90
#define W25X_J_DeviceID		    		0x9F


#define WIP_FLAG                  		0x01  /* Write In Progress (WIP) flag */
#define DUMMY_BYTE						0x00


void SPI_Config(void);
u8 FLASH_SendByte(u8 data);
u32 FLASH_ReadDriverID(void);
void FLASH_CSS_Select(u8 Status);
void FLASH_WriteEnable(void);
void FLASH_WaitWriteEnd(void);



#endif
