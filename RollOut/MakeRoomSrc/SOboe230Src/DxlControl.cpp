#include "stdafx.h"
#include "MainFrm.h"
#include "resource.h"
#include "DxlControl.h"
#include "MemoChip.h"
#include "MemoData.h"
#include "GetMemoData.h"

CSemaphore	CDxlControl::sm_cSmpDoMenu;

CDxlControl::CDxlControl( void) : CObject()
{
	m_hInstance = NULL;
	m_unIFDataVer = 0;
	m_dwDxlWord = 0;

	// ����������
	m_lpfInitializeChip = NULL;
	// �I������
	m_lpfUnInitializeChip = NULL;
	// �傫���Čv�Z
	m_lpfCalcDefaultRect = NULL;
	// �\��
	m_lpfDrawChip = NULL;
	// �}�E�X�N���b�N
	m_lpfMouseClik = NULL;
	// �T�C�Y�ύX
	m_lpfResize = NULL;
	// ���Љ��^�A�C�R����
	m_lpfZoom = NULL;
	// �f�[�^�X�V�F�ҏW�ɂ�蔭��
	m_lpfUpdate = NULL;
	// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
	m_lpfGetMemoString = NULL;
	// �|�b�v�A�b�v���j���[
	m_lpfGetDrawExMenu = NULL;
	// �|�b�v�A�b�v���j���[
	m_lpfDoDrawExMenu = NULL;
	// �f�[�^�ۑ����̕ҏW
	m_lpfGetSaveData = NULL;
	// �^�C�}�����擾
	m_lpfnGetTimerSpan = NULL;
	// �^�C�}����
	m_lpfnDoTimer = NULL;
	// �^�C�}�J�E���^
	m_nTimerSpan = 0;

	// �}�E�X�N���b�N�F�g��
	m_lpfMouseClikEx = NULL;
	// ���Ђ̃A�N�e�B�u�F�V�K
	m_lpfActive = NULL;
}

CDxlControl::~CDxlControl( void)
{
}

