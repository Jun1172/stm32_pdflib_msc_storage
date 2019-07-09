#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pdflib.h"

static char pdf_buf[512];

#define LPDF_VERSIOIN_TEXT          "0.0.4"
#define LPDF_FILE_HEADER_LEN        15
#define LPDF_BASEFONT_NUM           14
const char LPDF_FILE_HEADER[] = "%%PDF-1.4\n%%\xAA\xBB\xCC\xDD\n";
const char *font_list[] = {
    "Courier",
    "Courier-Bold",
    "Courier-Oblique",
    "Courier-BoldOblique",
    "Helvetica",
    "Helvetica-Bold",
    "Helvetica-Oblique",
    "Helvetica-BoldOblique",
    "Times-Roman",
    "Times-Bold",
    "Times-Italic",
    "Times-BoldItalic",
    "Symbol",
    "ZapfDingbats",
    NULL
};


const char * LPDF_GetVersion(void)
{
    return LPDF_VERSIOIN_TEXT;
}

LPDF_BOOL LPDF_HasDoc(LPDF_Doc pdf)
{
    if(!pdf)
        return LPDF_FALSE;

    if(pdf->error != LPDF_NOERROR) 
    {
        return LPDF_FALSE;
    } 
    else
        return LPDF_TRUE;
}
LPDF_Doc LPDF_New(void)
{
	static LPDF_Doc_Rec pdf;
	LPDF_INT length = 0, i = 0;
	LPDF_UINT writeBytes = 0;
	FRESULT fr;
    
    fr = LPDF_FOPEN(&pdf.file,"tmp.pdf",FA_CREATE_ALWAYS|FA_WRITE);
	if(fr)
	{
		printf("err:%d\n",fr);
		LPDF_PRINTF("create tmp.pdf failed!\r\n");
		return NULL;
	}
    LPDF_FPRINTF(&pdf.file,LPDF_FILE_HEADER);   /* 写入文件头 */
    pdf.offset = LPDF_FILE_HEADER_LEN;
   
    pdf.obj[0].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"1 0 obj\n<<\n/Type /Catalog\n/Pages 17 0 R\n>>\nendobj\n\n");
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;
    
    pdf.obj[1].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"2 0 obj\n<<\n/Creator (light pdf library %s)\n"
                    "/Auther (anobodykey)\n>>\nendobj\n\n",LPDF_VERSIOIN_TEXT);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;
    
    for(i = 0; i < LPDF_BASEFONT_NUM-2; i++)
    {
        pdf.obj[2+i].offset = pdf.offset;
        length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Font\n/Subtype /Type1\n"
                        "/BaseFont /%s\n/Encoding /WinAnsiEncoding\n>>\n"
                        "endobj\n\n",3+i,font_list[i]);
        LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
        pdf.offset += length;
    }
    for(; i < LPDF_BASEFONT_NUM; i++)
    {
        pdf.obj[2+i].offset = pdf.offset;
        length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Font\n/Subtype /Type1\n"
                        "/BaseFont /%s\n>>\nendobj\n\n",3+i,font_list[i]);
        LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
        pdf.offset += length;
    }
    /* 添加中文字体,只支持一种 */
    pdf.obj[2+i].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Font\n/Subtype /Type0\n"
                        "/BaseFont /SimSun\n/Encoding /GB-EUC-H\n/DescendantFonts [ %d 0 R ]\n"
                        ">>\nendobj\n\n",3+i,4+i);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    pdf.obj[3+i].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Font\n/Subtype /CIDFontType0\n"
                        "/BaseFont /SimSun\n/DW 1000\n/DW2 [ 880 -1000 ]\n"
                        "/W [ 668 [ 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 ]",4+i);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    length = LPDF_SPRINTF(pdf_buf," 696 [ 500 500 500 500 ] 814 [ 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500");
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    length = LPDF_SPRINTF(pdf_buf," 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500");
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    length = LPDF_SPRINTF(pdf_buf," 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500");
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    length = LPDF_SPRINTF(pdf_buf," 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 500 ] 7716 [ 500 ] ]\n");
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    length = LPDF_SPRINTF(pdf_buf,"/FontDescriptor %d 0 R\n/CIDSystemInfo <<\n/Registry (Adobe)\n"
                        "/Ordering (GB1)\n/Supplement 0\n>>\n>>\nendobj\n\n",5+i);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    pdf.obj[4+i].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /FontDescriptor\n/FontName /SimSun\n/Ascent 859\n"
                        "/Descent -140\n/CapHeight 683\n/MissingWidth 500\n/Flags 7\n"
                        "/FontBBox [ 0 -140 996 855 ]\n/ItalicAngle 0\n/StemV 78\n>>\nendobj\n\n",5+i);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;

    pdf.objNumber = 6+i; /* 空出5+i留作pages对象 */
    pdf.pagesNumber = 5+i; /* 记录pages对象的目标号 */

    pdf.obj[0].offset = pdf.offset;
    length = LPDF_SPRINTF(pdf_buf,"1 0 obj\n<<\n/Type /Catalog\n/Pages %d 0 R\n>>\nendobj\n\n",pdf.pagesNumber+1);
    LPDF_FWRITE(&pdf.file,pdf_buf,length,&writeBytes);
    pdf.offset += length;
  
    return (LPDF_Doc)&pdf;
}

