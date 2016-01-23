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

	// 初期化処理
	m_lpfInitializeChip = NULL;
	// 終了処理
	m_lpfUnInitializeChip = NULL;
	// 大きさ再計算
	m_lpfCalcDefaultRect = NULL;
	// 表示
	m_lpfDrawChip = NULL;
	// マウスクリック
	m_lpfMouseClik = NULL;
	// サイズ変更
	m_lpfResize = NULL;
	// 小片化／アイコン化
	m_lpfZoom = NULL;
	// データ更新：編集により発生
	m_lpfUpdate = NULL;
	// 描画する文字列：_DT_EXCHANGE指定時のみ処理
	m_lpfGetMemoString = NULL;
	// ポップアップメニュー
	m_lpfGetDrawExMenu = NULL;
	// ポップアップメニュー
	m_lpfDoDrawExMenu = NULL;
	// データ保存時の編集
	m_lpfGetSaveData = NULL;
	// タイマ周期取得
	m_lpfnGetTimerSpan = NULL;
	// タイマ処理
	m_lpfnDoTimer = NULL;
	// タイマカウンタ
	m_nTimerSpan = 0;

	// マウスクリック：拡張
	m_lpfMouseClikEx = NULL;
	// 小片のアクティブ：新規
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
	
	// 初期化処理
	m_lpfInitializeChip = ( INITIALIZECHIP)GetProcAddress( m_hInstance, "InitializeChip");
	// 終了処理
	m_lpfUnInitializeChip = ( UNINITIALIZECHIP)GetProcAddress( m_hInstance, "UnInitializeChip");
	// 大きさ再計算
	m_lpfCalcDefaultRect = ( CALCDEFAULTRECT)GetProcAddress( m_hInstance, "CalcDefaultRect");
	// 表示
	m_lpfDrawChip = ( DRAWCHIP)GetProcAddress( m_hInstance, "DrawChip");
	// マウスクリック
	m_lpfMouseClik = ( MOUSECLIK)GetProcAddress( m_hInstance, "MouseClik");
	// サイズ変更
	m_lpfResize = ( RESIZE)GetProcAddress( m_hInstance, "Resize");
	// 小片化／アイコン化
	m_lpfZoom = ( ZOOM)GetProcAddress( m_hInstance, "Zoom");
	// データ更新：編集により発生
	m_lpfUpdate = ( UPDATE)GetProcAddress( m_hInstance, "Update");
	// 描画する文字列：_DT_EXCHANGE指定時のみ処理
	m_lpfGetMemoString = ( GETMEMOSTRING)GetProcAddress( m_hInstance, "GetMemoString");
	// ポップアップメニュー
	m_lpfGetDrawExMenu = ( GETDRAWEXMENU)GetProcAddress( m_hInstance, "GetDrawExMenu");
	// ポップアップメニュー
	m_lpfDoDrawExMenu = ( DODRAWEXMENU)GetProcAddress( m_hInstance, "DoDrawExMenu");
	// データ保存時の編集
	m_lpfGetSaveData = ( GETSAVEDATA)GetProcAddress( m_hInstance, "GetSaveData");
	// タイマ周期取得
	m_lpfnGetTimerSpan = ( GETTIMERSPAN)GetProcAddress( m_hInstance, "GetTimerSpan");
	// タイマ処理
	m_lpfnDoTimer = ( DOTIMER)GetProcAddress( m_hInstance, "DoTimer");

	// マウスクリック：拡張
	m_lpfMouseClikEx = ( MOUSECLIKEX)GetProcAddress( m_hInstance, "MouseClickEx");
	// 小片のアクティブ：新規
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
		// 初期化処理
		m_lpfInitializeChip = NULL;
		// 終了処理
		m_lpfUnInitializeChip = NULL;
		// 大きさ再計算
		m_lpfCalcDefaultRect = NULL;
		// 表示
		m_lpfDrawChip = NULL;
		// マウスクリック
		m_lpfMouseClik = NULL;
		// サイズ変更
		m_lpfResize = NULL;
		// 小片化／アイコン化
		m_lpfZoom = NULL;
		// データ更新：編集により発生
		m_lpfUpdate = NULL;
		// 描画する文字列：_DT_EXCHANGE指定時のみ処理
		m_lpfGetMemoString = NULL;
		// ポップアップメニュー
		m_lpfGetDrawExMenu = NULL;
		// ポップアップメニュー
		m_lpfDoDrawExMenu = NULL;
		// データ保存時の編集
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

	// 初期化処理
	m_lpfInitializeChip = cStrSrc.m_lpfInitializeChip;
	// 終了処理
	m_lpfUnInitializeChip = cStrSrc.m_lpfUnInitializeChip;
	// 大きさ再計算
	m_lpfCalcDefaultRect = cStrSrc.m_lpfCalcDefaultRect;
	// 表示
	m_lpfDrawChip = cStrSrc.m_lpfDrawChip;
	// マウスクリック
	m_lpfMouseClik = cStrSrc.m_lpfMouseClik;
	// サイズ変更
	m_lpfResize = cStrSrc.m_lpfResize;
	// 小片化／アイコン化
	m_lpfZoom = cStrSrc.m_lpfZoom;
	// データ更新：編集により発生
	m_lpfUpdate = cStrSrc.m_lpfUpdate;
	// 描画する文字列：_DT_EXCHANGE指定時のみ処理
	m_lpfGetMemoString = cStrSrc.m_lpfGetMemoString;
	// ポップアップメニュー
	m_lpfGetDrawExMenu = cStrSrc.m_lpfGetDrawExMenu;
	// ポップアップメニュー
	m_lpfDoDrawExMenu = cStrSrc.m_lpfDoDrawExMenu;
	// データ保存時の編集
	m_lpfGetSaveData = cStrSrc.m_lpfGetSaveData;
	// タイマ周期取得
	m_lpfnGetTimerSpan = cStrSrc.m_lpfnGetTimerSpan;
	// タイマ処理
	m_lpfnDoTimer = cStrSrc.m_lpfnDoTimer;

	return *this;
}

