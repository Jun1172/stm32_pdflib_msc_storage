/*
*@ 用途：应用程序
*@ 时间：20190708
*@ 作者：JUN
*/
#ifndef __PROCESS_H__
#define __PROCESS_H__

/*包含头文件*/
#include "stm32f1xx_hal.h"

/*引用头文件*/
#include "fatfs.h"
#include "w25qxx.h"
#include "pdflib.h"
#include "usb_device.h"

#define USB_PULLUP_Pin  GPIO_PIN_12
#define USB_PULLUP_Port GPIOB

/*
 @函数功能：初始化
 @输入变量：无
 @输出变量：无
*/
void processInit(void);

/*
 @函数功能：应用进程
 @输入变量：无
 @输出变量：无
*/
void runProcess(void);
	
#endif /* __STMFLASH_H__ */