LPDF_STATUS LPDF_SaveToFile(LPDF_Doc pdf,const char *file_name)
{
    LPDF_UINT16 i = 0, child_pages = 0;
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;

    pdf->obj[pdf->pagesNumber].offset = pdf->offset;
    
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Pages\n/Kids [ ",pdf->pagesNumber+1);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    for(i = pdf->pagesNumber+1; i < pdf->objNumber; i++)
    {
		if(LPDF_OBJ_PAGE == pdf->obj[i].type)
		{
			length = LPDF_SPRINTF(pdf_buf,"%d 0 R\n",i+1);
        	LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
        	pdf->offset += length;
			child_pages += 1;
		}
    }
    length = LPDF_SPRINTF(pdf_buf,"]\n/Count %d\n>>\nendobj\n\n",child_pages);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    
    LPDF_FPRINTF(&pdf->file,"xref\n0 %d\n0000000000 65535 f\r\n",pdf->objNumber+1);
    for(i = 0 ; i < pdf->objNumber; i++)
    {
        LPDF_FPRINTF(&pdf->file,"%010d 00000 n\r\n",pdf->obj[i].offset);
    }
    LPDF_FPRINTF(&pdf->file,"trailer\n<<\n/Size %d\n/Root 1 0 R\n/Info 2 0 R\n>>\nstartxref\n%d\n%%%%EOF",
                pdf->objNumber+1,pdf->offset);
    LPDF_FCLOSE(&pdf->file);
    LPDF_REMOVE(file_name);
    LPDF_RENAME("tmp.pdf",file_name);
    return LPDF_OK;
}

LPDF_STATUS LPDF_Free(LPDF_Doc pdf)
{
    memset(pdf,sizeof(LPDF_Doc_Rec),0);
    return LPDF_OK;
}
/**
  * @brief 添加一个新的page到pdf文档中,此时添加的page不写入文件,等到保存页面时再写入
  */
LPDF_Page LPDF_AddPage(LPDF_Doc pdf)
{
    LPDF_INT length = 0;
    LPDF_Page page = NULL;
	LPDF_UINT writeBytes = 0;
    
	if(!LPDF_HasDoc(pdf))
		return NULL;
    page = (LPDF_Page)&pdf->obj[pdf->objNumber];
	page->type = LPDF_OBJ_PAGE;
	page->number = pdf->objNumber;
    page->attr[LPDF_PAGE_WIDTH] = 595;
    page->attr[LPDF_PAGE_HEIGHT] = 842;
	page->attr[LPDF_PAGE_CTX_LENGTH] = 0;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_PAGE_DESCRIPTION;
    page->attr[LPDF_PAGE_FONTTYPE] = LPDF_FONT_INVALID;
	page->attr[LPDF_PAGE_FONTSIZE] = 0;
    page->doc = pdf;
	
	pdf->obj[pdf->objNumber+1].offset = pdf->offset;
	length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<</Length %d 0 R>>\nstream\n",
                        pdf->objNumber+2, pdf->objNumber+3);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
	pdf->offset += length;
	pdf->objNumber+=3;
    return page;
}