BOOL CDxlControl::SetInstance( const CDrawExLib* pcDrawExLib)
{
	m_hInstance = pcDrawExLib->GetInstance();
	if( NULL == m_hInstance)return FALSE;

	m_unDrawType = pcDrawExLib->GetDrawType();
	m_unStyle = pcDrawExLib->GetDxlStyle();
	m_unIFDataVer = pcDrawExLib->GetDataVersion();
	
	// ����������
	m_lpfInitializeChip = ( INITIALIZECHIP)GetProcAddress( m_hInstance, "InitializeChip");
	// �I������
	m_lpfUnInitializeChip = ( UNINITIALIZECHIP)GetProcAddress( m_hInstance, "UnInitializeChip");
	// �傫���Čv�Z
	m_lpfCalcDefaultRect = ( CALCDEFAULTRECT)GetProcAddress( m_hInstance, "CalcDefaultRect");
	// �\��
	m_lpfDrawChip = ( DRAWCHIP)GetProcAddress( m_hInstance, "DrawChip");
	// �}�E�X�N���b�N
	m_lpfMouseClik = ( MOUSECLIK)GetProcAddress( m_hInstance, "MouseClik");
	// �T�C�Y�ύX
	m_lpfResize = ( RESIZE)GetProcAddress( m_hInstance, "Resize");
	// ���Љ��^�A�C�R����
	m_lpfZoom = ( ZOOM)GetProcAddress( m_hInstance, "Zoom");
	// �f�[�^�X�V�F�ҏW�ɂ�蔭��
	m_lpfUpdate = ( UPDATE)GetProcAddress( m_hInstance, "Update");
	// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
	m_lpfGetMemoString = ( GETMEMOSTRING)GetProcAddress( m_hInstance, "GetMemoString");
	// �|�b�v�A�b�v���j���[
	m_lpfGetDrawExMenu = ( GETDRAWEXMENU)GetProcAddress( m_hInstance, "GetDrawExMenu");
	// �|�b�v�A�b�v���j���[
	m_lpfDoDrawExMenu = ( DODRAWEXMENU)GetProcAddress( m_hInstance, "DoDrawExMenu");
	// �f�[�^�ۑ����̕ҏW
	m_lpfGetSaveData = ( GETSAVEDATA)GetProcAddress( m_hInstance, "GetSaveData");
	// �^�C�}�����擾
	m_lpfnGetTimerSpan = ( GETTIMERSPAN)GetProcAddress( m_hInstance, "GetTimerSpan");
	// �^�C�}����
	m_lpfnDoTimer = ( DOTIMER)GetProcAddress( m_hInstance, "DoTimer");

	// �}�E�X�N���b�N�F�g��
	m_lpfMouseClikEx = ( MOUSECLIKEX)GetProcAddress( m_hInstance, "MouseClickEx");
	// ���Ђ̃A�N�e�B�u�F�V�K
	m_lpfActive = ( ACTIVE)GetProcAddress( m_hInstance, "Active");


	BOOL lbGetProcError = FALSE;
	if( NULL == m_lpfInitializeChip)lbGetProcError = TRUE;
	if( NULL == m_lpfUnInitializeChip)lbGetProcError = TRUE;
	if( NULL == m_lpfCalcDefaultRect)lbGetProcError = TRUE;
	if( NULL == m_lpfDrawChip)lbGetProcError = TRUE;
	if( NULL == m_lpfMouseClik)lbGetProcError = TRUE;
	if( NULL == m_lpfUpdate)lbGetProcError = TRUE;
	if( NULL == m_lpfGetMemoString)lbGetProcError = TRUE;
	if( NULL == m_lpfGetDrawExMenu)lbGetProcError = TRUE;
	if( NULL == m_lpfDoDrawExMenu)lbGetProcError = TRUE;
	if( NULL == m_lpfGetSaveData)lbGetProcError = TRUE;

	if( lbGetProcError)
	{
		// ����������
		m_lpfInitializeChip = NULL;
		// �I������
		m_lpfUnInitializeChip = NULL;
		// �傫���Čv�Z
		m_lpfCalcDefaultRect = NULL;
		// �\��
		m_lpfDrawChip = NULL;
		// �}�E�X�N���b�N
		m_lpfMouseClik = NULL;
		// �T�C�Y�ύX
		m_lpfResize = NULL;
		// ���Љ��^�A�C�R����
		m_lpfZoom = NULL;
		// �f�[�^�X�V�F�ҏW�ɂ�蔭��
		m_lpfUpdate = NULL;
		// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
		m_lpfGetMemoString = NULL;
		// �|�b�v�A�b�v���j���[
		m_lpfGetDrawExMenu = NULL;
		// �|�b�v�A�b�v���j���[
		m_lpfDoDrawExMenu = NULL;
		// �f�[�^�ۑ����̕ҏW
		m_lpfGetSaveData = NULL;

		return FALSE;
	}

	return TRUE;
}

HINSTANCE CDxlControl::GetInstance( void) const
{
	return m_hInstance;
}


const CDxlControl& CDxlControl::operator=( CDxlControl& cStrSrc)
{
	m_hInstance			= cStrSrc.m_hInstance;
	m_unDrawType		= cStrSrc.m_unDrawType;
	m_unStyle			= cStrSrc.m_unStyle;

	// ����������
	m_lpfInitializeChip = cStrSrc.m_lpfInitializeChip;
	// �I������
	m_lpfUnInitializeChip = cStrSrc.m_lpfUnInitializeChip;
	// �傫���Čv�Z
	m_lpfCalcDefaultRect = cStrSrc.m_lpfCalcDefaultRect;
	// �\��
	m_lpfDrawChip = cStrSrc.m_lpfDrawChip;
	// �}�E�X�N���b�N
	m_lpfMouseClik = cStrSrc.m_lpfMouseClik;
	// �T�C�Y�ύX
	m_lpfResize = cStrSrc.m_lpfResize;
	// ���Љ��^�A�C�R����
	m_lpfZoom = cStrSrc.m_lpfZoom;
	// �f�[�^�X�V�F�ҏW�ɂ�蔭��
	m_lpfUpdate = cStrSrc.m_lpfUpdate;
	// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
	m_lpfGetMemoString = cStrSrc.m_lpfGetMemoString;
	// �|�b�v�A�b�v���j���[
	m_lpfGetDrawExMenu = cStrSrc.m_lpfGetDrawExMenu;
	// �|�b�v�A�b�v���j���[
	m_lpfDoDrawExMenu = cStrSrc.m_lpfDoDrawExMenu;
	// �f�[�^�ۑ����̕ҏW
	m_lpfGetSaveData = cStrSrc.m_lpfGetSaveData;
	// �^�C�}�����擾
	m_lpfnGetTimerSpan = cStrSrc.m_lpfnGetTimerSpan;
	// �^�C�}����
	m_lpfnDoTimer = cStrSrc.m_lpfnDoTimer;

	return *this;
}

