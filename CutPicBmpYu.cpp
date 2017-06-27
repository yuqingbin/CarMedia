#include "StdAfx.h"
#include "CutPicBmpYu.h"

CutPicBmpYu::CutPicBmpYu(void)
{
	pBuf = NULL;
	pNewBuf = NULL;
	bmpWidth = 0;
	bmpHeight = 0;
	biBitCount = 0;
	linebyte = 0;
	newlinebyte = 0;
}

CutPicBmpYu::~CutPicBmpYu(void)
{
	if(pNewBuf)
	{
		delete[] pNewBuf;
		pNewBuf = NULL;
	}
	if(pBuf)
	{
		delete[] pBuf;
		pBuf = NULL;
	}
	if(rotateBmpBuf)
	{
		delete[] rotateBmpBuf;
		rotateBmpBuf = NULL;
	}
	if(rotateBmpBuf1)
	{
		delete[] rotateBmpBuf1;
		rotateBmpBuf1 = NULL;
	}
}
BOOL CutPicBmpYu::ReadBmp(TCHAR *pFileIn)
{
	FILE *fp = _wfopen(pFileIn,L"rb");
	if(!fp)
	{
		return FALSE;
	}
	fseek(fp,sizeof(BITMAPFILEHEADER),0);
	ZeroMemory(&bh,sizeof(BITMAPINFOHEADER));
	fread(&bh,sizeof(BITMAPINFOHEADER),1,fp);
	bmpWidth = bh.biWidth;
	bmpHeight = bh.biHeight;
	biBitCount = bh.biBitCount;
	linebyte = (bmpWidth*biBitCount/8+3)/4*4;
	pBuf = new unsigned char[linebyte*bmpHeight];
	fread(pBuf,1,linebyte*bmpHeight,fp);
	fclose(fp);
	return true;
}
BOOL CutPicBmpYu::SaveBmp(TCHAR *pFileOut,long Width,long Height)
{
	float FYZOOMRATIO = (float)((Height-0.5)/bmpHeight);
	float FXZOOMRATIO = (float)((Width-0.5)/bmpWidth);
	//int Colortablesize = 0;
	newlinebyte = (Width*biBitCount/8+3)/4*4;
	long  i,j,k;
	long i_0,j_0;

	pNewBuf = new unsigned char[Height*newlinebyte];

	if(biBitCount == 24)
	{
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{
				for(k=0;k<3;k++)
				{
					i_0 = (long)(i/FYZOOMRATIO+0.5);
					j_0 = (long)(j/FXZOOMRATIO+0.5);
					if((j_0>=0)&&(j_0<bmpWidth)&&(i_0>=0)&&(i_0<bmpHeight))
					{
						*(pNewBuf+i*newlinebyte+j*3+k) = *(pBuf+i_0*linebyte+j_0*3+k);
					}
					else
					{
						*(pNewBuf+i*newlinebyte+j*3+k) = 255;
					}
				}
			}
		}
	}
	///旋转
#ifdef  ROTATE90
	{
		//unsigned char *rotateBmpBuf;//宣旋转后的位图空间指针
		long rotateBmpHeight = Width;//旋转后的位图高newBmpWidth
		long rotateBmpWidth  = Height;//旋转后的位图宽newBmpHeight
		long rotateLineByte  = (rotateBmpWidth*biBitCount/8+3)/4*4;//计算旋转后每行的字节数
		rotateBmpBuf = new unsigned char[rotateLineByte*rotateBmpHeight];//申请旋转后位图所需的内存空间

		long  i,j,k;
		long i_0,j_0;
		int lineByte = (Width*biBitCount/8+3)/4*4;
		if(biBitCount == 24)
		{
			for(i=0;i<Height;i++)
			{
				for(j=0;j<Width;j++)
				{
					for(k=0;k<3;k++)
					{
						i_0 = j;
						j_0 = Height-i;
						if((j_0>=0)&&(j_0<Height)&&(i_0>=0)&&(i_0<Width))
						{
							*(rotateBmpBuf+i_0*rotateLineByte+j_0*3+k) = *(pNewBuf+i*lineByte+j*3+k);
						}
						else
						{
							*(rotateBmpBuf+i*rotateLineByte+j*3+k) = 255;
						}
					}
				}
			}
		}
		
	}
