#include "StdAfx.h"
#include "MP3INF.h"
#include "stdio.h"

MP3INF::MP3INF(void)
{
}

MP3INF::~MP3INF(void)
{

}
BOOL MP3INF::ReadMp3Info(TCHAR *Filename,MP3INFO *mp3Info)
{
	
	FILE *fp;
	BYTE pbuf[128];
	memset(pbuf,0,128);
	fp = _wfopen(Filename,L"rb");
	if(!fp)
	{
		return FALSE;
	}
    fseek(fp,-128,SEEK_END);          //模拟器是128  开发板-128
	fread(pbuf,1,128,fp);

	if(!((pbuf[0] == 'T'|| pbuf[0] == 't')
		&&(pbuf[1] == 'A'|| pbuf[1] == 'a')
		&&(pbuf[2] == 'G'|| pbuf[2] == 'g')))
	{
      return FALSE;
	}
	char title[31] = {0};    //必须用31 不然有的歌曲会断开
	char artist[31] = {0};   //
	char album[31] = {0};    //

	memcpy(title,pbuf+3,30);
	memcpy(artist,pbuf+33,30);
	memcpy(album,pbuf+63,30);


	DWORD dwA = MultiByteToWideChar(CP_ACP,0,artist ,-1,NULL,0); //字符的转换
	DWORD dwAl = MultiByteToWideChar(CP_ACP,0,album ,-1,NULL,0);
    DWORD dwT = MultiByteToWideChar(CP_ACP,0,title,-1,NULL,0);
	TCHAR *pArtist;
	TCHAR *pTitle;
	TCHAR *pAlbum;
	pArtist = new TCHAR[dwA];
	pTitle = new TCHAR[dwT];
	pAlbum = new TCHAR[dwAl];
    MultiByteToWideChar(CP_ACP,0,album ,-1,pAlbum,dwAl);
    MultiByteToWideChar(CP_ACP,0,artist ,-1,pArtist,dwA);
    MultiByteToWideChar(CP_ACP,0,title ,-1,pTitle,dwT);
	
    lstrcpy(mp3Info->Album,pAlbum);
	lstrcpy(mp3Info->Artist,pArtist);
	lstrcpy(mp3Info->Title,pTitle);
    
	if(pArtist)
	{
	 delete[] pArtist;
	 pArtist = NULL;
	}
	if(pTitle)
	{
     delete[] pTitle;
	 pTitle = NULL;
	}
	if(pAlbum)
	{
     delete[] pAlbum;
	 pAlbum = NULL;
	}
	fclose(fp);
	return TRUE;
}
