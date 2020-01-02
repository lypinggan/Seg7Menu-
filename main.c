
#include <AT89X55.H>
#include "sysos.h"
#include "key.h"
#include "menu.h"
#include "seg7font.h"
#include "SegDynamicScan.h"

#define BIT0 (1<<0)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)

unsigned short config[10];
unsigned char aKey;

//菜单描述
code const CMenuItem menuList[]=
{
	{{0xd4,0,0x06,0},MENU_VALUTYPE_U16,-9999,9999,&config[0],CValueMenu_Proc},
	{{0xd4,0,0x5b,0},MENU_VALUTYPE_U16,0,100,&config[1],CValueMenu_Proc},
	{{0xd4,0,0x4f,0},MENU_VALUTYPE_U16,0,100,&config[2],CValueMenu_Proc},
	{{0xd4,0,0x66,0},MENU_VALUTYPE_U16,0,100,&config[3],CValueMenu_Proc},
	{{0xd4,0,0x6d,0},MENU_VALUTYPE_U16,0,100,&config[4],CValueMenu_Proc},
	{{0xd4,0,0x7d,0},MENU_VALUTYPE_U16,0,100,&config[5],CValueMenu_Proc},
	{{0xd4,0,0x07,0},MENU_VALUTYPE_U16,0,100,&config[6],CValueMenu_Proc},
	{{0xd4,0,0x7f,0},MENU_VALUTYPE_U16,0,100,&config[7],CValueMenu_Proc},
	{{0xd4,0,0x6f,0},MENU_VALUTYPE_U16,0,100,&config[8],CValueMenu_Proc},
	{0,0,0,0,0,0}
};

unsigned char keyTimer,cursorTimer;

//1ms定时器常量
#define TIME_TIME0_SYSTEM 0xff00

void systemInit(void)
{
	//初始化定时器0
	//定时器0作为步进电机运行定时器
	TMOD = BIT0|BIT4;		//定时器0、1都为模式1
	TR0 = 0;
	TH0 = (unsigned char)(TIME_TIME0_SYSTEM>>8);
	TL0 = (unsigned char)TIME_TIME0_SYSTEM;
	ET0 = 1;
	TR0 = 1;

	EA = 1;
}
void main(void)
{

	systemInit();
	CMenu_MenuInit(menuList);
	
	while (1)
	{
		OS_BLOCK_BEGIN(taskMask.m_timermS)
		{
			//扫描显示
			SegDynamicScan();

			if (++keyTimer > 10)
			{
				keyTimer = 0;
				OS_TASK_READY(taskMask.m_keyScan);
			}
			if (++cursorTimer>50)
			{
				cursorTimer=0;
				OS_TASK_READY(taskMask.m_cursor);
			}
		}
		OS_BLOCK_END();
		//按键扫描
		OS_BLOCK_BEGIN(taskMask.m_keyScan)
		{
			P2 = 0xff;

			if (!P2_0) {aKey=WM_KEY_ADD;}
			else if (!P2_1){aKey = WM_KEY_SET;}
			else if (!P2_2){aKey = WM_KEY_SUB;}
			else if (!P2_3){aKey = WM_KEY_LEFT;}
			else if (!P2_4){aKey = WM_KEY_RIGHT;}
			else {aKey = WM_KEY_NULL;}

			CMenu_KeyEvent(aKey);
		}
		OS_BLOCK_END()
		//光标
		OS_BLOCK_BEGIN(taskMask.m_cursor)
		{
			CMenu_CursorEvent();
		}
		OS_BLOCK_END()
		
	}
}

void interrupt_time0_handle(void) interrupt 1
{
	TR0 = 0;
	TH0 = (unsigned char)(TIME_TIME0_SYSTEM>>8);
	TL0 = (unsigned char)TIME_TIME0_SYSTEM;
	TR0 = 1;
	OS_TASK_READY(taskMask.m_timermS);
}
//菜单开始
void CMenu_Start(void)
{
}
//菜单结束
void CMenu_Quit(void)
{
	CMenu_MenuInit(menuDC.theMenu.m_pMenuList);
}

