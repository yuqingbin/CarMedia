#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <TCHAR.h>
//#include <Shlwapi.h>
#include <assert.h>
#include "comm1.h"

/*
#ifdef UNICODE
	#define TCHAR wchar_t
#else
	#define TCHAR char
#endif // !UNICODE

#ifdef UNICODE
	#define GetModuleFileName GetModuleFileNameW
#else
	#define GetModuleFileName GetModuleFileNameA
#endif // !UNICODE

#ifdef UNICODE
	#define StrChr wcschr
#else
	#define StrChr strchr
#endif // !UNICODE

#ifdef UNICODE
	#define _tcscpy wcscpy
#else
	#define _tcscpy strcpy
#endif // !UNICODE


#ifdef UNICODE
	#define _tlen wcslen
#else
	#define _tlen strlen
#endif // !UNICODE

#ifdef UNICODE
	#define _tstrstr wcsstr
#else
	#define _tstrstr strstr
#endif // !UNICODE
*/
/*=====================================================================
 * function: find number of row and col in file#
 * nrow is total number of row in file readfilename
 * ncol is total number of col in first row of file readfilename
 * bool valuse  train_test : always is setup as 1
 */

void commf_get_data_info(const char *readfilename,
                               int *nrow,
                        int *ncol)
{
        int chars,
            tmprows,cols;
        char value,
             tmpval,
             *tmpline=NULL;
        FILE *input;
		errno_t err;

        if((err=fopen_s(&input,readfilename, "r"))!=0)
        {
                printf(" Can't find file %s ! \n",readfilename);
                exit(-1);
        }
        cols=0;
        chars=0;
        tmprows=0;
        value='d';

        while (value!='\n')
        {
                value=fgetc(input);
                chars++;
                tmpval=value;
                if ((value=='\t') || (value==' '))
                {
                        cols++;
                        while ((value=='\t') || (value==' '))
                        {
                                value=fgetc(input);
                                chars++;
                        }
                }
        }
        fclose(input);
        if ((tmpval!='\t') && (tmpval!=' ')) cols++;
        //printf(" cols=%d\n",cols);
        *ncol=cols;
        tmpline=(char *)calloc(chars+5000, sizeof(char));

       if( (err=fopen_s(&input,readfilename, "r"))==0)
	   {
		
			while (! feof(input))
			{
					fgets(tmpline, chars+5000, input);
					tmprows++;
			}
			if((tmpline[0]=='\n')||strchr(tmpline,'\n')) tmprows--;  /*add '\n' check strchr(tmpline,'\n')*/
			if(strlen(tmpline)==1)
			if((tmpline[0]=='\n')||strchr(tmpline,'\n')||tmpline[0]==' ') tmprows+=-1;
			fclose(input);
	        
					*nrow=tmprows;
	       
			//printf("nrows=%d\n",tmprows);
        }
}

/*
 * FUNCTION::Find out the position of substring s2 last-occurred in source string s1,

 * int outFLAG::FLAG==1, print out the substring beginning from the position of the last occurrance s2 to the end of the string s1
 *  int outFLAG::FLAG==2, print out the substring from beginning of s1 to the position of the last occurrance s2-1.
 */
TCHAR *commf_strrstr(TCHAR const *s1, TCHAR const *s2,int outFLAG)
{
	register TCHAR *last;
	register TCHAR *current;
    assert((s1!=NULL)&&(s2!=NULL));
	last = NULL;
	
	if (*s2 != '\0')
	{
		//查找 s2 在 s1 中第一次出现的位置
		current = (TCHAR*)_tstrstr(s1, s2);
		if(current==NULL)
		{
			//printf("%s doesn't occur in %s !!!\n",s2,s1);
			return NULL;
		}
		while (current != NULL)
		{
			last = current;
			current = _tstrstr(last+1, s2);
		}
	}
	
	if(outFLAG==1)
	{
		printf("In outFLAG==1\n");
		return last;
	}
	else
	{
		
		int slen=last-s1;
		printf("In outFLAG==2,slen=%d\n",slen);
	   
		TCHAR *outstr=(TCHAR*)calloc(slen+1,sizeof(TCHAR));
		TCHAR *address=(TCHAR *)outstr;

		for(int i=0;i<slen;i++)
		{
			if((*outstr++=*s1++)!='\0'){};
		}
		outstr=(TCHAR)'\0';
	    printTCHAR(outstr);
		return address;


	}
}


	

