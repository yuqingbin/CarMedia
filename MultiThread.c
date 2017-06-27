/**  
* @file MultiThread.c  
* @brief 实现了跨平台多线程、互斥、信号功能.  
* @author Donhao  
* @date 2010-3-16 12:51:42  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  

////在下面几种情况下，同步是重要的： 
//· 在两个或更多个进程内的线程可以合用一个同步变量。注意，同步变量应当被一个进程初始化，在第二次初始化时，该同步变量被设置为解锁状态。 
//· 同步是唯一保证共享数据持久的办法。 
//· 一个进程可以映射一个文件并通过一个线程将其加锁，修改完成之后，该线程释放文件锁并恢复文件。在文件加锁的过程中，任何程序中的任何 线程想要加锁时都会阻塞，直至解锁； 
//当你创建一个线程时，其实那个线程是一个循环，不像上面那样只运行一次的。这样就带来了一个问题，
//在那个死循环里要找到合适的条件退出那个死循环，那么是怎么样实现它的呢？在Windows里往往是采用事件的方式，
//当然还可以采用其它的方式。在这里先介绍采用事件的方式来通知从线程运行函数退出来，它的实现原理是这样，
//在那个死循环里不断地使用WaitForSingleObject函数来检查事件是否满足，如果满足就退出线程，不满足就继续运行。
//当在线程里运行阻塞的函数时，就需要在退出线程时，先要把阻塞状态变成非阻塞状态，比如使用一个线程去接收网络
//数据，同时使用阻塞的SOCKET时，那么要先关闭SOCKET，再发送事件信号，才可以退出线程的。
//下面就来演示怎么样使用事件来通知线程退出来。



#include "MultiThread.h"   
  #ifdef UNICODE 
#define CreateMutex  CreateMutexW 
#else 
#define CreateMutex  CreateMutexA 
#endif // !UNICODE 

#ifdef WIN32   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* pargs)   
{   
    HANDLE h             = 0;   
    THREAD tmpThread = 0;   
    BOOL rt          = FALSE;   
  
    if (NULL == thread)   
    {   
        h = (HANDLE)_beginthreadex(NULL, stackSize, myfunc, pargs, 0, &tmpThread);   
    }   
    else  
    {   
        h = (HANDLE)_beginthreadex(NULL, stackSize, myfunc, pargs, 0, thread);   
    }   
  
    if (h > 0)   
    {   
        rt = SetThreadPriority(h, priority);   
  
        if (FALSE == rt)   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
 //get the calling thread ID,a owner capability for the current thread 
THREAD Thread_Self(void)   
{   
    THREAD td = GetCurrentThreadId();   
  
    return td;   
}   
  //初始化一个互斥锁
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  //创建一个有名或无名的互斥量对象
	//如果函数成功执行，将返回一个互斥量对象的句柄。如果在CreateMutex()执行前已经存在有相同名字的互斥量，
	//函数将返回这个已经存在互斥量的句柄，并且可以通过GetLastError()得到错误代码ERROR_ALREADY_EXIST。
	//CreateMutex() 用于有独占要求的程序 (在其进程运行期间不允许其他使用此端口设备的程序运行，
	//或不允许同名程序运行)。如有同名程序运行，则通过 GetLastError()得到错误代码 ERROR_ALREADY_EXIST。

 //TRUE代表主线程拥有互斥对象 但是主线程没有释放该对象  互斥对象谁拥有 谁释放
    //FLASE代表当前没有线程拥有这个互斥对象

    *pMutex = CreateMutex(NULL, FALSE, NULL);   
  
    if (0 == (*pMutex))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
//解除互斥状态 
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    int rt = 0;   
  //用CloseHandle来释放Mutex时，只是释放当前进程获得的副本句柄，而不会删除该Mutex内核对象。

    rt = CloseHandle(mutex);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   

//给一个互斥锁加锁 用互斥锁可以使线程顺序执行。互斥锁通常只允许一个线程执行一个关键部分的代码，来同步线程。互斥锁也可以用来保护单线程代码。 
//如果两个进程有共享且可写的内存，且做了相应的初始化设置后(参见mmap(2))，互斥锁可以实现进程间的线程同步。 
//互斥锁在使用前一定要初始化。 
//多线程等待一个互斥锁时，其获得互斥锁的顺序是不确定的。

BOOL Mutex_Lock(MUTEX mutex)   
{   
    DWORD rt = 0;   
  
    rt = WaitForSingleObject(mutex, INFINITE);   
   //The process terminated
    if (WAIT_OBJECT_0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
//解锁
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    int rt = 0;   
//ReleaseMutex只能由创建该Mutex的进程执行，不然总是返回错误。
  
    rt = ReleaseMutex(mutex);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
 /*  该函数创建一个Event同步对象,并返回该对象的Handle.创建或打开一个命名的或无名的事件对象   
	Param1:lpEventAttributes是事件的属性。
	Param2:bManualReset是复位方式:指事件手动复位(TRUE)，还是自动复位状态。
	Param3:bInitialState是初始状态:初始化的状态是否处于有信号的状态。,true,有信号,false无信号 
	Param4:lpName是事件的名称，如果有名称，可以跨进程共享事件
	*/
    *pCond = CreateEvent(NULL, FALSE, FALSE, NULL);   
  
    if (0 == (*pCond))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = CloseHandle(cond);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    int rt = 0;   
  //分别将EVENT置为这两种状态分别是发信号与不发信号。

    rt = SetEvent(cond);   
  
    if (0 == rt)   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    DWORD rt = TRUE;   
  
    rt = Mutex_Unlock(mutex);   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    rt = WaitForSingleObject(cond, INFINITE);   
  
    if (WAIT_OBJECT_0 != rt)   
    {   
        return FALSE;   
    }   
  
    rt = Mutex_Lock(mutex);   
  
    return rt;   
}   
  
