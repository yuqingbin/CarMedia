#include "StdAfx.h"
#include "Char2Wide.h"

Char2Wide::Char2Wide(void)
{
}

Char2Wide::~Char2Wide(void)
{
}

void Char2Wide::char2wide(char *str,TCHAR *outStr)
{
  DWORD dwLen = MultiByteToWideChar(CP_ACP,0,str ,-1,NULL,0);
  TCHAR *tmpstr;
  tmpstr = new TCHAR[dwLen];
  MultiByteToWideChar(CP_ACP,0,str ,-1,tmpstr,dwLen);
  lstrcpy(outStr,tmpstr);   //把变换后的内容复制到outStr指针指向的地址里
  delete tmpstr;
}
void Char2Wide::wide2char(TCHAR *str,char *outstr)
{
	DWORD dwLen = WideCharToMultiByte(CP_OEMCP,NULL,str,-1,NULL,0,NULL,FALSE);
	char *tmpstr;
	tmpstr = new char[dwLen];
	WideCharToMultiByte(CP_OEMCP,NULL,str,-1,tmpstr,dwLen,NULL,FALSE);
	strcpy(outstr,tmpstr);   //把变换后的内容复制到outStr指针指向的地址里
	delete tmpstr;
}