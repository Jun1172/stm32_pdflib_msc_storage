/*
*pdflib测试代码:
*创建helloworld
*Created on: 2019-7-9
*Author: jun
*/
#include "demo.h"

#if HELLOWORLD_DEMO

/*
函数功能：创建helloworld
输入参数：无
输出参数：无
*/
void helloworld_demo_api(void)
{
	LPDF_Doc pdf;
	LPDF_Page page[1];
	LPDF_UINT16 width = 0,height = 0;

	pdf = LPDF_New();
	page[0] = LPDF_AddPage(pdf);
	width = LPDF_Page_GetWidth(page[0]);
	height = LPDF_Page_GetHeight(page[0]);
	LPDF_Page_SetRGBFill(page[0], 1.0, 0.0, 0.0);
	LPDF_Page_BeginText(page[0]);
	LPDF_Page_MoveTextPos(page[0], 100, 100);

	LPDF_Page_SetFontAndSize(page[0], font_list[0], 24);
	LPDF_Page_ShowText(page[0], "helloworld");
	LPDF_Page_MoveTextPos(page[0], 0, -30);

	LPDF_Page_EndText(page[0]);

	LPDF_Page_SaveContext(page[0]);
	LPDF_SaveToFile(pdf, "helloworld.pdf");
}

#endif

