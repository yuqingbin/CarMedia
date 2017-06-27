/******************************************
lib: ddraw.lib
note: 内存不足时，创建overlay会失败
*******************************************/
#pragma once
#include "ddraw.h"
#include "atlstr.h"  
class COverlay
{
public:
	COverlay(void);
	~COverlay(void);
	int SetBackGround(DWORD colorARGB);
	int  SetAENBit(BYTE Plane, BOOL bEnable,BYTE Alpha);
	BOOL UninitOverlay();
	BOOL CreateSurface(int Ovlaywidth,int Ovheight);                                   //Ovlaywidth  必须是偶数
	BOOL DrawBktoOverlay(TCHAR *pFile,int x,int y,int width,int height,int Sx,int Sy);
	BOOL UpdateOverlay(LPRECT rDes,LPRECT rSrc,DWORD flg,BYTE alpha);
    BOOL DrawImageToOverlay(TCHAR *pFile,int x,int y);
	BOOL DrawTextToOverlay(TCHAR *pFile,TCHAR *pStr,LPRECT rRect);
	BOOL DrawTextToOverlay(TCHAR *pStr,int x,int y,int wx,int wy);
	BOOL DrawLineToOverlay(BYTE R,BYTE G,BYTE B,int xPos,int yPos);
	BOOL DrawLineToOverlay2(BYTE R,BYTE G,BYTE B,int xPos,int yPos);
	BOOL DrawImageToOverlay2(TCHAR *pFile,int x,int y);
	BOOL DrawHdcmemToOverlay(HDC hdcmem,int dx,int dy,int width,int height,int sx,int sy);
	BOOL SetOverlayPosition(int x,int y);
private:
	LPDIRECTDRAW  lpDD;
	LPDIRECTDRAWSURFACE lpPrimary,lpOverlay;
	DDSURFACEDESC ddsd;
    HFONT hfont;
};
