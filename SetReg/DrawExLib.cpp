#include "stdafx.h"
#include "resource.h"
#include "DrawExLib.h"

CDrawExLib::CDrawExLib() : CObject()
{
	m_unDxlDataVersion	= 0;	// 処理可能な最新バージョン
	m_pcStrMenuString	= NULL;	// デフォルトの処理名称
	m_unDXLID			= 0;	// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	m_unDrawType		= 0;	// dxlの処理内容
	m_unUnsupportMenu	= 0;	// サポートしない機能

	m_hInstance			= NULL;	// 
	m_pcStrDxlPath		= NULL;	// ファイル名

	m_nUseCount			= 0;
}

CDrawExLib::~CDrawExLib()
{
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	if( m_pcStrMenuString)
	{
		delete m_pcStrMenuString;
		m_pcStrMenuString = NULL;
	}
	if( m_pcStrDxlPath)
	{
		delete m_pcStrDxlPath;
		m_pcStrDxlPath = NULL;
	}
}

BOOL CDrawExLib::LoadDxl( HWND hWnd)
{
	if( NULL == m_pcStrDxlPath)return TRUE;
	if( m_pcStrDxlPath->IsEmpty())return TRUE;
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}

	m_hInstance = LoadLibrary( *m_pcStrDxlPath);
	if( NULL == m_hInstance)
	{
		DispErrorMsg( hWnd, GetLastError());
		return TRUE;
	}

	// 機能確認
#if	0
	DXLSPECIFICATION	stDxlSpec;
	DRAWEXSPECIFICATION	lpfDrawExSpecifcation;
	lpfDrawExSpecifcation = ( DRAWEXSPECIFICATION)GetProcAddress( m_hInstance, "DrawExSpecification");
	if( lpfDrawExSpecifcation)
	{
		lpfDrawExSpecifcation( &stDxlSpec);
		m_unDxlDataVersion	= stDxlSpec.unUpperStructVersion;	// 処理可能な最新バージョン
		m_unDXLID			= stDxlSpec.unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
		m_unDrawType		= stDxlSpec.unDrawType;				// dxlの処理内容
		m_unUnsupportMenu	= stDxlSpec.unUnsupportMenu;		// サポートしない機能
		m_cStrDxlMenu		= stDxlSpec.szMenuString;			// デフォルトのメニュー
		return FALSE;
	}
	else
	{
		DispErrorMsg( hWnd, GetLastError());
		return TRUE;
	}
#else
	DXLSPECIFICATION*	pstDxlSpec;
	TRACE( "SAMPLE");
	pstDxlSpec = ( DXLSPECIFICATION*)GlobalAlloc( GPTR, sizeof( DXLSPECIFICATION));
	DRAWEXSPECIFICATION	lpfDrawExSpecifcation;
	lpfDrawExSpecifcation = ( DRAWEXSPECIFICATION)GetProcAddress( m_hInstance, "DrawExSpecification");
	if( lpfDrawExSpecifcation)
	{
		lpfDrawExSpecifcation( pstDxlSpec);
		m_unDxlDataVersion	= pstDxlSpec->unUpperStructVersion;	// 処理可能な最新バージョン
		m_unDXLID			= pstDxlSpec->unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
		m_unDrawType		= pstDxlSpec->unDrawType;				// dxlの処理内容
		m_unUnsupportMenu	= pstDxlSpec->unUnsupportMenu;		// サポートしない機能
		m_cStrDxlMenu		= pstDxlSpec->szMenuString;			// デフォルトのメニュー
		GlobalFree( pstDxlSpec);
		return FALSE;
	}
	else
	{
		GlobalFree( pstDxlSpec);
		DispErrorMsg( hWnd, GetLastError());
		return TRUE;
	}
#endif
}

BOOL CDrawExLib::FreeDxl( void)
{
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	m_unDxlDataVersion	= 0;	// 処理可能な最新バージョン
	m_unDXLID			= 0;	// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	m_unDrawType		= 0;	// dxlの処理内容
	m_unUnsupportMenu	= 0;	// サポートしない機能

	return FALSE;
}

BOOL CDrawExLib::AboutDrawExLib( HWND hWnd)
{
	DRAWEXABOUT	lpfDrawExAbount;
	lpfDrawExAbount = ( DRAWEXABOUT)GetProcAddress( m_hInstance, "DrawExAbout");
	if( lpfDrawExAbount)
	{
		lpfDrawExAbount( hWnd);
	}
	return FALSE;
}

BOOL CDrawExLib::Configure( HWND hWnd)
{
	DRAWEXCONFIGURE	lpfDrawExConfigure;
	lpfDrawExConfigure = ( DRAWEXCONFIGURE)GetProcAddress( m_hInstance, "DrawExConfigure");
	if( lpfDrawExConfigure)
	{
		lpfDrawExConfigure( hWnd);
	}
	return FALSE;
}

void CDrawExLib::DispErrorMsg( HWND hWnd, DWORD dwCode)
{
    LPTSTR lpszTemp = NULL;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
                           NULL,
                           dwCode,
                           LANG_NEUTRAL,
                           (LPTSTR)&lpszTemp,
                           0,
                           NULL );

	CString	cStrMsgDisp;
    if ( lpszTemp )
	{
		cStrMsgDisp = lpszTemp;
		LocalFree((HLOCAL) lpszTemp);
	}
	else
	{
		cStrMsgDisp.Format( "unknow error code = %d", dwCode);
	}

	::MessageBox( hWnd, cStrMsgDisp, NULL, MB_OK | MB_ICONHAND);
}
