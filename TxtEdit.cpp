/*****************************************************************************************
* TxtEdit类:文本排版，生成临时文件
******************************************************************************************/
#include "stdafx.h"
#include "TxtEdit.h"

#define  MAXLEN  68   //定义每行输出的字符串最大长度为80
#define  LEN  m_szReadPara.size()   //定义LEN为每次读取文件一段字符串的长度

DWORD t1,t2;

// 构造函数,打开文件
TxtEdit::TxtEdit(TCHAR* txtname)
{	
	File.open(txtname, ios::in);//"\\Storage Card\\txt\\te.txt"
	FileOut.open("\\Storage Card\\out.txt", ios::in|ios::out|ios::trunc);//文件不存在时创建
	if (!File)   //判断文件能否打开
	{
		cout << "File can not be open." << endl;
		exit(-1);
	}
	if (!FileOut)   //判断文件能否打开
	{
		cout << "FileOut can not be open." << endl;
		exit(-1);
	}
	shortCh1 = FALSE;
	shortCh2 = FALSE;
	LineNum = 0;//排版后的总行数
	PageNum = 0; //排版后的总页数

	//获得文件大小
	std::streampos begpos=File.tellg();
	File.clear();
	File.seekg(0,ios::end);
	std::streampos endpos=File.tellg();
	Txtsize = endpos - begpos;	
	File.clear();
	File.seekg(0,ios::beg);
}

//析构函数，关闭文件

TxtEdit::~TxtEdit()
{
	File.close();   
	FileOut.close();
	//DeleteFile(L"\\Storage Card\\out.txt"); //删除临时排版后的文件
}
//读取文件内容，按段读取

void TxtEdit::Read()
{
	char chCh;
	while (!File.eof())   //开始读取文件内容
	{
		m_szReadPara = "";   //初始化字符串
	
		if (getline(File,m_szReadPara))
		{  
			if ("" == m_szReadPara)  //没读取到内容，则中断循环
				break;

			WordProcess(m_szReadPara);  //调用文字处理函数
		}
		else
		{
			//cout<<"GETLINE FAILED"<<endl; 
		}      
	}

}
//对读取的每段进行文字处理，匀齐排版后输出

void TxtEdit::WordProcess(string strstr)
{
	int len = strstr.length();

	if (len <= MAXLEN)  //如果该段少于80个字符则直接输出
	{
		FileOut<<strstr.substr(0, len)<<endl;  //zh<<endl
		LineNum++;
		return;
	}
	else
	{
		string str4=shortString(strstr);
		WordProcess(str4);
	}						
}
string TxtEdit::shortString(string str)
{
	int leng=str.length();
    
	int i=0;
	
	while (i<MAXLEN)
	{
		char ichar = str.at(i);
		if (ichar <= -1 )
		{
			if (i==MAXLEN-3)
			{
				i += 2;
				if (str.at(MAXLEN-1)<0)
				{
					shortCh2 = TRUE;
				}
				else
				{
					shortCh2 = FALSE;
				}
				break; //已到末尾，跳出循环
			}
			else
			{
				shortCh2 = FALSE;
			}
			i+=2;

		}
		else
		{
			if (i==MAXLEN-2)
			{
				i+=1;
				if (str.at(i)<0)
				{
					shortCh2 = TRUE;
				}
				else
				{
					shortCh2 = FALSE;
				}
			}
 			   i++;
		}
	}
	
	if(shortCh2)//是半个汉字
	{
		FileOut<<str.substr(0, MAXLEN-1)<<' '<<endl;  //zh<<endl
		LineNum++;
	
		int n=leng-(MAXLEN-1);
		return str.substr(MAXLEN-1,n);
	}
	else
	{
		FileOut<<str.substr(0, MAXLEN)<<endl;  //zh
		LineNum++;
		return str.substr(MAXLEN,leng-MAXLEN);
	}
}

