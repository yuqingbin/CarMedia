/////////////////////////////////////////////////////////////////
//MediaControl 播放类:基于directshow接口的
//播放音视频文件的类
//
//BY yuqinbin
/////////////////////////////////////////////////////////////////


#pragma once
#include "dshow.h"
#define WM_GRAPHNOTIFY   WM_USER + 321

class MediaControl
{
public:
      MediaControl(void);
	  ~MediaControl(void);
private:
	IGraphBuilder	*m_pGB;
	IMediaControl	*m_pMC;
	IMediaEventEx	*m_pME; 
	IVideoWindow	*m_pVW;
	IBasicVideo		*m_pBV;
	IBasicAudio		*m_pBA;
	IMediaSeeking	*m_pMS;
	IMediaPosition	*m_pMP;
	HWND m_hWindow;
	long m_volume;
public:
	void DshowInit(void);
	void DshowUninit(void);
	int DshowCreate(HWND inWindow,TCHAR *strFileName,HWND hNotifyWnd);
	void DshowPlay();
	void DshowPause();
	BOOL DshowStop();
	void DshowFitWindow(HWND inWindow);
	void DshowInitWindow(HWND inWindow);
	BOOL DshowGetMediaEvent(long *lEventCode);
	void DshowSetNotifyWindow(HWND hNotifyWnd);
	BOOL DshowSetPositions(DWORD dwPos);
	LONGLONG DshowGetCurrentPos(DWORD &dwPos);
	BOOL DshowGetDuration(DWORD &dwLength); 
	BOOL DshowSetAudioVolume(long nValue);
	long DshowGetAudioVolume();
	BOOL DshowGetCurrentPosMESL(LONGLONG & dwPos);
	BOOL DshowFullScreen();
};