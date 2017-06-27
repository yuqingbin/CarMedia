/*****************************************************************************************
* ElecBook类: 文本读取，按页切换显示
******************************************************************************************/
#ifndef ELEC_BOOK
#define ELEC_BOOK
#include <commctrl.h> 
#include "atlstr.h"   
#include "TxtEdit.h"
#include "public.h"
#include "ElecBook.h"
#include <algorithm>
#include <vector>
#define LINE_LENGTH 100//84
#define LINAGE 13  //每页显示的行数
#define KEYPAGE_SPACE 8  //关键页间隔
class ElecBook
{
private:
	BOOL last; //是否到文件尾标志
	int Len;  //getline获得字符数
	//HDC hdc ;
	HBITMAP bmp,bmp1;
	HDC hdcBg;   //放有背景图
	HDC hdcMemTxt;  //用于组合背景和文字，然后显示
	int index; //页索引
	//std::streampos KeyPagepos[1024];  //存放每页的位置
	vector<std::streampos>EveryPagepos;
	int num;
	std::streampos Prepos;
	BOOL FILE_LAST;
	//线程同步临界区对象
	
	BOOL BUTTON_NEXT;
	BOOL BUTTON_PRE;

public:
	TxtEdit* cText;  //调用类的构造函数打开文件=new TxtEdit()
	std::streampos ExitPos;//退出线程执行主程序时的位置
	std::streampos ReadPos; //读取显示的当前的位置
	float Percent;
	std::streampos Endpos;
	
	ElecBook(HWND hWnd,TCHAR* path);
	~ElecBook();
	void InitBook(HWND hWnd);
	void ButtonDown(HWND hWnd, POINT point);
	void ButtonUp(HWND,  POINT point);
	void MouseMove(HWND hWnd, POINT point);
	void DoTimer(HWND hWnd,  WPARAM wParam);
	LRESULT PaintMain(HWND,UINT,WPARAM,LPARAM);

	void ChangePage(HWND hWnd,BOOL Dir); //翻页
	void ReadNextPage(char** nextstr); //读后一页
	void ReadPrePage(char** prestr);  //读前一页
	void CreateBgDC(HWND hWnd);     //创建背景内存
	void TxtDisp(HWND hWnd,TCHAR* str);  //显示一页内容
	void ReleaseVariable(); //删除变量
	void PrePointer();  //将指针移到前一页的开始位置
	void DispFirstPage(HWND hWnd);
};
#endif