LPDF_STATUS LPDF_Page_SetHeight(LPDF_Page page, LPDF_UINT16 value)
{
	if(value < 3 || value > 14400)
	{
		return LPDF_PAGE_INVALID_SIZE;
	}	
    page->attr[LPDF_PAGE_HEIGHT] = value;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_SetWidth(LPDF_Page page, LPDF_UINT16 value)
{
	if(value < 3 || value > 14400)
	{
		return LPDF_PAGE_INVALID_SIZE;
	}	
    page->attr[LPDF_PAGE_WIDTH] = value;
    return LPDF_OK;
}

LPDF_UINT16 LPDF_Page_GetHeight(LPDF_Page page)
{
    if(page)
    {
        return page->attr[LPDF_PAGE_HEIGHT];
    }
    
    return 0;
}

LPDF_UINT16 LPDF_Page_GetWidth(LPDF_Page page)
{
    if(page)
    {
        return page->attr[LPDF_PAGE_WIDTH];
    }
    
    return 0;
}

LPDF_STATUS LPDF_Page_CheckState(LPDF_Page page, LPDF_UINT mode)
{
    if(!page)
        return LPDF_INVALID_OBJECT;
    if(!(page->attr[LPDF_PAGE_GMODE]&mode))
        return LPDF_FAILED;

    return LPDF_OK;
}

LPDF_STATUS LPDF_Page_SetFontAndSize(LPDF_Page page,
                           const char *font_name, LPDF_UINT8 size)
{
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_INT i = 0,length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT|LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;
 
    while(font_list[i])
    {
        if(0 == strcmp(font_list[i],font_name))
        {
            page->attr[LPDF_PAGE_FONTTYPE] = i;
            page->attr[LPDF_PAGE_FONTSIZE] = size;
            length = LPDF_SPRINTF(pdf_buf,"/F%d %d Tf\n",i+1,size);
            LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
            pdf->offset += length;
            page->attr[LPDF_PAGE_CTX_LENGTH] += length;
            return LPDF_OK;
        }
        i+=1;
    }
    
    /* 检测中文字体 */
    if(0 == strcmp("SimSun",font_name))
    {
        page->attr[LPDF_PAGE_FONTTYPE] = i;
        page->attr[LPDF_PAGE_FONTSIZE] = size;
        length = LPDF_SPRINTF(pdf_buf,"/F%d %d Tf\n",i+1,size);
        LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
        pdf->offset += length;
        page->attr[LPDF_PAGE_CTX_LENGTH] += length;
        return LPDF_OK;
    }
    return LPDF_PAGE_INVALID_FONT;
}

/**
  * @brief 保存page内容,在每个page内容结束之后调用一次
  */
LPDF_STATUS LPDF_Page_SaveContext(LPDF_Page page)
{
    LPDF_INT length, i = 0,size = 0;
	LPDF_UINT writeBytes = 0;
	LPDF_BOOL hasImage = LPDF_FALSE;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;
     /* 对于每个page来说该接口只能调用一次即在GMODE为PAGE_DESCRIPTION的模式下 */
    length = LPDF_SPRINTF(pdf_buf,"endstream\nendobj\n\n");
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes); 
    pdf->offset += length;
	pdf->obj[page->number+2].offset = pdf->offset;
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n%d\nendobj\n\n",
                        page->number+3,page->attr[LPDF_PAGE_CTX_LENGTH]);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_OVER;
	
	/* 检测该page是否有添加image */
	for(i = 20; i < pdf->objNumber; i++)
	{
		/* 遍历全部obj对象 */
		if((LPDF_OBJ_IMAGE == pdf->obj[i].type) && 
			(page == (LPDF_Page)pdf->obj[i].attr[LPDF_IMAGE_OWNER]))
		{
			hasImage = LPDF_TRUE;
			pdf->obj[i].offset = pdf->offset;	/* 加载image obj 偏移地址 */
			length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /XObject\n/Name /Image%d\n"
								"/Subtype /Image\n/ColorSpace /",i+1,pdf->obj[i].attr[LPDF_IMAGE_NUMBER]); 
			LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
			pdf->offset += length;
			switch(pdf->obj[i].attr[LPDF_IMAGE_COLORSPACE])
			{
				case LPDF_CS_DEVICE_CMYK:
					break;
				case LPDF_CS_DEVICE_GRAY:
					LPDF_FPRINTF(&pdf->file,"DeviceGray\n");
					pdf->offset += 11;
					break;
				case LPDF_CS_DEVICE_RGB:
					LPDF_FPRINTF(&pdf->file,"DeviceRGB\n");
					pdf->offset += 10;
					break;
				default:
					break;
			}
			size = pdf->obj[i].attr[LPDF_IMAGE_WIDTH]*pdf->obj[i].attr[LPDF_IMAGE_HEIGHT]*pdf->obj[i].attr[LPDF_IMAGE_BITS]/8;
			length = LPDF_SPRINTF(pdf_buf,"/Width %d\n/Height %d\n/BitsPerComponent %d\n"
								"/Length %d\n>>\nstream\n",pdf->obj[i].attr[LPDF_IMAGE_WIDTH],
								pdf->obj[i].attr[LPDF_IMAGE_HEIGHT],pdf->obj[i].attr[LPDF_IMAGE_BITS],
								size);
			LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
			pdf->offset += length;
			LPDF_FWRITE(&pdf->file,(LPDF_BYTE *)pdf->obj[i].attr[LPDF_IMAGE_ADDR],size,&writeBytes);
			pdf->offset += size;
			length = LPDF_SPRINTF(pdf_buf,"\nendstream\nendobj\n\n");
			LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
			pdf->offset += length;
		}
	}
	
	/* 最后写入page对象 */
	page->offset = pdf->offset;	/* 加载page obj 地址 */
    length = LPDF_SPRINTF(pdf_buf,"%d 0 obj\n<<\n/Type /Page\n/Parent %d 0 R\n/Contents %d 0 R\n"
                    "/Resources <<\n/ProcSet [/PDF /TEXT /ImageB /ImageC /ImageI]\n"
                    "/Font <<\n",page->number+1,pdf->pagesNumber+1, page->number+2);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
	i = 0;
    while(font_list[i])
    {
        length = LPDF_SPRINTF(pdf_buf,"/F%d %d 0 R\n",i+1,3+i);
        LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
        pdf->offset += length;
        i+=1;
    }
    /* 追加中文字体对象 */
    length = LPDF_SPRINTF(pdf_buf,"/F%d %d 0 R\n",i+1,3+i);
	LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    LPDF_FPRINTF(&pdf->file,">>\n");
    pdf->offset += 3;
	if(hasImage)
	{
		LPDF_FPRINTF(&pdf->file,"/XObject <<\n");
		pdf->offset += 12;
		for(i = 20; i < pdf->objNumber; i++)
		{
			/* 遍历全部obj对象 */
			if((LPDF_OBJ_IMAGE == pdf->obj[i].type) && 
				(page == (LPDF_Page)pdf->obj[i].attr[LPDF_IMAGE_OWNER]))
			{
				length = LPDF_SPRINTF(pdf_buf,"/X%d %d 0 R\n",pdf->obj[i].attr[LPDF_IMAGE_NUMBER],i+1);
				LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
				pdf->offset += length;
			}
		}
		LPDF_FPRINTF(&pdf->file,">>\n");
		pdf->offset += 3;
	}
	LPDF_FPRINTF(&pdf->file,">>\n");
	pdf->offset += 3;
	length = LPDF_SPRINTF(pdf_buf,"/MediaBox [ 0 0 %d %d ]\n>>\nendobj\n\n",
                        page->attr[LPDF_PAGE_WIDTH], page->attr[LPDF_PAGE_HEIGHT]);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    return LPDF_OK;
}

