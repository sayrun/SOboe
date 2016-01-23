#ifndef	__CHIPEXLIB_H__
#define	__CHIPEXLIB_H__

#include "ChipEx_IF.h"

class CMemoData;

class CChipExLib : public CObject
{
public:
	CChipExLib();
	~CChipExLib();

	BOOL SetIndex( int nIndex);
	BOOL SetPath( const CString& cStrPath);
	BOOL SetMenu( const CString& cStrMenu);
	const CString& GetMenu( void);

	BOOL LoadLibrary();
	BOOL FreeLibrary();

	BOOL ChipExProc( HWND hWnd, CMemoData& cMemoData, BOOL& blReSize, BOOL& blAutoReSize);

	// égópíÜÅH
	static BOOL IsUsed();

protected:
	BOOL CreateIFData( IFDATA& stIFData, const CMemoData& cMemoData);
	BOOL ModifyIFData( IFDATA& stIFData, CMemoData& cMemoData, BOOL& blResize, BOOL& blAutoResize);
	BOOL ReleaseIFData( IFDATA& stIFData);

	typedef	BOOL ( *CHIPEXSPECIFICATION)( SPECIFICATION* pSpecification);
	typedef	BOOL ( *CHIPEXPROC)( HWND hWnd, UINT unMenuNum, IFDATA* pstIFData);

	SPECIFICATION	m_stSpecification;

	static CSemaphore	sm_cSmpDoCall;

	int		m_nIndex;
	CString	m_cStrMenu;
	CString	m_cStrPath;

	HINSTANCE	m_hInstance;
};

#include "ChipExLib.inl"
#endif	//__CHIPEXLIB_H__