/*
*@ 用途：应用程序
*@ 时间：20190708
*@ 作者：JUN
*/
#include "process.h"

#include "demo.h"

FATFS fatfs;         // Work area (file system object) for logical drive
FIL fsrc;            // file objects
UINT br;

//测试例程
#define NO_USER						0
#define AUTHOR_DEMO_PROCESS			1
#define BLANK_PAGE_DEMO_PROCESS		2
#define HELLOWORLD_DEMO_PROCESS		3
#define CHINESE_DEMO_PROCESS		4

uint8_t processCount = CHINESE_DEMO_PROCESS;

/*
 @函数功能：初始化
 @输入变量：无
 @输出变量：无
*/
void processInit(void)
{
	//启动文件系统
	MX_FATFS_Init();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USB_PULLUP_Port, USB_PULLUP_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin*/
	GPIO_InitStruct.Pin = USB_PULLUP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(USB_PULLUP_Port, &GPIO_InitStruct);
	
	HAL_Delay(100);
	HAL_GPIO_WritePin(USB_PULLUP_Port, USB_PULLUP_Pin, GPIO_PIN_SET);
	
	/* USER CODE END 1 */
	MX_USB_DEVICE_Init();	
	/* USER CODE BEGIN 2 */
	uint32_t timeOut;
	
	f_setlabel("123");//U盘命名
	
	timeOut = 1000;
	while( f_mount(&fatfs, "0:", 1) && timeOut --);
	if(timeOut == 0 || timeOut > 1000)
	{
		f_mkfs("0:", 0, 1);
	}
	/* USER CODE END 2 */
}

/*
 @函数功能：应用进程
 @输入变量：无
 @输出变量：无
*/
void runProcess(void)
{
	switch(processCount)
	{
		/*原作者的demo*/
		case AUTHOR_DEMO_PROCESS:
			author_demo_api();
			processCount = NO_USER;
			break;
		
		/*创建空白的界面*/
		case BLANK_PAGE_DEMO_PROCESS:
			blank_page_demo_api();
			processCount = AUTHOR_DEMO_PROCESS;
			break;
		
		/*创建helloworld例程*/
		case HELLOWORLD_DEMO_PROCESS:
			helloworld_demo_api();
			processCount = BLANK_PAGE_DEMO_PROCESS;
			break;
		
		/*创建中文显示*/
		case CHINESE_DEMO_PROCESS:
			chinese_demo_api();
			processCount = HELLOWORLD_DEMO_PROCESS;
			break;
		
		default :processCount = NO_USER;break;
	}
}



