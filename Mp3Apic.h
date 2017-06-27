/************************************************************************/
/* *读取MP3的内嵌专辑图片 
** 数据放到buffer里  buffer里是标准压缩的jpeg或者png数据
** 使用完记得删除
**BY yuqingbin
/************************************************************************/
#pragma once
#include "ImageDecoder.h"   //包含IMage头文件
class Mp3Apic
{
public:
	Mp3Apic(HWND hWnd);
	~Mp3Apic(void);
	BOOL ReadMp3Apic(TCHAR *inFile,char **Buffer);  //数据放在*Buffer
	int MP3Load(TCHAR *Filename, unsigned char **Buffer, int *Width, int *Height, int *Depth);
	
	
	HWND g_hWnd;
	DWORD FrameSize;
};