BOOL CDxlControl::InitializeChip( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom)
{
	if( NULL == m_lpfInitializeChip)return FALSE;

	DXLIFDATA	stDxlIfData;

	// ���̍\���̂̃T�C�Y
	stDxlIfData.m_nSize		= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? sizeof( DXLIFDATA) : sizeof( DXLIFDATA100);
	// ���̍\���̂̃o�[�W����
	stDxlIfData.m_nVersion	= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? _DXL_IF_DATA_VER_200 : _DXL_IF_DATA_VER_100;

	const CMemoData*	pcMemoData = cMemoChip.AccessMemoData();
	// ���Ђ̃^�C�g��
	CString	cStr;
	pcMemoData->GetTitle( cStr);
	lstrcpy( stDxlIfData.m_achszMemoTitle, cStr);
	// ���Ђ̓��e
	char* pszMemo = new char[ pcMemoData->GetMemoLength() + 1];
	lstrcpy( pszMemo, pcMemoData->GetMemo());
	stDxlIfData.m_lpcszMemoData = pszMemo;
	// �e�L�X�g�F
	pcMemoData->GetForeColor( stDxlIfData.m_clrTextColor);
	// �w�i�F
	pcMemoData->GetBackColor( stDxlIfData.m_clrBackColor);
	// �t�H���g�X�^�C��
	pcMemoData->GetFont( &stDxlIfData.m_stLogFont);

	int nSize = pcMemoData->GetDxlExtData() + sizeof( DXLSAVEDATA);
	BYTE*	pabyData = new BYTE [ nSize];
	DXLSAVEDATA* lpstSaveData = ( DXLSAVEDATA*)pabyData;
	lpstSaveData->m_nDataSize = pcMemoData->GetDxlExtData();
	if( pcMemoData->GetDxlExtData())
	{
		pcMemoData->GetDxlExtData( lpstSaveData->m_abyData, pcMemoData->GetDxlExtData());
	}
	stDxlIfData.m_blZoom = blZoom;
	stDxlIfData.m_pstSaveData = lpstSaveData;

	if( _DXL_IF_DATA_VER_200 <= m_unIFDataVer)
	{
		// Ver.2 �g��
		stDxlIfData.m_lpfnGetMemoData = GetMemoData;
		// cMemoChip��n���͔̂��Ɋ댯�𔺂���
		// ��Ԃ���m_dwDxlWord����cMemoChip���������������}�V���������
		stDxlIfData.m_lpGetMemoID = ( LPVOID)&cMemoChip;
	}

	// �������̌Ăяo��
	m_lpfInitializeChip( &m_dwDxlWord, hWnd, &stDxlIfData);

	delete [] pszMemo;
	delete [] pabyData;

	return TRUE;
}

BOOL CDxlControl::UnInitializeChip( HWND hWnd)
{
	if( NULL == m_lpfUnInitializeChip)return FALSE;

	m_lpfUnInitializeChip( m_dwDxlWord, hWnd);

	return TRUE;
}

BOOL CDxlControl::CalcDefaultRect( HWND hWnd, SIZE* pSize, const CMemoData& cMemoData)
{
	if( NULL == m_lpfCalcDefaultRect)return FALSE;

	HLOCAL hLocal = NULL;
	LPSTR lpsz = NULL;
	if( IsNeedString())
	{
		CString	cStr;
		cStr = cMemoData.GetMemo();
		int nLen = cStr.GetLength() + 1;
		hLocal = LocalAlloc( LHND, nLen);
		lpsz = ( LPSTR)LocalLock( hLocal);
		lstrcpy( lpsz, cStr);
	}
	m_lpfCalcDefaultRect( m_dwDxlWord, hWnd, pSize, lpsz);
	if( IsNeedString())
	{
		LocalUnlock( hLocal);
		LocalFree( hLocal);
	}

	return TRUE;
}

