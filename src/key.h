/***********************************************************************************************
文件名	key.h
日期	2013.3.1
作者	张诗星	陈东辉
文件说明	按键状态机头文件
修订说明:

2013.3.1  	张诗星 	初始版本
2014.2.24	张诗星	增加对连按支持 取消直接的按键消息支持
***********************************************************************************************/

/*头文件--------------------------------------------------------------------------------------*/

#ifndef _MY_KEY_H_
#define _MY_KEY_H_

/*用户配置------------------------------------------------------------------------------------*/
#define KEY_TIME_M_PUSH		20		/*进入连按延时*/

/*宏常量--------------------------------------------------------------------------------------*/
// 按键状态常量定义
#define KEY_STATE_PULLED 	0		/*已释放状态*/
#define KEY_STATE_PULL 		1		/*进入释放状态*/	
#define KEY_STATE_PUSH 		2		/*进入按下状态*/
#define KEY_STATE_PUSHCHECK 3 		/*按键按下检查状态，用于去除干扰*/
#define KEY_STATE_PUSHED 	4		/*已按下状态*/
#define KEY_STATE_M_PUSHED 	5		/*已连按状态*/

/*类型定义-------------------------------------------------------------------------------------*/

//类型定义
typedef struct
{
	unsigned char m_keyState;	/*按键状态*/
	unsigned short m_keyCount;	/*按键计时计数器*/
}CKey;
/*外部变量-------------------------------------------------------------------------------------*/
//按键输入
extern CKey keyInput;

/*接口声明-------------------------------------------------------------------------------------*/

// 按下按键
void CKey_PushKey(void);
// 松开按键
void CKey_PullKey(void);

//获取按键状态
#define CKey_GetState() ((const unsigned char)keyInput.m_keyState)
//设置按键状态
#define CKey_SetState(X) (keyInput.m_keyState=(X))
//重置按键为已释放状态
#define CKey_InitPulled() (keyInput.m_keyState=KEY_STATE_PULLED;)
//重置按键为已释放状态
#define CKey_InitPussed() (keyInput.m_keyState=KEY_STATE_PUSHED;)

#endif
