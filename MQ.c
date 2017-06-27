/**  
* @file MQ.c  
* @brief  
* @author Donhao  
* @date 2010-4-17 12:57:43  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
#include "MQ.h"   
  
#define MQ_INITIALIZE_FLAG 0x1122   
/**  读操作
 * 同理，读操作分如下几个步骤：
 *
 * 检查信号量； 
 * 获取锁； 
 * 判断队列是否为空； 
 * 若不为空，则读取 read_ops 处的数据，将 read_ops 增 1，并判断 read_pos 是否越界； 
 * 并释放锁。 
 */
/**  
* @brief MQ_Read  
*  
* Detailed description.  
* @param[in] pMQ 要读的消息队列  
* @param[in] pOutBuf 要输出消息的缓冲区  
* @param[in/out] pSize 作为输入参数时为输出的缓冲区消息大小，作为输出参数为读到的消息大小  
* @return BOOL  
*/  
BOOL MQ_Read(MQ_Struct* pMQ, UINT8* pOutBuf, UINT32* pSize)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pOutBuf) || (NULL == pSize)   
        || (0 == pMQ->msgNum) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = FALSE;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        pMQ->front = (pMQ->front + 1) % pMQ->maxMsgNum;   
  
        --(pMQ->msgNum);   
  /* check if write_pos if overflow. */

        if (pMQ->pMQMsg[pMQ->front].msgSize > *pSize)   
        {   
            rt = FALSE;   
        }   
        else  
        {   
            memcpy(pOutBuf, pMQ->pMQMsg[pMQ->front].pMsg, pMQ->pMQMsg[pMQ->front].msgSize);   
            *pSize = pMQ->pMQMsg[pMQ->front].msgSize;   
  
            rt = TRUE;   
        }   
  /* release lock*/

        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   

  
/**  
* @brief MQ_Get  
*  
* 仅读取数据，而不弹出.  
* @param[in] pMQ  要读的消息队列
* @param[in] index  
* @param[out] pOutBuf  要输出消息的缓冲区
* @param[out] pSize  输出参数为读到的消息大小
* @return BOOL  
*/  
BOOL MQ_Get(MQ_Struct* pMQ, UINT32 index, UINT8* pOutBuf, UINT32* pSize)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pOutBuf) || (NULL == pSize)   
        || (index >= pMQ->msgNum) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = FALSE;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        if (pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize < *pSize)   
        {   
            rt = FALSE;   
        }   
        else  
        {   
            memcpy(pOutBuf, pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].pMsg,   
                       pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize);   
            *pSize = pMQ->pMQMsg[(pMQ->front + 1 + index) % pMQ->maxMsgNum].msgSize;   
  
            rt = TRUE;   
        }   
  
        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   
/**写操作
 * 如上面的流程图介绍，写操作主要包括如下几步：
 * - 获取锁；
 * 判断队列是否已满； 
 * 若没满，将数据写入 write_pos 处，将 write_pos 增 1，并判断 write_pos 是否越界； 
 * 释放锁，并将信号量增 1。 
 */
/**
 *若往环形队列写数据，首先要获得锁， 若锁已被占用，则进入等待状态，否则进一步去判断环形队列是否已满。
 *若满了，则释放锁并返回；若队列未满，将数据写入 write_pos 位置，write_pos 增 1，释放锁并将信号量增 1，
 *表示已写入一个数据； 
*/
MQ_Write_Result_Enum MQ_Write(MQ_Struct* pMQ, const UINT8* pMsg, UINT32 size)   
{   
    BOOL rt = FALSE;   
  
    if ((NULL == pMQ) || (NULL == pMsg) || (size > pMQ->maxMsgSize)   
        || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        rt = MQ_Write_Result_Error;   
    }   
    else  
    {   
        Mutex_Lock(pMQ->mutex);   
  
        /** @brief 消息队列未满,放入队列,返回Ok */  
        pMQ->rear = (pMQ->rear + 1) % pMQ->maxMsgNum;   
  
        memcpy(pMQ->pMQMsg[pMQ->rear].pMsg, pMsg, size);   
        pMQ->pMQMsg[pMQ->rear].msgSize = size;   
 /**< 判断对列是否为满的的方法是看 (write_pos + 1)% QUEUE_SIZE == read_pos 是否成立>*/
        if ((pMQ->maxMsgNum - 1) ==  pMQ->msgNum)   
        {   
            pMQ->front = (pMQ->front + 1) % pMQ->maxMsgNum;   
  
            rt = MQ_Write_Result_Full;   
        }   
        else  
        {   
            ++(pMQ->msgNum);   
  
            rt = MQ_Write_Result_Ok;   
        }   
  
        Mutex_Unlock(pMQ->mutex);   
    }   
  
    return rt;   
}   
  
