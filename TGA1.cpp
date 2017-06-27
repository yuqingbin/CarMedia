#include <stdio.h>
#include <stdlib.h>
#include "TGA1.h"
#include "resource.h"
#include <windows.h>
#include "comm1.h"
#include "imageBMP.h"

#define INVERTED_BIT            (1 << 5)

void MergeBytes(RGBA_PIXEL *,unsigned char *,int);

RGBA_PIXEL* LoadTGA1(const char *fname, int *ncol, int *nrow, int *Depth)
{
   int n=0,i,j;
   int bytes2read,skipover = 0;
   unsigned char p[5];
   FILE *fptr;
   TGAHeader tgaHeader;
   RGBA_PIXEL *pix=NULL;

  
 	  
   /* Open the file */
   if ((fptr = fopen(fname,"rb")) == NULL) 
   {
      //fprintf(stderr,"File open failed\n");
	  OutputDebugString(TEXT("File open failed\r\n"));
	//   exit(-1);
   return 0;
   }
 
   /* Display the tgaHeader fields */
   tgaHeader.idlength = fgetc(fptr);
   fprintf(stderr,"ID length:         %d\n",tgaHeader.idlength);
  printf("ID length:         %d\n",tgaHeader.idlength);
   tgaHeader.colourmaptype = fgetc(fptr);
   fprintf(stderr,"Colourmap type:    %d\n",tgaHeader.colourmaptype);
   printf("Colourmap type:    %d\n",tgaHeader.colourmaptype);
   tgaHeader.datatypecode = fgetc(fptr);
   fprintf(stderr,"Image type:        %d\n",tgaHeader.datatypecode);
 printf("Image type:        %d\n",tgaHeader.datatypecode);
 
 fread(&tgaHeader.colourmaporigin,2,1,fptr);
   fprintf(stderr,"Colour map offset: %d\n",tgaHeader.colourmaporigin);
    printf("Colour map offset: %d\n",tgaHeader.colourmaporigin);

	fread(&tgaHeader.colourmaplength,2,1,fptr);
   fprintf(stderr,"Colour map length: %d\n",tgaHeader.colourmaplength); 
    printf("Colour map length: %d\n",tgaHeader.colourmaplength); 
   
	tgaHeader.colourmapdepth = fgetc(fptr);
   fprintf(stderr,"Colour map depth:  %d\n",tgaHeader.colourmapdepth);
   printf("Colour map depth:  %d\n",tgaHeader.colourmapdepth);
   
   fread(&tgaHeader.x_origin,2,1,fptr);
   fprintf(stderr,"X origin:          %d\n",tgaHeader.x_origin);
   printf("X origin:          %d\n",tgaHeader.x_origin);
   
   fread(&tgaHeader.y_origin,2,1,fptr);
   fprintf(stderr,"Y origin:          %d\n",tgaHeader.y_origin);
    printf("Y origin:          %d\n",tgaHeader.y_origin);

   fread(&tgaHeader.width,2,1,fptr);
   fprintf(stderr,"Width:             %d\n",tgaHeader.width);
     printf("Width:             %d\n",tgaHeader.width);
  
	 fread(&tgaHeader.height,2,1,fptr);
   fprintf(stderr,"Height:            %d\n",tgaHeader.height);
    printf("Height:            %d\n",tgaHeader.height);

   tgaHeader.bitsperpixel = fgetc(fptr);
   fprintf(stderr,"Bits per pixel:    %d\n",tgaHeader.bitsperpixel);
    printf("Bits per pixel:    %d\n",tgaHeader.bitsperpixel);
   
	tgaHeader.imagedescriptor = fgetc(fptr);
   fprintf(stderr,"Descriptor:        %d\n",tgaHeader.imagedescriptor);
OutputDebugString(TEXT("Before Allocate space for the image\n"));
   /* Allocate space for the image */
   if ((pix =(RGBA_PIXEL*) malloc(tgaHeader.width*tgaHeader.height*sizeof(RGBA_PIXEL))) == NULL) 
   {
       fprintf(stderr,"malloc of image failed\n");
	   OutputDebugString(TEXT("malloc of image failed\r\n"));
       return NULL;
   }
   
   *nrow=tgaHeader.height;
   *ncol=tgaHeader.width;
   *Depth=tgaHeader.bitsperpixel;
   for (i=0;i<tgaHeader.width*tgaHeader.height;i++)
   {
      pix[i].r = 0;
      pix[i].g = 0;
      pix[i].b = 0;
      pix[i].a = 0;
   }

   /* What can we handle */
   if (tgaHeader.datatypecode != 2 && tgaHeader.datatypecode != 10) 
   {
      fprintf(stderr,"Can only handle image type 2 and 10\n");
	  OutputDebugString(TEXT("Can only handle image type 2 and 10\r\n"));
      exit(-1);
   }
    
   if (tgaHeader.bitsperpixel != 16 && 
       tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32) {
      fprintf(stderr,"Can only handle pixel depths of 16, 24, and 32\n");
	   OutputDebugString(TEXT("Can only handle pixel depths of 16, 24, and 32\r\n"));
      exit(-1);
   }
   
   if (tgaHeader.colourmaptype != 0 && tgaHeader.colourmaptype != 1) {
      fprintf(stderr,"Can only handle colour map types of 0 and 1\n");
	    OutputDebugString(TEXT("Can only handle colour map types of 0 and 1\r\n"));
      exit(-1);
   }
  
   /* Skip over unnecessary stuff */
   skipover += tgaHeader.idlength;
   skipover += tgaHeader.colourmaptype * tgaHeader.colourmaplength;
   fprintf(stderr,"Skip over %d bytes\n",skipover);
   fseek(fptr,skipover,SEEK_CUR);

   /* Read the image */
   bytes2read = tgaHeader.bitsperpixel / 8;
   printf("bytes2read=%d, skipover=%d\n",bytes2read,skipover);
   //OutputDebugString(TEXT("Before  Read the image\r\n"));
   size_t out;
   int Index;
   if(!(tgaHeader.imagedescriptor & INVERTED_BIT))
	 {		 printf("In TAG1, Filename=%s, with Header.Descriptor=%d,tgaHeader.datatypecode=%d,tgaHeader.idlength=%d\n",fname,tgaHeader.imagedescriptor,tgaHeader.datatypecode,tgaHeader.idlength);
	 }
   //while (n < tgaHeader.width * tgaHeader.height) 

   for(int y = 0; y < *nrow; y++)
   {  
	   for(int x = 0; x < *ncol; x++)
        {
              Index= y * (*ncol) + x;
             if(!(tgaHeader.imagedescriptor & INVERTED_BIT))
			 		 Index= ((*nrow) - 1 - y) * (*ncol) + x;
					
			 

			  if (tgaHeader.datatypecode == 2) 
			  {     
				  /* Uncompressed */
					out=fread(p,1,bytes2read,fptr) ;	
					
				  if (out!= bytes2read) 
				  {
						fprintf(stderr,"Unexpected end of file at pixel %d\n",i); 
						//OutputDebugString(TEXT("Unexpected end of file at pixel == 2\r\n"));
						//printf("bytes2read=%d, out=%d\n",bytes2read,out);
						exit(-1);
				  }
				  MergeBytes(&(pix[Index]),p,bytes2read);
					 //n++;
			  } 
			  else if (tgaHeader.datatypecode == 10) /* Compressed */
			  {             
					 if (fread(p,1,bytes2read+1,fptr) != bytes2read+1) 
					 {
						fprintf(stderr,"Unexpected end of file at pixel %d\n",i);OutputDebugString(TEXT("Unexpected end of file at pixel == 10\r\n"));
						exit(-1);
					 }
					 j = p[0] & 0x7f;
					 MergeBytes(&(pix[Index]),&(p[1]),bytes2read);
					 //n++;
					 if (p[0] & 0x80) 
					 {         /* RLE chunk */
							for (i=0;i<j;i++) 
							{
							   MergeBytes(&(pix[Index]),&(p[1]),bytes2read);
							   //n++;
							}
					 } 
					 else 
					 {                   /* Normal chunk */
						for (i=0;i<j;i++) 
						{
						   if (fread(p,1,bytes2read,fptr) != bytes2read) 
						   {
							  fprintf(stderr,"Unexpected end of file at pixel %d\n",i);
							  exit(-1);
						   }
						   MergeBytes(&(pix[Index]),p,bytes2read);
						   //n++;
						}
					 }
			
				}//end if
			}//end of x
	}//end of y
 //OutputDebugString(TEXT("Before fclose()\r\n"));
   fclose(fptr);
   int t1=((81*32 + 31) >> 5) << 2;
   int t2=81*4;
   int dt1=t1-t2;
	 int b1=((81*24 + 31) >> 5) << 2;
   int b2=81*3;
   int db1=b1-b2;

   int a1=((320*24 + 31) >> 5) << 2;
   int a2=320*3;
   int da1=a1-a2;
   printf("t1=%d,t2=%d,dt1=%d\n",t1,t2,dt1);
  printf("a1=%d,a2=%d,da1=%d\n",a1,a2,da1);
   printf("b1=%d,b2=%d,db1=%d\n",b1,b2,db1);
	   
	return pix;
 }

   void writeTGA32(char * outTGA,RGBA_PIXEL *pixels,int nrows,int ncols)
   {
	FILE *fptr;
	   /* Write the result as a uncompressed TGA */
   if ((fptr = fopen(outTGA,"wb+")) == NULL) 
   {
      fprintf(stderr,"Failed to open outputfile\n");
      exit(-1);
   }
   putc(0,fptr);
   putc(0,fptr);
   putc(2,fptr);                         /* uncompressed RGB */
   putc(0,fptr); putc(0,fptr);
   putc(0,fptr); putc(0,fptr);
   putc(0,fptr);
   putc(0,fptr); putc(0,fptr);           /* X origin */
   putc(0,fptr); putc(0,fptr);           /* y origin */
   putc((ncols & 0x00FF),fptr);
   putc((ncols & 0xFF00) / 256,fptr);
   putc((nrows & 0x00FF),fptr);
   putc((nrows & 0xFF00) / 256,fptr);
   putc(32,fptr);                        /* 32 bit bitmap */
   putc(0,fptr);
   int index;
   for (int i=0;i<nrows;i++)
	   for(int j=0;j<ncols;  j++)
   {
	   index=(nrows-1-i)*ncols+j;
      putc(pixels[index].b,fptr);
      putc(pixels[index].g,fptr);
      putc(pixels[index].r,fptr);
      putc(pixels[index].a,fptr);
   }
   fclose(fptr);

}

