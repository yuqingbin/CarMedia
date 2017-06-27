#include "StdAfx.h"
#include "Mp3Apic.h"

Mp3Apic::Mp3Apic(HWND hWnd)
{
	g_hWnd = hWnd;
	
}

Mp3Apic::~Mp3Apic(void)
{
	
}
BOOL Mp3Apic::ReadMp3Apic(TCHAR *inFile,char **Buffer)
{
	FILE *fp;
	fp = _wfopen(inFile,L"rb");
	if(!fp)
		return FALSE;//MessageBox(hWnd,L"打开文件失败",L"",NULL);
	BYTE *pBuf;
	pBuf = new BYTE[10];  //标签头，标签帧头
	fseek(fp,0,0);
	fread(pBuf,1,10,fp);  //读取标签头
	//判断是不是ID3V2
	if((pBuf[0]!='I')|(pBuf[1]!='D')|(pBuf[2]!='3'))
	{
		fclose(fp);
		delete[] pBuf;
		return FALSE;//break;
	}
	//是ID3V2继续
	//ID3V2  大小
	DWORD ID3V2Size = (pBuf[6]&0x7f)*0x200000+(pBuf[7]&0x7f)*0x400+(pBuf[8]&0x7f)*0x80+(pBuf[9]&0x7f);
	fread(pBuf,1,10,fp); //第一个标签帧头
	DWORD Pos = 0;
	while((pBuf[0]!='A')|(pBuf[1]!='P')|(pBuf[2]!='I')|(pBuf[3]!='C')&&(Pos<ID3V2Size)) //在标签体大小范围内查找专辑图片标签
	{
		FrameSize = pBuf[4]*0x1000000+pBuf[5]*0x10000+pBuf[6]*0x100+pBuf[7]; //标签帧体大小
		fseek(fp,FrameSize,SEEK_CUR); //移动到下标签帧
		Pos = Pos+10+FrameSize;
		fread(pBuf,1,10,fp);
	}
	//查看是否找到
	if((pBuf[0]!='A')|(pBuf[1]!='P')|(pBuf[2]!='I')|(pBuf[3]!='C'))
	{
		delete[] pBuf;
		fclose(fp);
		return FALSE;
	}
	//计算专辑图片标签体大小
	FrameSize = pBuf[4]*0x1000000+pBuf[5]*0x10000+pBuf[6]*0x100+pBuf[7]; //标签体大小
	//
	BYTE *pDataBuf;
	pDataBuf = new BYTE[FrameSize];
	BYTE *pDataBufTemp;
	pDataBufTemp = pDataBuf;
	*Buffer = new char[FrameSize];
	fread(pDataBuf,1,FrameSize,fp);
	//图片类型
	memcpy(pBuf,pDataBuf,10);
	int i=0;
	if(((pBuf[1]=='i')|(pBuf[1]=='I'))&&((pBuf[2]=='m')|(pBuf[2]=='M'))&&((pBuf[3]=='a')|(pBuf[3]=='A'))
		&&((pBuf[4]=='g')|(pBuf[4]=='G'))&&((pBuf[5]=='e')|(pBuf[5]=='E'))&&(pBuf[6]=='/')&&((pBuf[7]=='j')|(pBuf[7]=='J'))&&((pBuf[8]=='p')|(pBuf[8]=='P')))
	{
		//jpeg压缩格式图片
		BYTE *buf;
		buf = new BYTE[4];
		pDataBuf = pDataBuf+10;
		memcpy(buf,pDataBuf,1);
here:  while((buf[0] != 0xff))
	   {
		   pDataBuf++;
		   memcpy(buf,pDataBuf,1);
		   i++;
		   if(i>100)
			   break;
	   }
	   memcpy(buf,pDataBuf+1,3);
	   if(i>100)
	   {
		   if(buf)
		   {  
			   delete[] buf;
			   buf = NULL;
		   }
		   goto finish1;
	   }
	   if((buf[0]!=0xD8)|(buf[1]!=0xFF)|(buf[2]!=0xE0))
	   {
		   pDataBuf++;
		   memcpy(buf,pDataBuf,1);
		   i++;
		   goto here;
	   }
	   
		 delete[] buf;
		 buf = NULL;
	}
	else
		if(((pBuf[1]=='i')|(pBuf[1]=='I'))&&((pBuf[2]=='m')|(pBuf[2]=='M'))&&((pBuf[3]=='a')|(pBuf[3]=='A'))
			&&((pBuf[4]=='g')|(pBuf[4]=='G'))&&((pBuf[5]=='e')|(pBuf[5]=='E'))&&(pBuf[6]=='/')&&((pBuf[7]=='p')|(pBuf[7]=='P'))&&((pBuf[8]=='n')|(pBuf[8]=='N')))
		{
			BYTE *buf;
			buf = new BYTE[4];
			pDataBuf = pDataBuf+10;
			memcpy(buf,pDataBuf,1);
here1:  while((buf[0] != 0x89))
		{
			pDataBuf++;
			memcpy(buf,pDataBuf,1);
			i++;
			if(i>100)
				break;
		}
		memcpy(buf,pDataBuf+1,3);
		if(i>100)
		{
			if(buf)
			{  
				delete[] buf;
				buf = NULL;
			}
			goto finish1;
		}
		if((buf[0]!=0x50)|(buf[1]!=0x4E)|(buf[2]!=0x47))
		{
			pDataBuf++;
			memcpy(buf,pDataBuf,1);
			i++;
			goto here1;
		}
			delete[] buf;
			buf = NULL;
	   }
		memcpy(*Buffer,pDataBuf,FrameSize);
		if(pBuf)
		{  
			delete[] pBuf;
			pBuf = NULL;
		}
		if(pDataBufTemp)
		{
			delete[] pDataBufTemp;
			pDataBufTemp = NULL;
		}
		fclose(fp);
		return TRUE;

finish1 :
		if(pBuf)
		{  
			delete[] pBuf;
			pBuf = NULL;
		}
		if(pDataBufTemp)
		{
			delete[] pDataBufTemp;
			pDataBufTemp = NULL;
		}
		fclose(fp);
	    return FALSE;
}


