///////////////////////////////////////////////////////////
//DBHandle 数据库处理类
//记录结构三个参数：ID、原图路径、裁剪图路径
//BY yuqingbin
///////////////////////////////////////////////////////////
#pragma once
#include "windows.h"
#define MAX_IDLEN 64
#define MAX_PATH  260
#define PID_ID  MAKELONG(CEVT_I2,1) 
#define PID_PATH  MAKELONG(CEVT_LPWSTR,2)
#define PID_CUTPATH  MAKELONG(CEVT_LPWSTR,3)
#define numOfProps  3

typedef struct  
{
	INT strID;                     
	TCHAR strPATH[MAX_PATH];
	TCHAR strCUTPATH[MAX_PATH];
}MYRECORDINFO;   //记录结构

typedef struct
{
	int nItem;
	MYRECORDINFO myRecordinfo;
}MYLVDATA; 

class DBHandle
{
public:
	DBHandle(void);
	~DBHandle(void);
	HANDLE  myOpenDataBase(HWND,int*);    //打开数据表
	void myGetItemRecord(int,MYLVDATA*); //读取的是第几个记录
	BOOL mySetItemRecord(MYLVDATA*);    //添加记录自动添加到尾部
	BOOL myDeleteItemRecord(int);    //删除记录
	void DoCreateDB(HWND hWnd,TCHAR *DBPath,int Mode);     //打开或创建数据库 DBPath 数据库路径  Mode：打开方式
	int  totalNum ;
};
