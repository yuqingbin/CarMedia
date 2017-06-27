#include "SearchFile.h"
#include "CeFileFind.h"
#include "comm1.h"
//#ifdef _WINDOWS_ 
//	#undef _WINDOWS_ 
//		#include "afxcmn.h" 
//#end
#define MAX_COUNT  20480

CPathSave  AudioPath[MAX_COUNT] = {0};

CPathSave  VideoPath[MAX_COUNT] = {0};

int AudioCount = 0; /*Find number of audio file*/
int VideoCount = 0;  /*Find number of video file*/

void Search(CString lpPath,int Typeflg)
{
	#ifdef _WIN32_WCE
		CCeFileFind ff; 
	#endif
	#ifndef _WIN32_WCE
		CFileFind ff;
	#endif
		printf("In Search\n");
	 CString filepath;
	 lpPath = lpPath + L"\\" ;

	 lpPath = lpPath + L"*.*";
	 //printf("lpPath=%s\n",lpPath);
	
	 BOOL res = ff.FindFile(lpPath);
	 printf("In search res=%d\n",res);
	 while (res)
	 {
		  res = ff.FindNextFileW ();
		  filepath = ff.GetFilePath (); 
		  //printf("filepath=%s\n",filepath);
		  if (ff.IsDirectory ()&& !ff.IsDots ())
		  {
				Search(filepath,Typeflg);
		  }
		  else if (!ff.IsDirectory () && !ff.IsDots())  
		  {
				 if (Typeflg == 1)
				 {
					   if (AudioFileNamejudge(ff.GetFileName ()))
					   {   
							SaveAudioPath(ff.GetFilePath(),ff.GetLength());
					   }
				 }
				 if (Typeflg == 2)
				 {
					   if (VideoFileNamejudge(ff.GetFileName ()))
					   {
							SaveVideoPath(ff.GetFilePath(),ff.GetLength());
					   }
				 }
			}
		} 
}


int AudioFileNamejudge(CString FileName)
{
 if (FileName.Find (L".mp3") > -1 || FileName.Find (L".MP3") > -1)
  {
   return 1;
  }
  else
	  if (FileName.Find (L".wav") > -1 || FileName.Find (L".WAV") > -1)
	  {
	   return 1;
	  }
	  else
		  if (FileName.Find (L".wma") > -1 || FileName.Find (L".WMA") > -1)
		  {
		   return 1;
		  }
	   else
		  if (FileName.Find (L".aac") > -1 || FileName.Find (L".AAC") > -1)
		  {
		   return 1;
		  }
   else
   {
	   return 0;
   }
}

int VideoFileNamejudge(CString FileName)
{
	if (FileName.Find (L".avi") > -1 || FileName.Find (L".AVI") > -1)
	{
	 return 1;
	}
	
	else
		if (FileName.Find (L".mpg") > -1 || FileName.Find (L".MPG") > -1)
		{
		 return 1;
		}
		else
			if (FileName.Find(L".mp4") > -1 || FileName.Find (L".MP4") > -1)
			{
			 return 1;
			}
			else
				if(FileName.Find (L".mpeg") > -1 || FileName.Find (L".MPEG") > -1)
				{
				 return 1;
				}
				else
					if (FileName.Find (L".vob") > -1 || FileName.Find (L".VOB") > -1)
					{
					 return 1;
					}
					else
						if (FileName.Find (L".mkv") > -1 || FileName.Find (L".MKV") > -1)
						{
						 return 1;
						}
						else
							if (FileName.Find(L".mov") > -1 || FileName.Find (L".MOV") > -1)
							{
							 return 1;
							}
							else
								if (FileName.Find (L".rmvb") > -1 || FileName.Find (L".RMVB") > -1)
								{
								 return 1;
								}
								else 
									if (FileName.Find (L".3gp") > -1 || FileName.Find (L".3GP") > -1)
									{
									  return 1;
									}
									else
										if (FileName.Find (L".dat") > -1 || FileName.Find (L".DAT") > -1)
										{
										 return 1;
										}
										else
											if (FileName.Find (L".wmv") > -1 || FileName.Find (L".WMV") > -1)
											{
											 return 1;
											}
											else
												if (FileName.Find(L".asf") > -1 || FileName.Find (L".ASF") > -1)
												{
												 return 1;
												}
												else
												    if (FileName.Find(L".264") > -1 || FileName.Find (L".264") > -1)
													{
													 return 1;
													}
	else
	{
		return 0;
	}	
}
void SaveAudioPath(CString FilePath,DWORD filelenght)
{
	AudioPath[AudioCount].num = AudioCount;
	//AudioPath[AudioCount].pathname = new TCHAR[FilePath.GetLength ()];
	TCHAR unused[_MAX_PATH];
    TCHAR fName[_MAX_FNAME];
    _tsplitpath_s(FilePath,unused,MAX_PATH,unused,MAX_PATH,AudioPath[AudioCount].filename,MAX_PATH,unused,MAX_PATH);
	//AudioPath[AudioCount].filename = fName;
	//strcpy(AudioPath[AudioCount].filename,fName);
	float size = 0;
    size = (float)filelenght/1024/1024;
	_stprintf(AudioPath[AudioCount].filelenght,L"%.1f%s",size,L"MB");
	
	for (int i = 0;i<FilePath.GetLength ()+1;i++)
	{
		AudioPath[AudioCount].pathname[i] = FilePath.GetAt (i);
	}
	printf("AudioCount=%d",AudioCount);
	printTCHAR(AudioPath[AudioCount].pathname);
	AudioCount++;
	printf("AudioCount=%d\n");
}
void SaveVideoPath(CString FilePath,DWORD filelenght)
{
 VideoPath[VideoCount].num = VideoCount;
 TCHAR unused[_MAX_PATH];
    TCHAR fName[_MAX_FNAME];
    _tsplitpath_s(FilePath,unused,MAX_PATH,unused,MAX_PATH,VideoPath[VideoCount].filename,MAX_PATH,unused,MAX_PATH);
	//VideoPath[VideoCount].filename = fName;
	//strcpy(VideoPath[VideoCount].filename,fName);
	float size = 0;
  size = (float)filelenght/1024/1024;
  _stprintf(VideoPath[VideoCount].filelenght,L"%.1f%s",size,L"MB");
 //VideoPath[VideoCount].pathname = new TCHAR[FilePath.GetLength ()];
	for (int i = 0;i<FilePath.GetLength ()+1;i++)
	{
	 VideoPath[VideoCount].pathname[i] = FilePath.GetAt (i);
	}
 VideoCount++;
}