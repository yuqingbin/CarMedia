/////////////////////////////////
//多字节和宽字节之间的转换类
//BY yuqingbin
/////////////////////////////////
#pragma once

class Char2Wide
{
public:
	Char2Wide(void);
	~Char2Wide(void);
	void char2wide(char * str,TCHAR *outstr);
	void wide2char(TCHAR *str,char *outstr);
};