/*
*In source string "s1", find the first occurance substring "s2"
* if FLAG==2, RETURN the remaining string of  the first occurance substring
* if FLAG==1, RETURN　the substring from the beginning to the first occurance substring 
*/
TCHAR * commf_str_substr(TCHAR *s1, TCHAR *s2,int outFLAG)
{
	TCHAR *p1, *p2;
	if(outFLAG==2)
	{
		assert((s1!=NULL)&&(s2!=NULL));
	    
		p1 = s1;
		p2 = s2;
		
		while (*s1 != '\0' && *s2 != '\0') 
		{
		   if (*s1++ != *s2++) {
				s2 = p2;
				  s1 = ++p1; //从下一个字符开始搜索needle       
		   }
		}
		if (*s2 == '\0')
		   return p1;

		return NULL;
	}
	else
	{
		p1=(TCHAR*)_tstrstr(s1,s2);
		if(p1==NULL)
				return NULL;
		int slen=p1-s1;
		
		TCHAR *outstr=(TCHAR*)calloc(slen+1,sizeof(TCHAR));
		TCHAR *address=(TCHAR*)outstr;
		for(int i=0;i<slen;i++)
		{
			if((*outstr++=*s1++)!='\0') {}
		}
		outstr='\0';
		return address;
	}
} 


/*
* FUNCTION find the last chr occurred in source string srcstr
* Input : TCHAR *srcstr
* OUTPUT: desstr is string peeling off the last char m_char
* OUTPUT: destination string length as deslen; it also be index of the last chr occurred position; if deslen<=0, then destr=NULL;
*/

int commf_findLastchr(TCHAR m_chr, TCHAR *srcstr,TCHAR *desstr,int *deslen )
{
	int flag=0;
	int len=0; //_tlen(str);
	int i=0;
    TCHAR *p1=srcstr;
    TCHAR *p2=srcstr;
	*deslen=-1;
    if(srcstr==NULL)
	{	
		OutputDebugString(L"INPUT STRING IS NULL\n"); 
		OutputDebugString(_T("\r\n"));
		return 0;

	}
	while (*p1) 
	{
		
			len++; p1++;
	}
    for(int i=len-1;i>=0;i--)
	{	if(p1[i]==m_chr)
		{
			flag=1;
			*deslen=i+1;
			break;
		}
		
	}
	if(flag && *deslen)
	{
		desstr=(TCHAR*)calloc(*deslen+1,sizeof(TCHAR));
	    for(int i=0;i<*deslen;i++)
		{
			desstr[i]=p2[i];
		} 
		desstr[*deslen]='\0';
	}
    
	return flag;
}


//　#include <windows.h> 　　
//#include <stdio.h>

/*
 * Get Current executable file path
 * return: szCurPath is path name after remove filename
 * Return : if return 1, success; if 0, otherwise. 
 */