/**  
* @brief MQ_Init  
*  
* Detailed description.  
* @param[in] pMQ 要初始化的消息队列  
* @param[in] maxMsgNum 消息队列大小  
* @param[in] maxMsgSize 每条消息的大小  
* @return BOOL  
*/  
/**  
*队列初始化
*初始化主要完成三个任务：
*
*为环形队列分配内存； 
*初始化互斥锁，用 pthread_mutex_init 完成； 
*初始化信号量，用 sem_init 完成。 
*/ 
BOOL MQ_Init(MQ_Struct* pMQ, UINT32 maxMsgNum, UINT32 maxMsgSize)   
{   
    BOOL rt  = FALSE;   
    UINT32 i = 0;   
    UINT32 j = 0;   
  
    /** @brief 如果最大消息个数或每条消息的最大字节数为0,则返回错误 */  
  
    if ((NULL == pMQ) || (0 == maxMsgNum) || (0 == maxMsgSize))   
    {   
        return FALSE;   
    }   
  
    if (MQ_INITIALIZE_FLAG == pMQ->initialized)   
    {   
        return TRUE;   
    }   
  
    pMQ->pMQMsg     = NULL;   
  
    pMQ->msgNum     = 0;   
    pMQ->front      = 0;   
    pMQ->rear       = 0;   
    pMQ->maxMsgNum  = maxMsgNum;   
    pMQ->maxMsgSize = maxMsgSize;   
    pMQ->pMQMsg  = malloc(sizeof(MQ_Msg_Struct) * pMQ->maxMsgNum);   
  
    if (NULL == pMQ)   
    {   
        return FALSE;   
    }   
  
    for (i = 0; i < pMQ->maxMsgNum; ++i)   
    {   
        pMQ->pMQMsg[i].pMsg = NULL;   
        pMQ->pMQMsg[i].pMsg = malloc(pMQ->maxMsgSize);   
  
        /** @brief 如果初始化失败,则释放之前分配的资源 */  
  
        if (NULL == pMQ)   
        {   
            for (j = i - 1; j >= 0; --j)   
            {   
                free(pMQ->pMQMsg[j].pMsg);   
                pMQ->pMQMsg[j].pMsg = NULL;   
            }   
  
            free(pMQ->pMQMsg);   
  
            pMQ->pMQMsg = NULL;   
            return FALSE;   
        }   
        else  
        {   
            pMQ->pMQMsg[i].msgSize = 0;   
            memset(pMQ->pMQMsg[i].pMsg, 0, pMQ->maxMsgSize);   
        }   
    }   
  
    rt = Mutex_Init(&(pMQ->mutex));   
  
    if (FALSE == rt)   
    {   
        return FALSE;   
    }   
  
    pMQ->initialized = MQ_INITIALIZE_FLAG;   
  
    return TRUE;   
}   

/**在System V提供的消息队列的操作函数中，清空消息队列的方法我知道的有一下两种：
 *1. 删除以后，重新建立消息队列。
 *2. type赋值为0，一个一个读出来。
 */

/**  
* @brief MQ_Clear  
*  
* Detailed description.  
* @param[in] pMQ 要清空的消息队列  
* @return BOOL  
*/  
BOOL MQ_Clear(MQ_Struct* pMQ)   
{   
    if ((NULL == pMQ) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        return FALSE;   
    }   
  
    Mutex_Lock(pMQ->mutex);   
  
    pMQ->front  = 0;   
    pMQ->rear   = 0;   
    pMQ->msgNum = 0;   
  
    Mutex_Unlock(pMQ->mutex);   
  
    return TRUE;   
}

/**  
* @brief MQ_Get_Msg_Num 要获得消息队列消息个数  
*  
* Detailed description.  
* @param[in] pMQ 要获得消息个数的 消息队列
* @return int 若失败则返回-1,否则返回消息个数  
*/  
int MQ_Get_Msg_Num(const MQ_Struct* pMQ)   
{   
    if ((NULL == pMQ) || (MQ_INITIALIZE_FLAG != pMQ->initialized))   
    {   
        return -1;   
    }   
    else  
    {   
        return pMQ->msgNum;   
    }   
}  


//本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/donhao/archive/2010/09/04/5862922.aspx#