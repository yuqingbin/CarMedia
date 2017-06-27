//////////////////////////////////////////////////////////
//wince CFileFind 类的实现
//
//BY  yuqingbin
//////////////////////////////////////////////////////////


#if !defined _CEFILEFIND_H_
#define _CEFILEFIND_H_

// CeFileFind.h : header file

#include <afxwin.h>
/////////////////////////////////////////////////////////////////////////////
// CCeFileFind window

class CCeFileFind : public CWnd
{
// Construction
public:
	CCeFileFind( );

public:
// Operations
	void Close();
	virtual BOOL FindNextFile( );
	virtual BOOL FindFile( LPCTSTR pstrName = NULL);

public:
// Attributes

	//Gets the length of the found file, in bytes. 
	DWORD	GetLength() const;
				
	//Gets the name, including the extension, of the found file 
	CString GetFileName() const;				

	//Gets the whole path of the found file. 
	CString GetFilePath() const;			

	//Gets the whole path of the found file. 
	CString GetRoot() const;			

	// to get the time the specified file was created
	virtual BOOL GetCreationTime( FILETIME* pFileTime ) const;
	virtual BOOL GetCreationTime( CTime& refTime ) const;


	//Gets the time that the file was last accessed. 
	virtual BOOL GetLastAccessTime( CTime& refTime ) const;
	virtual BOOL GetLastAccessTime( FILETIME* pFileTime ) const;


	//Gets the time the file was last changed and saved. 
	virtual BOOL GetLastWriteTime( FILETIME* pFileTime ) const;
	virtual BOOL GetLastWriteTime( CTime& refTime ) const;

	//Indicates the desired file attributes of the file to be found. 
	virtual BOOL MatchesMask( DWORD dwMask ) const;
	
	//Determines if the name of the found file has the name "." or "..", 
	//indicating that is actually a directory. 
	virtual BOOL IsDots( ) const;
	
	//Determines if the found file is read-only. 
	BOOL IsReadOnly( ) const;

	//Determines if the found file is a directory. 
	BOOL IsDirectory( ) const;
				
	//Determines if the found file is compressed. 
	BOOL IsCompressed( ) const;

	//Determines if the found file is a system file. 
	BOOL IsSystem( ) const;

	//Determines if the found file is hidden. 
	BOOL IsHidden( ) const;

	//Determines if the found file is temporary. 
	BOOL IsTemporary( ) const;

	//Determines if the found file is normal (in other words, has no other attributes). 
	BOOL IsNormal( ) const;

	//Determines if the found file is archived. 
	BOOL IsArchived( ) const;



// Implementation
public:
	virtual ~CCeFileFind();

private:

	LPWIN32_FIND_DATA m_pfiledata;
	LPWIN32_FIND_DATA m_pNextdata;
	CString	m_csRoot;
	HANDLE m_hFileHandle;

	char	m_chDirSeparator;

	void AssertDoneNext() const;
};

#endif // !defined _CEFILEFIND_H_
