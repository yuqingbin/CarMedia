/*****************************************************************************************
* ElecBook类: 文本读取，按页切换显示
******************************************************************************************/
#include "stdafx.h"
#include "ElecBook.h"

ElecBook::ElecBook(HWND hWnd,TCHAR* path)
{
	index=0; //页索引
	num=0;
	FILE_LAST = FALSE;
	cText = new TxtEdit(path);
	last = FALSE;
	HDC txtDC = GetDC(hWnd);
	bmp = CreateCompatibleBitmap(txtDC,775,407);   //
	hdcMemTxt = CreateCompatibleDC(txtDC);
	SelectObject(hdcMemTxt,bmp);

	CreateBgDC(hWnd);
	//如果打开的不是上次退出时看的，则重新排版	
	ReleaseDC(hWnd,txtDC);
	
}
ElecBook::~ElecBook()
{
	if (cText)
	{
		delete cText;
		cText = NULL;
	}
	if(hdcMemTxt) //
	{
		
		DeleteDC(hdcMemTxt);
		DeleteObject(bmp);
		
	}
	
	if(hdcBg) //
	{
		
		DeleteDC(hdcBg);
		DeleteObject(bmp1);
	}
	EveryPagepos.clear();
}
void ElecBook::InitBook(HWND hWnd)
{

}
void ElecBook::ButtonDown(HWND hWnd, POINT point)
{
	HDC txtDC = GetDC(hWnd);
	//读取上一行，下一行
	RECT re,rNext,rPre;
	rPre.left =350;rPre.top = 424;rPre.right = 376;rPre.bottom = 480;  //上一页
	rNext.left =617;rNext.top = 424;rNext.right =643;rNext.bottom = 480; //下一页
	
	if (PtInRect(&rPre,point))    //读取上一页
	{
		BUTTON_NEXT = TRUE;
		DrawBmp(txtDC,350,424,26,39,L"\\Storage Card\\USBDATA\\PrePage2.bmp");					
	}
	if (PtInRect(&rNext,point))   //读取下一页
	{
		BUTTON_PRE = TRUE;
		DrawBmp(txtDC,617,424,26,39,L"\\Storage Card\\USBDATA\\NextPage2.bmp");					
	}
	ReleaseDC(hWnd,txtDC);
}
void ElecBook::ButtonUp(HWND hWnd,  POINT point)
{
	//EnterCriticalSection(&(g_critical_section));

	POINT UpPoint;
	UpPoint.x = point.x;
	UpPoint.y = point.y;
	HDC txtDC = GetDC(hWnd);

	//读取上一页，下一页
	RECT re,rNext,rPre;
	rPre.left =350;rPre.top = 424;rPre.right = 376;rPre.bottom = 480;  //上一页
	rNext.left =617;rNext.top = 424;rNext.right =643;rNext.bottom = 480; //下一页

	if (PtInRect(&rPre,UpPoint))   //读取上一页
	{
		if (BUTTON_NEXT)
		{
			DrawBmp(txtDC,350,424,26,39,L"\\Storage Card\\USBDATA\\PrePage1.bmp");
			cText->FileOut.clear();
			cText->FileOut.seekg(ReadPos); 
			ChangePage(hWnd,FALSE);
			BUTTON_NEXT = FALSE;
		}
	}
	if (PtInRect(&rNext,UpPoint))   //读取下一页
	{
		if (BUTTON_PRE)
		{
			DrawBmp(txtDC,617,424,26,39,L"\\Storage Card\\USBDATA\\NextPage1.bmp");
			cText->FileOut.clear();
			cText->FileOut.seekg(ReadPos); 
			ChangePage(hWnd,TRUE);
			BUTTON_PRE = FALSE;
		}					
	}
	ReleaseDC(hWnd,txtDC);
	//LeaveCriticalSection(&(g_critical_section));
}
LRESULT ElecBook::PaintMain(HWND hWnd,UINT,WPARAM,LPARAM)
{
	
	HDC hdc = GetDC(hWnd);
	DrawBmp(hdc,0,0,800,480,L"\\Storage Card\\USBDATA\\BookBack.bmp");//电子书大背景

	DispFirstPage(hWnd);
	ReadPos = cText->FileOut.tellg();

    ReleaseDC(hWnd,hdc);
	return TRUE;
}
void ElecBook::MouseMove(HWND hWnd, POINT point)
{

}
void ElecBook::DoTimer(HWND hWnd,  WPARAM wParam)
{

}

