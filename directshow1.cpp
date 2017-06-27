
#include "directshow.h"
#include <stdio.h>


//dshow接口初始化
MediaControl::MediaControl() 
{
 //将DSHOW接口置空
	m_pGB = NULL;
	m_pMC = NULL; 
	m_pME = NULL;
	m_pVW = NULL;
	m_pBA = NULL;
	m_pBV = NULL;
	m_pMS = NULL;
	//初始化 COM 环境
	CoInitialize(NULL); 
}


MediaControl::~MediaControl() 
{
 DshowUninit();
 //释放COM 环境
 CoUninitialize();
}


void MediaControl::DshowInit() 
{
    m_pGB = NULL;
	m_pMC = NULL; 
	m_pME = NULL;
	m_pVW = NULL;
	m_pBA = NULL;
	m_pBV = NULL;
	m_pMS = NULL;
	m_pMP = NULL;
	//初始化COM接口
	CoInitialize(NULL); 
}


//dshow接口释放
void MediaControl::DshowUninit()  
{
 if (m_pMC != NULL)
	{
		//停止视频播放
		m_pMC->Stop(); 
		m_pMC->Release();
		m_pMC = NULL;
	}
	
	//2,释放媒体事件接口
	if (m_pME != NULL)
	{
		//消息通知窗口置空
		m_pME->SetNotifyWindow(NULL, 0, 0);
		m_pME->Release();
		m_pME = NULL;
	}

	//3,释放视频播放窗口接口
	if (m_pVW != NULL)
	{
		//隐藏视频窗口
		m_pVW->put_Visible(OAFALSE);
		//设置视频播放窗口句柄为空
		m_pVW->put_Owner(NULL);
		m_pVW->Release();
		m_pVW = NULL;
	}

	//4,释放基础音频流接口
	if (m_pBA != NULL)
	{
		m_pBA->Release();
		m_pBA = NULL;
	}

	//5，释放基础视频流接口
	if (m_pBV != NULL)
	{
		m_pBV->Release();
		m_pBV = NULL;
	}

	//6，释放媒体搜索接口
	if (m_pMS != NULL)
	{
		m_pMS->Release();
		m_pMS = NULL;
	}

	//7,最后释放FilterGpaph接口
	if (m_pGB != NULL)
	{
		m_pGB->Release();
		m_pGB = NULL;
	}
    
	
	
}


/*******************************************************
*创建播放窗口 并渲染文件
*InPut : inWindow      显示视频音频流的窗口句柄
         strFileName   要播放的文件路径
		 hNotifyWnd    接收媒体事件的窗口
*OutPut : none
********************************************************/
int MediaControl::DshowCreate(HWND inWindow,TCHAR *strFileName,HWND hNotifyWnd) 
{
  int FLAG=1;
	DshowUninit();
  HRESULT hResult; 
  hResult = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&m_pGB);
  hResult = m_pGB->RenderFile(strFileName,NULL);
 if (hResult != S_OK)
 {
	
   //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
   return 0;
 }

  hResult = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC); 
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  hResult = m_pGB->QueryInterface(IID_IMediaSeeking,(void**)&m_pMS);  
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  GUID guid_timeFormat = TIME_FORMAT_MEDIA_TIME;
  m_pMS->SetTimeFormat(&guid_timeFormat); 

  hResult = m_pGB->QueryInterface(IID_IMediaEventEx,(void**)&m_pME);
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  m_pME->SetNotifyWindow((OAHWND)hNotifyWnd, WM_GRAPHNOTIFY, 0);                 //播放结束时会向接受窗口发送一个结束消息 然后响应该消息  

  hResult = m_pGB->QueryInterface(IID_IVideoWindow, (void **)&m_pVW); 
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  m_pVW->put_Owner((OAHWND)inWindow);
  m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

  hResult = m_pGB->QueryInterface(IID_IBasicVideo, (void **)&m_pBV);
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  hResult = m_pGB->QueryInterface(IID_IBasicAudio, (void **)&m_pBA);  
  if (hResult != S_OK)
  {

	  //MessageBox(hNotifyWnd,L"不支持的媒体格式",NULL,NULL);
	  return 0;
  }
  
  DshowFitWindow(inWindow);

  DshowSetAudioVolume(m_volume);
  return FLAG;
}


void MediaControl::DshowSetNotifyWindow(HWND hNotifyWnd)
{
 m_pME->SetNotifyWindow((OAHWND)hNotifyWnd, WM_GRAPHNOTIFY, 0); 
}


//播放
void MediaControl::DshowPlay() 
{
 //m_pMC->Pause();
 m_pMC->Run();
}

//暂停
void MediaControl::DshowPause() 
{
  m_pMC->Pause();
}


//停止
BOOL MediaControl::DshowStop() 
{
 m_pMC->Stop();
 LONGLONG pos = 0;
 m_pMS->SetPositions (&pos, AM_SEEKING_AbsolutePositioning ,     //结构体成员实现  API函数  接口直接调用
                                   NULL, AM_SEEKING_NoPositioning);
 return true;
}


//适应窗口大小显示
void MediaControl::DshowFitWindow(HWND inWindow) 
{
    m_hWindow = inWindow;
	RECT rc;
	GetClientRect(inWindow,&rc);
	long lngHeight = 0;
	long lngWidth = 0;
	m_pBV->get_SourceHeight(&lngHeight);//原始大小
	m_pBV->get_SourceWidth(&lngWidth);
	
	m_pVW->put_Visible(OATRUE);
	//m_pVW->put_Owner((OAHWND)m_hWindow);
	
	m_pVW->put_Left(rc.left );
	m_pVW->put_Top(rc.top );
	m_pVW->put_Width(rc.right -rc.left );
	m_pVW->put_Height(rc.bottom - rc.top );
	m_pVW->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	
	m_pVW->put_MessageDrain((OAHWND)m_hWindow);
}


