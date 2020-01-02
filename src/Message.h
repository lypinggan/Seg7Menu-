/***********************************************************************************************
文件名	Message.h
日期	2014.02.16
作者	张诗星	陈东辉
文件说明	基本消息支持
修订说明:

2014.02.16  张诗星 初始版本
2014.03.13	张诗星 修订消息传送方式为精简类型
					为了减少消息处理的递归调用 修改了消息处理方法
***********************************************************************************************/

#ifndef _LIB_SMENU_MESSAGE_
#define _LIB_SMENU_MESSAGE_

/*头文件--------------------------------------------------------------------------------------*/
//#include ""

/*用户配置------------------------------------------------------------------------------------*/

/*常量定义------------------------------------------------------------------------------------*/

//消息定义
#define WM_NULL			0					/*消息传递结束*/

//系统消息
#define WM_SYS_OFFSET 	1					/*系统消息起始偏移*/

#define WM_INIT			WM_SYS_OFFSET+0		/*初始化消息*/
#define WM_PAINT 		WM_SYS_OFFSET+1		/*重绘消息*/
#define WM_CLEAR		WM_SYS_OFFSET+2		/*清屏消息*/
#define WM_DESTROY		WM_SYS_OFFSET+3		/*销毁消息*/
#define WM_UPDATE		WM_SYS_OFFSET+4		/*数值调节刷新消息*/
#define WM_TIMER		WM_SYS_OFFSET+5		/*定时消息 用于闪烁处理等*/

//菜单消息
#define WM_MENU_PREV	WM_SYS_OFFSET+6		/*上个菜单*/
#define WM_MENU_NEXT	WM_SYS_OFFSET+7		/*下个菜单*/

//按键消息
#define WM_KEY_OFFSET	WM_SYS_OFFSET+32	/*按键消息*/

#define WM_KEY_NULL		0					/*无按键*/

#define WM_KEY_SET		WM_KEY_OFFSET+0		/*SET按键*/
#define WM_KEY_LEFT		WM_KEY_OFFSET+1		/*左按键*/
#define WM_KEY_RIGHT	WM_KEY_OFFSET+2		/*右按键*/
#define WM_KEY_ADD		WM_KEY_OFFSET+3		/*加按键*/
#define WM_KEY_SUB		WM_KEY_OFFSET+4		/*减按键*/
#define WM_KEY_MENU		WM_KEY_OFFSET+5		/*按键消息*/
#define WM_KEY_RETURN	WM_KEY_OFFSET+6		/*返回按键*/

//用户消息偏移
#define WM_USER_OFFSET 	WM_SYS_OFFSET+128	/*用户消息偏移*/

/*类型定义------------------------------------------------------------------------------------*/

//消息类型
typedef unsigned char Msg;			/*Message类型*/
typedef void (*MsgPrce)(void);		/*消息例程函数指针*/
//全局消息结构
typedef struct
{
	Msg m_msg;				/*消息*/
	MsgPrce m_msgPrce;		/*当前消息目标例程*/
	
	MsgPrce m_theObject;	/*消息对象*/
}CMsgDC;

/*变量引用------------------------------------------------------------------------------------*/
extern CMsgDC msgDC;		/*全局消息变量*/

/*接口函数------------------------------------------------------------------------------------*/

//设置消息栈的消息例程对象
#define CMsgPr_SetMsgObject(X) (msgDC.m_theObject=(MsgPrce)(X))
//从消息队列获取消息
unsigned char CMsgPr_GetMessage(void);
//给自己投递派送消息
void CMsgPr_PollMessage(Msg aMsg);
//直接发送消息
void CMsgPr_SendMessage(MsgPrce pmsgPrce, Msg aMsg);
//父类消息处理
void CMsgPr_ParentMessage(MsgPrce pmsgPrce, Msg aMsg);
//结束消息传递
void CMsgPr_EndMessage(void);

#endif