LPDF_STATUS LPDF_Page_BeginText(LPDF_Page page)
{
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;
    LPDF_FPRINTF(&pdf->file,"BT\n");
    pdf->offset += 3;
    page->attr[LPDF_PAGE_CTX_LENGTH] += 3;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_TEXT_OBJECT;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_EndText(LPDF_Page page)
{
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    LPDF_FPRINTF(&pdf->file,"ET\n");
    pdf->offset += 3;
    page->attr[LPDF_PAGE_CTX_LENGTH] += 3;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_PAGE_DESCRIPTION;
    return LPDF_OK;
}

LPDF_STATUS LPDF_Page_ShowText(LPDF_Page page, const char *text)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_BOOL isCNSInside = LPDF_FALSE;
    LPDF_UINT8 *pStr = (LPDF_UINT8 *)text;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    if(LPDF_FONT_INVALID == page->attr[LPDF_PAGE_FONTTYPE])
        return LPDF_OK;
    /* 检测显示字符中是否包含中文,即多字节编码的数据 */
    while(*pStr)
    {
        if(*pStr >= 0x80)
        {
            isCNSInside = LPDF_TRUE;
            break;
        }
        pStr++;
    }
    if(isCNSInside)
    {
        LPDF_FPRINTF(&pdf->file,"<");
        length = 1;
        while(*text)
        {
            LPDF_FPRINTF(&pdf->file,"%02X",(LPDF_UINT8)*text);
            text+=1;
            length+=2;
        }
        LPDF_FPRINTF(&pdf->file,"> Tj\n");
        length += 5;
    }
    else
    {
    length = LPDF_SPRINTF(pdf_buf,"(%s) Tj\n",text);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    }
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    return LPDF_OK;
    
}
LPDF_STATUS LPDF_Page_MoveTextPos(LPDF_Page page,
                        LPDF_INT x, LPDF_INT y)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    length = LPDF_SPRINTF(pdf_buf,"%d %d Td\n",x,y);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;

    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_TextOut(LPDF_Page page, 
                        LPDF_UINT16 x, LPDF_UINT16 y,
                        const char *text)
{
    return LPDF_OK;
}