// ���̊֐��̎��s�c�c����͊o����������`��̈�̃T�C�Y��n���Ă��Ȃ����Ƃł���Ǝv��
BOOL CDxlControl::DrawChip( HWND hWnd, HDC hDC, const CMemoData& cMemoData)
{
	if( NULL == m_lpfDrawChip)return FALSE;

	HLOCAL hLocal = NULL;
	LPSTR lpsz = NULL;
	if( IsNeedString())
	{
		CString	cStr;
		cStr = cMemoData.GetMemo();
		int nLen = cStr.GetLength() + 1;
		hLocal = LocalAlloc( LHND, nLen);
		lpsz = ( LPSTR)LocalLock( hLocal);
		lstrcpy( lpsz, cStr);
	}
	m_lpfDrawChip( m_dwDxlWord, hWnd, hDC, lpsz);
	if( IsNeedString())
	{
		LocalUnlock( hLocal);
		LocalFree( hLocal);
	}

	return TRUE;
}

// Click�̒Ԃ�Ⴄ���B�`(--;
DWORD CDxlControl::MouseClik( HWND hWnd, POINT* pstPoint)
{
	DWORD	dwResult = 0;
	if( NULL == m_lpfMouseClikEx)
	{
		if( NULL == m_lpfMouseClik)return FALSE;

		if( m_lpfMouseClik( m_dwDxlWord, hWnd, pstPoint))
		{
			dwResult = _TM_REDRAW;
		}
	}
	else
	{
		dwResult = m_lpfMouseClikEx( m_dwDxlWord, hWnd, pstPoint);
	}

	return dwResult;
}

BOOL CDxlControl::Resize( HWND hWnd, SIZE* pstSize)
{
	if( NULL == m_lpfResize)return FALSE;

	m_lpfResize( m_dwDxlWord, hWnd, pstSize);

	return TRUE;
}

BOOL CDxlControl::Zoom( HWND hWnd, BOOL blZoom)
{
	if( NULL == m_lpfZoom)return FALSE;

	m_lpfZoom( m_dwDxlWord, hWnd, blZoom);

	return TRUE;
}

BOOL CDxlControl::UpDate( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom)
{
	if( NULL == m_lpfUpdate)return FALSE;

	DXLIFDATA	stDxlIfData;

	// ���̍\���̂̃T�C�Y
	stDxlIfData.m_nSize		= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? sizeof( DXLIFDATA) : sizeof( DXLIFDATA100);
	// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�
	stDxlIfData.m_nVersion	= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? _DXL_IF_DATA_VER_200 : _DXL_IF_DATA_VER_100;

	const CMemoData*	pcMemoData = cMemoChip.AccessMemoData();
	// ���Ђ̃^�C�g��
	CString	cStr;
	pcMemoData->GetTitle( cStr);
	lstrcpy( stDxlIfData.m_achszMemoTitle, cStr);
	// ���Ђ̓��e
	cStr = pcMemoData->GetMemo();
	char* pszMemo = new char[ cStr.GetLength() + 1];
	lstrcpy( pszMemo, cStr);
	stDxlIfData.m_lpcszMemoData = pszMemo;
	// �e�L�X�g�F
	stDxlIfData.m_clrTextColor	= cMemoChip.GetInternalForeColor();
	// �w�i�F
	stDxlIfData.m_clrBackColor	= cMemoChip.GetInternalBackColor();
	// �t�H���g�X�^�C��
	pcMemoData->GetFont( &stDxlIfData.m_stLogFont);
	// ���Ё^�A�C�R��
	stDxlIfData.m_blZoom		= blZoom;

	int nSize = pcMemoData->GetDxlExtData() + sizeof( DXLSAVEDATA);
	BYTE*	pabyData = new BYTE [ nSize];
	DXLSAVEDATA* lpstSaveData = ( DXLSAVEDATA*)pabyData;
	lpstSaveData->m_nDataSize = pcMemoData->GetDxlExtData();
	if( pcMemoData->GetDxlExtData())
	{
		pcMemoData->GetDxlExtData( lpstSaveData->m_abyData, pcMemoData->GetDxlExtData());
	}
	stDxlIfData.m_pstSaveData = lpstSaveData;

	if( _DXL_IF_DATA_VER_200 <= m_unIFDataVer)
	{
		// Ver.2 �g��
		stDxlIfData.m_lpfnGetMemoData = GetMemoData;
		stDxlIfData.m_lpGetMemoID = ( LPVOID)&cMemoChip;
	}

	//
	BOOL blResult = m_lpfUpdate( m_dwDxlWord, hWnd, &stDxlIfData);

	delete [] pszMemo;
	delete [] pabyData;

	return blResult;
}

