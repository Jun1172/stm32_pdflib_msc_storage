/*
*@ 用途：应用程序
*@ 时间：20190708
*@ 作者：JUN
*/
#include "process.h"

FATFS fatfs;         // Work area (file system object) for logical drive
FIL fsrc;            // file objects
UINT br;

//测试例程
#define NO_USER				0
#define EXAMPLE1			1
uint8_t processCount = EXAMPLE1;

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

/*******************************************************************************
 * Code
 ******************************************************************************/
const LPDF_BYTE RAW_IMAGE_DATA[128] = {
    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xfc,
    0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xf0,
    0xf3, 0xf3, 0xff, 0xe0, 0xf3, 0xf3, 0xff, 0xc0,
    0xf3, 0xf3, 0xff, 0x80, 0xf3, 0x33, 0xff, 0x00,
    0xf3, 0x33, 0xfe, 0x00, 0xf3, 0x33, 0xfc, 0x00,
    0xf8, 0x07, 0xf8, 0x00, 0xf8, 0x07, 0xf0, 0x00,
    0xfc, 0xcf, 0xe0, 0x00, 0xfc, 0xcf, 0xc0, 0x00,
    0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xfe, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00,
    0xff, 0xf8, 0x0f, 0xe0, 0xff, 0xf0, 0x0f, 0xe0,
    0xff, 0xe0, 0x0c, 0x30, 0xff, 0xc0, 0x0c, 0x30,
    0xff, 0x80, 0x0f, 0xe0, 0xff, 0x00, 0x0f, 0xe0,
    0xfe, 0x00, 0x0c, 0x30, 0xfc, 0x00, 0x0c, 0x30,
    0xf8, 0x00, 0x0f, 0xe0, 0xf0, 0x00, 0x0f, 0xe0,
    0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 @函数功能：测试例程1
 @输入变量：无
 @输出变量：无
 @输出变量：创建一个PDF，写入测试字符串
*/
static void example1(void)
{
//	struct pdf_info info = {
//        .creator = "My software",
//        .producer = "My software",
//        .title = "My document",
//        .author = "My name",
//        .subject = "My subject",
//        .date = "Today"
//    };
//    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
////    pdf_set_font(pdf, "Times-Roman");
//    pdf_append_page(pdf);
//    pdf_add_text(pdf, NULL, "This is text", 12, 50, 20, PDF_BLACK);
//    pdf_add_line(pdf, NULL, 50, 24, 150, 24, 3, PDF_BLACK);
//    pdf_save(pdf, "output.pdf");
//    pdf_destroy(pdf);
		
		
	LPDF_Doc pdf;
	LPDF_Page page[2];
	LPDF_Image image;
	LPDF_UINT16 width = 0,height = 0, i = 0;

	pdf = LPDF_New();
	page[0] = LPDF_AddPage(pdf);
	width = LPDF_Page_GetWidth(page[0]);
	height = LPDF_Page_GetHeight(page[0]);
	LPDF_Page_SetRGBFill(page[0], 1.0, 0.0, 0.0);
	LPDF_Page_BeginText(page[0]);
	LPDF_Page_MoveTextPos(page[0], 50, height-20);
	while(font_list[i])
	{
		LPDF_Page_SetFontAndSize(page[0], font_list[i], 24);
		LPDF_Page_ShowText(page[0], "abcdefgABCDEFG12345!\043$\045&+-@?");
		LPDF_Page_MoveTextPos(page[0], 0, -30);
		i++;
	}
	LPDF_Page_SetFontAndSize(page[0],"SimSun",20);
	LPDF_Page_ShowText(page[0], "中文字体");
	LPDF_Page_EndText(page[0]);

	image = LPDF_LoadRawImageFromMem(pdf,RAW_IMAGE_DATA,32,32,LPDF_CS_DEVICE_GRAY,1);
	LPDF_Page_DrawImage(page[0], image, 50, 50, 32, 32);

	LPDF_Page_SaveContext(page[0]);
	page[1] = LPDF_AddPage(pdf);
	LPDF_Page_MoveTo(page[1], width/2-50, height-200);
	LPDF_Page_LineTo(page[1], width/2-50, height-300);
	LPDF_Page_LineTo(page[1], width/2-50+100, height-300);
	LPDF_Page_LineTo(page[1], width/2-50+100, height-200);
	LPDF_Page_LineTo(page[1], width/2-50, height-200);
	LPDF_Page_LineTo(page[1], width/2-50+40, height-140);
	LPDF_Page_LineTo(page[1], width/2-50+40+100, height-140);
	LPDF_Page_LineTo(page[1], width/2-50+100, height-200);
	LPDF_Page_Stroke(page[1]);
	LPDF_Page_MoveTo(page[1], width/2-50+40+100, height-140);
	LPDF_Page_LineTo(page[1], width/2-50+40+100, height-240);
	LPDF_Page_LineTo(page[1], width/2-50+100, height-300);
	LPDF_Page_Stroke(page[1]);
	LPDF_Page_SetLineWidth(page[1],2.5);
	LPDF_Page_MoveTo(page[1],width/2-50,height-300);
	LPDF_Page_LineTo(page[1],width/2-50,height-500);
	LPDF_Page_Stroke(page[1]);
	LPDF_Page_SaveContext(page[1]);
	LPDF_SaveToFile(pdf, "red.pdf");
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
		case EXAMPLE1:
			example1();
			processCount = NO_USER;
			break;
		default :processCount = NO_USER;break;
	}
}