LPDF_STATUS LPDF_Page_SetRGBStroke(LPDF_Page page,
                         LPDF_REAL r,
                         LPDF_REAL g,
                         LPDF_REAL b)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT|LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;

    length = LPDF_SPRINTF(pdf_buf,"%.1f %.1f %.1f RG\n",r,g,b);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    return LPDF_OK;
}

LPDF_STATUS LPDF_Page_SetRGBFill(LPDF_Page page,
                         LPDF_REAL r,
                         LPDF_REAL g,
                         LPDF_REAL b)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_TEXT_OBJECT|LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;

    length = LPDF_SPRINTF(pdf_buf,"%.1f %.1f %.1f rg\n",r,g,b);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_MoveTo (LPDF_Page page,
                         LPDF_UINT16 x,
                         LPDF_UINT16 y)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PAGE_DESCRIPTION|LPDF_GMODE_PATH_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    length = LPDF_SPRINTF(pdf_buf,"%d %d m\n",x,y);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_PATH_OBJECT;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_LineTo (LPDF_Page page,
                         LPDF_UINT16 x,
                         LPDF_UINT16 y)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PATH_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    length = LPDF_SPRINTF(pdf_buf,"%d %d l\n",x,y);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_Stroke(LPDF_Page page)
{
    LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PATH_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    LPDF_FPRINTF(&pdf->file,"S\n");
    pdf->offset += 2;
    page->attr[LPDF_PAGE_CTX_LENGTH] += 2;
    page->attr[LPDF_PAGE_GMODE] = LPDF_GMODE_PAGE_DESCRIPTION;
    return LPDF_OK;
}
LPDF_STATUS LPDF_Page_DrawImage(LPDF_Page page,
                      LPDF_Image image,
                      LPDF_UINT16 x,
                      LPDF_UINT16 y,
                      LPDF_UINT16 width,
                      LPDF_UINT16 height)
{
    LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
	LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PAGE_DESCRIPTION);
    if(LPDF_OK != ret)
        return ret;
    length = LPDF_SPRINTF(pdf_buf,"q\n%d 0 0 %d %d %d cm\n/X%d Do\nQ\n",
                        width,height,x,y,image->attr[LPDF_IMAGE_NUMBER]);
    LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    image->attr[LPDF_IMAGE_OWNER] = (LPDF_UINT32)page;
    return LPDF_OK;
}
LPDF_Image LPDF_LoadRawImageFromFile(LPDF_Doc pdf,
                      const char *filename,
                      LPDF_UINT width,
                      LPDF_UINT height,
                      LPDF_ColorSpace color_space)
{	
	if(!LPDF_HasDoc(pdf))
        return NULL;
    
    return NULL;
}
/**
  * @brief
  * @param 
  */
