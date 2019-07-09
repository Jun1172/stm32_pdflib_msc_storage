/*
 * pdflib测试代码
 *
 *  Created on: 2019-7-9
 *      Author: jun
 */
#ifndef __DEMO_H__
#define __DEMO_H__

#include "pdflib.h"

/*原作者的demo*/
#define AUTHOR_DEMO 0	

/*创建空白的界面*/
#define BLANK_PAGE_DEMO 1	

#if AUTHOR_DEMO
/*
函数功能：创建一个PDF文件了，写入测试数据
输入参数：无
输出参数：无
*/
void author_demo_api(void);
#endif

#if BLANK_PAGE_DEMO
/*
函数功能：创建空白的页面
输入参数：无
输出参数：无
*/
void blank_page_demo_api(void);
#endif
			
#endif
