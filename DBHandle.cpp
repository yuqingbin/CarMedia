#include "StdAfx.h"
#include "DBHandle.h"

HWND g_hWnd;
CEGUID ceGuid;
HANDLE ceDBOpen;
WORD wProps;
int nStorecdBy = PID_ID;
int CountNum = 0;
DBHandle::DBHandle(void)
{
}

DBHandle::~DBHandle(void)
{
	if(ceDBOpen)
	{
		CloseHandle(ceDBOpen);
	}
	CeUnmountDBVol(&ceGuid);
}
void DBHandle::DoCreateDB(HWND hWnd,TCHAR *DBPath,int Mode)
{
	int nCnt;
	if(!CeMountDBVol(&ceGuid,DBPath,Mode))  //打开已经存在的
	{
		//创建数据库失败
		//MessageBox(hWnd,L"open error",L"DB ERROR",NULL);
		DWORD ER = GetLastError();
		printf("lasterror111111111111%ld\n",ER);
		if(ER == 5)
		{
          CeUnmountDBVol(&ceGuid);
		  MessageBox(hWnd,L"图片数据库拒绝访问，稍后重试",L"",NULL);
		}
	}
	//打开或创建数据表
	ceDBOpen = myOpenDataBase(hWnd,&totalNum);
	if(ceDBOpen == INVALID_HANDLE_VALUE)
	{
		//创建数据表失败
		MessageBox(hWnd,L"open db error",L"DB ERROR",NULL);
	}
}
HANDLE  DBHandle::myOpenDataBase(HWND hWnd,int* nCnt)
{
	HANDLE dbTable;
	CEOID ceOid;
	CEDBASEINFO ceDbi;
	CEOIDINFO oidInfo;
	int re;
	ceOid = 0;
	dbTable = CeOpenDatabaseEx(&ceGuid,&ceOid,TEXT("myPicA"),NULL,0,NULL);  //读取按顺序
	if(dbTable == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,L"数据表不存在",L"",NULL);
	}
	CeOidGetInfoEx(&ceGuid,ceOid,&oidInfo);
	*nCnt = oidInfo.infDatabase.wNumRecords;
	printf("!!!!!!!!!!1111 num :%d\n",*nCnt);   // 数据也获得了
	printf("aopen db success\n");

	return dbTable;
}
void DBHandle::myGetItemRecord(int nItem,MYLVDATA* myLvdata)
{
	CEOID ceOid;
	DWORD index;
	DWORD dwresize;
	PCEPROPVAL pRecord = NULL;
	int i;

	ceOid = CeSeekDatabase(ceDBOpen,CEDB_SEEK_BEGINNING,nItem,&index);

	if(ceOid == 0)
	{
		//不存在
	}
	ceOid = CeReadRecordProps(ceDBOpen,CEDB_ALLOWREALLOC,&wProps,NULL,(LPBYTE*)(&pRecord),&dwresize);
	if(ceOid == 0)
	{
		// 读取失败
	}
	for (i=0;i<wProps;i++)
	{
		switch (pRecord->propid)
		{
		case PID_ID:
			//lstrcpy(myLvdata->myRecordinfo.strID,pRecord->val.lpwstr);
			myLvdata->myRecordinfo.strID = pRecord->val.iVal;
			break;
		case PID_PATH:
			lstrcpy(myLvdata->myRecordinfo.strPATH,pRecord->val.lpwstr);
			break;
		case PID_CUTPATH:
			lstrcpy(myLvdata->myRecordinfo.strCUTPATH,pRecord->val.lpwstr);
			break;
		}
		pRecord++;
	}
}
BOOL DBHandle::mySetItemRecord(MYLVDATA *myLvdata)
{
	CEOID ceOid;
	DWORD index;
	PCEPROPVAL pRecord = NULL;
	TCHAR *pStrMem = NULL;
	int nLen = sizeof(CEPROPVAL)*numOfProps + MAX_IDLEN + MAX_PATH*2;

	pRecord = (PCEPROPVAL)LocalAlloc(LMEM_FIXED,nLen);

	pStrMem = (TCHAR *)((LPBYTE)pRecord + (sizeof(CEPROPVAL)*numOfProps));

	memset(pRecord,0,LocalSize(pRecord));
	CountNum++;
	pRecord->propid = PID_ID;
	//pRecord->val.lpwstr = pStrMem;
	//lstrcpy(pStrMem,myLvdata->myRecordinfo.strID);
	//memcpy(pStrMem,myLvdata->myRecordinfo.strID,MAX_IDLEN);
	pRecord->val.iVal = CountNum;
	pStrMem+= lstrlen(pStrMem)+1;
	pRecord++;

	pRecord->propid = PID_PATH;
	pRecord->val.lpwstr = pStrMem;
	memcpy(pStrMem,myLvdata->myRecordinfo.strPATH,MAX_PATH);
	pStrMem+= lstrlen(pStrMem)+1;
	pRecord++;

	pRecord->propid = PID_CUTPATH;
	pRecord->val.lpwstr = pStrMem;
	memcpy(pStrMem,myLvdata->myRecordinfo.strCUTPATH,MAX_PATH);
	//ceOid = CeSeekDatabase(ceDBOpen,CEDB_SEEK_BEGINNING,1,&index);
	ceOid = CeWriteRecordProps(ceDBOpen,0,numOfProps,pRecord-2);
	if(ceOid == 0)
	{
		return FALSE;
	}
	if(pRecord)
		LocalFree(pRecord);
	if(pStrMem)
	{
		delete pStrMem;
		pStrMem = NULL;
	}
	return TRUE;
}


BOOL DBHandle::myDeleteItemRecord(int nItem)                             //把文件也删除
{
	DWORD dwIndex;
	if(nItem <0)
	{ 
		return FALSE;
	}

	MYLVDATA pP;
	myGetItemRecord(nItem,&pP);
	if(pP.myRecordinfo.strID)     //判断不是空的就可以
	{
		DeleteFile(pP.myRecordinfo.strCUTPATH);
		DeleteFile(pP.myRecordinfo.strPATH);
	}

	CEOID ceoid;
	ceoid = CeSeekDatabase(ceDBOpen,CEDB_SEEK_BEGINNING,nItem,&dwIndex);  //删除第nItem个记录 
	if(ceoid == 0)
	{
		return FALSE;
	}
	if(!CeDeleteRecord(ceDBOpen,ceoid))
		return FALSE;
	myOpenDataBase(g_hWnd,&totalNum);
	return TRUE;
}