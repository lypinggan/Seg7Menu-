/***********************************************************************************************
文件名	Message.c
日期	2014.02.16
作者	张诗星	陈东辉
文件说明	基本消息支持
修订说明:

2014.02.16  张诗星 初始版本
2014.03.13	张诗星	修订消息传送方法，减少函数递归调用
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/
#include "Message.h"
//#include ""

/*全局变量------------------------------------------------------------------------------------*/
CMsgDC msgDC;		/*全局消息变量*/

/*接口函数------------------------------------------------------------------------------------*/

//从消息队列获取消息 并移除消息
unsigned char CMsgPr_GetMessage(void)
{
	unsigned char amsg = msgDC.m_msg;
	msgDC.m_msg = WM_NULL;					/*获取消息后清除该消息*/
	return amsg;
}
//向自身派发消息 可防止函数递归调用
void CMsgPr_PollMessage(Msg aMsg)
{
	msgDC.m_msgPrce = msgDC.m_theObject;	/*当前消息目标*/
	//保存消息数据
	msgDC.m_msg = aMsg;	
}
//向控件直接发送消息 将立即调用消息函数 多次调用将导致函数递归
void CMsgPr_SendMessage(MsgPrce pmsgPrce, Msg aMsg)
{
	msgDC.m_theObject = pmsgPrce;		/*消息对象*/
	msgDC.m_msgPrce = pmsgPrce;			/*当前消息目标*/

	//保存消息数据
	msgDC.m_msg = aMsg;					/*消息*/
	//循环调用消息例程
	while (1)
	{
		if ((0==msgDC.m_msgPrce)||		/*消息目标有效 并且消息有效*/
			(WM_NULL==msgDC.m_msg))
			break;
		msgDC.m_msgPrce();	/*调用消息例程*/
	}
}
//父类消息处理
void CMsgPr_ParentMessage(MsgPrce pmsgPrce, Msg aMsg)
{
	msgDC.m_msg = aMsg;	
	msgDC.m_msgPrce = pmsgPrce;
}
//结束消息传递
void CMsgPr_EndMessage(void)
{
	msgDC.m_msgPrce = 0;			/*结束消息传递*/
	msgDC.m_msg = WM_NULL;
}