#elif LINUX   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct* args)   
{   
    THREAD tmpThread = -1;   
    THREAD rt        = -1;   
  
    if (NULL == thread)   
    {   
        rt = pthread_create(&tmpThread, NULL, myfunc, &args);   
    }   
    else  
    {   
        rt = pthread_create(thread, NULL, myfunc, &args);   
    }   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
  
    return rt;   
}   
  
THREAD Thread_Self(void)   
{   
    THREAD rt = pthread_self();   
  
    return rt;   
}   
  
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    int rt = 0;   
  
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  
    rt = pthread_mutex_init(pMutex, NULL);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_destroy(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Lock(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_lock(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_mutex_unlock(&mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    int rt = 0;   
  
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
  
    rt = pthread_cond_init(pCond, NULL);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = pthread_cond_destroy(&cond);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    int rt = 0;   
  
    rt = pthread_cond_wait(&cond, &mutex);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    int rt = 0;   
  
    rt = pthread_cond_signal(&cond);   
  
    if (0 == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
#else   
BOOL Thread_Create(THREAD* thread, THREAD_FUNC myfunc, char* threadName, int stackSize, int priority, Thread_Arg_Struct * pargs)   
{   
    THREAD tmpThread = 0;   
  
    if (NULL == thread)   
    {   
        tmpThread = taskSpawn(threadName, priority, VX_FP_TASK, stackSize, (FUNCPTR)myfunc, (int)pargs, 1, 2, 3, 4, 5, 6, 7, 8, 9);   
  
        if (ERROR == tmpThread)   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
    else  
    {   
        *thread = taskSpawn(threadName, priority, VX_FP_TASK, stackSize, (FUNCPTR)myfunc, (int)pargs, 1, 2, 3, 4, 5, 6, 7, 8, 9);   
  
        if (ERROR == (*thread))   
        {   
            return FALSE;   
        }   
        else  
        {   
            return TRUE;   
        }   
    }   
}   
  
THREAD Thread_Self(void)   
{   
    THREAD td = taskIdSelf();   
  
    return td;   
}   
  
BOOL Mutex_Init(MUTEX* pMutex)   
{   
    if (NULL == pMutex)   
    {   
        return FALSE;   
    }   
  
    *pMutex = semBCreate(SEM_Q_FIFO, SEM_FULL);   
  
    if (NULL == (*pMutex))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Mutex_Destroy(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semTake(mutex, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        semDelete(mutex);   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Lock(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semTake(mutex, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Mutex_Unlock(MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = semGive(mutex);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Init(CONDITION* pCond)   
{   
    if (NULL == pCond)   
    {   
        return FALSE;   
    }   
  
    *pCond = semBCreate(SEM_Q_FIFO, SEM_EMPTY);   
  
    if (NULL == (*pCond))   
    {   
        return FALSE;   
    }   
    else  
    {   
        return TRUE;   
    }   
}   
  
BOOL Condition_Destroy(CONDITION cond)   
{   
    STATUS rt = OK;   
  
    rt = semTake(cond, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        semDelete(cond);   
  
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Wait(CONDITION cond, MUTEX mutex)   
{   
    STATUS rt = OK;   
  
    rt = Mutex_Unlock(mutex);   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    rt = semTake(cond, WAIT_FOREVER);   
  
    if (OK == rt)   
    {   
        rt =  Mutex_Lock(mutex);   
  
        return rt;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
BOOL Condition_Signal(CONDITION cond)   
{   
    STATUS rt = OK;   
  
    if (NULL == cond)   
    {   
        return FALSE;   
    }   
  
    rt = semGive(cond);   
  
    if (OK == rt)   
    {   
        return TRUE;   
    }   
    else  
    {   
        return FALSE;   
    }   
}   
  
#endif  


//本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/donhao/archive/2010/09/04/5862935.aspx