BOOL CDxlControl::InitializeChip( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom)
{
	if( NULL == m_lpfInitializeChip)return FALSE;

	DXLIFDATA	stDxlIfData;

	// この構造体のサイズ
	stDxlIfData.m_nSize		= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? sizeof( DXLIFDATA) : sizeof( DXLIFDATA100);
	// この構造体のバージョン
	stDxlIfData.m_nVersion	= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? _DXL_IF_DATA_VER_200 : _DXL_IF_DATA_VER_100;

	const CMemoData*	pcMemoData = cMemoChip.AccessMemoData();
	// 小片のタイトル
	CString	cStr;
	pcMemoData->GetTitle( cStr);
	lstrcpy( stDxlIfData.m_achszMemoTitle, cStr);
	// 小片の内容
	char* pszMemo = new char[ pcMemoData->GetMemoLength() + 1];
	lstrcpy( pszMemo, pcMemoData->GetMemo());
	stDxlIfData.m_lpcszMemoData = pszMemo;
	// テキスト色
	pcMemoData->GetForeColor( stDxlIfData.m_clrTextColor);
	// 背景色
	pcMemoData->GetBackColor( stDxlIfData.m_clrBackColor);
	// フォントスタイル
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
		// Ver.2 拡張
		stDxlIfData.m_lpfnGetMemoData = GetMemoData;
		// cMemoChipを渡すのは非常に危険を伴うね
		// 手間だがm_dwDxlWordからcMemoChipを検索した方がマシかもしれん
		stDxlIfData.m_lpGetMemoID = ( LPVOID)&cMemoChip;
	}

	// 初期化の呼び出し
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

// この関数の失敗……それは覚え書側から描画領域のサイズを渡していないことであると思う
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

// Clickの綴り違うしィ～(--;
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

	// この構造体のサイズ
	stDxlIfData.m_nSize		= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? sizeof( DXLIFDATA) : sizeof( DXLIFDATA100);
	// この構造体のバージョン：_IF_DATA_VERを指定の事
	stDxlIfData.m_nVersion	= ( _DXL_IF_DATA_VER_200 <= m_unIFDataVer) ? _DXL_IF_DATA_VER_200 : _DXL_IF_DATA_VER_100;

	const CMemoData*	pcMemoData = cMemoChip.AccessMemoData();
	// 小片のタイトル
	CString	cStr;
	pcMemoData->GetTitle( cStr);
	lstrcpy( stDxlIfData.m_achszMemoTitle, cStr);
	// 小片の内容
	cStr = pcMemoData->GetMemo();
	char* pszMemo = new char[ cStr.GetLength() + 1];
	lstrcpy( pszMemo, cStr);
	stDxlIfData.m_lpcszMemoData = pszMemo;
	// テキスト色
	stDxlIfData.m_clrTextColor	= cMemoChip.GetInternalForeColor();
	// 背景色
	stDxlIfData.m_clrBackColor	= cMemoChip.GetInternalBackColor();
	// フォントスタイル
	pcMemoData->GetFont( &stDxlIfData.m_stLogFont);
	// 小片／アイコン
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
		// Ver.2 拡張
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
	// 小片の内容
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