//读取第一页
void ElecBook::DispFirstPage(HWND hWnd)
{
	TCHAR DispStr[1200];
	ZeroMemory(DispStr,1200);
	char *buff = NULL;
	index = 0;
	cText->FileOut.clear();
	cText->FileOut.seekg(0,ios_base::beg); 
	std::streampos pos=cText->FileOut.tellg();//得到当前位置
	
	EveryPagepos.push_back(pos);
	
	buff = new char[1500];
	ZeroMemory(buff,1500);  //初始化数组
	char str[LINE_LENGTH];
	for (int i=0;i<LINAGE;i++)
	{
		ZeroMemory(str,LINE_LENGTH);
		if(cText->FileOut.getline(str,LINE_LENGTH))//getline(fin,sstr)  //zh
		{
			strcat(str,"\n");
			strcat(buff,str);	
			ReadPos = cText->FileOut.tellg();
		}
		else
		{  
			//到最后一页时，使指针在最后一页开始
			cText->FileOut.clear();
			cText->FileOut.seekg(Prepos);
			ReadPos = cText->FileOut.tellg();
			break;
		}
	}
	//转换字符编码
	Mbyte2Wchar(buff,DispStr);
	delete[] buff;
	buff = NULL;
	//由双缓冲显示内容
	TxtDisp(hWnd,DispStr);
}
void ElecBook::ChangePage(HWND hWnd,BOOL Dir)
{
	TCHAR DispStr[1500];
	ZeroMemory(DispStr,1500);
	char *buff = NULL;
	Prepos = cText->FileOut.tellg();//得到当前位置
	if(Dir)
	{
		if (index > cText->PageNum)//到了文件末尾  12.27
		{
			//MessageBox(NULL,L">endpos",L"",NULL);
			return;
		}
		else
		{
			ReadNextPage(&buff);
		}
	}
	else
	{
		ReadPrePage(&buff);
	}
	Mbyte2Wchar(buff,DispStr);//转换字符编码
	delete[] buff;
    buff = NULL;
   Percent = 100*(float)index/(cText->PageNum-1);  //index从0开始
   
	//由双缓冲显示内容
	TxtDisp(hWnd,DispStr);
}
//向下翻页   //可以用vector实现存储索引
void ElecBook::ReadNextPage(char** nextstr)
{
	*nextstr = new char[1500];
	ZeroMemory(*nextstr,1500);  //初始化数组
	char str[LINE_LENGTH];
    int keypage=0;
		index++;
	//if (index%KEYPAGE_SPACE == 0)
	//{
	//	std::streampos pos=cText->FileOut.tellg();//得到当前位置
	//    keypage = (index)/KEYPAGE_SPACE;
	//	KeyPagepos[keypage] = pos;
	//}
	//Pagepos[index]=pos;
		
	Prepos = cText->FileOut.tellg(); 
	if (index == EveryPagepos.size())
	{
		EveryPagepos.push_back(Prepos);
	}
	
	for (int i=0;i<LINAGE;i++)
	{
		ZeroMemory(str,LINE_LENGTH);
		if (!cText->FileOut.eof())
		{
			if(cText->FileOut.getline(str,LINE_LENGTH))//getline(fin,sstr)  //zh
			{
				strcat(str,"\n");
				strcat(*nextstr,str);	
				ReadPos = cText->FileOut.tellg();
			}
			else
			{  
				//到最后一页时，使指针在最后一页开始
				cText->FileOut.clear();
				cText->FileOut.seekg(Prepos);
				ReadPos = cText->FileOut.tellg();
				break;
			}
		}	
	}
}
//向上翻页
void ElecBook::ReadPrePage(char** prestr)
{	
	*prestr = new char[1500];
	ZeroMemory(*prestr,1500);  
	char str[LINE_LENGTH];
	char str11[LINE_LENGTH];
	int keypage;
	int pageoff;
	index--;
	if (index <= 0)
	{
		index = 0;
		//return;
	}
	//keypage = index/KEYPAGE_SPACE;  //得到关键页
	//cText->FileOut.clear();
	//cText->FileOut.seekg(KeyPagepos[keypage]);
	//pageoff = index%KEYPAGE_SPACE;
	
	cText->FileOut.clear(); 
	cText->FileOut.seekg(EveryPagepos[index]);

	//将指针移到上一页开始
    //PrePointer();
	//while (pageoff!=0)
	/*for(pageoff;pageoff>0;pageoff--)
	{
		for (int i=0;i<LINAGE;i++)
		{
			if (cText->FileOut.getline(str11,LINE_LENGTH,'\n'))
			{
			}
			else
			{
				break;
			}
		}
	}*/
		
	for (int i=0;i<LINAGE;i++)
	{
		if(cText->FileOut.getline(str,LINE_LENGTH,'\n'))//getline(fin,sstr)  //zh
		{
			strcat(str,"\n");
			strcat(*prestr,str);
			ReadPos = cText->FileOut.tellg();
		}
		else
		{
			ReadPos = cText->FileOut.tellg();
			break;
		}
	}
	/*ReadPos = cText->FileOut.tellg();*/
}

