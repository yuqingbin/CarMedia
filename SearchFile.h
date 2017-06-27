///////////////////////////////////////////////////////
//音视频文件检索
//
//BY  yuqingbin
//////////////////////////////////////////////////////


//#include <atlstr.h>
#include "afx.h"
#pragma once

/*
 *Search t0 find audio and video file path and size
 */
void Search(CString lpPath,int Typeflg);   //lpPath 盘符   Typeflg：音频或视频 1：音频 2：视频

int AudioFileNamejudge(CString FileName);
void SaveAudioPath(CString FilePath,DWORD filelenght);
void SaveVideoPath(CString FilePath,DWORD filelenght);
int VideoFileNamejudge(CString FileName);


class CPathSave
{
    public:
	
	int num;
    TCHAR pathname[_MAX_PATH];
	TCHAR filename[_MAX_FNAME];
	TCHAR filelenght[_MAX_PATH];
};