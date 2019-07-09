/*
*pdflib测试代码:
*创建空白页面测试
*Created on: 2019-7-9
*Author: jun
*/
#include "demo.h"

#if BLANK_PAGE_DEMO

/*
函数功能：创建空白的页面
输入参数：无
输出参数：无
*/
void blank_page_demo_api(void)
{
	LPDF_Doc pdf;
	LPDF_Page page[5];

	pdf = LPDF_New();
	page[0] = LPDF_AddPage(pdf);
	LPDF_Page_SaveContext(page[0]);
	page[1] = LPDF_AddPage(pdf);
	LPDF_Page_SaveContext(page[1]);
	page[2] = LPDF_AddPage(pdf);
	LPDF_Page_SaveContext(page[2]);
	page[3] = LPDF_AddPage(pdf);
	LPDF_Page_SaveContext(page[3]);
	page[4] = LPDF_AddPage(pdf);
	LPDF_Page_SaveContext(page[4]);
	LPDF_SaveToFile(pdf, "blank.pdf");
}

#endif