int commf_GetExePath(TCHAR *szCurPath)
{
	int flag=0;
	TCHAR szPath[MAX_PATH];  //,MAX_PATH是预定义的宏，一般是个数字常量如256

	if( !GetModuleFileName( NULL, szPath, MAX_PATH))
	{	printf("GetModuleFileName failed (%d)\n", GetLastError());
		MessageBox(NULL,L"Failed to get executable file path !!!",L"ERROR",MB_OK);
		return flag;
	}
	else
	{	flag=1;
		//获取文件路径。
			OutputDebugString(szPath);
            OutputDebugString(_T("\r\n"));
			TCHAR *p=szPath;
			while(StrChr(p,'\\'))
			{
				p=StrChr(p,'\\');
				p++;
			}
			//p--;
	*p='\0';
		_tcscpy_s(szCurPath,MAX_PATH,szPath);
//获取文件路径。
		//OutputDebugString(L"获取文件路径\n"); 
		OutputDebugString(L"Get file path:\n"); 
		OutputDebugString(_T("\r\n"));
		OutputDebugString(szCurPath);
        OutputDebugString(_T("\r\n"));
		TCHAR *rootpath1=commf_strrstr(szCurPath, L"bin",2);
		printf("OUTPUT ROOTPATH1:\n");	printTCHAR(rootpath1);	
		OutputDebugString(rootpath1);
        OutputDebugString(_T("\r\n"));
		
			
		/* Peel executable dir "bin\" to get root dir */
		TCHAR *rootpath2=commf_strrstr(szCurPath, L"BIN",2);
		printf("OUTPUT ROOTPATH2:\n");		
		OutputDebugString(rootpath2);
        OutputDebugString(_T("\r\n"));
			printTCHAR(rootpath2);
		if(rootpath2!=NULL)
		{	_tcscpy_s(szCurPath,MAX_PATH,rootpath2);
			flag=1;
		}
		else if(rootpath1!=NULL)
		{	_tcscpy_s(szCurPath,MAX_PATH,rootpath1);
			flag=1;
		}
		else
		{
			MessageBox(NULL,L"Can't Find \'bin\' or \'BIN\' Dir !!!",L"ERROR",MB_OK);
			flag=0;
		}
		printf("szCurPath="); 
		printTCHAR(szCurPath);
		if(rootpath1!=NULL)
		{			free(rootpath1);
		rootpath1=NULL;
		}
		if(rootpath2!=NULL)
		{			free(rootpath2);
					rootpath2=NULL;
		}
	}
	return flag;
}
 

/*Convert char * to TCHAR *
* INPUT:: char *my_char
* OUTPUT::TCHAR *my_tchar
*/

void commf_pChar2pTChar(TCHAR *my_tchar,char *my_str)
{
	while((*my_tchar++ = (TCHAR) *my_str++) != '\0');
    *my_tchar='\0';
}

/*Convert TCHAR * to char *
* OUTPUT:: char *my_char
* INPUT::TCHAR *my_tchar
*/


void commf_pTchar2pChar(char * my_str,TCHAR* my_tchar)
{ 

	while( (*my_str++ = (char)*my_tchar++) != '\0') ;
	*my_str='\0';
}



/*!***********************************************************************
 @Function		OsDisplayDebugString
 @Input			str		string to output
 @Description	Prints a debug string
*************************************************************************/
void OsDisplayDebugString(char *  str)
{
	if(str)
	{
#ifdef UNICODE
		wchar_t	strc[1024];
		int		i;

		for(i = 0; (str[i] != '\0') && (i < (sizeof(strc) / sizeof(*strc))); ++i)
		{
			strc[i] = (wchar_t)str[i];
		}

		strc[i] = '\0';

		OutputDebugString(strc);
#else
		OutputDebugString(str);
#endif
	}
}


//#include <stdio.h>
//#include <stdarg.h>
//#include <ctype.h>
//void __cdecl odprintf(const char *format, ...)
//{
//char buf[4096], *p = buf;
//va_list args;
//va_start(args, format);
//p += _vsnprintf(p, sizeof buf - 1, format, args);
//va_end(args);
//while ( p > buf  &&  isspace(p[-1]) )
//*--p = '\0';
//*p++ = '\r';
//*p++ = '\n';
//*p   = '\0';
//OutputDebugString(buf);
//}


void printTCHAR(TCHAR * tcharString)
{
//         FILE *input;
//		 errno_t err;
         //****************
		 //转换
		 DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,tcharString,-1,NULL,0,NULL,FALSE);
		 char *psText;
		 psText = new char[dwNum];
		 if (!psText)
		 {
		  delete[] psText;
		 }
		 WideCharToMultiByte(CP_OEMCP,NULL,tcharString,-1,psText,dwNum,NULL,FALSE);

		 //****************
	
      /*  if((err=fopen_s(&input,psText, "r"))!=0)
        {
                printf(" Can't find file %s ! \n",filename);
                exit(-1);
        }*/
		 printf(" %s ! \n",psText);
		  if (psText!=NULL)
		 {
		  delete[] psText;
			psText=NULL;
		 }
 
}

