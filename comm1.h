#define TEXT(quote) __TEXT(quote)   // r_winnt

#ifdef UNICODE
#define __TEXT(quote) L##quote
#else
#define __TEXT(quote) quote
#endif

#ifdef UNICODE
#define _T(quote) L##quote
#else
#define _T(quote) quote
#endif

#ifdef UNICODE
#define _tsprintf_s swprintf_s
#else
#define _tsprintf_s sprintf_s
#endif

#ifdef UNICODE
#define _tsprintf swprintf
#else
#define _tsprintf sprintf
#endif

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
	#define _tcscpy_s wcscpy_s
#else
	#define _tcscpy_s strcpy_s
#endif // !UNICODE

#ifdef UNICODE
	#define _tcscat_s wcscat_s
#else
	#define _tcscat_s strcat_s
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

void commf_get_data_info(const char *readfilename,
                               int *nrow,
                        int *ncol);

int commf_GetExePath(TCHAR *szCurPath);


/*Convert char * to TCHAR *
* INPUT:: char *my_char
* OUTPUT::TCHAR *my_tchar
*/
void commf_pChar2pTChar(TCHAR *my_tchar,char *my_str);


/*Convert TCHAR * to char *
* OUTPUT:: char *my_char
* INPUT::TCHAR *my_tchar
*/
void commf_pTchar2pChar(char * my_str,TCHAR* my_tchar);

/*
* FUNCTION find the last chr occurred in source string srcstr
* Input : TCHAR *srcstr
* OUTPUT: desstr is string peeling off the last char m_char
* OUTPUT: destination string length as deslen; it also be index of the last chr occurred position; if deslen<=0, then destr=NULL;
*/

int commf_findLastchr(TCHAR m_chr, TCHAR *srcstr,TCHAR *desstr,int *deslen );


/*
*In source string "s1", find the first occurance substring "s2"
* if outFLAG==2, RETURN the remaining string of  the first occurance substring
* if FLAG==1, RETURN　the substring from the beginning to the first occurance substring 
*/
//const char * commf_str_substr(const char *src, const char *needle);
TCHAR * commf_str_substr(TCHAR *s1, TCHAR *s2,int outFLAG);

/*
 * FUNCTION::Find out the position of substring s2 last-occurred in source string s1,
 * Loop is done char-by-char move pointer backward
 * 找出字符串s2在s1中最后出现的位置。那个循环就是一个字符一个字符地往后移动指针，
 * 然后查找在新的字符串中还有没有要查找的字符。
 * int outFLAG::FLAG==1, print out the substring beginning from the position of the last occurrance s2 to the end of the string s1
 *  int outFLAG::FLAG==2, print out the substring from beginning of s1 to the position of the last occurrance s2-1.
 */
TCHAR *commf_strrstr(TCHAR const *s1, TCHAR const *s2,int outFLAG);

/*!***********************************************************************
 @Function		OsDisplayDebugString
 @Input			str		string to output
 @Description	Prints a debug string
*************************************************************************/
void OsDisplayDebugString(char  *  str);
void printTCHAR(TCHAR * tcharString);

/*#ifdef   _cplusplus  
#define   EXTERN_C   extern   "C "  
#else  
#define   EXTERN_C   extern  
#endif  

#ifdef   _cplusplus  
extern   "C "{  
#endif  
EXTERN_C   int   func(int   a,   int   b);  
#ifdef   _cplusplus  
}  
#endif  
*/

/*
* srcStr [in]
* desStr [in]
* int m [in] input number
* int len [in] number of digits for input number
*/
void commf_NumberString(TCHAR *desStr,TCHAR srcStr[],int m, int len,int strlenLimit );
	

void **commf_newMatrix(int nrow,int ncol,size_t elementsize);
void commf_freeMatrix(void **Mat,int nrow);
