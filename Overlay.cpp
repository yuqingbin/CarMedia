#include "StdAfx.h"
#include "Overlay.h"

#define LCD_WINCTRL0_AEN	(1<<1)
#define LCD_WINCTRL0_A_N(N) ((N)<<2)
#define OVERLAY_CONFIG_GET	(1<<0)
#define LCD_OVERLAY_CONFIG  0x229c74
#define LCD_BACKGROUND_GET 0x229c64
#define LCD_BACKGROUND_SET 0x229c60
#define OVERLAY_PLANE 3

typedef struct {
	ULONG ndx;
	ULONG winctrl0;						// X,Y Alpha
	ULONG winctrl1;						// Priority, pipe, format, CCO, Pixel ordering, size
	ULONG winctrl2;					    // Colorkey mode, Double Buffer, RAM, Linewidth, Scale
	ULONG bufctrl;
	ULONG flags;
	ULONG reserved0;
	ULONG reserved1;
} OVERLAY_IOCTL,*POVERLAY_IOCTL;

DDPIXELFORMAT ddpfOverlayFormats =
{sizeof(DDPIXELFORMAT), DDPF_RGB|DDPF_ALPHAPIXELS, 0, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};


BOOL COverlay::UninitOverlay()
{
	if(lpOverlay)
	{
	 lpOverlay->Release();
	 lpOverlay = NULL;
	}
	if(lpPrimary)
	{
	 lpPrimary->Release();
	 lpPrimary = NULL;
	}
	if(lpDD)
	{
	 lpDD->Release();
	 lpDD = NULL;
	}
	return 1;
}
COverlay::COverlay(void)
{
	hfont = CreateFont(30,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("微软雅黑"));

}

COverlay::~COverlay(void)
{
	UninitOverlay();
	DeleteObject(hfont);
}

int COverlay::SetBackGround(DWORD colorARGB)
{
	HDC _hLCD = GetDC(NULL);
	DWORD color;
	int retval;

	ExtEscape(_hLCD, LCD_BACKGROUND_GET, 0, NULL,sizeof(color), (LPSTR)&color); 

	RETAILMSG(1, (TEXT("background color = 0x%x\r\n"), color));


	retval = ExtEscape(_hLCD, LCD_BACKGROUND_SET, sizeof(colorARGB),(LPCSTR)&colorARGB,0, NULL); 

	return retval;
}

int COverlay::SetAENBit(BYTE Plane, BOOL bEnable,BYTE Alpha)
{
	int retval;
	HDC hDC = GetDC(NULL);
	OVERLAY_IOCTL ovlIoctl;

	ovlIoctl.flags = OVERLAY_CONFIG_GET;
	ovlIoctl.ndx = Plane;

	retval = ExtEscape(hDC, LCD_OVERLAY_CONFIG, sizeof(ovlIoctl), (LPCSTR)&ovlIoctl, 0, NULL);

	ovlIoctl.flags = 0;

	if (bEnable)
	{
		ovlIoctl.winctrl0 |= LCD_WINCTRL0_AEN;
		ovlIoctl.winctrl0 |= LCD_WINCTRL0_A_N(Alpha);
	}
	else
	{
		ovlIoctl.winctrl0 &= ~LCD_WINCTRL0_AEN;
	}

	retval = ExtEscape(hDC, LCD_OVERLAY_CONFIG, sizeof(ovlIoctl), (LPCSTR)&ovlIoctl, 0, NULL);

	return retval;
}

BOOL COverlay::CreateSurface(int Ovwidth,int Ovheight)
{
	UninitOverlay();
	DirectDrawCreate(NULL,&lpDD,NULL);
	lpDD->SetCooperativeLevel(NULL,DDSCL_NORMAL);

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
 
	HRESULT hr = lpDD->CreateSurface(&ddsd,&lpPrimary,NULL);
	if(FAILED(hr))
		return 0;
	SetBackGround(0);                                       //设置overlay背景色
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags =DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY|DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth = Ovwidth;
	ddsd.dwHeight = Ovheight;
	ddsd.ddpfPixelFormat = ddpfOverlayFormats;
    hr = lpDD->CreateSurface(&ddsd,&lpOverlay,NULL);
	if(FAILED(hr))
		return 0;
  return 1;
}

