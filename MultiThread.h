/**  
* @file MultiThread.h  
* @brief 实现了跨平台(Windows、Linux、VxWorks)线程、互斥、信号  
* @author Donhao  
* @date 2010-3-16 12:49:30  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
  
#ifndef _MULTITHREAD_H   
#define _MULTITHREAD_H   
  
/**  
* @CommonOSHeadFile  
* @brief 跨平台系统头文件  
*  
* Windowx、Linux、VxWorks共同使用的头文件.  
* @{  
*/  
#include <string.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <ctype.h>   
/** @} */  
  
/**  
* @OSHeadFile  
* @brief 操作系统相关头文件  
*  
* Windowx、Linux、VxWorks各自使用的头文件.  
* @{  
*/  
#ifdef WIN32   
#define OS_WINDOWS WIN32   
#include <time.h>   
#include <winsock2.h>   
#include "process.h"   
#pragma comment(lib, "Ws2_32.lib")   
#include <direct.h>   
#include "tlhelp32.h"   
#include "shellapi.h"   
#elif LINUX   
#include <arpa/inet.h>   
#include <dirent.h>   
#include <errno.h>   
#include <fcntl.h>   
#include <math.h>   
#include <netinet/ip_icmp.h>   
#include <netdb.h>   
#include <netinet/in.h>   
#include <netinet/ip.h>   
#include <netinet/tcp.h>   
#include <pthread.h>   
#include <sched.h>   
#include <signal.h>   
#include <string.h>   
#include <stropts.h>   
#include <sys/procfs.h>   
#include <sys/socket.h>   
#include <sys/stat.h>   
#include <sys/time.h>   
#include <sys/types.h>   
#include <sys/vfs.h>   
#include <time.h>   
#include <unistd.h>   
#else   
#include <vxworks.h>   
#include <config.h>   
#include <configAll.h>   
#include <dosFsLib.h>   
#include <errnoLib.h>   
#include <flioctl.h>   
#include <fldrvvxw.h>   
#include <ftpLib.h>   
#include <hostLib.h>   
#include <inetLib.h>   
#include <intLib.h>   
#include <loadLib.h>   
#include <ioLib.h>   
#include <logLib.h>   
#include <msgQLib.h>   
#include <netinet/in.h>   
#include <netinet/ip.h>   
#include <netinet/tcp.h>   
#include <semLib.h>   
#include <signal.h>   
#include <sioLib.h>   
#include <sockLib.h>   
#include <sntp.h>   
#include <sntpcLib.h>   
#include <symLib.h>   
#include <sysLib.h>   
#include <sys/socket.h>   
#include <sys/types.h>   
#include <taskLib.h>   
#include <tickLib.h>   
#include <time.h>   
#include <wdLib.h>   
#endif   
/** @} */  
  
#ifdef WIN32   
typedef unsigned        THREAD;   
typedef HANDLE          MUTEX;   
typedef HANDLE          CONDITION;   
typedef unsigned        THREAD_FUNC_RETURN;   
typedef THREAD_FUNC_RETURN(__stdcall* THREAD_FUNC)(void* args);   
#define THREAD_CALL __stdcall   
#elif LINUX   
typedef pthread_t       THREAD;   
typedef pthread_mutex_t MUTEX;   
typedef pthread_cond_t  CONDITION;   
typedef void*           THREAD_FUNC_RETURN;   
typedef THREAD_FUNC_RETURN(* THREAD_FUNC)(void* args);   
#define THREAD_CALL   
#else   
typedef int             THREAD;   
typedef SEM_ID          MUTEX;   
typedef SEM_ID          CONDITION;   
typedef int             THREAD_FUNC_RETURN;   
typedef FUNCPTR         THREAD_FUNC;   
#define THREAD_CALL   
#endif   
  
/** 任务参数结构体  */  
  
typedef struct  
{   
    int args0;   
    int args1;   
    int args2;   
    int args3;   
    int args4;   
    int args5;   
    int args6;   
    int args7;   
    int args8;   
    int args9;   
}Thread_Arg_Struct;   
  
/**  
* @name Thread  
* @brief 实现了线程创建、销毁、获得线程ID  
* @{  
*/  
/**  
* @brief Thread_Create  
*  
* @param[out] thread 线程句柄(若为NULL则不输出线程句柄)  
* @param[in] myfunc 线程函数  
* @param[in] threadName 线程名(仅VxWorks用到)  
* @param[in] stackSize 线程堆栈大小(仅VxWorks用到)  
* @param[in] priority 优先级(仅VxWorks用到)  
* @param[in] args 参数  
* @return BOOL  
*/  
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* pArgs);   
  
/**  
* @brief Thread_Self  
*  
* @return THREAD  
*/  
THREAD Thread_Self(void);   
/** @} */  
  
/**  
* @name Mutex  
* @brief 互斥器  
*  
* @{  
*/  
/**  
* @brief Mutex_Init  
*  
* 初始化互斥器.  
* @param[out] mutex  
* @return BOOL  
*/  
BOOL Mutex_Init(MUTEX* pMutex);   
  
/**  
* @brief Mutex_Destroy  
*  
* 销毁互斥器.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Destroy(MUTEX mutex);   
  
/**  
* @brief Mutex_Lock  
*  
* 获得互斥器资源.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Lock(MUTEX mutex);   
  
/**  
* @brief Mutex_Unlock  
*  
* 释放互斥器.  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Mutex_Unlock(MUTEX mutex);   
/** @} */  
  
  
/**  
* @name Condition  
* @brief 事件  
* @{  
*/  
  
/**  
* @brief Condition_Init  
*  
* 初始化Condition.  
* @param[out] cond  
* @return BOOL  
*/  
BOOL Condition_Init(CONDITION* pCond);   
  
/**  
* @brief Condition_Destroy  
*  
* 销毁Condition.  
* @param[in] cond  
* @return BOOL  
*/  
BOOL Condition_Destroy(CONDITION cond);   
  
/**  
* @brief Condition_Wait  
*  
* 等待Condition.  
* @param[in] cond  
* @param[in] mutex  
* @return BOOL  
*/  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex);   
  
/**  
* @brief Condition_Signal  
*  
* 通知Condition.  
* @param[in] cond  
* @return BOOL  
*/  
BOOL Condition_Signal(CONDITION cond);   
/** @} */  
#endif  

//
//本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/donhao/archive/2010/09/04/5862935.aspx