/***********************************************************************************************
文件名	Menu_00.c
日期	2014.02.28
作者	张诗星	陈东辉
文件说明	菜单模式00 单屏无浏览模式
修订说明:

2013.02.16  张诗星 初始版本
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/
#include "Menu.h"
#include "Seg7Font.h"
//#include ""

#if SMENU_MODE_SETUP==0

/*接口函数------------------------------------------------------------------------------------*/

//菜单管理基类 用于管理菜单跳转
void CMenuObject_Proc(void)
{
	switch(CMsgPr_GetMessage())	
	{
		//初始化消息
		case WM_INIT:
			/*计算当前菜单项指针 以方便访问*/
			menuDC.pMenuItemPtr=menuDC.theMenu.m_pMenuList+menuDC.theMenu.m_menuIndex;
			//判断是菜单否结束
			if (0 == CMenu_GetMenuItemPtr()->m_pfun){CMenu_Quit();break;}
			#if SMENU_01_CURSOR==1
			CMenu_CursorTurnOff();					/*关闭光标定时器*/
			#endif
			CMsgPr_PollMessage(WM_PAINT);			/*投递显示消息*/
			break;
		//下个菜单项
		case WM_MENU_NEXT:	
			++menuDC.theMenu.m_menuIndex;
			CMsgPr_PollMessage(WM_INIT);			/*投递初始化消息*/
			break;
		//绘制消息
		case WM_PAINT:
			CMenu_DrawMenuText();					/*绘制菜单标题*/
			break;
		//设置键
		case WM_KEY_SET:
			//按下SET键进入数值调整控件
			CMenu_SetFocus(CMenu_GetMenuItemPtr()->m_pfun);	//当前输入焦点
			CMsgPr_PollMessage(WM_INIT);					//INIT消息
			break;
		default:					/*默认消息结束消息传递*/
			CMsgPr_EndMessage();
			break;
	}
}
//菜单数值调整基类 所有数值调整的父类
void CValueObject_Proc(void)
{
	switch(CMsgPr_GetMessage())
	{
		case WM_INIT:		/*初始化菜单控件消息*/
			CMsgPr_PollMessage(WM_PAINT);			/*投递显示消息*/
			#if SMENU_01_CURSOR==1
			CMenu_CursorTurnOn();					/*打开光标定时器*/
			#endif
			break;
		case WM_KEY_SET:	/*设置键消息*/
			//SET键按下进入菜单管理控件
			CMenu_SetFocus(CMenuObject_Proc);		//设置输入焦点
			CMsgPr_PollMessage(WM_MENU_NEXT);		/*给下个菜单项发送初始化消息*/
			break;
		default:									/*默认消息结束消息传递*/
			CMsgPr_EndMessage();
			break;
	}
}
//数值菜单消息例程
void CValueMenu_Proc(void)
{
	unsigned char amsg;
	//消息处理
	switch(amsg=CMsgPr_GetMessage())
	{
		case WM_INIT:			/*初始化消息*/
			//属性
			if (CMenu_GetMenuItemPtr()->attri|MENU_VALUTYPE_U16)	/*双字节*/
				menuDC.cValue = *(unsigned short*)CMenu_GetMenuItemPtr()->m_pdata;
			else							/*单字节*/
				menuDC.cValue = *(unsigned char*)CMenu_GetMenuItemPtr()->m_pdata;
			
			CMenu_SetValueLimit();		//限幅处理
			CMsgPr_ParentMessage(CValueObject_Proc,WM_INIT);/*基类初始化消息*/
			break;
		case WM_PAINT:				/*显示消息*/
			CMenu_DrawMenuValue(SMENU_VALUE_POSIT,VALUE_ZERO_MIN,menuDC.cValue);
			break;
		case WM_KEY_SET:			/*设置键消息*/
			//限幅处理
			CMenu_SetValueLimit();		//限幅处理
			//保存数值
			if (CMenu_GetMenuItemPtr()->attri|MENU_VALUTYPE_U16)	/*双字节*/
				*(unsigned short*)CMenu_GetMenuItemPtr()->m_pdata = menuDC.cValue;
			else							/*单字节*/
				*(unsigned char*)CMenu_GetMenuItemPtr()->m_pdata = menuDC.cValue;
			CMsgPr_ParentMessage(CValueObject_Proc,WM_KEY_SET);	/*基类SET消息*/
			break;
		case WM_KEY_ADD:	/*加按键消息*/
			#if SMENU_01_CURSOR==1
			menuDC.cValue+=CursorIncr[menuDC.cursorIndex];
			#else
			++menuDC.cValue;
			#endif
			CMenu_SetValueLimit();				//限幅处理
			CMsgPr_PollMessage(WM_PAINT);	/*投递显示消息*/
			break;
		case WM_KEY_SUB:	/*减按键消息*/
			#if SMENU_01_CURSOR==1
			menuDC.cValue-=CursorIncr[menuDC.cursorIndex];
			#else
			--menuDC.cValue;
			#endif
			CMenu_SetValueLimit();				//限幅处理
			CMsgPr_PollMessage(WM_PAINT);	/*投递显示消息*/
			break;
		//left 
		#if SMENU_01_CURSOR==1
		case WM_KEY_LEFT:
			//调整位循环
			#if SMENU_01_DIG_LOOP==0	//打开调整位循环
			if (menuDC.cursorIndex<(CURSOR_NUMBER-1)) {++menuDC.cursorIndex;}
			#else						//关闭调整位循环
			if (++menuDC.cursorIndex>(CURSOR_NUMBER-1)) {menuDC.cursorIndex = 0;}
			#endif
			break;
		case WM_KEY_RIGHT:
			//调整位循环
			#if SMENU_01_DIG_LOOP==0	//打开调整位循环
			if (menuDC.cursorIndex>0){--menuDC.cursorIndex;}
			#else						//关闭调整位循环
			if (!menuDC.cursorIndex){menuDC.cursorIndex=(CURSOR_NUMBER-1);}
			else {--menuDC.cursorIndex;}
			#endif
			break;
		//定时器消息
		case WM_TIMER:
			if (MENU_CURSOR_SHOW==CMenu_GetCursorState())
			{
				CMenu_DrawMenuValue(SMENU_VALUE_POSIT,menuDC.cursorIndex+1,menuDC.cValue);
				menuDC.cursorTimer=MENU_CURSOR_HIDE;
			}
			else
			{
				segDisBuff[SMENU_VALUE_POSIT+CURSOR_NUMBER-1-menuDC.cursorIndex]=CURSOR_CHARACTER;
				menuDC.cursorTimer=MENU_CURSOR_SHOW;
			}
			break;
		#endif
		default:
			CMsgPr_ParentMessage(CValueObject_Proc,amsg);/*未处理消息传给父类*/
			break;
	}
}
#endif