BOOL COverlay::DrawBktoOverlay(TCHAR *pFile,int Dx,int Dy,int width,int height,int Sx,int Sy)
{
	HDC hdc=NULL;
	HRESULT hr = lpOverlay->GetDC(&hdc);
	HDC hdcmem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(pFile);
	HBRUSH holdbr = (HBRUSH)SelectObject(hdcmem,hbmp);
	if(!BitBlt(hdc,Dx,Dy,width,height,hdcmem,Sx,Sy,SRCCOPY))                  //overlay表面的图片看不到
		return 0;
	SelectObject(hdcmem,holdbr);
	DeleteDC(hdcmem);
	DeleteObject(hbmp);

	lpOverlay->ReleaseDC(hdc);
	return 1;
}

BOOL COverlay::UpdateOverlay(LPRECT rDes,LPRECT rSrc,DWORD flg,BYTE alpha)
{

	HRESULT hr = lpOverlay->UpdateOverlay(rSrc, lpPrimary, rDes, flg,NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,L"update overlay error",L"",NULL);
		return 0;
	}
	if(flg == DDOVER_SHOW)
	  SetAENBit(OVERLAY_PLANE,true,alpha);
	return 1;
}

BOOL COverlay::DrawImageToOverlay(TCHAR *pFile,int x,int y)
{
	BITMAPINFOHEADER bh;
	FILE *fp = _wfopen(pFile,L"rb");
	if(fp == NULL)
		return 0;
	fseek(fp,sizeof(BITMAPFILEHEADER),0);

	ZeroMemory(&bh,sizeof(BITMAPINFOHEADER));
	fread(&bh,sizeof(BITMAPINFOHEADER),1,fp);
	int bmpWidth = bh.biWidth;
	int bmpHeight = bh.biHeight;
	int biBitCount = bh.biBitCount;
	BYTE *pBuf = new BYTE[bmpWidth*bmpHeight*4];
	BYTE *pBuftemp = pBuf;                       
	fread(pBuf,1,bmpWidth*bmpHeight*4,fp);
	fclose(fp);

	ZeroMemory(&ddsd,sizeof(ddsd));  
	ddsd.dwSize=sizeof(ddsd);  

	lpOverlay->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);

	BYTE* pDisplayMemOffset = (BYTE*) ddsd.lpSurface+y*ddsd.lPitch+x*ddsd.lXPitch;   //起始坐标

	for(int i=0;i<bmpHeight;i++)
	{
		for(int j=0;j<bmpWidth;j++)
		{
			memcpy(pDisplayMemOffset+j*4+0,pBuf+0,1);
			memcpy(pDisplayMemOffset+j*4+1,pBuf+1,1);
			memcpy(pDisplayMemOffset+j*4+2,pBuf+2,1);

			pBuf+=4;
		}
		pDisplayMemOffset+=ddsd.lPitch;
	}
	lpOverlay->Unlock(NULL);
	delete[] pBuftemp;
	pBuftemp = NULL;
	return 1;
}

BOOL COverlay::DrawLineToOverlay(BYTE R,BYTE G,BYTE B,int xPos,int yPos)
{
	ZeroMemory(&ddsd,sizeof(ddsd));  
	ddsd.dwSize=sizeof(ddsd);  

	lpOverlay->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);

	BYTE* pDisplayMemOffset = (BYTE*) ddsd.lpSurface+yPos*3200;
	for(int j=0;j<xPos;j++)
	{
	 memset(pDisplayMemOffset+j*4,B,1);
	 memset(pDisplayMemOffset+j*4+1,G,1);
	 memset(pDisplayMemOffset+j*4+2,R,1);
	}
	lpOverlay->Unlock(NULL);
   return 1;
}

