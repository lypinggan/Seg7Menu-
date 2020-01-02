/***********************************************************************************************
文件名	SegDynamicScan.c
日期	2014.4.13
作者	张诗星	陈东辉
修订	2014.4.13
文件说明	LED数码管动态扫描支持
修订说明	初始版本
2014.04.13	张诗星	初始版本
***********************************************************************************************/
/*头文件--------------------------------------------------------------------------------------*/
#include "Seg7Font.h"
#include "SegDynamicScan.h"

//#include ""


/*全局变量------------------------------------------------------------------------------------*/
//显示缓冲区
unsigned char segDisBuff[SEG_DIGIT_NUM];

code const unsigned char SegDigCode[]=
{SEG7_CODE_0,SEG7_CODE_1,SEG7_CODE_2,SEG7_CODE_3,SEG7_CODE_4,
SEG7_CODE_5,SEG7_CODE_6,SEG7_CODE_7,SEG7_CODE_8,SEG7_CODE_9};
/*接口函数------------------------------------------------------------------------------------*/

//用户接口
void SegDynamicScan(void)
{
	static unsigned char scanIndex=0;		

	//消隐
	SegDyn_Hidden();
	//输出数据
	SegDyn_OutputData(scanIndex);

	if (++scanIndex>(SEG_DIGIT_NUM-1))
		scanIndex=0;
}