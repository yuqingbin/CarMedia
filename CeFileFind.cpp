// CeFileFind.cpp : implementation file
//
//#include <assert.h>
// #include<cassert> 


#include "CeFileFind.h"
//#ifdef _WINDOWS_ 
//	#undef _WINDOWS_ 
//		#include "afxcmn.h" 
//#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DELETE_POINTER(ptr)		if( ptr != NULL )	\
								{					\
									delete ptr;		\
									ptr = NULL;		\
								}

#define DIR_SEPERATOR		'\\'

/////////////////////////////////////////////////////////////////////////////
// CCeFileFind

CCeFileFind::CCeFileFind()
			:m_hFileHandle(NULL), // initialize to NULL
			m_pfiledata(NULL)
{
}

CCeFileFind::~CCeFileFind()
{
	Close();
}


// Operations
BOOL CCeFileFind::FindFile(LPCTSTR pstrName)
{

	Close();

	// if NULL , wild card search 
	if( NULL == pstrName )
	{
		m_csRoot = DIR_SEPERATOR;
		pstrName = _T("\\*.*");
		
	}
	else
	{

		m_csRoot = pstrName;
		int nPos = m_csRoot.ReverseFind( '\\' );
		if( nPos == 0 )
			m_csRoot = '\\';
		else
			m_csRoot = m_csRoot.Left( nPos );
	
	}

	m_pNextdata = new WIN32_FIND_DATA;
	// search for file
	m_hFileHandle = FindFirstFile( pstrName, m_pNextdata );
	
	if ( m_hFileHandle == INVALID_HANDLE_VALUE )
	{
		Close();
		return FALSE;
	}

	// file was found
	return TRUE;
}


BOOL CCeFileFind::FindNextFile()
{
	ASSERT(m_hFileHandle != NULL);

	if (m_hFileHandle == NULL)
		return FALSE;

	if (m_pfiledata == NULL)
		m_pfiledata = new WIN32_FIND_DATA;

	AssertDoneNext();

	LPWIN32_FIND_DATA pTemp = m_pfiledata;
	m_pfiledata = m_pNextdata;
	m_pNextdata = pTemp;

	return ::FindNextFile(m_hFileHandle, m_pNextdata);
}


void CCeFileFind::Close()
{
	DELETE_POINTER( m_pfiledata );
	DELETE_POINTER( m_pNextdata );

	if( m_hFileHandle!= NULL && m_hFileHandle != INVALID_HANDLE_VALUE )
	{
		::FindClose( m_hFileHandle );
		m_hFileHandle = NULL;
	}
}

BOOL CCeFileFind::MatchesMask(DWORD dwMask) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if ( m_pfiledata != NULL)
		return (!!(m_pfiledata->dwFileAttributes & dwMask) );
	else
		return FALSE;
}


CString CCeFileFind::GetRoot() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
		return m_csRoot;
	else
		return L"";

}

BOOL CCeFileFind::GetLastAccessTime(FILETIME* pTimeStamp) const
{
	ASSERT(m_hFileHandle != NULL);
	ASSERT(pTimeStamp != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = m_pfiledata -> ftLastAccessTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::GetLastWriteTime(FILETIME* pTimeStamp) const
{
	ASSERT(m_hFileHandle != NULL);
	ASSERT(pTimeStamp != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = m_pfiledata -> ftLastWriteTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::GetCreationTime(FILETIME* pTimeStamp) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = m_pfiledata -> ftCreationTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::GetLastAccessTime(CTime& refTime) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		refTime = CTime( m_pfiledata -> ftLastAccessTime );
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::GetLastWriteTime(CTime& refTime) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		refTime = CTime( m_pfiledata -> ftLastWriteTime );
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::GetCreationTime(CTime& refTime) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		refTime = CTime( m_pfiledata -> ftCreationTime );
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CCeFileFind::IsDots() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	// return TRUE if the file name is "." or ".." and
	// the file is a directory

	BOOL bResult = FALSE;
	if (m_pfiledata != NULL && IsDirectory())
	{
		LPWIN32_FIND_DATA pFindData = m_pfiledata;
		if (pFindData->cFileName[0] == '.')
		{
			if (pFindData->cFileName[1] == '\0' ||
				(pFindData->cFileName[1] == '.' &&
				 pFindData->cFileName[2] == '\0'))
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

BOOL CCeFileFind::IsArchived( ) const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;

}

BOOL CCeFileFind::IsCompressed() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes == FILE_ATTRIBUTE_COMPRESSED )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}


BOOL CCeFileFind::IsDirectory() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CCeFileFind::IsHidden() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}


BOOL CCeFileFind::IsNormal() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_NORMAL )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CCeFileFind::IsReadOnly() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_READONLY )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CCeFileFind::IsSystem() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_SYSTEM )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}


BOOL CCeFileFind::IsTemporary() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
	{
		if( m_pfiledata -> dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY )
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}


CString CCeFileFind::GetFilePath() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	CString csResult = m_csRoot;

	if (csResult[csResult.GetLength()-1] != DIR_SEPERATOR )
		csResult += DIR_SEPERATOR;
	csResult += GetFileName();
	return csResult;
}


CString CCeFileFind::GetFileName() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	CString ret;

	if (m_pfiledata != NULL)
		ret = m_pfiledata->cFileName;
	return ret;
}

DWORD CCeFileFind::GetLength() const
{
	ASSERT(m_hFileHandle != NULL);
	AssertDoneNext();

	if (m_pfiledata != NULL)
		return m_pfiledata -> nFileSizeLow;
	else
		return 0;
}


void CCeFileFind::AssertDoneNext() const
{
	// if you trip the ASSERT in the else side, you've called
	// a Get() function without having done at least one
	// FindNextFile() call

	if (m_hFileHandle == NULL)
		ASSERT( m_pfiledata == NULL && m_pNextdata == NULL);
	else
		ASSERT( m_pfiledata != NULL && m_pNextdata != NULL);
}

