#include "stdafx.h"
#include "resource.h"
#include "DrawExLib.h"

CDrawExLib::CDrawExLib() : CObject()
{
	m_unDxlDataVersion	= 0;	// �����\�ȍŐV�o�[�W����
	m_pcStrMenuString	= NULL;	// �f�t�H���g�̏�������
	m_unDXLID			= 0;	// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	m_unDrawType		= 0;	// dxl�̏������e
	m_unUnsupportMenu	= 0;	// �T�|�[�g���Ȃ��@�\

	m_hInstance			= NULL;	// 
	m_pcStrDxlPath		= NULL;	// �t�@�C����

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

	// �@�\�m�F
#if	0
	DXLSPECIFICATION	stDxlSpec;
	DRAWEXSPECIFICATION	lpfDrawExSpecifcation;
	lpfDrawExSpecifcation = ( DRAWEXSPECIFICATION)GetProcAddress( m_hInstance, "DrawExSpecification");
	if( lpfDrawExSpecifcation)
	{
		lpfDrawExSpecifcation( &stDxlSpec);
		m_unDxlDataVersion	= stDxlSpec.unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
		m_unDXLID			= stDxlSpec.unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
		m_unDrawType		= stDxlSpec.unDrawType;				// dxl�̏������e
		m_unUnsupportMenu	= stDxlSpec.unUnsupportMenu;		// �T�|�[�g���Ȃ��@�\
		m_cStrDxlMenu		= stDxlSpec.szMenuString;			// �f�t�H���g�̃��j���[
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
		m_unDxlDataVersion	= pstDxlSpec->unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
		m_unDXLID			= pstDxlSpec->unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
		m_unDrawType		= pstDxlSpec->unDrawType;				// dxl�̏������e
		m_unUnsupportMenu	= pstDxlSpec->unUnsupportMenu;		// �T�|�[�g���Ȃ��@�\
		m_cStrDxlMenu		= pstDxlSpec->szMenuString;			// �f�t�H���g�̃��j���[
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
	m_unDxlDataVersion	= 0;	// �����\�ȍŐV�o�[�W����
	m_unDXLID			= 0;	// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	m_unDrawType		= 0;	// dxl�̏������e
	m_unUnsupportMenu	= 0;	// �T�|�[�g���Ȃ��@�\

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
