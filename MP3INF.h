/************************************************************************/
/* 读取MP3信息
专辑名、歌曲、艺术家

BY yuqingbin
*/
/************************************************************************/
#pragma once

typedef struct  
{
	TCHAR Tag[3];
	TCHAR Title[30];
	TCHAR Artist[30];
	TCHAR Album[30];
}MP3INFO;

class MP3INF
{
public:
	MP3INF(void);
	~MP3INF(void);
	BOOL ReadMp3Info(TCHAR *Filename,MP3INFO *mp3Info);
	//Char2Wide  *ch2wi;
};
