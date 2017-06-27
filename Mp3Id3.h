/////////////////////////////////////////////////////////////
//读取MP3歌曲、艺术家、专辑名信息
//
//BY  yuqingbin
/////////////////////////////////////////////////////////////


#pragma once
//#include "afx.h"
#include "tchar.h"
#include "atlstr.h" 


typedef struct pp //MP3信息的结构
{
	char Identify[3]; //TAG三个字母//这里可以用来鉴别是不是文件信息内容	
	char Title[31];   //歌曲名，30个字节  
	char Artist[31];  //歌手名，30个字节
	char Album[31];   //所属唱片，30个字节
	char Year[5];	  //年，4个字节
	char Comment[29]; //注释，28个字节
	unsigned char reserved;  //保留位，1个字节
	unsigned char reserved2; //保留位，1个字节
	unsigned char reserved3; //保留位，1个字节
} MP3INFO;


//MP3INFO* ReadMp3Id3(CString strPlayListName);  //lpTitle 歌曲名  lpArtist 艺术家  lpAlbum 专辑名
MP3INFO* ReadMp3Id3(TCHAR* strPlayListName);  //lpTitle 歌曲名  lpArtist 艺术家  lpAlbum 专辑名