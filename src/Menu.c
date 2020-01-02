/***********************************************************************************************
文件名	Menu.c
日期	2014.02.16
作者	张诗星	陈东辉
文件说明	基本菜单支持
修订说明:

2013.02.16  张诗星 初始版本
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/
#include "Menu.h"

//#include ""

/*全局变量------------------------------------------------------------------------------------*/
//全局菜单
CMenuDC menuDC;				/*全局菜单变量*/

//光标调整常量
#if SMENU_01_CURSOR==1
code const unsigned short CursorIncr[]=
{1,10,100,1000,10000};
#endif

/*接口函数------------------------------------------------------------------------------------*/

//菜单初始化
void CMenu_MenuInit(CMenuItem* pRootMenu)
{
	//初始化菜单变量
	menuDC.theMenu.m_menuIndex = 0;			/*菜单索引初始化*/
	menuDC.theMenu.m_pMenuList = pRootMenu;	/*要显示的菜单列表*/
	menuDC.pMenuItemPtr = pRootMenu;		/*当前菜单项指针*/

	#if SMENU_01_CURSOR==1
	menuDC.cursorIndex = 0;					/*光标数字调整位*/
	menuDC.cursorTimer = MENU_CURSOR_OFF;	/*光标定时器*/
	#endif

	CMenu_SetFocus(CMenuObject_Proc);				/*输入焦点*/
	CMsgPr_SendMessage(CMenu_GetFocus(),WM_INIT);	/*初始化控件*/
	CMsgPr_EndMessage();							/*结束消息传递*/

	//回调菜单开始函数
	CMenu_Start();
}
//数值限幅处理
void CMenu_SetValueLimit(void)
{
	int max;				/*最大值*/
	int min;				/*最小值*/

	max = CMenu_GetMenuItemPtr()->max;	/*最大值*/
	min = CMenu_GetMenuItemPtr()->min;	/*最小值*/
	
	//限幅处理
	if (menuDC.cValue>max){menuDC.cValue=max;}
	else if (menuDC.cValue<min){menuDC.cValue=min;}
}
//设置输入焦点
void CMenu_SetFocus(MsgPrce aFocus)
{
	menuDC.pFocus=aFocus;			/*当前输入焦点*/
	CMsgPr_SetMsgObject(aFocus);	/*当前消息目标*/
}

//光标调整常量
#if SMENU_01_CURSOR==1
//打开光标定时器
void CMenu_CursorTurnOn(void)
{
	menuDC.cursorTimer=MENU_CURSOR_SHOW;	/*光标显示*/
	menuDC.cursorIndex=0;					/*光标索引初始化*/
}
#endif

//按键事件，需定时调用
void CMenu_KeyEvent(unsigned char aKey)
{
	//按键状态机处理
	if (WM_KEY_NULL == aKey){CKey_PullKey();}
	else {CKey_PushKey();}

	//按键事件派发
	if (CKey_GetState() == KEY_STATE_PUSH)
	{
		CMenu_MenuEvent(aKey);		/*按键事件*/
		CMenu_ResetCursorTimer();	/*复位光标定时器*/
	}
	else if (CKey_GetState()==KEY_STATE_M_PUSHED&& 
		aKey != WM_KEY_SET)
		CMenu_MenuEvent(aKey);
}
//显示菜单标题
void CMenu_DrawMenuText(void)
{
	unsigned char i;
	//拷贝显示数据
	for (i=0;i<SMENU_TITLE_LEN;++i)
		segDisBuff[i+SMENU_TITLE_POSIT]=CMenu_GetMenuItemPtr()->str[i];
}
//显示菜单数值
void CMenu_DrawMenuValue(unsigned char x,unsigned char width, unsigned short value)
{
	unsigned char strTemp[5],i;

	for (i=0;i<5;++i)
	{
		strTemp[4-i]=SegDigCode[value%10];
		value /= 10;
	}
	//灭0处理
	for (i=0;i<5;++i)
	{
		if (i>=(5-width))
			break;

		if (strTemp[i]==SEG7_CODE_0)
			strTemp[i]=SEG7_CODE_NULL;
		else
			break;
	}
	//显示
	for (i=0;i<CURSOR_NUMBER;++i)
		segDisBuff[CURSOR_NUMBER+x-1-i]=strTemp[4-i];
}

#if SMENU_01_CURSOR==1
//光标定时器
void CMenu_CursorEvent(void)
{
	//光标定时器打开 定时发送定时消息
	if (MENU_CURSOR_OFF==menuDC.cursorTimer)
		return;

	CMenu_MenuEvent(WM_TIMER);
}
#endif
