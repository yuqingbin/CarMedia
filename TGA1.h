#ifndef _TGA1_H_
#define _TGA1_H_
/*
   The following is rather crude demonstration code to read 
   uncompressed and compressed TGA files of 16, 24, or 32 bit 
   TGA. 
   Hope it is helpful.
*/
#include <windows.h>

typedef struct {
   char  idlength;
   char  colourmaptype;
   char  datatypecode;
   short int colourmaporigin;
   short int colourmaplength;
   char  colourmapdepth;
   short int x_origin;
   short int y_origin;
   short width;
   short height;
   char  bitsperpixel;
   char  imagedescriptor;
} TGAHeader;

typedef struct {
   unsigned char r,g,b,a;
} RGBA_PIXEL;

RGBA_PIXEL * LoadTGA1(const char *Filename,  int *ncol, int *nrow, int *Depth);
  void writeTGA32(char * outTGA,RGBA_PIXEL *pixels,int nrows,int ncols);

  //通过得到的像素rgb和rgba值imageData构造bmp文件显示tga
int ShowTga(HDC hDC,LPRECT lpDCRect,int nTgaRow,int nTgaCol,int nTgaBits,unsigned char *imageData);		//在指定矩形窗口显示tga
int DisplayTGA(HDC hDC,LPRECT lpDCRect,char *inTGA);
int OnPaintTga(HDC hDC,LPRECT lpDCRect,int nTgaRow,int nTgaCol,int nTgaBits,unsigned char *imageData);
#endif // _TGA1_H_