LPDF_Image LPDF_LoadRawImageFromMem(LPDF_Doc pdf,
                      const LPDF_BYTE *buf,
                      LPDF_UINT width,
                      LPDF_UINT height,
                      LPDF_ColorSpace color_space,
                      LPDF_UINT bits_per_component)
{
    LPDF_Image image = NULL;
    
    if(!LPDF_HasDoc(pdf))
        return NULL;

    if((1 != bits_per_component) && (2 != bits_per_component) &&
       (4 != bits_per_component) && (8 != bits_per_component))
    {
        pdf->error = LPDF_INVALID_IMAGE;
        return NULL;
    }
    
    if((LPDF_CS_DEVICE_RGB != color_space) && (LPDF_CS_DEVICE_GRAY != color_space) &&
       (LPDF_CS_DEVICE_CMYK != color_space))
    {
        pdf->error = LPDF_INVALID_COLOR_SPACE;
        return NULL;
    }
    
    if(0 != ((width*bits_per_component)%8))
    {
        pdf->error = LPDF_INVALID_IMAGE_WIDTH;
        return NULL;
    }
    
    image = (LPDF_Image)&pdf->obj[pdf->objNumber];
    image->type = LPDF_OBJ_IMAGE;
	image->number = pdf->objNumber;
    image->attr[LPDF_IMAGE_WIDTH] = width;
    image->attr[LPDF_IMAGE_HEIGHT] = height;
    image->attr[LPDF_IMAGE_BITS] = bits_per_component;
    image->attr[LPDF_IMAGE_COLORSPACE] = color_space;
    image->attr[LPDF_IMAGE_ADDR] = (LPDF_UINT32)buf;
    image->attr[LPDF_IMAGE_NUMBER] = ++pdf->imageNumber;
    image->doc = pdf;
	pdf->objNumber+=1;
    return image;
}

LPDF_STATUS LPDF_Page_SetLineWidth(LPDF_Page page,
                         LPDF_REAL line_width)
{
	LPDF_INT length = 0;
	LPDF_UINT writeBytes = 0;
	LPDF_Doc pdf = (LPDF_Doc)page->doc;
    LPDF_STATUS ret = LPDF_Page_CheckState(page,LPDF_GMODE_PAGE_DESCRIPTION|LPDF_GMODE_PATH_OBJECT);
    if(LPDF_OK != ret)
        return ret;
    length = LPDF_SPRINTF(pdf_buf,"%.1f w\n",line_width);
	LPDF_FWRITE(&pdf->file,pdf_buf,length,&writeBytes);
    pdf->offset += length;
    page->attr[LPDF_PAGE_CTX_LENGTH] += length;
    return LPDF_OK;
}

