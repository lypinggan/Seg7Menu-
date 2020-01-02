/***********************************************************************************************
文件名	Menu.h
日期	2014.02.16
作者	张诗星	陈东辉
文件说明	基本菜单支持
修订说明:

2013.02.16  张诗星 初始版本
2013.03.28	张诗星 加入数码位调整闪动 和菜单分层支持
***********************************************************************************************/

#ifndef _LIB_SMENU_MENU_
#define _LIB_SMENU_MENU_

/*头文件--------------------------------------------------------------------------------------*/
#include "Message.h"
#include "Key.h"
#include "SegDynamicScan.h"
#include "Seg7Font.h"

//#include ""

/*用户配置------------------------------------------------------------------------------------*/
//最大菜单层数
//#define SMENU_PLIE_MAX		4		/*最大菜单层数*/

//光标消隐字符
#define CURSOR_CHARACTER SEG7_CODE_LL	/*光标消隐时显示的字符*/
//标题字符显示起始位置
#define SMENU_TITLE_POSIT	0			/*左边第一个数码管为0 从左到右*/
//标题字符数
#define SMENU_TITLE_LEN		4			/*显示标题时显示的字符数目*/
//数值显示起始位置
#define SMENU_VALUE_POSIT	4			/*左边第一个数码管为0*/
//光标最大数字位 数值字符数
#define CURSOR_NUMBER		4			/*光标位最大值*/
//数值最少显示位数
#define VALUE_ZERO_MIN 		1			/*最少显示位数 用于灭0处理*/
//
/*--------------------------------菜单模式--------------------------------
0	单屏模式
1	单屏带浏览模式
2	全显模式
3	全显带浏览模式
*/
#define SMENU_MODE_SETUP	3		/*菜单模式*/
//光标支持开关
#define SMENU_01_CURSOR		1		/*0不支持光标 1支持光标*/
//数字调整循环移动开关
#define SMENU_01_DIG_LOOP	1		/*0光标不能循环移位 1光标循环移位*/

//移植函数配置

//菜单创建 用户定义该函数当菜单初始化时被回调
void CMenu_Start(void);
//菜单结束 用户定义该函数当菜单结束时被回调
void CMenu_Quit(void);
//复位光标定时器 按键按下时回调 使其显示一段时间再闪烁
#define CMenu_ResetCursorTimer() (cursorTimer=0)

/*常量定义------------------------------------------------------------------------------------*/

//菜单项数值类型
#define MENU_VALUTYPE_U8	0x00	/*u8一字节类型*/	
#define MENU_VALUTYPE_U16 	0x01	/*u16双字节类型*/

//光标控制常量
#define MENU_CURSOR_OFF		0x00	/*光标定时器关闭*/
#define MENU_CURSOR_SHOW	0x01	/*光标显示*/
#define MENU_CURSOR_HIDE	0x02	/*光标消隐*/

/*错误检查------------------------------------------------------------------------------------*/
#if CURSOR_NUMBER==0
	#error "SMENU_CURSOR_MAX must be greater than 0!"
#endif

#if SMENU_TITLE_LEN>SEG_DIGIT_NUM
	#error "SMENU_TITLE_LEN value is set too large"
#endif

#if CURSOR_NUMBER>SEG_DIGIT_NUM
	#error "CURSOR_NUMBER value is set too large"
#endif

#if SMENU_VALUE_POSIT+CURSOR_NUMBER>SEG_DIGIT_NUM
	#error "SMENU_VALUE_POSIT+CURSOR_NUMBER value is set too large"
#endif

#if CURSOR_NUMBER>5
	#error "CURSOR_NUMBER must be less than 6!"
#endif
/*类型定义------------------------------------------------------------------------------------*/

//菜单配置项类
typedef struct
{
	unsigned char str[SMENU_TITLE_LEN];	/*显示字符串*/
	unsigned char attri;	/*属性*/
	int min;				/*数值最小值*/
	int max;				/*数值最大值*/
	void* m_pdata;			/*数据指针*/
	void* m_pfun;			/*控件例程函数*/
}CMenuItem;

//菜单索引
typedef struct
{
	unsigned char m_menuIndex;	/*当前菜单项索引*/
	CMenuItem* m_pMenuList;		/*当前菜单列表指针*/
}
CMenuI;

//全局菜单变量
typedef struct
{
	CMenuI theMenu;				/*当前菜单*/
	CMenuItem* pMenuItemPtr;	/*当前菜单项指针*/
	MsgPrce pFocus;				/*当前输入焦点*/

	signed int cValue;			/*当前值*/

	#if SMENU_01_CURSOR==1
	unsigned char cursorIndex;	/*数字调整位*/
	unsigned char cursorTimer;	/*光标定时器*/
	#endif
}
CMenuDC;						/*全局菜单上下文*/

/*外部变量------------------------------------------------------------------------------------*/

extern CMenuDC menuDC;			/*全局菜单变量*/
extern unsigned char cursorTimer;
extern code const unsigned short CursorIncr[];
extern code const unsigned char SegDigCode[];
/*宏函数--------------------------------------------------------------------------------------*/

//获取当前菜单项指针
#define CMenu_GetMenuItemPtr() ((CMenuItem*const)menuDC.pMenuItemPtr)
//获取当前菜单项索引
#define CMenu_GetMenuIndex() ((const unsigned char)(menuDC.theMenu.m_menuIndex))
//设置输入焦点
//#define CMenu_SetFocus(X) (menuDC.pFocus=(X),CMsgPr_SetMsgObject(X))
void CMenu_SetFocus(MsgPrce aFocus);
//获取当前输入焦点
#define CMenu_GetFocus() ((const MsgPrce)menuDC.pFocus)
//派发菜单事件
#define CMenu_MenuEvent(X) (CMsgPr_SendMessage(CMenu_GetFocus(),X))

/*用户接口------------------------------------------------------------------------------------*/

//初始化菜单 用户使用菜单前调用
void CMenu_MenuInit(CMenuItem* pRootMenu);
//按键事件 需用户定时调用
void CMenu_KeyEvent(unsigned char aKey);

#if SMENU_01_CURSOR==1
//光标定时器
void CMenu_CursorEvent(void);
//打开光标定时器
//#define CMenu_CursorTurnOn() (menuDC.cursorTimer=MENU_CURSOR_SHOW,menuDC.cursorIndex=0)
void CMenu_CursorTurnOn(void);
//关闭光标定时器
#define CMenu_CursorTurnOff() (menuDC.cursorTimer=MENU_CURSOR_OFF)
//获取光标定时器
#define CMenu_GetCursorState() ((const unsigned char)menuDC.cursorTimer)
#else
#define CMenu_CursorEvent()
#endif

//菜单管理基类 用于管理菜单跳转
void CMenuObject_Proc(void);
//菜单数值调整基类
void CValueObject_Proc(void);
//通用数值调整菜单消息例程
void CValueMenu_Proc(void);
//限幅处理
void CMenu_SetValueLimit(void);
//显示菜单标题 回调函数
void CMenu_DrawMenuText(void);
//显示菜单数值 回调函数
void CMenu_DrawMenuValue(unsigned char x,unsigned char width, unsigned short value);

#endif