void MergeBytes(RGBA_PIXEL *pixel,unsigned char *p,int bytes)
{
	   if (bytes == 4) 
	   {
		  pixel->r = p[2];
		  pixel->g = p[1];
		  pixel->b = p[0];
		  pixel->a = p[3];
	   } 
	   else if (bytes == 3) 
	   {
		  pixel->r = p[2];
		  pixel->g = p[1];
		  pixel->b = p[0];
		  pixel->a = 0;
	   } 
	   else if (bytes == 2) 
	   {
		  pixel->r = (p[1] & 0x7c) << 1;
		  pixel->g = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
		  pixel->b = (p[0] & 0x1f) << 3;
		  pixel->a = (p[1] & 0x80);
	   }
}


//通过得到的像素rgb和rgba值imageData构造bmp文件显示tga
int ShowTga(HDC hDC,LPRECT lpDCRect,int nTgaRow,int nTgaCol,int nTgaBits,unsigned char *imageData)		//在指定矩形窗口显示tga
{
	BITMAPINFO bi;
	//填充位图文件头
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = nTgaCol;
	bi.bmiHeader.biHeight = nTgaRow;
	bi.bmiHeader.biPlanes = 1; // Must be set to one
	
	if(nTgaBits == 24)		//24bit
		bi.bmiHeader.biBitCount = 24;
	else	//32bit
		bi.bmiHeader.biBitCount = 32;

	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biClrImportant=0;
	bi.bmiHeader.biClrUsed=0;
	if(nTgaBits == 24)	//24bit
		bi.bmiHeader.biSizeImage=((nTgaCol*3+3)/4*4)*nTgaRow;
	else		//32bit
		bi.bmiHeader.biSizeImage=((nTgaCol*4+3)/4*4)*nTgaRow;

	bi.bmiHeader.biXPelsPerMeter=0;
	bi.bmiHeader.biYPelsPerMeter=0;	
	//真彩色图24、32位无调色板，后面直接是位图数据
	
	//设置显示模式
	::SetStretchBltMode(hDC,COLORONCOLOR);

	int nSuccess = 0;	//成功标志

	//判断是调用StretchDIBits()还是SetDIBitsToDevice()来绘制DIB对象
	if(((lpDCRect->right - lpDCRect->left) == nTgaCol) &&
		((lpDCRect->bottom - lpDCRect->top) == nTgaRow))
	{
		printf("原始大小，不用拉伸\n");
		//原始大小，不用拉伸
		nSuccess = ::SetDIBitsToDevice(hDC,	//hDC
			lpDCRect->left,	//DestX
			lpDCRect->top,	//DestY
			lpDCRect->right - lpDCRect->left,	//nDestWidth
			lpDCRect->bottom - lpDCRect->top,	//nDestHeight
			0,	//SrcX
			0,	//SrcY
			0,	//nStartScan
			(WORD)nTgaRow,	//nNumScans
			imageData,	//lpBits
			&bi,	//lpBitsInfo
			DIB_RGB_COLORS);	//wUsage
	}
	else
	{printf("非原始大小，拉伸,wsize=%d,hsize=%d,nTgaCol=%d,nTgaRow=%d\n",lpDCRect->right - lpDCRect->left,lpDCRect->bottom - lpDCRect->top,nTgaCol,nTgaRow);
		//非原始大小，拉伸
		nSuccess = ::StretchDIBits(hDC,	//hDC
			lpDCRect->left,	//DestX
			lpDCRect->top,	//DestY
			lpDCRect->right - lpDCRect->left,	//nDestWidth
			lpDCRect->bottom - lpDCRect->top,	//nDestHeight
			0,	//SrcX
			0,	//SrcY
			nTgaCol,	//wSrcWidth
			nTgaRow,	//wSrcHeight
			imageData,	//lpBits
			&bi,	//lpBitsInfo
			DIB_RGB_COLORS,	//wUsage
			SRCCOPY);	//dwRop
	}
	return nSuccess;
}


