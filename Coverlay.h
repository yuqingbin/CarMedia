#pragma once
#include "ddraw.h"
class Coverlay
{
public:
	Coverlay(void);
	~Coverlay(void);
	BOOL CreateDdraw(HWND hWnd,int width,int height,TCHAR *bmpPath);
	//BOOL CreateSurface();
	BOOL UpdateOverlay(LPRECT rDes);
private:
	LPDIRECTDRAW  lpDD;
	LPDIRECTDRAWSURFACE  lpPrimary;
	LPDIRECTDRAWSURFACE  lpOverlay;
	DDSURFACEDESC        ddsd;
	DDCAPS  ddcaps;
	DDOVERLAYFX ofx;
    LPRECT rSrc;
	int   overWidth,overHeight;
};
