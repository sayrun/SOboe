#include "stdafx.h"
#include "MainFrm.h"
#include "resource.h"
#include "DrawExLib.h"

CDrawExLib::CDrawExLib() : CObject()
{
	m_unDxlDataVersion	= 0;	// 処理可能な最新バージョン
	m_pcStrMenuString	= NULL;	// デフォルトの処理名称
	m_unDXLID			= 0;	// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	m_unDrawType		= 0;	// dxlの処理内容
	m_unStyle			= 0;	// dxlのスタイル

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

BOOL CDrawExLib::LoadDxl( void)
{
	if( NULL == m_pcStrDxlPath)return FALSE;
	if( m_pcStrDxlPath->IsEmpty())return FALSE;
	if( m_hInstance)
	{
		FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}

	m_hInstance = LoadLibrary( *m_pcStrDxlPath);
	if( NULL == m_hInstance)return FALSE;

	// 機能確認
	DXLSPECIFICATION	stDxlSpec;

	DRAWEXSPECIFICATION	lpfDrawExSpecifcation;
	lpfDrawExSpecifcation = ( DRAWEXSPECIFICATION)GetProcAddress( m_hInstance, "DrawExSpecification");
	if( lpfDrawExSpecifcation)
	{
		lpfDrawExSpecifcation( &stDxlSpec);
		m_unDxlDataVersion	= stDxlSpec.unUpperStructVersion;	// 処理可能な最新バージョン
		m_unDXLID			= stDxlSpec.unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
		m_unDrawType		= stDxlSpec.unDrawType;				// dxlの処理内容
		m_unStyle			= stDxlSpec.unStyle;				// dxlスタイル
	}

	return TRUE;
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
	m_unStyle			= 0;	// dxlスタイル

	return TRUE;
}

BOOL CDrawExLib::DoEditHelper( HWND hWnd, EDITDATA* pstEditData) const
{
	// 特殊編集：入力ヘルパー
	EDITHELPER lpfEditHelper;
	lpfEditHelper = ( EDITHELPER)GetProcAddress( m_hInstance, "EditHelper");
	if( lpfEditHelper)
	{
		return lpfEditHelper( hWnd, pstEditData);
	}
	return IDCANCEL;
}

BOOL CDrawExLib::About( HWND hWnd) const
{
	DRAWEXABOUT lpfAbout;
	lpfAbout = ( DRAWEXABOUT)GetProcAddress( m_hInstance, "DrawExAbout");
	if( lpfAbout)lpfAbout( hWnd);
	return TRUE;
}

BOOL CDrawExLib::ExchangeSendString( EDITDATA* pstEditData) const
{
	int nLength = 0;
	EXCHANGESENDSTRINGEX	lpfExchangeSendStringEx;
	lpfExchangeSendStringEx = ( EXCHANGESENDSTRINGEX)GetProcAddress( m_hInstance, "ExchangeSendStringEx");
	if( NULL != lpfExchangeSendStringEx)
	{
		nLength = lpfExchangeSendStringEx( pstEditData);
	}
	else
	{
		EXCHANGESENDSTRING	lpfExchangeSendString;
		lpfExchangeSendString = ( EXCHANGESENDSTRING)GetProcAddress( m_hInstance, "ExchangeSendString");
		if( NULL != lpfExchangeSendString)
		{
			nLength = lpfExchangeSendString( &pstEditData->m_hEdit);
		}
	}

	return ( ( 0 < nLength) ? TRUE : FALSE);
}
