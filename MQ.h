/**  
* @file MQ.h  
* @brief 实现了消息队列的封装  
* @author Donhao  
* @date 2010-4-17 12:57:57  
* @version  
* <pre><b>All rights reserved.</b></pre>  
* <pre><b>modification:</b></pre>  
* <pre>Write modifications here.</pre>  
*/  
#ifndef _MQ_H   
#define _MQ_H   
//#include "MultiThread.h"  
/** @brief 写消息队列返回值 */  
typedef enum  
{   
    MQ_Write_Result_Ok    = 0x11, /**< 队列未满   */  
    MQ_Write_Result_Error = 0x22, /**< 写队列失败   */  
    MQ_Write_Result_Full  = 0x33  /**< 队列已满 */  
}MQ_Write_Result_Enum;   
  
typedef struct  
{   
    unsigned long msgSize;   
    unsigned char* pMsg;   
} MQ_Msg_Struct;   
  
typedef struct  
{   
    unsigned long initialized; /**< 是否初始化了         */  
    unsigned long maxMsgNum;   /**< MQ中最大消息个数     */  
    unsigned long maxMsgSize;  /**< 每条消息的最大字节数 */  
    int front;                 /**< 头指针               */  
    int rear;                  /**< 尾指针               */  
    unsigned long msgNum;      /**< 消息队列中消息个数   */  
    MUTEX mutex;               /**< 保护消息队列的互斥器 */  
    MQ_Msg_Struct* pMQMsg;     /**< 指向第一条消息    */  
} MQ_Struct;   
  
/**  
* @brief MQ_Init  
*  
* Detailed description.  
* @param[in] pMQ 要初始化的消息队列  
* @param[in] maxMsgNum 消息队列大小  
* @param[in] maxMsgSize 每条消息的大小  
* @return BOOL  
*/  
BOOL MQ_Init(MQ_Struct* pMQ, unsigned long maxMsgNum, unsigned long maxMsgSize);   
  
/**  
* @brief MQ_Get_Msg_Num  
*  
* Detailed description.  
* @param[in] pMQ 要获得消息个数的消息队列  
* @return int 若失败则返回-1,否则返回消息个数  
*/  
int MQ_Get_Msg_Num(const MQ_Struct* pMQ);   
  
/**  
* @brief MQ_Write  
*  
* Detailed description.  
* @param[in] pMQ 要写的消息队列  
* @param[in] pMsg 要写的消息  
* @param[in] size 要写的消息大小  
* @return MQ_Write_Result_Enum  
*/  
MQ_Write_Result_Enum MQ_Write(MQ_Struct* pMQ, const unsigned char* pMsg, unsigned long size);   
  
/**  
* @brief MQ_Read  
*  
* Detailed description.  
* @param[in] pMQ 要读的消息队列  
* @param[in] pOutBuf 要输出消息的缓冲区  
* @param[in/out] pSize 作为输入参数时为输出的缓冲区消息大小，作为输出参数为读到的消息大小  
* @return BOOL  
*/  
BOOL MQ_Read(MQ_Struct* pMQ, unsigned char* pOutBuf, unsigned long* pSize);   
  
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
BOOL MQ_Get(MQ_Struct* pMQ, unsigned long index, unsigned char* pOutBuf, unsigned long* pSize);   
  
/**  
* @brief MQ_Clear  
*  
* Detailed description.  
* @param[in] pMQ 要清空的消息队列  
* @return BOOL  
*/  
BOOL MQ_Clear(MQ_Struct* pMQ);   
  
#endif  


//本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/donhao/archive/2010/09/04/5862922.aspx#