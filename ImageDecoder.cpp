/*
ImageDecoder.cpp
  Class ImageDecoder Implementation, base upon imaging com interface.
*/

#include "ImageDecoder.h"
//#include "debug.h"

ImageDecoder::ImageDecoder(TCHAR* ImageFileName)
{
	HRESULT hr;
	ImageInfo ii;

	lstrcpy(this->filename, ImageFileName);
	
	hr = CoCreateInstance(CLSID_ImagingFactory, 
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IImagingFactory, 
                          (void**) &pImagingFactory);
    if(FAILED(hr))
    {
		//printf("CoCreateInstance failed, hr:0x08x\n", hr);
		goto finish;
	}
	//printf("Opening %s", filename);

	if(FAILED(hr = CreateStreamOnFile(filename, &pStream)))
	{
		//printf("CreateStreamOnFile failed, hr: 0x%08x\n", hr);
		goto finish;
	}

	GetCodecCLSID(pImagingFactory, NULL, NULL, eDecoder);
	
	if(FAILED(hr = pImagingFactory->CreateImageDecoder(pStream, DecoderInitFlagBuiltIn1st, &pImageDecoder)))
	{
		//printf("CreateImageDecoder failed, hr: 0x%08x\n", hr);
		goto finish;
	}
		
    if (FAILED(hr = pImageDecoder->GetFrameCount(&FrameDimensionTime, &nFrameCount)))
	{
        //printf("GetFrameCount failed, hr: 0x%08x\n", hr);
        nFrameCount = 0;
        //JPG file will fail this call, just pass it
		//goto finish;
	}

	pImageDecoder->GetImageInfo(&ii);
	
    this->ii = ii;

	if(FAILED(hr = pImagingFactory->CreateNewBitmap(ii.Width, ii.Height, ii.PixelFormat, &pBitmapImage)))
	{
		//printf("CreateNewBitmap failed, hr = 0x%08x\n", hr);
		goto finish;
	}
	
	if(FAILED(hr = pBitmapImage->QueryInterface(IID_IImageSink, (void**)&pImageSink)))
	{
		//printf("QueryInterface for ImageSink from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}
	
	if(FAILED(hr = pBitmapImage->QueryInterface(IID_IImage, (void**)&pImage)))
	{
		//printf("QueryInterface for Image from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}

	if (FAILED(hr = pBitmapImage->QueryInterface(IID_IBasicBitmapOps, (void**)&pBasicBitmapOps)))
	{
		//printf("QueryInterface for BasicBitmapOps from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}
	state = Init;
	goto finish_ok;

finish:
	state = UnInit;
finish_ok:
	Angle = 0;
	pBitmapImageRotate = NULL;
	pImageRotate = NULL;
    nSelectedFrame = 0;
	return;
}

ImageDecoder::ImageDecoder(HRSRC hRes)
{
	HRESULT hr;
	ImageInfo ii;

	//lstrcpy(this->filename, ImageFileName);

	hr = CoCreateInstance(CLSID_ImagingFactory, 
                          NULL, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IImagingFactory, 
                          (void**) &pImagingFactory);
    if(FAILED(hr))
    {
		//printf("CoCreateInstance failed, hr:0x08x\n", hr);
		goto finish;
	}
	
	//printf("Opening %s"), filename);

	//if(FAILED(hr = CreateStreamOnFile(filename, &pStream)))
	if(FAILED(hr = CreateStreamOnResource(hRes, &pStream)))
	{
		//printf("CreateStreamOnFile failed, hr: 0x%08x\n", hr);
		goto finish;
	}

	GetCodecCLSID(pImagingFactory, NULL, NULL, eDecoder);
	
	if(FAILED(hr = pImagingFactory->CreateImageDecoder(pStream, DecoderInitFlagBuiltIn1st, &pImageDecoder)))
	{
		//printf("CreateImageDecoder failed, hr: 0x%08x\n", hr);
		goto finish;
	}
		
    if (FAILED(hr = pImageDecoder->GetFrameCount(&FrameDimensionTime, &nFrameCount)))
	{
        //printf("GetFrameCount failed, hr: 0x%08x\n", hr);
        nFrameCount = 0;
        //JPG file will fail this call, just pass it
		//goto finish;
	}

	pImageDecoder->GetImageInfo(&ii);
	
    this->ii = ii;

	if(FAILED(hr = pImagingFactory->CreateNewBitmap(ii.Width, ii.Height, ii.PixelFormat, &pBitmapImage)))
	{
		//printf("CreateNewBitmap failed, hr = 0x%08x\n", hr);
		goto finish;
	}
	
	if(FAILED(hr = pBitmapImage->QueryInterface(IID_IImageSink, (void**)&pImageSink)))
	{
		//printf("QueryInterface for ImageSink from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}
	
	if(FAILED(hr = pBitmapImage->QueryInterface(IID_IImage, (void**)&pImage)))
	{
		//printf("QueryInterface for Image from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}

	if (FAILED(hr = pBitmapImage->QueryInterface(IID_IBasicBitmapOps, (void**)&pBasicBitmapOps)))
	{
		//printf("QueryInterface for BasicBitmapOps from BitmapImage failed, hr: 0x%08x\n", hr);
		goto finish;
	}
	state = Init;
	goto finish_ok;

finish:
	state = UnInit;
finish_ok:
	Angle = 0;
	pBitmapImageRotate = NULL;
	pImageRotate = NULL;
    nSelectedFrame = 0;
	return;
}

ImageDecoder::~ImageDecoder()
{
    if(pImageRotate)
	{
		pImageRotate->Release();
		pImageRotate = NULL;
	}

	if(pBitmapImageRotate)
	{
		pBitmapImageRotate->Release();
		pBitmapImageRotate = NULL;
	}
	if(pBasicBitmapOps)
	{
		pBasicBitmapOps->Release();
		pBasicBitmapOps = NULL;
	}
    if(pImage)
	{
		pImage->Release();
        pImage = NULL;
	}
	if(pImageSink)	
	{
		pImageSink->Release();
		pImageSink = NULL;
	}
	if(pBitmapImage)
	{
		pBitmapImage->Release();
        pBitmapImage = NULL;
	}
    if(pImageDecoder)
	{
		pImageDecoder->Release();
		pImageDecoder = NULL;
	}
    if(pStream)
	{
		pStream->Release();
        pStream = NULL;
	}
    if(pImagingFactory)
	{
		pImagingFactory->Release();
		pImagingFactory = NULL;
	}
	return;
}

void ImageDecoder::Decode() //decode the image
{
	HRESULT hr;
	if (FAILED(hr = pImageDecoder->BeginDecode(pImageSink, NULL)))
	{
		//printf("BeginDecode into Bitmap Image failed, hr = 0x%08d\n", hr);
		goto finish;
	}
	
	while (E_PENDING == (hr = pImageDecoder->Decode()))
	{
		Sleep(0);
	}

	hr = pImageDecoder->EndDecode(hr);
	
	if (FAILED(hr))
	{
		//printf("Decoding failed, hr = 0x%08x\n", hr);
		goto finish;
	}
	state = Decoded;
finish:
	return;
}

void ImageDecoder::Rotate(int Angle)
{
	this->Angle = Angle;
}

StateType ImageDecoder::State()
{
	return state;
}

#if 0
TCHAR* ImageDecoder::FileName()
{
	//return filename;
}
#endif

int ImageDecoder::Width()
{
	return ii.Width;
}

int ImageDecoder::Height()
{
	return ii.Height;
}

UINT ImageDecoder::FrameCount()
{
    return nFrameCount;
}

void ImageDecoder::SelectNextFrame()
{
    HRESULT hr;
    nSelectedFrame++;
    if(nSelectedFrame > (nFrameCount - 1))
    {
        nSelectedFrame = 0;
    }

    ASSERT(pBitmapImage);
    pBitmapImage->Release();

    ASSERT(pImageSink);    
    pImageSink->Release();

    ASSERT(pImage);
    pImage->Release();
    
    if (FAILED(hr = pImageDecoder->SelectActiveFrame(&FrameDimensionTime, nSelectedFrame)))
    {
        ASSERT(0);
    }
    if (FAILED(hr = pImagingFactory->CreateNewBitmap(ii.Width, ii.Height, ii.PixelFormat, &pBitmapImage)))
	{
		//printf("CreateNewBitmap failed, hr = 0x%08x\n", hr);
	    ASSERT(0);
	}
	
    if (FAILED(hr = pBitmapImage->QueryInterface(IID_IImageSink, (void**)&pImageSink)))
    {
        //printf("QueryInterface for ImageSink from BitmapImage failed, hr: 0x%08x\n", hr);
        ASSERT(0);
    }
    
    if (FAILED(hr = pBitmapImage->QueryInterface(IID_IImage, (void**)&pImage)))
    {
        //printf("QueryInterface for Image from BitmapImage failed, hr: 0x%08x\n", hr);
        ASSERT(0);
    }
}
void ImageDecoder::Draw(HDC hdc, RECT &dstRect)
{
	RECT rc;
	HRESULT hr;
	float fAngle = (float)Angle;
	
	GetDestRect(ii.Width, ii.Height, dstRect.right, dstRect.bottom, rc);
	
	if(Angle != 0)
	{
		if(pImageRotate)
		{
			pImageRotate->Release();
			pImageRotate = NULL;
		}
		if(pBitmapImageRotate)
		{
			pBitmapImageRotate->Release();
			pBitmapImageRotate = NULL;
		}

		if(FAILED(hr = pBasicBitmapOps->Rotate(fAngle, InterpolationHintDefault, &pBitmapImageRotate)))
		{
			//printf("Rotate Image failed, hr: 0x%08x\n", hr);
		}
		
		if(FAILED(hr = pBitmapImageRotate->QueryInterface(IID_IImage, (void**)&pImageRotate)))
		{
			//printf("QueryInterface for Image from BitmapImage failed, hr: 0x%08x\n", hr);
		}

		pImageRotate->Draw(hdc, &dstRect, NULL);
	}
	else
	{
		pImage->Draw(hdc, &dstRect, NULL);
	}
	pImage->Release();

}

void ImageDecoder::Flip()
{
}

void ImageDecoder::Resize()
{
}

void ImageDecoder::AdjustBrightness()
{
}

void ImageDecoder::AdjustContrast()
{
}

void ImageDecoder::AdjustGamma()
{
}


BOOL ImageDecoder::GetCodecCLSID(IImagingFactory* pImagingFactory, CLSID * pclsid, WCHAR * wszMimeType, CodecType ctCodec)
{
    UINT uiCount;
    ImageCodecInfo * codecs;
    HRESULT hr;
    BOOL fRet = FALSE;
    TCHAR * tszCodec;

    if(eEncoder == ctCodec)
    {
        hr = pImagingFactory->GetInstalledEncoders(&uiCount, &codecs);
        tszCodec = TEXT("Encoder");
    }
    else
    {
        hr = pImagingFactory->GetInstalledDecoders(&uiCount, &codecs);
        tszCodec = TEXT("Decoder");
    }
    if(FAILED(hr))
    {
        //printf("GetInstalled%ss returned 0x%08x\n", tszCodec, hr);
        return FALSE;
    }

    for(UINT i = 0; i < uiCount; i++)
    {
        //printf("%s %d of %d: MimeType = %s\n"), tszCodec, i + 1, uiCount, codecs[i].MimeType);
        if (wszMimeType && !wcscmp(wszMimeType, codecs[i].MimeType))
        {
            *pclsid = codecs[i].Clsid;
            fRet = TRUE;
            break;
        }
    }
    CoTaskMemFree(codecs);
    return fRet;
}

HRESULT ImageDecoder::CreateStreamOnFile(const TCHAR * tszFilename, IStream ** ppStream)
{
    HRESULT hrRet = S_OK;
    HGLOBAL hg = NULL;
    HANDLE hFile = NULL;
    DWORD dwSize, dwRead;
    BYTE* pbLocked = NULL;

    // Open the file
    hFile = CreateFile(tszFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(INVALID_HANDLE_VALUE == hFile)
    {
        //printf("CreateFile failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    dwSize = GetFileSize(hFile, NULL);
    if(0xffffffff == dwSize)
    {
        //printf("GetFileSize failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    // Open a memory object
    hg = GlobalAlloc(GMEM_MOVEABLE, dwSize);
    if(NULL == hg)
    {
        //printf("GlobalAlloc failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    // Ge a pointer to the memory we just allocated
    pbLocked = (BYTE*) GlobalLock(hg);
    if(NULL == pbLocked)
    {
        //printf("GlobalLock failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    // copy the file
    if(!ReadFile(hFile, pbLocked, dwSize, &dwRead, NULL))
    {
        //printf("ReadFile failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    GlobalUnlock(hg);
    
    // Create the stream
    hrRet = CreateStreamOnHGlobal(hg, TRUE, ppStream);

    CloseHandle(hFile);
    return hrRet;
error:
    if(pbLocked)
        GlobalUnlock(hg);
    if(hg)
        GlobalFree(hg);
    if(hFile)
        CloseHandle(hFile);
    return hrRet;
}


HRESULT ImageDecoder::CreateStreamOnResource(HRSRC hRes, IStream ** ppStream)
{
    HRESULT hrRet = S_OK;
    HGLOBAL hg = NULL;
    //HANDLE hFile = NULL;
    DWORD dwSize; //dwRead;
    BYTE* pbLocked = NULL;

    // Open the file
	#if 0
	hFile = CreateFile(tszFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(INVALID_HANDLE_VALUE == hFile)
    {
        //printf("CreateFile failed with GLE = %d"), GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    dwSize = GetFileSize(hFile, NULL);
    if(0xffffffff == dwSize)
    {
        //printf("GetFileSize failed with GLE = %d"), GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }
	#endif
	
	HGLOBAL hGbl = LoadResource(GetModuleHandle(NULL), hRes);
	BYTE* pbData = (BYTE*)LockResource(hGbl);
	dwSize = SizeofResource(GetModuleHandle(NULL), hRes);

	
    // Open a memory object
    hg = GlobalAlloc(GMEM_MOVEABLE, dwSize);
    if(NULL == hg)
    {
        //printf("GlobalAlloc failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    // Ge a pointer to the memory we just allocated
    pbLocked = (BYTE*) GlobalLock(hg);
    if(NULL == pbLocked)
    {
        //printf("GlobalLock failed with GLE = %d\n", GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }

    // copy the file
    memcpy(pbLocked, pbData, dwSize);
    #if 0
    if(!ReadFile(hFile, pbLocked, dwSize, &dwRead, NULL))
    {
        //printf("ReadFile failed with GLE = %d"), GetLastError());
        hrRet = 0x80000000 + GetLastError();
        goto error;
    }
	#endif

    GlobalUnlock(hg);
    
    // Create the stream
    hrRet = CreateStreamOnHGlobal(hg, TRUE, ppStream);

    //CloseHandle(hFile);
    return hrRet;
error:
    if(pbLocked)
        GlobalUnlock(hg);
    if(hg)
        GlobalFree(hg);
//    if(hFile)
//        CloseHandle(hFile);
    return hrRet;
}


void ImageDecoder::
GetDestRect(int srcWidth, int srcHeight, int dstWidth, int dstHeight, RECT &out)
{
	int x, y, width, height;
	float XRatio, YRatio;
	x = 0;
    y = 0;
	if(Angle == 0 || Angle == 360 || Angle == 180)
	{
		width = srcWidth;
		height = srcHeight;
	}
	else
	{
		width = srcHeight;
		height = srcWidth;
	}
	XRatio = (float)dstWidth/(float)width;
	YRatio = (float)dstHeight/(float)height;

	if(XRatio < YRatio)
	{
		height = (int)((float)height * XRatio);
		width = dstWidth;
		y = (dstHeight - height)/2;
		
	}
	else
	{
		height = dstHeight;
		width = (int)((float)width * YRatio);
		x = (dstWidth - width)/2;
	}

	out.left = x;
	out.right = x + width;
	out.top = y;
	out.bottom = y + height;

}

#include "new.h"
#include "cassert"
PictureCutYu::PictureCutYu(void)
{
}
PictureCutYu::PictureCutYu(HWND hWnd)
{
	m_Wnd = hWnd;
	//hdc = GetDC(hWnd);
	pImgFactory = NULL;
	pImage  = NULL;
	pBitmap = NULL;
	pBitmapResize = NULL;
	pBitmapOps = NULL;
	rect.top = 0;
	rect.left = 0;
	
}
PictureCutYu::~PictureCutYu(void)
{
	if(pImgFactory)
	{
		pImgFactory->Release();
		pImgFactory = NULL;
	}
	if(pImage)
	{
		pImage->Release();
		pImage = NULL;
	}
	if(pBitmap)
	{
		pBitmap->Release();
		pBitmap = NULL;
	}
	if(pBitmapResize)
	{
		pBitmapResize->Release();
		pBitmapResize = NULL;
	}
	if(pBitmapOps)
	{
		pBitmapOps->Release();
		pBitmapOps = NULL;
	}
	
}
void out_ofmemory()
{
	MessageBox(NULL,L"memory not enough",L"",NULL);
}
BOOL PictureCutYu::CutJpeg(TCHAR *pFileIn,TCHAR *pFileOut,int width,int height)  //没有清理干净
{
	//set_new_handler(out_ofmemory);                     //检测内存情况
	hdc = GetDC(m_Wnd);
	BYTE *lpBitmapBits = NULL;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_ImagingFactory,NULL,CLSCTX_ALL,IID_IImagingFactory,(void **)&pImgFactory); 

	if(FAILED(hr))  
	{
		//OutputDebugString(L"create COM error");    //hr是0
		//return 0;
	}
	hr=pImgFactory->CreateImageFromFile(pFileIn,&pImage);

	if(SUCCEEDED(hr))
	{  
		pImgFactory->CreateBitmapFromImage(pImage,width,height,PixelFormat24bppRGB,InterpolationHintDefault,&pBitmap); //丢失
		pBitmap->QueryInterface(IID_IBasicBitmapOps, (void**)&pBitmapOps);
		pBitmapOps->Resize(width,height,PixelFormat24bppRGB,InterpolationHintDefault,&pBitmapResize);	
	}
	else
	{
		printf("%x\n",hr);    //0x80004005 未指定的错误
		//MessageBox(m_Wnd,L"create image error",L"",NULL);
		printf("create image error\n");                           //AU13 一直出错
		if(pImage)
		{
			pImage->Release();
			pImage = NULL;
		}
		if(pBitmap)
		{
			pBitmap->Release();
			pBitmap = NULL;
		}
		if(pBitmapOps)
		{
			pBitmapOps->Release();
			pBitmapOps = NULL;
		}
		if(pBitmapResize)
		{
			pBitmapResize->Release();
			pBitmapResize = NULL;
		}
		if(pImgFactory)
		{
			pImgFactory->Release();
			pImgFactory = NULL;
		}
		if(hbitmap)
			DeleteObject(hbitmap);
		ReleaseDC(m_Wnd,hdc);
		return FALSE;
	}
	BitmapData bmpdata = {0};
	bmpdata.Reserved = 0;
	bmpdata.Height = height;
	bmpdata.Width = width;
	bmpdata.PixelFormat=PixelFormat24bppRGB;
	//////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&bitmapInfo,sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = width;
	bitmapInfo.bmiHeader.biHeight = height;   ////51板子正的  模拟器 负的
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;//GetPixelFormatSize(Imageinfo.PixelFormat);
	int LineByte = (width*bitmapInfo.bmiHeader.biBitCount/8+3)/4*4;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= LineByte*height;

	rect.right = width;
	rect.bottom = height;
	pBitmapResize->LockBits(&rect,ImageLockModeRead |ImageLockModeWrite,bmpdata.PixelFormat,&bmpdata);

	hbitmap = CreateDIBSection(0, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (void**)&lpBitmapBits,0,0);
	if (bmpdata.Stride < 0)
	{
		bmpdata.Stride = bmpdata.Stride * -1;
		bmpdata.Scan0 = (void*) ( ((DWORD)bmpdata.Scan0 + bmpdata.Stride) - LineByte*height);
	}
	memcpy(lpBitmapBits,bmpdata.Scan0,LineByte*height);  //数据
	pBitmapResize->UnlockBits(&bmpdata);


	//////////////////////////////////////////////////////////////////////////////////保存文件/////////////////////////
	ZeroMemory(&bfh,sizeof(BITMAPFILEHEADER));
	bfh.bfType=0x4D42;
	bfh.bfOffBits=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);
	bfh.bfSize= bfh.bfOffBits+LineByte*height;

	FILE *fp = NULL;
	int num = 0;
agine: if(!(fp = _wfopen(pFileOut,L"wb")))
	   {
		   num++;
		   Sleep(500);
		   if(num == 2) 
		   {   
			   if(pImage)
			   {
				   pImage->Release();
				   pImage = NULL;
			   }
			   if(pBitmap)
			   {
				   pBitmap->Release();
				   pBitmap = NULL;
			   }
			
			   if(pBitmapOps)
			   {
				   pBitmapOps->Release();
				   pBitmapOps = NULL;
			   }
			   if(pBitmapResize)
			   {
				   pBitmapResize->Release();
				   pBitmapResize = NULL;
			   }
			   if(hbitmap)
				   DeleteObject(hbitmap);
			   ReleaseDC(m_Wnd,hdc);
			   printf("create file error!\n");
			   return FALSE; 
		   }
		   goto agine;
	   }

	   fwrite(&bfh,sizeof(bfh),1,fp);
	   fwrite(&bitmapInfo.bmiHeader,sizeof(bitmapInfo.bmiHeader),1,fp);
	   fwrite(lpBitmapBits,LineByte*height,1,fp);
	   fclose(fp);

	   //////////////清理工作///////////////
	   if(pImage)
	   {
		   pImage->Release();
		   pImage = NULL;
	   }
	   if(pBitmap)
	   {
		   pBitmap->Release();
		   pBitmap = NULL;
	   }
	   if(lpBitmapBits)
	   {
		   delete[] lpBitmapBits;
		   lpBitmapBits = NULL;
	   }
	   if(pBitmapOps)
	   {
		   pBitmapOps->Release();
		   pBitmapOps = NULL;
	   }
	   if(pBitmapResize)
	   {
		   pBitmapResize->Release();
		   pBitmapResize = NULL;
	   }
	   if(pImgFactory)
	   {
		   pImgFactory->Release();
		   pImgFactory = NULL;
	   }
	   if(hbitmap)
		   DeleteObject(hbitmap);
	   ReleaseDC(m_Wnd,hdc);
	   //CoFreeUnusedLibraries();
	   return true;
}
BOOL PictureCutYu::CutJpeg(TCHAR *pFileIn,BYTE **pBuffer,int width,int height)  //没有清理干净
{
	//set_new_handler(out_ofmemory);                     //检测内存情况
	hdc = GetDC(m_Wnd);
	BYTE *lpBitmapBits = NULL;    

	HRESULT hr;
	hr = CoCreateInstance(CLSID_ImagingFactory,NULL,CLSCTX_ALL,IID_IImagingFactory,(void **)&pImgFactory); 

	if(FAILED(hr))  
	{
		printf("%d\n",hr);    //hr是0
		printf("create COM error!!\n");

	}

	hr=pImgFactory->CreateImageFromFile(pFileIn,&pImage);

	if(SUCCEEDED(hr))
	{  
		pImgFactory->CreateBitmapFromImage(pImage,width,height,PixelFormat24bppRGB,InterpolationHintDefault,&pBitmap); //丢失
		pBitmap->QueryInterface(IID_IBasicBitmapOps, (void**)&pBitmapOps);
		pBitmapOps->Resize(width,height,PixelFormat24bppRGB,InterpolationHintDefault,&pBitmapResize);	
	}
	else
	{
		int error = GetLastError();
		printf("%d\n",error);    //0x80004005 未指定的错误
		printf("create image error\n");                           //AU13 一直出错
		if(pImage)
		{
			pImage->Release();
			pImage = NULL;
		}
		if(pBitmap)
		{
			pBitmap->Release();
			pBitmap = NULL;
		}
		if(pBitmapOps)
		{
			pBitmapOps->Release();
			pBitmapOps = NULL;
		}
		if(pBitmapResize)
		{
			pBitmapResize->Release();
			pBitmapResize = NULL;
		}
		if(pImgFactory)
		{
			pImgFactory->Release();
			pImgFactory = NULL;
		}
		if(hbitmap)
			DeleteObject(hbitmap);
		ReleaseDC(m_Wnd,hdc);
		return FALSE;
	}
	BitmapData bmpdata = {0};
	bmpdata.Reserved = 0;
	bmpdata.Height = height;
	bmpdata.Width = width;
	bmpdata.PixelFormat=PixelFormat24bppRGB;
	//////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&bitmapInfo,sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = width;
	bitmapInfo.bmiHeader.biHeight = -height;   ////51板子正的  模拟器 负的
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;//GetPixelFormatSize(Imageinfo.PixelFormat);
	int LineByte = (width*bitmapInfo.bmiHeader.biBitCount/8+3)/4*4;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage	= LineByte*height;

	//lpBitmapBits = new BYTE[LineByte*height];    //不能再新建立了
    *pBuffer = new BYTE[LineByte*height];

	rect.right = width;
	rect.bottom = height;
	pBitmapResize->LockBits(&rect,ImageLockModeRead |ImageLockModeWrite,bmpdata.PixelFormat,&bmpdata);
	hbitmap = CreateDIBSection(0, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (void**)&lpBitmapBits,0,0);
	if (bmpdata.Stride < 0)
	{
		bmpdata.Stride = bmpdata.Stride * -1;
		bmpdata.Scan0 = (void*) ( ((DWORD)bmpdata.Scan0 + bmpdata.Stride) - LineByte*height);
	}
	memcpy(lpBitmapBits,bmpdata.Scan0,LineByte*height);  //数据
	BYTE temp;
	
	for(int i=0; i<50; i++)		      //高	      //翻转像素的
	{
		for(int j=0; j<100; j++)	  //宽
		{          
			for (int k=0;k<3;k++)
			{
				temp = lpBitmapBits[j*3+i*100*3+k];
				lpBitmapBits[j*3+i*100*3+k] = lpBitmapBits[100*(100-i-1)*3+j*3+k];
				lpBitmapBits[100*(100-i-1)*3+j*3+k] = temp;
			}
		}	  
	}
	
	memcpy(*pBuffer,lpBitmapBits,LineByte*height);
	pBitmapResize->UnlockBits(&bmpdata);
	 DeleteObject(hbitmap);

	   if(pImage)
	   {
		   pImage->Release();
		   pImage = NULL;
	   }
	   if(pBitmap)
	   {
		   pBitmap->Release();
		   pBitmap = NULL;
	   }
	  
	   if(pBitmapOps)
	   {
		   pBitmapOps->Release();
		   pBitmapOps = NULL;
	   }
	   if(pBitmapResize)
	   {
		   pBitmapResize->Release();
		   pBitmapResize = NULL;
	   }
	   if(pImgFactory)
	   {
		   pImgFactory->Release();
		   pImgFactory = NULL;
	   }
	   
		 
	   ReleaseDC(m_Wnd,hdc);
	 
	   return true;
}