/*Two strings concatenation
* str1 [in]
* str2 [in]
* str [out]
*/
void commf_tStrCat(TCHAR *str,TCHAR str1[],TCHAR str2[]) 
{ 
  int i,j; 
  for(i=0;str1[i]!='\0';i++) 
   str[i]=str1[i]; 
   
  for(j=0;str2[j]!='\0';j++) 
    str[i+j]=str2[j]; 

   str[i+j]='\0'; 
} 

/*
* srcStr [in]
* desStr [in]
* int m [in] input number
* int len [in] number of digits for input number
* strlenLimit
*/
void commf_NumberString(TCHAR *desStr,TCHAR srcStr[],int m, int len,int strlenLimit)
{
	TCHAR str2[12];
	TCHAR strtmp[200];
	//_tsprintf(str2,12,"%S",m);
	if(len==3)
	{
		if(m<10)
		{
			TCHAR str1[]=L"00";
			//commf_tStrCat(strtmp,str1,str2[]);
			_tsprintf(strtmp,L"00%d %s\0",m,srcStr);
		}	
		else if((m>=10)&&(m<100))
		{
			TCHAR str1[]=L"0";
			//commf_tStrCat(strtmp,str1,str2[]); 
			_tsprintf(strtmp,L"0%d %s\0",m,srcStr);
		}
		else
		{
			//_tstrcpy(strtmp,str2);
			_tsprintf(strtmp,L"%d %s\0",m,srcStr);
		}
		//commf_tStrCat(desStr,strtmp,desStr); 
	}
	else if(len==2)
	{
		if(m<10)
		{
			TCHAR str1[]=L"0";
			//commf_tStrCat(strtmp,str1,str2[]); 
			_tsprintf(strtmp,L"0%d %s\0",m,srcStr);
		}	
		else
		{
			//_tstrcpy(strtmp,str2);
			//commf_tStrCat(strtmp,str1,str2[]); 
			_tsprintf(strtmp,L"%d %s\0",m,srcStr);
		}
		//commf_tStrCat(desStr,strtmp,desStr); 
	}
	else
	{
			_tsprintf(strtmp,L"%d %s\0",m,srcStr);
	}
	int Length=wcslen(strtmp);
	if(Length<strlenLimit)
    {
			_tcscpy(desStr,strtmp);
	}
	else //if(Length>=14)
	{
		for(int j=0;j<strlenLimit;j++)
		{
			desStr[j]=strtmp[j];
		}
		desStr[strlenLimit]='.';
		desStr[strlenLimit+1]='.';
		desStr[strlenLimit+2]='.';
		desStr[strlenLimit+3]='\0';

	}
}

void **commf_newMatrix(int nrow,int ncol,size_t elementsize)
{
	void **Mat=NULL;
	int FLAG=1;
	Mat=(void**)calloc(nrow,sizeof(void*));
	if(Mat==NULL)
	{	printf("In commf_newMatrix, Failed to allocate memory!!!\n");
		
		return NULL;
	}
	for(int i=0;i<nrow;i++)
	{
		Mat[i]=(void*)calloc(ncol,elementsize);
		if(Mat[i]==NULL)
		{	printf("In commf_newMatrix, Failed to allocate memory!!!\n");
			FLAG=-1;
			break;
		}
	}
	if(FLAG==-1)
	{	
		if(Mat)
		{	free(Mat);
			Mat=NULL;
		}
		return NULL;
	}
	else 
		return Mat;

}

void commf_freeMatrix(void **Mat,int nrow)
{
	if(Mat)
	{
		for(int i=0;i<nrow;i++)
		{
			if(Mat[i])
			{
				free(Mat[i]);
				Mat[i]=NULL;
			}
		}
		free(Mat);
		Mat=NULL;
	}
	
	

}