BOOL CDxlControl::GetMemoString( CString& cStr, const CMemoData& cMemoData)
{
	if( NULL == m_lpfGetMemoString)return FALSE;

	LPSTR lpsz = NULL;
	if( IsNeedString())
	{
		CString	cStrBase;
		cStrBase = cMemoData.GetMemo();
		int nLen = cStrBase.GetLength() + 1;
		lpsz = new char[ nLen];
		lstrcpy( lpsz, cStrBase);
	}

	int nLength = m_lpfGetMemoString( m_dwDxlWord, NULL, 0, lpsz);

	if( 0 >= nLength)
	{
		cStr = cMemoData.GetMemo();
	}
	else
	{
		char*	pszData = new char [nLength + 1];
		m_lpfGetMemoString( m_dwDxlWord, pszData, nLength + 1, lpsz);
		cStr = pszData;
		delete [] pszData;
	}

	if( IsNeedString())
	{
		delete [] lpsz;
	}

	return TRUE;
}

HMENU CDxlControl::GetDrawExMenu( void)
{
	if( NULL == m_lpfGetDrawExMenu)return NULL;
	return m_lpfGetDrawExMenu( m_dwDxlWord);
}

BOOL CDxlControl::DoDrawExMenu( HWND hWnd, UINT unMenuID)
{
	CSingleLock	cSlk( &sm_cSmpDoMenu);

	if( cSlk.Lock( 0))
	{
		if( NULL == m_lpfDoDrawExMenu)return NULL;
		return m_lpfDoDrawExMenu( m_dwDxlWord, hWnd, unMenuID);
	}
	return 0;
}

BOOL CDxlControl::GetSaveData( HWND hWnd, CMemoData& cMemoData)
{
	if( NULL == m_lpfGetSaveData)return NULL;
	// ���Ђ̓��e
	HGLOBAL	hGlobal = GlobalAlloc( GHND, sizeof( DXLSAVEDATA));

	BOOL blResult = m_lpfGetSaveData( m_dwDxlWord, hWnd, &hGlobal);

	if( blResult)
	{
		DXLSAVEDATA* lpstSaveData = ( DXLSAVEDATA*)GlobalLock( hGlobal);
		if( lpstSaveData)
		{
			cMemoData.SetDxlExtData( lpstSaveData->m_abyData, lpstSaveData->m_nDataSize);
		}
		GlobalUnlock( hGlobal);
	}
	GlobalFree( hGlobal);

	return blResult;
}

DWORD CDxlControl::CheckTimer()
{
	if( !IsSupportTimer())return FALSE;

	DWORD dwResult = 0;
	if( 0 <= m_nTimerSpan)
	{
		if( 0 >= m_nTimerSpan)
		{
			dwResult = DoTimer();
			m_nTimerSpan = GetTimerSpan();
		}
		else
		{
			m_nTimerSpan--;
		}
	}
	else
	{
		m_nTimerSpan = GetTimerSpan();
	}

	return dwResult;
}

BOOL CDxlControl::Active( HWND hWnd, BOOL blActive)
{
	if( NULL == m_lpfActive)return FALSE;

	m_lpfActive( m_dwDxlWord, hWnd, blActive);

	return TRUE;
}
