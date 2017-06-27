/*
ImageDecoder.h
  Declare ImageDecoder class
  v 1.0
*/
#ifndef _IMAGEDECODER_H_
#define _IMAGEDECODER_H_

#define INITGUID
#include <windows.h>
#include <imaging.h>
#include <GdiPlusEnums.h>

enum CodecType {eDecoder, eEncoder};
enum StateType {UnInit, Init, Decoded, Finish};
class ImageDecoder
{
public:
	ImageDecoder(TCHAR* ImageFileName);
    ImageDecoder(HRSRC hRes);
	~ImageDecoder();
	StateType State();
	//TCHAR* FileName();
	int Width();
	int Height();
    UINT FrameCount();
	void Decode(); //decode the image
	void Draw(HDC hdc, RECT& dstRect); //draw current frame
	void Rotate(int Angle);
	void Flip();
	void Resize();
	void AdjustBrightness();
	void AdjustContrast();
	void AdjustGamma();
    void SelectNextFrame();
private:
	StateType state;
	TCHAR filename[MAX_PATH]; //Image source file name
    ImageInfo ii;
	int Angle; //Image Rotate Angle
    UINT nFrameCount;//Image frame count, time based
    UINT nSelectedFrame;//Active frame index;
	IImage *pImage;
	IImagingFactory *pImagingFactory;
	IImageSink *pImageSink;
	IImageDecoder *pImageDecoder;
	IBitmapImage *pBitmapImage;
	IBitmapImage *pBitmapImageRotate;
	IImage *pImageRotate;
	IStream *pStream;
	IBasicBitmapOps *pBasicBitmapOps;
	HRESULT CreateStreamOnFile(const TCHAR * tszFilename, IStream ** ppStream);
    HRESULT CreateStreamOnResource(HRSRC hRes, IStream ** ppStream);
	BOOL GetCodecCLSID(IImagingFactory* pImagingFactory, CLSID * pclsid, WCHAR * wszMimeType, CodecType ctCodec);
	void GetDestRect(int srcWidth, int srcHeight, int dstWidth, int dstHeight, RECT &pos);
};
class PictureCutYu
{
public:
	PictureCutYu(void);
	PictureCutYu(HWND hWnd);
	~PictureCutYu(void);
	BOOL CutJpeg(TCHAR *pFileIn,TCHAR *pFileOut,int width,int height);
	BOOL CutJpeg(TCHAR *pFileIn,BYTE **pBuffer,int width,int height);
private:
	HDC hdc;
	IImagingFactory *pImgFactory;
	IImage *pImage;
	ImageInfo Imageinfo;
	HBITMAP hbitmap;
	IBitmapImage *pBitmap;
	IBitmapImage *pBitmapResize;
	IBasicBitmapOps *pBitmapOps;
	BITMAPINFO bitmapInfo;
	//IImageDecoder *pImageDecoder;
	//IStream *pStream ;
	//IImageSink *pImageSink;
	HWND m_Wnd;
	BITMAPFILEHEADER bfh;
	RECT rect;

};

#endif