//通过得到的像素rgb和rgba值imageData构造bmp文件显示tga
int DisplayTGA(HDC hDC,LPRECT lpDCRect,char *inTGA)		//在指定矩形窗口显示tga
{
	int nrow,ncol,Depth,t,index;
	unsigned char *buf=NULL;
	RGBA_PIXEL *rgba=LoadTGA1(inTGA, &ncol,&nrow, &Depth);
	if(Depth==24)
	{	buf=(unsigned char *)calloc(nrow*ncol*3,sizeof(unsigned char));
		t=0;
		for(int i=nrow-1;i>=0;i--)	
		{	for(int j=0;j<ncol;j++)
			{
				index=i*ncol+j;
				buf[t]=rgba[index].b;
				buf[t+1]=rgba[index].g;
				buf[t+2]=rgba[index].r;
				t=t+3;
			}
		}
	}
	else if(Depth==32)
	{		
		buf=(unsigned char *)calloc(nrow*ncol*4,sizeof(unsigned char));
		t=0;
		for(int i=nrow-1;i>=0;i--)	
		{	for(int j=0;j<ncol;j++)
			{
				index=i*ncol+j;
				buf[t]=rgba[index].b;
				buf[t+1]=rgba[index].g;
				buf[t+2]=rgba[index].r;
				buf[t+3]=rgba[index].a;

				t=t+4;
			}
		}
	}
	else
	{
		printf("Unrecogned image format !\n");
		return false;
	}
	if(rgba)
	{
		free(rgba);
		rgba=NULL;
	}
	if(!OnPaintTga(hDC,lpDCRect,nrow,ncol,Depth,buf))	
	//if(!ShowTga(hDC,lpDCRect,nrow,ncol,Depth,buf))
	{
		printf("Display DIB false\n");	
		return false;
	}
	if(buf)
	{
		free(buf);
		buf=NULL;
	} 
	return true;
}


