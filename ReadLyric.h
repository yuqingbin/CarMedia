////////////////////////////////////////////////////////////
//读取并分离歌词
//
//BY  yuqingbin
////////////////////////////////////////////////////////////


#pragma once
#include "afx.h"


typedef struct 
{
	_int64 Time; //LONGLONG Time; //时间戳
	 TCHAR    Lyric[1000];    //歌词一句
}LYRIC; // //歌词时间戳结构体

bool SplitLyric(CString path); //读取并把歌词切分成句