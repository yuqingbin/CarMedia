#ifdef UNDER_CE
	#include "afx.h"
#endif
//#include "afx.h"
#include "Mp3Id3.h"
#include <stdio.h>
#include <stdlib.h>
#include "comm1.h"


MP3INFO* ReadMp3Id3(TCHAR *strPlayListName)
{
    FILE *fp; // CFile file;
	MP3INFO *mp3;
	//初始化
    mp3=(MP3INFO *) new BYTE[sizeof(MP3INFO)];
	memset(mp3,0,sizeof(MP3INFO)); //ZeroMemory(mp3,sizeof(MP3INFO));
  if ( _wfopen_s( &fp, strPlayListName, L"rb" ) != 0 )
	 //if (!file.Open (strPlayListName,CFile::modeRead ))
	{
		 MessageBox(NULL,L"Can not open file.",L"ERROR",MB_OKCANCEL );
		//printf("Can not open file %s.\n",strPlayListName);
		 return NULL;
	}
	fseek(fp,-128,SEEK_END );//file.Seek(128,CFile::end);   //模拟器128,开发板-128
	BYTE pbuf[128];
	memset(pbuf,0,sizeof(pbuf));
	fread(pbuf,128,sizeof(BYTE),fp); //file.Read(pbuf,128);
	if(!((pbuf[0] == 'T'|| pbuf[0] == 't')
		&&(pbuf[1] == 'A'|| pbuf[1] == 'a')
		&&(pbuf[2] == 'G'|| pbuf[2] == 'g')))
	{
		//AfxMessageBox(L"Not a standard MP3 format!");
		
		printTCHAR(strPlayListName);printf(" is not mp3 file\n");
		fclose(fp);//file.Close ();
		return mp3;
	}
	else
	{
	
		memcpy(mp3->Title,pbuf+3,30);
		memcpy(mp3->Artist ,pbuf+33,30);
		memcpy(mp3->Album ,pbuf+63,30);
		printf("mp3->Title=%s,mp3->Artist=%s,mp3->Album=%s\n",mp3->Title,mp3->Artist,mp3->Album);
		
		/*DWORD dwA = MultiByteToWideChar(CP_ACP,0,mp3->Artist ,-1,NULL,0); //字符的转换
		DWORD dwAl = MultiByteToWideChar(CP_ACP,0,mp3->Album ,-1,NULL,0);
		DWORD dwT = MultiByteToWideChar(CP_ACP,0,mp3->Title,-1,NULL,0);
		TCHAR *pArtist;
		TCHAR *pTitle;
		TCHAR *pAlbum;
		pArtist = new TCHAR[dwA];
		pTitle = new TCHAR[dwT];
		pAlbum = new TCHAR[dwAl];
		MultiByteToWideChar(CP_ACP,0,mp3->Album ,-1,pAlbum,dwAl);
		MultiByteToWideChar(CP_ACP,0,mp3->Artist ,-1,pArtist,dwA);
		MultiByteToWideChar(CP_ACP,0,mp3->Title ,-1,pTitle,dwT);*/

	}

     fclose(fp); //file.Close ();
    return mp3;
}