int Mp3Apic::MP3Load(TCHAR *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth)
{
	IImage *pImage = NULL;
	IImagingFactory *pImagingFactory= NULL;
	IBitmapImage *pBitmap= NULL;
   
	int FrameSize;
    FILE *fp;
	fp = _wfopen(Filename,L"rb");
	if(!fp)
		return FALSE;//MessageBox(hWnd,L"打开文件失败",L"",NULL);
	BYTE *pBuf;
	pBuf = new BYTE[10];  //标签头，标签帧头
	fseek(fp,0,0);
	fread(pBuf,1,10,fp);  //读取标签头
	//判断是不是ID3V2
	if((pBuf[0]!='I')|(pBuf[1]!='D')|(pBuf[2]!='3'))
	{
		fclose(fp);
		delete[] pBuf;
		return FALSE;//break;
	}
	//是ID3V2继续
	//ID3V2  大小
	DWORD ID3V2Size = (pBuf[6]&0x7f)*0x200000+(pBuf[7]&0x7f)*0x400+(pBuf[8]&0x7f)*0x80+(pBuf[9]&0x7f);
	fread(pBuf,1,10,fp); //第一个标签帧头
	DWORD Pos = 0;

	while((pBuf[0]!='A')|(pBuf[1]!='P')|(pBuf[2]!='I')|(pBuf[3]!='C')&&(Pos<ID3V2Size)) //在标签体大小范围内查找专辑图片标签
	{
		FrameSize = pBuf[4]*0x1000000+pBuf[5]*0x10000+pBuf[6]*0x100+pBuf[7]; //标签帧体大小
		fseek(fp,FrameSize,SEEK_CUR); //移动到下标签帧
		Pos = Pos+10+FrameSize;
		fread(pBuf,1,10,fp);
	}
	//查看是否找到
	if((pBuf[0]!='A')|(pBuf[1]!='P')|(pBuf[2]!='I')|(pBuf[3]!='C'))
	{
		delete[] pBuf;
		fclose(fp);
		return FALSE;
	}
	//计算专辑图片标签体大小
	FrameSize = pBuf[4]*0x1000000+pBuf[5]*0x10000+pBuf[6]*0x100+pBuf[7]; //标签体大小
	//
	BYTE *pDataBuf;
	pDataBuf = new BYTE[FrameSize];
	BYTE *pDataBufTemp;
	pDataBufTemp = pDataBuf;            //保存初始地址

	fread(pDataBuf,1,FrameSize,fp);
	//图片类型
	memcpy(pBuf,pDataBuf,10);
	int i=0;
	if(((pBuf[1]=='i')|(pBuf[1]=='I'))&&((pBuf[2]=='m')|(pBuf[2]=='M'))&&((pBuf[3]=='a')|(pBuf[3]=='A'))
		&&((pBuf[4]=='g')|(pBuf[4]=='G'))&&((pBuf[5]=='e')|(pBuf[5]=='E'))&&(pBuf[6]=='/')&&((pBuf[7]=='j')|(pBuf[7]=='J'))&&((pBuf[8]=='p')|(pBuf[8]=='P')))
	{
		//jpeg压缩格式图片
		BYTE *buf;
		buf = new BYTE[4];
		pDataBuf = pDataBuf+10;
		memcpy(buf,pDataBuf,1);
here:  while((buf[0] != 0xff))
	   {
		   pDataBuf++;
		   memcpy(buf,pDataBuf,1);
		   i++;
		   if(i>100)
			   break;
	   }
	   memcpy(buf,pDataBuf+1,3);

	   if(i>100)
	   {
		   delete[] buf;
		   buf = NULL;
		   goto finish1;
	   }

	   if((buf[0]!=0xD8)|(buf[1]!=0xFF)|(buf[2]!=0xE0))
	   {
		   pDataBuf++;
		   memcpy(buf,pDataBuf,1);
		   i++;
		   goto here;
	   }
	   delete[] buf;
	   buf = NULL;

	}
	else
		if(((pBuf[1]=='i')|(pBuf[1]=='I'))&&((pBuf[2]=='m')|(pBuf[2]=='M'))&&((pBuf[3]=='a')|(pBuf[3]=='A'))
			&&((pBuf[4]=='g')|(pBuf[4]=='G'))&&((pBuf[5]=='e')|(pBuf[5]=='E'))&&(pBuf[6]=='/')&&((pBuf[7]=='p')|(pBuf[7]=='P'))&&((pBuf[8]=='n')|(pBuf[8]=='N')))
		{
			BYTE *buf;
			buf = new BYTE[4];
			pDataBuf = pDataBuf+10;
			memcpy(buf,pDataBuf,1);
here1:  while((buf[0] != 0x89))
		{
			pDataBuf++;
			memcpy(buf,pDataBuf,1);
			i++;
			if(i>100)
				break;
		}
		memcpy(buf,pDataBuf+1,3);
		if(i>100)
		{
			delete[] buf;
			buf = NULL;
			goto finish1;
		}
		if((buf[0]!=0x50)|(buf[1]!=0x4E)|(buf[2]!=0x47))
		{
			pDataBuf++;
			memcpy(buf,pDataBuf,1);
			i++;
			goto here1;
		}
		delete[] buf;
		buf = NULL;

		}
		////////////////////////////////////////
		CoCreateInstance(CLSID_ImagingFactory, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IImagingFactory, 
			(void**) &pImagingFactory);
		int width = 300;
		int height = 300;
		*Width = width;
		*Height = height;
        *Depth = 24;
        pImagingFactory->CreateImageFromBuffer(pDataBuf,FrameSize-10-i,BufferDisposalFlagNone,&pImage);
        pImagingFactory->CreateBitmapFromImage(pImage,width,height,PixelFormat24bppRGB,InterpolationHintDefault,&pBitmap); //丢失
		

		BitmapData bmpdata = {0};
		
		bmpdata.Reserved = 0;
		bmpdata.Height = height;
		bmpdata.Width = width;
		bmpdata.PixelFormat= PixelFormat24bppRGB;

		//////////////////////////////////////////////////////////////////////////////////////
		BITMAPINFO bitmapInfo;
		ZeroMemory(&bitmapInfo,sizeof(BITMAPINFO));
		
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth = width;
		bitmapInfo.bmiHeader.biHeight = -height;   ////51板子正的  模拟器 负的
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 24;//GetPixelFormatSize(Imageinfo.PixelFormat);
		int LineByte = (300*bitmapInfo.bmiHeader.biBitCount/8+3)/4*4;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
		bitmapInfo.bmiHeader.biSizeImage	= LineByte*height;

		BYTE* lpBitmapBits = NULL;      //
   
		RECT rect = {0,0,0,0};
		rect.right = width;
		rect.bottom = height;
		pBitmap->LockBits(&rect,ImageLockModeRead |ImageLockModeWrite,bmpdata.PixelFormat,&bmpdata);         //评估板出现错误
        

		HBITMAP hbitmap = CreateDIBSection(0, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (void**)&lpBitmapBits,0,0);
		if (bmpdata.Stride < 0)
		{
			bmpdata.Stride = bmpdata.Stride * -1;
			bmpdata.Scan0 = (void*) ( ((DWORD)bmpdata.Scan0 + bmpdata.Stride) - LineByte*height);
		}
		memcpy(lpBitmapBits,bmpdata.Scan0,LineByte*height);  //数据现在在
		pBitmap->UnlockBits(&bmpdata);
        
        *Buffer= (unsigned char*) malloc(3 * (*Width) * (*Height));
        memcpy(*Buffer,lpBitmapBits,3 * (*Width) * (*Height));
        
        
		DeleteObject(hbitmap);

		delete[] pBuf;
		pBuf = NULL;

		delete[] pDataBufTemp;
		pDataBufTemp = NULL;

		pImage->Release();    //释放接口
        pImage = NULL;
		pImagingFactory->Release();  
		pImagingFactory = NULL;
		pBitmap->Release();
		pBitmap = NULL;

        

		fclose(fp);
		return TRUE;  //成功返回

finish1 :

		delete[] pBuf;
		pBuf = NULL;

		delete[] pDataBufTemp;
		pDataBufTemp = NULL;

		fclose(fp);
		return FALSE;
}