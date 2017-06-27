/*****************************************************************************************
* TxtEdit类:文本排版，生成临时文件
******************************************************************************************/

#ifndef _TxtEdit_H_
#define _TxtEdit_H_

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TxtEdit
{
private:
	BOOL shortCh1;
	BOOL shortCh2;
	char m_szFileName[30];  //文件名
	/*fstream File;  */
	//ofstream FileOut;
	string m_szReadPara;  //用来存储每次读取文件一段的内容

public:
	fstream File;  
	fstream FileOut;
	std::streampos Txtsize;
	int LineNum; //排版后的总行数
	int PageNum; //排版后的总页数

enum TEXTENCODING { ASCII, UNI16_BE, UNI16_LE, UTF_8 };

//构造函数,打开文件

	TxtEdit(TCHAR* txtname);

//析构函数,关闭文件

	~TxtEdit();

//读取文件内容，按段读取

	void Read();

//对读取的每段进行文字处理，匀齐排版后输出

	void WordProcess(string strstr);
	string shortString(string str);
};

#endif