#endif
#ifdef  ROTATE180
	{
		//unsigned char *rotateBmpBuf;//宣旋转后的位图空间指针
		long rotateBmpHeight = Width;//旋转后的位图高newBmpWidth
		long rotateBmpWidth  = Height;//旋转后的位图宽newBmpHeight
		long rotateLineByte  = (rotateBmpWidth*biBitCount/8+3)/4*4;//计算旋转后每行的字节数
		rotateBmpBuf1 = new unsigned char[rotateLineByte*rotateBmpHeight];//申请旋转后位图所需的内存空间

		long  i,j,k;
		long i_0,j_0;
		int lineByte = (Width*biBitCount/8+3)/4*4;
		if(biBitCount == 24)
		{
			for(i=0;i<Height;i++)
			{
				for(j=0;j<Width;j++)
				{
					for(k=0;k<3;k++)
					{
						i_0 = j;
						j_0 = Height-i;
						if((j_0>=0)&&(j_0<Height)&&(i_0>=0)&&(i_0<Width))
						{
							*(rotateBmpBuf1+i_0*rotateLineByte+j_0*3+k) = *(rotateBmpBuf+i*lineByte+j*3+k);
						}
						else
						{
							*(rotateBmpBuf1+i*rotateLineByte+j*3+k) = 255;
						}
					}
				}
			}
		}

	}
#endif
	//////////////////////保存图片文件/////////////////////////

	if(pFileOut == NULL)  //不保存文件 保留数据块地址
		goto over;
	int num = 0;
agine:	FILE *ffp = _wfopen(pFileOut,L"wb");
    if(!ffp)
	{   
		num++;
		Sleep(200);
		if(num == 2)
			return FALSE;
		goto agine;  
	}
	ZeroMemory(&Sbfh,sizeof(BITMAPFILEHEADER));
	Sbfh.bfReserved1 = 0;
	Sbfh.bfReserved2 = 0;
	Sbfh.bfType = 0x4d42;
	Sbfh.bfOffBits = 54;
	Sbfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + newlinebyte*Height;
	fwrite(&Sbfh,sizeof(BITMAPFILEHEADER),1,ffp);

	ZeroMemory(&Sbi,sizeof(BITMAPINFOHEADER));
	Sbi.biBitCount = biBitCount;
	Sbi.biClrImportant = 0;
	Sbi.biClrUsed = 0;
	Sbi.biCompression = 0;
	Sbi.biHeight = Height;
	Sbi.biPlanes = 1;
	Sbi.biSize = 40;
	Sbi.biSizeImage = newlinebyte*Height;
	Sbi.biWidth = Width;
	Sbi.biXPelsPerMeter = 0;
	Sbi.biYPelsPerMeter = 0;
	fwrite(&Sbi,sizeof(BITMAPINFOHEADER),1,ffp);
	fwrite(pNewBuf,Height*newlinebyte,1,ffp);
	fclose(ffp);

	if(pNewBuf)    //数据块地址
	{
		delete[] pNewBuf;
		pNewBuf = NULL;
	}
over:	if(pBuf)            
	{
		delete[] pBuf;
		pBuf = NULL;
	}
	if(rotateBmpBuf)
	{
		delete[] rotateBmpBuf;
		rotateBmpBuf = NULL;
	}
	if(rotateBmpBuf1)
	{
		delete[] rotateBmpBuf1;
		rotateBmpBuf1 = NULL;
	}
	return TRUE;
}

BOOL CutPicBmpYu::CutBmp(TCHAR *pFileIn,TCHAR *pFileOut,long Width,long Height,BYTE **Buffer)
{
	if(!ReadBmp(pFileIn))
	{
		if(pBuf)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
		return FALSE;
	}
	if(!SaveBmp(pFileOut,Width,Height))
	{
		if(pNewBuf)
		{
			delete[] pNewBuf;
			pNewBuf = NULL;
		}
		if(pBuf)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
		if(rotateBmpBuf)
		{
			delete[] rotateBmpBuf;
			rotateBmpBuf = NULL;
		}
		if(rotateBmpBuf1)
		{
			delete[] rotateBmpBuf1;
			rotateBmpBuf1 = NULL;
		}
		return FALSE;
	}
	*Buffer = pNewBuf;
	return TRUE;
}