//将一页字符放在背景上写入另一内存hdc
void ElecBook::TxtDisp(HWND hWnd,TCHAR* str)
{
	HDC hdc = GetDC(hWnd);
	//HBITMAP bmp = CreateCompatibleBitmap(hdc,775,407);   //  改为全局
	/*HDC hdcMemTxt = CreateCompatibleDC(hdc);
	SelectObject(hdcMemTxt,bmp);*/
	/*DeleteObject(bmp);*/
	BitBlt(hdcMemTxt,0,0,775,407,hdcBg,0,0,SRCCOPY);

    RECT rect;
	rect.top = 10;rect.bottom = 407;rect.left = 45;rect.right = 800;//每页文本的显示区域
	
	SetTextColor(hdcMemTxt,RGB(20,20,20));
	SetBkMode(hdcMemTxt,1);
	HFONT hFont = CreateFont(25,0,0,0,FW_LIGHT,0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH,_T("微软雅黑"));
	HFONT hOldFont3 = (HFONT)SelectObject(hdcMemTxt,hFont);
	
	DrawTextW(hdcMemTxt,str,-1,&rect,DT_EDITCONTROL|DT_WORDBREAK);
	SelectObject(hdcMemTxt,hOldFont3);
	
	BitBlt(hdc,13,65,775,407,hdcMemTxt,0,0,SRCCOPY);
//显示当前的百分数
	RECT rPer;
	rPer.left = 376;rPer.right = 617;rPer.top = 424;rPer.bottom = 480;
	TCHAR per[10];
	_stprintf(per,L"%.2f ",Percent);
	lstrcat(per,L"%");
	DrawTextW(hdc,per,-1,&rPer,DT_CENTER);

	DeleteObject(hOldFont3);
	//SelectObject(hdcMemTxt,hOldFont3);
	DeleteObject(hFont);
	ReleaseDC(hWnd,hdc);
}
//创建背景的memhdc
void ElecBook::CreateBgDC(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HBITMAP bmp2;
	HBRUSH pOldBrush;

	hdcBg=CreateCompatibleDC(hdc);
	HDC hdcMem1 = CreateCompatibleDC(hdc);
	bmp1 = CreateCompatibleBitmap(hdc,775,407);        
	SelectObject(hdcBg,bmp1);
	
	/////////////////////////////////////////////
	bmp2 = (HBITMAP)::SHLoadDIBitmap(L"\\Storage Card\\USBDATA\\txtback.bmp");  //byd1300
	//bmp2 = (HBITMAP)::SHLoadDIBitmap(L"\\SDMMC\\FM\\AMbg.bmp");  //byd1300
	pOldBrush = (HBRUSH)SelectObject(hdcMem1,bmp2);         //选入背景

	BitBlt(hdcBg,0,0,775,407,hdcMem1,0,0,SRCCOPY);
	//SelectObject(hdcBg,pOldBrush);
	DeleteDC(hdcMem1);  //注意要先删除hdcMem1，再删bmp2，否则会引起内存泄露
	DeleteObject(bmp2);
	ReleaseDC(hWnd,hdc);
}