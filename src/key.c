/***********************************************************************************************
文件名	key.c
日期	2013.3.1
作者	张诗星	陈东辉
文件说明	按键状态机文件
修订说明:

2013.3.1  张诗星 初始版本
***********************************************************************************************/
/*头文件--------------------------------------------------------------------------------------*/
#include "key.h"


/*全局变量------------------------------------------------------------------------------------*/

//按键输入
CKey keyInput;

/*接口函数------------------------------------------------------------------------------------*/

//按下按键
void CKey_PushKey(void)
{
	//按键状态机
	switch(CKey_GetState())
	{
		/*按键释放状态进入按键按下状态*/
		case KEY_STATE_PULLED:
		case KEY_STATE_PULL:
			CKey_SetState(KEY_STATE_PUSH);		/*进入按键按下状态*/
			break;
		/*按键按下状态*/
		case KEY_STATE_PUSH:
			CKey_SetState(KEY_STATE_PUSHCHECK);	/*进入按键按下确认状态*/
			break;
		/*按键按下确认状态*/
		case KEY_STATE_PUSHCHECK:	
			keyInput.m_keyState = KEY_STATE_PUSHED;		/*进入按键已按下状态*/
			keyInput.m_keyCount = KEY_TIME_M_PUSH;		/*设置连按延时*/
			break;
		/*按键已按下状态，延时进入连按状态*/
		case KEY_STATE_PUSHED:
			if (!keyInput.m_keyCount)
				keyInput.m_keyState = KEY_STATE_M_PUSHED;	/*连按延时到，进入已连按状态*/
			break;
		/*按键已进入连按状态*/
		case KEY_STATE_M_PUSHED:			/*连按状态*/
			break;
		default:
			keyInput.m_keyState = KEY_STATE_PULL;
			break;
	}

	/*计时处理*/
	if (keyInput.m_keyCount){--keyInput.m_keyCount;}
}
//松开按键
void CKey_PullKey(void)
{
	switch(CKey_GetState())
	{
		/*按键进入释放状态*/
		case KEY_STATE_PULL:		/*按键释放*/
			keyInput.m_keyState = KEY_STATE_PULLED;	/*进入按键已释放状态*/
			break;
		/*按键已释放状态*/
		case KEY_STATE_PULLED:		/*按键已释放*/
			break;
		default:
			keyInput.m_keyState = KEY_STATE_PULL;	/*进入按键释放状态*/
			break;
	}
	/*计时处理*/
	if (keyInput.m_keyCount){--keyInput.m_keyCount;}
}