//通过得到的像素rgb和rgba值imageData构造bmp文件显示tga
int OnPaintTga(HDC hDC,LPRECT lpDCRect,int nTgaRow,int nTgaCol,int nTgaBits,unsigned char *imageData)		//在指定矩形窗口显示tga
{

	

	IMAGE_STRUCT *bmp=NULL;

	imageBMP *imgBMP=new imageBMP( nTgaRow,nTgaCol);
	bmp=imgBMP->image_struct;
	bmp->bytes_per_pixel=3;
	PIXEL *ptr=bmp->pixel;
	int numOfPixel=nTgaRow*nTgaCol;
	for(int i=0;i<numOfPixel;i++)
	{
			ptr->red=imageData[i*3]; //(bufTGA[i*3]); 
			ptr->green=imageData[i*3+1]; //(bufTGA[i*3+1]); 
			ptr->blue=imageData[i*3+2]; //bufTGA[i*3+2]); 
			//printf("(r=%d,c=%d),",ptr->red,_CHAR2INT(bufTGA[i*3]));
			ptr++;
	}
	//imgBMP->Write_BMP(bmp,"F:\\apple3-QinQinGLES2-2011-5-19\\bin\\test14.bmp");
	if(imgBMP)
	{	delete imgBMP;
		imgBMP=NULL;
	}	
	BITMAPINFO bi;
	//填充位图文件头
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = nTgaCol;
	bi.bmiHeader.biHeight = nTgaRow;
	bi.bmiHeader.biPlanes = 1; // Must be set to one
	
	if(nTgaBits == 24)		//24bit
		bi.bmiHeader.biBitCount = 24;
	else	//32bit
		bi.bmiHeader.biBitCount = 32;

	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biClrImportant=0;
	bi.bmiHeader.biClrUsed=0;
	//bi.bmiHeader.biSizeImage=nTgaCol*nTgaRow*nTgaBits/8;
	if(nTgaBits == 24)	//24bit
		bi.bmiHeader.biSizeImage=(((nTgaCol*3+3)>>2)<<2)*nTgaRow;
	else		//32bit
		bi.bmiHeader.biSizeImage=(((nTgaCol*4+3)>>2)<<2)*nTgaRow;

	bi.bmiHeader.biXPelsPerMeter=0;
	bi.bmiHeader.biYPelsPerMeter=0;
	//bi.bmiColors[0].rgbBlue = 0;
	//bi.bmiColors[0].rgbGreen = 0;
	//bi.bmiColors[0].rgbRed = 0;
	//bi.bmiColors[0].rgbReserved = 0;
	//真彩色图24、32位无调色板，后面直接是位图数据
	
	//	HWND hwnd = WindowFromDC(hDC);

	//	HDC hdc,//DC可以用桌面的 
	//hdc = GetDC(GetDlgItem(hwnd,IDC_BITMAPFRAME));
	//hdc=::GetDC(hwnd);//DC可以用桌面的 
	//hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); //为屏幕创建设备描述表 
	
	HDC	hdcMEM; //获得是内存DC 
	hdcMEM = CreateCompatibleDC(hDC); //获得是内存DC 
		int nSuccess = 0;	//成功标志
	
	//unsigned char *pBits;;//=imageData;

	//HBITMAP   hbmp=::CreateDIBSection(hDC,&bi,DIB_RGB_COLORS,(void **)&pBits,NULL,0);

	//

	//int WidthBytes=((int)((nTgaCol*nTgaBits+31)/32))*4;

	//printf("WidthBytes=%d,nTgaCol=%d,nTgaBits=%d\n",WidthBytes,nTgaCol,nTgaBits);
	//if(hbmp)
	//{
	//	if(nTgaBits == 24)
	//		 memcpy(pBits,imageData,nTgaRow*nTgaCol*3);
	//	
	//	if(nTgaBits == 32)
	//		 
	//		memcpy(pBits,imageData,nTgaRow*nTgaCol*4);
	//	/*for(int k=0;k<nTgaRow;k++)
	//		SetDIBits(hDC,hbmp,k,1,imageData+WidthBytes*k,&bi,DIB_RGB_COLORS);	*/
	//	// 把新位图选到内存设备描述表中 
	//	HBITMAP pOld=(HBITMAP)SelectObject(hdcMEM,hbmp);
	//	
	//	//GetObject(hbmp,sizeof(BITMAP));

	//	//GetClientRect(GetDlgItem(hwnd,IDC_BITMAPFRAME),lpDCRect);
	//	/*InflateRect(lpDCRect,-20,-20);*/

	//	// Hide the application window.  
	//	//  ShowWindow(hwnd, SW_HIDE);  


	//	//原样贴图
	//
	//	// 把内存设备描述表拷贝到屏幕设备描述表中 
	//		nSuccess =BitBlt(hDC,lpDCRect->left,lpDCRect->top,lpDCRect->right - lpDCRect->left,lpDCRect->bottom - lpDCRect->top,hdcMEM,0,0  ,SRCCOPY);	
	//		SelectObject(hdcMEM,pOld);//得到屏幕位图的句柄 
	//		DeleteObject(hbmp);
	//}
// Redraw the application window.  
 
//  ShowWindow(hwnd, SW_SHOW);  

			::SetStretchBltMode(hDC,COLORONCOLOR);
		///*判断是调用StretchDIBits()还是SetDIBitsToDevice()来绘制DIB对象*/
		if(((lpDCRect->right - lpDCRect->left) == nTgaCol) &&
			((lpDCRect->bottom - lpDCRect->top) == nTgaRow))
		{
			printf("原始大小，不用拉伸\n");
			//原始大小，不用拉伸
			nSuccess = ::SetDIBitsToDevice(hDC,	//hDC
				lpDCRect->left,	//DestX
				lpDCRect->top,	//DestY
				lpDCRect->right - lpDCRect->left,	//nDestWidth
				lpDCRect->bottom - lpDCRect->top,	//nDestHeight
				0,	//SrcX
				0,	//SrcY
				0,	//nStartScan
				(WORD)nTgaRow,	//nNumScans
				imageData,	//lpBits
				&bi,	//lpBitsInfo
				DIB_RGB_COLORS);	//wUsage
		}
		else
		{printf("非原始大小，拉伸,wsize=%d,hsize=%d,nTgaCol=%d,nTgaRow=%d\n",lpDCRect->right - lpDCRect->left,lpDCRect->bottom - lpDCRect->top,nTgaCol,nTgaRow);
			//非原始大小，拉伸
			nSuccess = ::StretchDIBits(hDC,	//hDC
				lpDCRect->left,	//DestX
				lpDCRect->top,	//DestY
				lpDCRect->right - lpDCRect->left,	//nDestWidth
				lpDCRect->bottom - lpDCRect->top,	//nDestHeight
				0,	//SrcX
				0,	//SrcY
				nTgaCol,	//wSrcWidth
				nTgaRow,	//wSrcHeight
				imageData,	//lpBits
				&bi,	//lpBitsInfo
				DIB_RGB_COLORS,	//wUsage
				SRCCOPY);	//dwRop
		}
	
		//ReleaseDC(hwnd,hdc);
	DeleteObject( hdcMEM);
	return nSuccess;
}