//全屏显示
BOOL MediaControl::DshowFullScreen ()
{
    LONG lMode = 0;
	static HWND hDrain=0;

    m_pVW->get_FullScreenMode(&lMode);   //  得到屏幕状态   //全屏  也是API实现的
	
	if (lMode == OAFALSE)
    {
		// Save current message drain
        //m_pVW->get_MessageDrain((OAHWND *) &hDrain);

        // Set message drain to application main window
        m_pVW->put_MessageDrain((OAHWND)m_hWindow );
		
		//设置全屏幕
        lMode = OATRUE;
        m_pVW->put_FullScreenMode(lMode);
    }
    else
    {
        //切换到非全屏模式
        lMode = OAFALSE;
        m_pVW->put_FullScreenMode(lMode);

		DshowFitWindow(m_hWindow);

        m_pVW->SetWindowForeground(-1);
	}
   return true;
}
//初始大小
void MediaControl::DshowInitWindow(HWND inWindow) 
{
    m_hWindow = inWindow;
	RECT rc;
	GetClientRect(inWindow,&rc);
	long lngHeight = 0;
	long lngWidth = 0;
	m_pBV->get_SourceHeight(&lngHeight);//原始大小
	m_pBV->get_SourceWidth(&lngWidth);
	
	//m_pVW->put_Visible(OATRUE);           //视频不影响其他
	m_pVW->put_Owner((OAHWND)m_hWindow);
	
	m_pVW->put_Left((rc.right -rc.left)/5 );
	m_pVW->put_Top((rc.bottom - rc.top)/5 );
	m_pVW->put_Width((rc.right -rc.left)/5*3 );
	m_pVW->put_Height((rc.bottom - rc.top)/5*3 );
	m_pVW->put_WindowStyle(WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	
	m_pVW->put_MessageDrain((OAHWND)m_hWindow);
}


//媒体事件
BOOL MediaControl::DshowGetMediaEvent(long *lEventCode)
{
   LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

	if (m_pME == NULL)
	{
		return FALSE;
	}

	hr = m_pME->GetEvent(&evCode, &evParam1, &evParam2, 0);   
    if (SUCCEEDED(hr))
	{
        *lEventCode = evCode;
        hr = m_pME->FreeEventParams(evCode, evParam1, evParam2);
		
		return TRUE;
	}
	return FALSE;
}

//设置当前位置
BOOL MediaControl::DshowSetPositions(DWORD dwPos)
{
	printf("IN DshowSetPositions dwPos=%d\n",dwPos);
  LONGLONG llPos = dwPos * 10000 * 1000;
	if (m_pMS == NULL)
	{
		return FALSE;
	}
	//设置视频播放当前位置
	HRESULT hr = m_pMS->SetPositions(&llPos,AM_SEEKING_AbsolutePositioning ,
                                   NULL, AM_SEEKING_NoPositioning);        
	if (hr == S_OK)  
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//获得当前位置
LONGLONG MediaControl::DshowGetCurrentPos(DWORD &dwPos)
{
    dwPos = 0;
	LONGLONG llPos = 0;
	if (m_pMS== NULL)
	{
		return FALSE;
	}
	
	//得到当前视频播放位置
	HRESULT hResult = m_pMS->GetCurrentPosition(&llPos);
	if (hResult != S_OK)
	{
		return FALSE;
	}

	//转换成以秒为单位
	llPos = llPos / 10000 ;
	llPos = llPos / 1000;
	dwPos  = (DWORD)llPos;
	return dwPos;
}

//获得总时间长度
BOOL MediaControl::DshowGetDuration(DWORD &dwLength)
{
 dwLength = 0;
	if (m_pMS == NULL)
	{
		return FALSE;
	}

	//得到视频总时间长度
	LONGLONG llDuration;
	HRESULT hResult = m_pMS->GetDuration(&llDuration);       //API实现的获得持续时间
	if (hResult != S_OK)
	{
		return FALSE;
	}

	//转换成以秒为单位
	llDuration = llDuration / 10000 ;
	llDuration = llDuration / 1000;
	dwLength  = (DWORD)llDuration;
	return TRUE;
}



//设置声音
BOOL MediaControl::DshowSetAudioVolume(long nValue)
{
 m_volume = nValue;
  if (m_pBA)
	  return SUCCEEDED(m_pBA->put_Volume (nValue));
  return false;
}

//获得声音
long MediaControl::DshowGetAudioVolume()
{
  long volume = 0;

  if(m_pBA)
  { 
	 m_pBA->get_Volume (&volume);
  }
  
 
 return volume;
}


//获得当前位置  毫秒为单位
BOOL MediaControl::DshowGetCurrentPosMESL(LONGLONG & dwPos)
{
    dwPos = 0;
	LONGLONG llPos = 0;
	if (m_pMS== NULL)
	{
		return FALSE;
	}
	
	//得到当前视频播放位置
	HRESULT hResult = m_pMS->GetCurrentPosition(&llPos);
	if (hResult != S_OK)
	{
		return FALSE;
	}

	
	llPos = llPos / 10000 ;  //获得毫秒
	dwPos  = llPos;
	return TRUE;
}