BOOL COverlay::DrawLineToOverlay2(BYTE R,BYTE G,BYTE B,int xPos,int yPos)
{
	ZeroMemory(&ddsd,sizeof(ddsd));  
	ddsd.dwSize=sizeof(ddsd);  

	lpOverlay->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);

	BYTE* pDisplayMemOffset = (BYTE*) ddsd.lpSurface+yPos*3200;
	for(int j=800;j>xPos;j--)
	{
		memset(pDisplayMemOffset+j*4,B,1);
		memset(pDisplayMemOffset+j*4+1,G,1);
		memset(pDisplayMemOffset+j*4+2,R,1);
	}
	lpOverlay->Unlock(NULL);
	return 1;
}
BOOL COverlay::DrawHdcmemToOverlay(HDC hdcmem,int dx,int dy,int width,int height,int sx,int sy)
{
	HDC hdc = NULL;
	lpOverlay->GetDC(&hdc);
    BitBlt(hdc,dx,dy,width,height,hdcmem,sx,sy,SRCCOPY);
	lpOverlay->ReleaseDC(hdc);
	return 1;
}
BOOL COverlay::SetOverlayPosition(int x,int y)
{
	HRESULT hr = S_FALSE;
	hr = lpOverlay->SetOverlayPosition(x,y);
	if(FAILED(hr))
		return 0;
	return 1;
}
BOOL COverlay::DrawTextToOverlay(TCHAR *pFile,TCHAR *pStr,LPRECT rRect)  //pFile 字的背景色
{
	BITMAPINFOHEADER bh;
	FILE *fp = _wfopen(pFile,L"rb");
	if(fp == NULL)
		return 0;
	fseek(fp,sizeof(BITMAPFILEHEADER),0);

	ZeroMemory(&bh,sizeof(BITMAPINFOHEADER));
	fread(&bh,sizeof(BITMAPINFOHEADER),1,fp);
  BITMAPINFO bmi;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = bh.biWidth;
  bmi.bmiHeader.biHeight = bh.biHeight;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage = bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight*4; 

  return 1;
}

BOOL COverlay::DrawTextToOverlay(TCHAR *pStr,int x,int y,int wx,int wy)
{
	
	ZeroMemory(&ddsd,sizeof(ddsd));  
	ddsd.dwSize=sizeof(ddsd);  
	lpOverlay->Lock(NULL, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
	BYTE* pDisplayMemOffset = (BYTE*) ddsd.lpSurface+y*ddsd.lPitch+x*ddsd.lXPitch;   //起始坐标

	for(int i=0;i<wy;i++)
	{
		for(int j=0;j<wx;j++)
		{
			memset(pDisplayMemOffset+j*4+0,120,1);     //RGB(120,120,120) 背景色
			memset(pDisplayMemOffset+j*4+1,120,1);
			memset(pDisplayMemOffset+j*4+2,120,1);
		}
		pDisplayMemOffset+=ddsd.lPitch;
	}
	lpOverlay->Unlock(NULL);

	HDC hdc = NULL;
	lpOverlay->GetDC(&hdc);
    
	SetBkMode(hdc,1);
	SetTextColor(hdc,RGB(241,241,241));   
	RECT rc = {x,y,wx+x,wy+y};
	HFONT holdfont = (HFONT)SelectObject(hdc,hfont);
	DrawText(hdc,pStr,-1,&rc,DT_VCENTER);
    SelectObject(hdc,holdfont);
	lpOverlay->ReleaseDC(hdc);

	return 1;
}

BOOL COverlay::DrawImageToOverlay2(TCHAR *pFile,int x,int y)
{
   HDC hdc = NULL;
   lpOverlay->GetDC(&hdc);
   HDC hdcmem = CreateCompatibleDC(hdc);
   HBITMAP hbmp = (HBITMAP)SHLoadDIBitmap(pFile);
   HBRUSH hold = (HBRUSH)SelectObject(hdcmem,hbmp);
   BITMAP bmp;
   GetObject(hbmp,sizeof(BITMAP),&bmp);
   BitBlt(hdc,x,y,bmp.bmWidth,bmp.bmHeight,hdcmem,0,0,SRCCOPY);
   SelectObject(hdcmem,hold);
   DeleteDC(hdcmem);
   DeleteObject(hbmp);
   lpOverlay->ReleaseDC(hdc);
   return 1;
}