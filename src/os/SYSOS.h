/***********************************************************************************************
文件名	SYSOS.h
日期	2013.1.13
作者	张诗星	陈东辉
修订	2013.1.13
文件说明	协作式多任务OS
修订说明	初始版本
2013.06.07	张诗星	增加任务退出宏
2013.08.06	张诗星	增加空闲任务和统计任务用于统计CPU使用率
***********************************************************************************************/
#ifndef _SYS_OS_ZX
#define _SYS_OS_ZX

/*头文件--------------------------------------------------------------------------------------*/
//#include ""

/*用户配置------------------------------------------------------------------------------------*/
//任务标志类型
typedef struct
{
	unsigned char m_timermS:1;
	unsigned char m_keyScan:1;
	unsigned char m_cursor:1;
}OS_CTaskMask;

//开关配置
#define OS_01_STATITASK		0			/*使能统计CPU使用率*/
/*宏操作--------------------------------------------------------------------------------------*/

//任务块开始
#define OS_BLOCK_BEGIN(X) 		\
	if((volatile)(X))			\
	{							\
		(X)=0;					\
		
//任务块结束
#define OS_BLOCK_END() 			\
	continue;					\
	}							\

//结束任务
#define OS_TASK_END()	continue

//任务标志有效
#define OS_TASK_READY(X) ((X)=1)
//任务阻塞
#define OS_TASK_BLOCK(X) ((X)=0)

/*外部变量------------------------------------------------------------------------------------*/

extern OS_CTaskMask taskMask;

#if OS_01_STATITASK==1
extern unsigned long OS_IdleCtr;		/*空闲任务计数变量*/
extern unsigned long OS_IdleCtrMax;		/*空闲任务计数全速运行最大值*/
extern unsigned char OS_CPUPerc;		/*CPU使用百分比*/

//空闲任务
void OS_IdleTask(void);
//统计任务 每秒调用一次
void OS_StatisTask(void);
//获取CPU使用率
#define OS_GetCPUPerc() ((const)(OS_CPUPerc))
#endif

#endif
