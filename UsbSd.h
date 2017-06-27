/************************************************************************/
/* USB模块类
USB各模块功能实现
*/
/************************************************************************/
#pragma once
#include "directshow.h"
#include "Char2Wide.h"
#include "ImageDecoder.h"
#include "MP3INF.h"
#include "Mp3Apic.h"
#include "Scene.h"
#include "CutPicBmpYu.h"
#include "CeFileManager.h"

//enum ListType{Audio,Video};

typedef struct FileInfoUsb   /*音频文件信息*/
{
  TCHAR name[_MAX_FNAME];
  TCHAR path[_MAX_PATH];
};

typedef struct    /*图片数据*/
{
	BYTE Data[100*100*3];
	BOOL Turn;
}PicBuffer;

class CUsbSd 
{
public:
	CUsbSd(void);
	~CUsbSd(void);
	void DoDelObject();
	void DoPaintInit(HWND hWnd);
	void DoPaintSelPic(HWND hWnd,BOOL FLAG);
	void DoPaintSelVid(HWND hWnd,BOOL FLAG);
	void DoPaintSelAud(HWND hWnd,BOOL FLAG);
    void DoPaintSelEbk(HWND hWnd,BOOL FLAG);
	

	void DoPaintWindows(HWND,int,int,int,int,HBITMAP,int,int); //绘图
	void DoLoadPicture();
	void DoFileInit(HWND hWnd,ManagerMode mode,FileFor filetype);                 //初始化  放到创建窗口时

	void DoPaintHengliang(HWND);
	void DoPaintAudioPliFace(HWND);
	void DoPaintAudioSlider(HWND,INT);

	int DoLbuttonDown(HWND,INT,INT);   //响应左键按下消息 WM_LBUTTONDWON
	int DoMouseMove(HWND,INT,INT);     //响应鼠标移动消息 WM_MOVE
	int DoLbuttonUp(HWND,INT,INT);     //响应左键抬起消息 WM_LBUTTONUP
	int DoLDbClick(HWND,LPARAM);
	int DoTime(HWND,INT);              //响应定时器消息   WM_TIMER
	int DoPaint(HWND);                //响应WM_PAINT消息
    void Doinit(HWND);

	void DoAudioMode(HWND);
	void DoDrawTotalTime(HWND);
	void DoDrawCurrentTime(HWND);
	void DoPaintPicBrowface(HWND);
	void DoPaintVideoFace(HWND);
	void DoPaintAudMode(HWND);
	void DoFullScanPic(HWND);
	void PaintVideoName(HWND);
	void DoPiantVideoTime();
	
   
	void MoveVideoList(HWND hWnd,int ypos);                               //视频列表
    void CreateVideoListDC(HWND hWnd,int width,int height,TCHAR *filebk);
	

	void DoCreateAudiDCmem(HWND hWnd);
	void CreateAudioListDC(HWND hWnd,int width,int height,TCHAR *filebk); //音频列表
	void MoveAudioList(HWND hWnd,int ypos);

	void MoveEbookList(HWND hWnd,int ypos);
	
    
	void PictureInit(int PicAllCount);          //图片浏览器 初始化纹理数组
    void PictureLoad(int firstnum,int numCount);    //加载纹理 多个
	BOOL PictureLoad(int number);    //加载纹理  单个
	void PictureLoad(int number,BYTE *buffer);    //加载纹理  单个
	

	PictureCutYu *JpegPicCut;
	CutPicBmpYu  *BmpPicCut;
	Mp3Apic      *Mp3Texture;
	BOOL PicIsBmp(TCHAR *FilePath);

    PicBuffer *lpPicBuffer;

private:
	HANDLE  PicThread;
	TGAImage img;
	MediaControl *Media;
	MP3INF *mp3;
	

public:
	HWND  VidWnd;
	HWND g_UsbWnd;
    BOOL PICSEL,AUDSEL,VIDSEL;     //菜单界面标示 
	BOOL MENUSHOW ;       //显示菜单界面
	BOOL BROWPIC ;      //图片浏览标示
	


	BOOL SINGLESCAN;
	int  PicNum;   /*0-19*/
	BOOL FULLPIC;
	int PicCurrentNum; /*0-pictotalnum总数*/


	FileInfoUsb *AudioFile;  //音频
	FileInfoUsb *PictureFile;
	FileInfoUsb *VideoFile;
	FileInfoUsb *EbookFile;

	int AudioTCount;   //音频总个数 使用版
	int VideoTCount;   //视频总个数 使用版
	int PictureTcount; //图片
	int EbookTCount;
	
};
