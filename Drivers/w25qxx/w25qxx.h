/*
*@ 用途：Flash驱动
*@ 时间：20190310
*@ 作者：JUN
*/
#ifndef _W25QXX_H_
#define _W25QXX_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define W25Q64_CS_Pin		GPIO_PIN_4
#define W25Q64_CS_GPIO_Port GPIOA
#define SPI_CLK_Pin 		GPIO_PIN_5
#define SPI_CLK_GPIO_Port 	GPIOA
#define SPI_MISO_Pin 		GPIO_PIN_6
#define SPI_MISO_GPIO_Port 	GPIOA
#define SPI_MOSI_Pin 		GPIO_PIN_7
#define SPI_MOSI_GPIO_Port 	GPIOA

#define FLASH_SECTOR_SIZE      512
#define FLASH_BLOCK_SIZE       1 
#define FLASH_SECTOR_COUNT   (uint32_t)((1*1024*1024)/FLASH_SECTOR_SIZE)
	 
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1  	0x01 
#define W25X_WriteStatusReg2  	0x31 
#define W25X_WriteStatusReg3  	0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr  	0xB7
#define W25X_Exit4ByteAddr    	0xE9
/* USER CODE END Includes */

//定义Flash配置
#define ConfigSpace 4*1024  //配置文件空间大小

//定义Flash空间表
#define W25Q80SPACE  (((8*1024*1024)>>3)  - ConfigSpace)
#define W25Q16SPACE  (((16*1024*1024)>>3) - ConfigSpace)
#define W25Q32SPACE  (((32*1024*1024)>>3) - ConfigSpace)
#define W25Q64SPACE  (((64*1024*1024)>>3) - ConfigSpace)
#define W25Q128SPACE (((128*1024*1024)>>3) - ConfigSpace)
#define W25Q256SPACE (((256*1024*1024)>>3) - ConfigSpace)

extern const uint32_t FlashSpace[6];
extern uint16_t FlashType;
extern uint32_t ConfigStartAddr;
extern void _Error_Handler(char *, int);

void MX_SPI1_Init(void);
uint32_t HAL_W25QXX_ReadID(void);
uint8_t HAL_W25QXX_Read_SR(uint8_t regno);
void HAL_W25QXX_Write_SR(uint8_t regno,uint8_t sr);
void HAL_W25QXX_Write_Enable(void);
void HAL_W25QXX_Write_Disable(void);
uint8_t HAL_W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
uint8_t HAL_W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void HAL_W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void HAL_W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);   
void HAL_W25QXX_Erase_Chip(void);
void HAL_W25QXX_Erase_Sector(uint32_t Dst_Addr);
void HAL_W25QXX_PowerDown(void);
void HAL_W25QXX_WAKEUP(void);

uint8_t W25qxxInit(void);
	
#endif

