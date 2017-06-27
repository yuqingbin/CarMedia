#include "ReadLyric.h"
//#include "tchar.h"
#include "fstream"
using namespace std;
//#ifdef _WINDOWS_ 
//	#undef _WINDOWS_ 
//		#include "afxcmn.h" 
//#endif

int     LineCount = 0;     //总行数
LYRIC stLyric[1000];      //时间戳和歌词

//读取并把歌词切分成句
bool SplitLyric(CString path)
{
	//初始化存储数组
	  for (int i = 0;i<1000;i++)
	  {
		   for(int j = 0;j<1000;j++)
		   {
				stLyric[i].Lyric [j] = 0;
				stLyric[i].Time = 0;
		   }
	  }
	   LineCount = 0;  //初始化总行数


	 path.Replace (L".mp3",L".lrc");      //歌词路径
	 path.Replace (L".MP3",L".lrc");      //路径大小写

	 ifstream file(path,ios::in);
	 if (!file)
	 {
		  path.Replace (L".lrc",L".LRC");    //解决歌词后缀大写问题
		  file.open (path,ios::in);
		  if(!file)
		  {
			//MessageBox(L"歌词没有发现");
			  file.close ();
			  return false;    //没有歌词
		  }
	  
	 }
	 char pbuf[1000];
	 while (file.getline (pbuf,1000))
	 {
		  DWORD dwNum = MultiByteToWideChar(CP_ACP,0,pbuf,-1,NULL,0);
		  TCHAR *pTmp;
		  pTmp = new TCHAR[dwNum];
		  MultiByteToWideChar(CP_ACP,0,pbuf,-1,pTmp,dwNum);
		  CString  Lyric;
		  Lyric = pTmp;
		  int pos = 0;
		  int ppos = 0;
		  LONGLONG tmpTime = 0; 
		  CString strLyric,bstrLyric,strTime,bstrTime;
		  pos = Lyric.Find (L"]");

		  //判断有时间戳
		  strTime = Lyric.Left (pos+1);
		  strTime.Remove (L'[');
		  strTime.Remove (L']');  //获得时间格式 03:25.68
		  ppos = strTime.Find (L'0');

		  if (pos != -1 || ppos != -1) //判断是否是 空行就是连时间戳都没有
		  {

			  strLyric = Lyric.Right (Lyric.GetLength ()-pos-1);  //获得歌词
			  
			  
			  //pc时间转换
			  char fenbuf[100];
			  char miaobuf[100];
			  char haombuf[100];
			  CString fen, //分
				  miao,   //秒
				  haom;   //毫秒
			  fen = strTime.Mid (0,2);
			  miao = strTime.Mid (3,2);
			  haom = strTime.Mid (6,strTime.GetLength ()-6);
			  for(int i = 0;i<fen.GetLength ()+1;i++)
			  {
				fenbuf[i]= fen.GetAt (i);
			  }
			  for(int i = 0;i<miao.GetLength ()+1;i++)
			  {
				miaobuf[i]= miao.GetAt (i);
			  }
			  for(int i = 0;i<haom.GetLength ()+1;i++)
			  {
				haombuf[i]= haom.GetAt (i);
			  }
		  
			  //atoi wince和pc不同
			  tmpTime = (atoi(fenbuf)*60 + atoi(miaobuf)); //转换成秒      //*1000 + atoi(haombuf); //时间转换 毫秒
			  //第一行的处理
			  if (tmpTime == 0)
			  {
				tmpTime = 1;  //一秒时出现
			  }
	   
			  //有补偿的时间标签处理
			  int bpos =0;
			  bpos = strLyric.Find (L"]");
			   if (bpos != -1)
			   {
					bstrLyric = strLyric; //还有时间标签的歌词
					strLyric = strLyric.Right (strLyric.GetLength ()-bpos-1);
				
			   }
			   //存储
			   for (int i = 0;i<strLyric.GetLength ()+1;i++)
			   {
					stLyric[LineCount].Lyric[i]  = strLyric.GetAt (i);
			   }
				stLyric[LineCount].Time = tmpTime;   

				LineCount++;
		    
				if (bpos != -1)
				{
					 //bstrLyric = bstrLyric.Right (bstrLyric.GetLength ()-bpos-1); 
					 bstrTime = bstrLyric.Left (bpos+1);
					 bstrTime.Remove (L'[');
					 bstrTime.Remove (L']');  //获得时间格式 03:25.68
					 char fenbuf[100];
					 char miaobuf[100];
					 char haombuf[100];
					 CString fen,miao,haom;
					 fen = bstrTime.Mid (0,2);
					 miao = bstrTime.Mid (3,2);
					 haom = bstrTime.Mid (6,bstrTime.GetLength ()-6);
					 for(int i = 0;i<fen.GetLength ()+1;i++)
					 {
					   fenbuf[i]= fen.GetAt (i);
					 }
					 for(int i = 0;i<miao.GetLength ()+1;i++)
					 {
						miaobuf[i]= miao.GetAt (i);
					 }
					 for(int i = 0;i<haom.GetLength ()+1;i++)
					 {
						haombuf[i]= haom.GetAt (i);
					 }
				  
					 //atoi wince和pc不同
					 tmpTime = (atoi(fenbuf)*60 + atoi(miaobuf));  //秒                        //*1000 + atoi(haombuf);

					 bstrLyric = bstrLyric.Right (bstrLyric.GetLength ()-bpos-1); 
					 for (int i = 0;i<bstrLyric.GetLength ()+1;i++)
					 {
						stLyric[LineCount].Lyric [i] = bstrLyric.GetAt (i);
					 }
					  stLyric[LineCount].Time  = tmpTime;
					  LineCount++;
				}//end if (bpos != -1)
		   

			}//end 判断是否是 空行就是连时间戳都没有
		delete pTmp;
		}// end while (file.getline (pbuf,1000))
		file.close ();
		return true;
}