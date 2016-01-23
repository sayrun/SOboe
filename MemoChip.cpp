// MemoChip.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <process.h>
#include "soboe.h"
#include "MainFrm.h"
#include "MemoChip.h"
#include "MemoEditSheet.h"
#include "MemoSendSheet.h"
#include "PassWordCheckDialog.h"
#include <mbctype.h>
#include "SendWorkerDialog.h"
#include "ResultCheckDialog.h"
#include "CelControl.h"
#include "SenderInfoDialog.h"

#define COMPILE_MULTIMON_STUBS
#include <MULTIMON.H>
#include "DirectEdit.h"
#ifdef _ALPHABLEND
#include "AlphaBlendDialog.h"
#endif

#include "ResizableMemoEditDialog.h"
#include ".\memochip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct tagRESULTPARAM
{
	INNERRESULTDATA	stInnerResultData;
	CEntryData		cEntry;
	CNetExLib*		pcNetExLib;
	HWND			hWnd;
	HANDLE			hCancelEvent;
}RESULTPARAM;

void ResultSendThread( void* lpvData)
{
	RESULTPARAM*	pstResultParam = ( RESULTPARAM*)lpvData;

	int nResult =  pstResultParam->pcNetExLib->SendResultData( &pstResultParam->cEntry, &pstResultParam->stInnerResultData, pstResultParam->hCancelEvent);

	delete pstResultParam;

	_endthread();
}


/////////////////////////////////////////////////////////////////////////////
// CMemoChip
IMPLEMENT_SERIAL( CMemoChip, CCmdTarget/*CWnd*/, VERSIONABLE_SCHEMA | _SCHEMA_ID)

// Windows 2000でサポートされているAlphaBlendAPI初期化
CMemoChip::LPSETLAYEREDWINDOWATTRIBUTES CMemoChip::m_lpfnSetLayeredWindowAttributes = CMemoChip::GetSetLayeredWindowAttributesProcAddress();
CMemoChip::LPSETLAYEREDWINDOWATTRIBUTES CMemoChip::GetSetLayeredWindowAttributesProcAddress( void)
{
#ifdef _ALPHABLEND
#pragma message( "透過式小片が利用できます。")
	LPSETLAYEREDWINDOWATTRIBUTES	lpfnSetLayeredWindowAttributes = NULL;
	HMODULE hModule = GetModuleHandle( TEXT( "user32.dll"));
	if( NULL != hModule)
	{
		lpfnSetLayeredWindowAttributes = ( LPSETLAYEREDWINDOWATTRIBUTES)GetProcAddress( hModule, TEXT( "SetLayeredWindowAttributes"));
	}

	return lpfnSetLayeredWindowAttributes;
#else
#pragma message( "透過式小片は利用できません。")
	// 今のところ正式版ではね。
	return NULL;
#endif
}

CMemoChip::CMemoChip()
{
	// 移動に関するフラグ
	m_blFrameDraw = true;
	m_blMouseClickStart = FALSE;
	m_blNowMove = false;
	m_blNowZMove = false;


	m_pcDxlControl = NULL;
	m_blBeforeVisible = true;
	m_blBlink = false;
	m_blDelOnQuit = false;
	m_blSmartClose = false;

	m_pcRgnSeeThrough = NULL;

	m_blOnceTop = false;

	m_pcToolTip = NULL;

	m_cStrUndo.Empty();

	m_pcEntryData = NULL;
	m_blPack = FALSE;
	m_blCheckOpen = FALSE;
	m_pcStrResultID = NULL;
	m_pstResults = NULL;

	m_blReceive = FALSE;

	m_pcReceiveTime = NULL;

	m_blSysResize = FALSE;

	m_hParentWnd = NULL;
}

CMemoChip::~CMemoChip()
{
	if( m_pcDxlControl)
	{
		delete m_pcDxlControl;
		m_pcDxlControl = NULL;
	}
	if( m_pcRgnSeeThrough)
	{
		delete m_pcRgnSeeThrough;
		m_pcRgnSeeThrough = NULL;
	}
	if( m_pcEntryData)
	{
		delete m_pcEntryData;
		m_pcEntryData = NULL;
	}
	if( m_pstResults)
	{
		delete m_pstResults;
		m_pstResults = NULL;
	}
	if( m_pcStrResultID)
	{
		delete m_pcStrResultID;
		m_pcStrResultID = NULL;
	}
	if( m_pcReceiveTime)
	{
		delete m_pcReceiveTime;
		m_pcReceiveTime = NULL;
	}
}

const CMemoChip& CMemoChip::operator=( const CMemoChip& cMemoChip)
{
	m_cMemoData		= cMemoChip.m_cMemoData;

	return *this;
}

void CMemoChip::Serialize( CArchive& ar )
{
	try
	{
		// なぜにここにデータ読み出し機構があるかといえば、互換性のためです
		// 旧バージョンはこのクラスも永続性のデータを保持していたのです。
		// 面倒なので、永続性の全てはCMemoDataへ移行しました。
		if( ar.IsStoring())
		{
			if( ::IsWindow( GetSafeHwnd()))
			{
				if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
				{
					if( m_pcDxlControl->IsSupportNativeData())
					{
						m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
					}
				}
				// 最後のWindowサイズを記憶しておく
				CRect	cRect;
				GetWindowRect( cRect);
				if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
				{
					m_cMemoData.SetNormalRect( cRect);
				}
				else
				{
					if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
					{
						m_cMemoData.SetTitleRect( cRect);
					}
					else
					{
						if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
						{
							m_cMemoData.SetIconRect( cRect);
						}
					}
				}
			}
			m_cMemoData.Store( ar);
		}
		else
		{
			UINT unVersion = ar.GetObjectSchema();
			m_cMemoData.Restore( ar, unVersion);
		}
	}
	catch( CFileException*)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException*)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException*)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}

BEGIN_MESSAGE_MAP(CMemoChip, CWnd)
	//{{AFX_MSG_MAP(CMemoChip)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(IDM_TOICON, OnToIcon)
	ON_COMMAND(IDM_TONORMAL, OnToNormal)
	ON_COMMAND(IDM_TOTITLE, OnToTitle)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_REEDIT, OnReedit)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_PASTE_APPEND, OnPasteAppend)
	ON_COMMAND(IDM_PASTE_OVERWRITE, OnPasteOverwrite)
	ON_COMMAND(IDM_PASTE_CREATE, OnPasteCreate)
	ON_COMMAND(IDM_PASTE_LINK, OnPasteLink)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_COMMAND(IDM_NEWMEMO, OnNewmemo)
	ON_COMMAND(IDM_COPY, OnCopy)
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDM_LINKEXEC, OnLinkexec)
	ON_COMMAND(IDM_DELONQUIT, OnDelonquit)
	ON_COMMAND(IDM_MEMOEXPORT, OnMemoexport)
	ON_COMMAND(IDM_TEXTSAVE, OnTextsave)
	ON_COMMAND(IDM_COPY_MEMODATA, OnCopyMemodata)
	ON_COMMAND(IDM_POSLOCK, OnPoslock)
	ON_COMMAND(IDM_SEETHROUGH, OnSeetrough)
	ON_COMMAND(IDM_HIDE, OnHide)
	ON_COMMAND(IDM_UNDO, OnUndo)
	ON_COMMAND(IDM_SEND, OnSend)
	ON_COMMAND(IDM_SENDER, OnSender)
	ON_COMMAND(IDM_REPLY, OnReply)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( _CEL_MENUTOP, _CEL_MENUBOTTOM, OnCelMenu)
	ON_COMMAND_RANGE( _DXL_MENUTOP, _DXL_MENUBOTTOM, OnDxlMenu)
	ON_COMMAND_RANGE( IDM_ABLEND_075, IDM_ABLEND_FREE, OnAlphaBlend)
	ON_WM_CTLCOLOR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

LPCTSTR AFXAPI RegisterMemoChipClass( UINT nClassStyle)
{
	LPCTSTR lpcszClassName = "OboeChipWndClass";
	HINSTANCE hInst = AfxGetInstanceHandle();

	// see if the class already exists
	WNDCLASS stWndClass;
	if( ::GetClassInfo( hInst, lpcszClassName, &stWndClass))
	{
		ASSERT( stWndClass.style == nClassStyle);
		return lpcszClassName;
	}

	// otherwise we need to register a new class
	stWndClass.style = nClassStyle;
	stWndClass.lpfnWndProc = DefWindowProc;
	stWndClass.cbWndExtra = 0;
	stWndClass.cbClsExtra = 0;
	stWndClass.hInstance = hInst;
	stWndClass.hIcon = NULL;
	stWndClass.hCursor = LoadCursor( NULL, IDC_ARROW);
	stWndClass.hbrBackground = NULL;
	stWndClass.lpszMenuName = NULL;
	stWndClass.lpszClassName = lpcszClassName;
	if (!AfxRegisterClass(&stWndClass))AfxThrowResourceException();

	// return thread-local pointer
	return lpcszClassName;
}

CString CMemoChip::m_cStrClassName;
/////////////////////////////////////////////////////////////////////////////
// CMemoChip メッセージ ハンドラ

BOOL CMemoChip::Create( UINT unID, HWND hParentWnd, BOOL blHide, HICON hIcon, CToolTipCtrl* pcToolTip)
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( m_cStrClassName.IsEmpty())
	{
		m_cStrClassName = RegisterMemoChipClass( CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW);
	}

	m_hParentWnd = hParentWnd;

	CRect	cRect;
	// 作成するWindowサイズ
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		cRect = m_cMemoData.GetNormalRect();
	}
	else
	{
		if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
		{
			cRect = m_cMemoData.GetTitleRect();
		}
		else
		{
			if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
			{
				cRect = m_cMemoData.GetIconRect();
			}
		}
	}
	// 領域の正常化！
	cRect = ClipRectToMonitor( cRect);

	if( blHide)
	{
		m_blShowBeforeOndeHide = m_cMemoData.IsShow();
	}

	m_pcToolTip = pcToolTip;

	// 設定の色をデフォルトの色として取得します。
	m_cMemoData.GetForeColor( m_clrInternalForeColor);
	m_cMemoData.GetBackColor( m_clrInternalBackColor);

	m_unID = unID;
	CString	cStrCaption;
	cStrCaption.Format( "OboeChip#%d", unID);
	//親を持たない理由は、みんなが前に来るのを防ぐため！
	BOOL blRe = CWnd::CreateEx( WS_EX_TOOLWINDOW, m_cStrClassName, cStrCaption, ( !blHide && m_cMemoData.IsShow()) ? ( WS_BORDER | WS_POPUP | WS_VISIBLE) : ( WS_BORDER | WS_POPUP), cRect.left, cRect.top, cRect.Width(), cRect.Height(), NULL, NULL);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	return blRe;
}

BOOL CMemoChip::CalcDefaultRect( BOOL blCentering)
{
	if( m_cMemoData.IsPosLock())return FALSE;

	CString	cStrCalBase;

	bool	blDxlCalc = false;

	if( !IsPack())
	{
		// Dxlで計算？
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsOwnerDraw())
			{
				blDxlCalc = true;
			}
			else
			{
				if( m_pcDxlControl->IsDataExchange())
				{
					m_pcDxlControl->GetMemoString( cStrCalBase, m_cMemoData);
				}
			}
		}
	}

	BOOL	blNoRect = m_cMemoData.GetNormalRect().IsRectNull();
	CSize	cSize;
	CRect	cRect;
	if( blDxlCalc)
	{
		SIZE	stSize;
		m_pcDxlControl->CalcDefaultRect( GetSafeHwnd(), &stSize, m_cMemoData);

		int nXMin = _ICON_WIDTH + ::GetSystemMetrics( SM_CXBORDER) * 2;
		int nYMin = _ICON_HEIGHT + ::GetSystemMetrics( SM_CYBORDER) * 2;
		stSize.cx = max( nXMin, stSize.cx);
		int nXMax = ::GetSystemMetrics( SM_CXSCREEN);
		stSize.cx = min( nXMax, stSize.cx);

		stSize.cy = max( nYMin, stSize.cy);

		cRect = CRect( 0, 0, stSize.cx, stSize.cy);
		CalcWindowRect( cRect);
	}
	else
	{
		if( cStrCalBase.IsEmpty())
		{
			if( IsPack())
			{
				CString	cStr;
				m_cMemoData.GetTitle( cStr);
				cStrCalBase.Format( IDS_PACKMSG, cStr);
			}
			else
			{
				cStrCalBase = m_cMemoData.GetMemo();
			}
		}
		if( m_cMemoData.IsSeeThrough())
		{
			SetWindowRgn( NULL, TRUE);
		}

		CDC*	pcDC;
		CPoint	cPoint = m_cMemoData.GetNormalRect().TopLeft();

		cRect = m_cMemoData.GetNormalRect();

		CFont	cFont;
		CFont*	pcFontOld;
		LOGFONT	stLogFont;
		m_cMemoData.GetFont( &stLogFont);
		cFont.CreateFontIndirect( &stLogFont);

		pcDC = GetDC();
		pcFontOld = pcDC->SelectObject( &cFont);
		cSize = pcDC->GetTextExtent( cStrCalBase);
		// 将来的にはこちらへ変更
		//int nWidth = ( _ICON_WIDTH > m_nDefaultWidth) ? ( GetSystemMetrics( SM_CXSCREEN) / 2) : m_nDefaultWidth;
		int nWidth = ::GetSystemMetrics( SM_CXSCREEN) / 2;
		cRect.right		= cRect.left + min( cSize.cx, nWidth);
		cRect.bottom	= cRect.top + cSize.cy;
		pcDC->DrawText( cStrCalBase, cRect, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
		pcDC->SelectObject( pcFontOld);
		ReleaseDC( pcDC);

		// 表示する際の内部のスペースというかマージン
		cRect.bottom	+= ( ::GetSystemMetrics( SM_CYFRAME)  * 2);
		cRect.right		+= ( ::GetSystemMetrics( SM_CXFRAME)  * 2);
		// 最低値に調整
		cRect.right = cRect.left + max( _ICON_WIDTH, cRect.Width());
		cRect.bottom = cRect.top + max( _ICON_HEIGHT, cRect.Height());

		// アイコン化時でもシースルーでの再計算は必要だからね
		if( m_cMemoData.IsSeeThrough())
		{
			CSize	cSize = cRect.Size();
			cRect.top = 0;
			cRect.bottom = cSize.cy;
			cRect.left = 0;
			cRect.right = cSize.cx;

			CDC*	pcDC = GetDC();
			if( m_pcRgnSeeThrough)
			{
				delete m_pcRgnSeeThrough;
				m_pcRgnSeeThrough = NULL;
			}
			m_pcRgnSeeThrough = CreateSeeThroughRgn( pcDC, cRect);
			ReleaseDC( pcDC);
		}
		// 指定通りのクライアント領域を得るための計算
		CalcWindowRect( cRect);
	}

	if( blNoRect || blCentering)
	{
		// 領域が無い場合や、指定されていれば、算出後にセンタリングする
		int cxScreen = ::GetSystemMetrics( SM_CXSCREEN);
		int cyScreen = ::GetSystemMetrics( SM_CYSCREEN);
		cSize.cx = min( cRect.Width(), cxScreen);
		cSize.cy = min( cRect.Height(), cyScreen);
		cRect.left = ( cxScreen - cSize.cx) / 2;
		cRect.top = ( cyScreen - cSize.cy) / 2;
		cRect.right = cRect.left + cSize.cx;
		cRect.bottom = cRect.top + cSize.cy;
	}
	else
	{
		// CalcWindowRectで位置がずれるのを戻します
		cSize = cRect.Size();
		cRect.left = m_cMemoData.GetNormalRect().left;
		cRect.top = m_cMemoData.GetNormalRect().top;
		cRect.right = cRect.left + cSize.cx;
		cRect.bottom = cRect.top + cSize.cy;
	}

	// 領域の正常化！
	cRect = ClipRectToMonitor( cRect);

	CRect	cOldRect = m_cMemoData.SetNormalRect( cRect);
	// 通常表示状態であれば、そのままリサイズしてしまう
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		MoveWindow( m_cMemoData.GetNormalRect());
		// 移動しない場合は強制的に再描画させる
		if( cOldRect == m_cMemoData.GetNormalRect())
		{
			InvalidateRect( NULL);
		}
		if( m_cMemoData.IsSeeThrough())
		{
			HRGN	hRgn;
			hRgn = CreateRectRgn( 0, 0, 0, 0);
			CombineRgn( hRgn, hRgn, *m_pcRgnSeeThrough, RGN_OR);
			SetWindowRgn( hRgn, TRUE);
		}
	}

	return TRUE;
}

BOOL CMemoChip::CalcTitleRect( CRect& cRectNew)
{
	// タイトルのサイズを計算します。
	CSize	cSize;
	CString	cStr;
	m_cMemoData.GetTitle( cStr);
	CDC*	pcDC;
	pcDC = GetDC();

	CFont	cFont;
	LOGFONT	stLogFont;
	m_cMemoData.GetFont( &stLogFont);
	stLogFont.lfHeight = -14;	// 高さを、14pixelに固定
	stLogFont.lfWidth = 0;
	cFont.CreateFontIndirect( &stLogFont);

	CFont* pcOldFont = pcDC->SelectObject( &cFont);
	/*
	cSize = pcDC->GetTextExtent( cStr);
	*/
	CRect	cRect;
	SystemParametersInfo( SPI_GETWORKAREA, 0, &cRect, 0);
	pcDC->DrawText( cStr, cRect, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
	cSize = cRect.Size();

	pcDC->SelectObject( pcOldFont);

	ReleaseDC( pcDC);

	cRectNew = m_cMemoData.GetTitleRect();
	cRectNew.right = cRectNew.left + cSize.cx + _ICON_WIDTH + ( ::GetSystemMetrics( SM_CXFRAME) * 2);
	cRectNew.bottom = cRectNew.top + _ICON_HEIGHT + ::GetSystemMetrics( SM_CYBORDER) * 2;

	return TRUE;
}

void CMemoChip::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CRect	cRect;
	GetClientRect( cRect);

	COLORREF clrFore = m_clrInternalForeColor;
	COLORREF clrBack = m_clrInternalBackColor;

	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		// 通常状態の描画処理
		CString	cStrText;
		// 描画拡張の調査
		bool blDxlDraw = false;
		if( !IsPack())
		{
			if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
			{
				if( m_pcDxlControl->IsOwnerDraw())
				{
					// 描画拡張型で、描画はdxlが行う
					blDxlDraw = true;
				}
				else
				{
					if( m_pcDxlControl->IsDataExchange())
					{
						// 文字変換型で、描画は本体が行う
						m_pcDxlControl->GetMemoString( cStrText, m_cMemoData);
					}
				}
			}
		}

		if( blDxlDraw)
		{
			// 描画の実行
			m_pcDxlControl->DrawChip( GetSafeHwnd(), dc.m_hDC, m_cMemoData);
			if( m_pcDxlControl->IsSupportResize())
			{
				DrawResizeArea( &dc, ( this == GetActiveWindow()));
			}
		}
		else
		{
			if( m_cMemoData.IsSeeThrough())
			{
				dc.FillSolidRect( cRect, m_clrInternalForeColor);
			}
			else
			{
				CBrush	cBrush( clrBack);
				dc.FillRect( cRect, &cBrush);
				DrawResizeArea( &dc, ( this == GetActiveWindow()));

				if( IsPack())
				{
					CString	cStr;
					m_cMemoData.GetTitle( cStr);
					cStrText.Format( IDS_PACKMSG, cStr);
				}
				else
				{
					if( cStrText.IsEmpty())
					{
						cStrText = m_cMemoData.GetMemo();
					}
				}
				// 通常表示形態での描画
				int nID = dc.SaveDC();
				if( 0 != nID)
				{
					CFont	cFont;
					LOGFONT	stLogFont;
					m_cMemoData.GetFont( &stLogFont);
					cFont.CreateFontIndirect( &stLogFont);
					dc.SelectObject( &cFont);

					dc.SetBkColor( clrBack);
					dc.SetBkMode( TRANSPARENT);
					dc.SetTextColor( clrFore);

					// 表示用マージンの設定
					cRect.top	+= ::GetSystemMetrics( SM_CYFRAME);
					cRect.left	+= ::GetSystemMetrics( SM_CXFRAME);

					dc.DrawText( cStrText, cRect, DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX | DT_WORDBREAK);

					dc.RestoreDC( nID);
				}
			}
		}
	}
	else
	{
		if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
		{
			// アイコンの描画処理
			int nID = dc.SaveDC();
			if( 0 != nID)
			{
				dc.FillSolidRect( cRect, clrBack);

				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);
				const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
				if( pcIconImgs)
				{
					dc.SetBkColor( clrBack);
					dc.SetTextColor( clrFore);
					pcIconImgs->Draw( m_cMemoData.GetIconIndex(), dc, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
				}
				else
				{
					CDC	cMemDC;
					if( cMemDC.CreateCompatibleDC( &dc))
					{
						CBitmap		cBitmap;
						if( cBitmap.LoadBitmap( IDB_ICONIMG))
						{
							CBitmap*	pcOldBitmap;
							pcOldBitmap = cMemDC.SelectObject( &cBitmap);
							dc.SetBkColor( clrBack);
							dc.SetTextColor( clrFore);
							dc.BitBlt( 0, 0, _ICON_WIDTH, _ICON_HEIGHT, &cMemDC, _ICON_WIDTH * m_cMemoData.GetIconIndex(), 0, SRCCOPY);
							cMemDC.SelectObject( pcOldBitmap);
							cBitmap.DeleteObject();
						}
						cMemDC.DeleteDC();
					}
				}
				if( IsDelOnQuit())
				{
					CPen	cPen( PS_SOLID, 0, RGB( 0xFF, 0x00, 0x00));
					CPen*	pcPenOld;

					pcPenOld = dc.SelectObject( &cPen);
					dc.MoveTo( cRect.right, cRect.top);
					dc.LineTo( cRect.left, cRect.bottom);
					dc.SelectObject( pcPenOld);
				}

				dc.RestoreDC( nID);
			}
		}
		else
		{
			if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
			{
				int nID = dc.SaveDC();
				if( 0 != nID)
				{
					dc.FillSolidRect( cRect, clrBack);

					CFont	cFont;
					LOGFONT	stLogFont;
					m_cMemoData.GetFont( &stLogFont);
					stLogFont.lfHeight = -14;
					stLogFont.lfWidth = 0;
					cFont.CreateFontIndirect( &stLogFont);
					dc.SelectObject( &cFont);

					dc.SetBkColor( clrBack);
					dc.SetBkMode( TRANSPARENT);
					dc.SetTextColor( clrFore);

					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);
					const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
					if( pcIconImgs)
					{
						dc.SetBkColor( clrBack);
						dc.SetTextColor( clrFore);
						pcIconImgs->Draw( m_cMemoData.GetIconIndex(), dc, CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					}
					else
					{
						CDC	cMemDC;
						if( cMemDC.CreateCompatibleDC( &dc))
						{
							CBitmap		cBitmap;
							if( cBitmap.LoadBitmap( IDB_ICONIMG))
							{
								CBitmap*	pcOldBitmap;
								pcOldBitmap = cMemDC.SelectObject( &cBitmap);
								dc.SetBkColor( clrBack);
								dc.SetTextColor( clrFore);
								dc.BitBlt( 0, 0, _ICON_WIDTH, _ICON_HEIGHT, &cMemDC, _ICON_WIDTH * m_cMemoData.GetIconIndex(), 0, SRCCOPY);
								cMemDC.SelectObject( pcOldBitmap);
								cBitmap.DeleteObject();
							}
							cMemDC.DeleteDC();
						}
					}

					CString	cStrText;
					m_cMemoData.GetTitle( cStrText);
					// 表示用マージンの設定
					cRect.left = _ICON_WIDTH + ::GetSystemMetrics( SM_CYFRAME);
					dc.DrawText( cStrText, cRect, DT_SINGLELINE | DT_VCENTER | /*DT_CENTER*/DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX | DT_WORDBREAK);

					if( IsDelOnQuit())
					{
						CPen	cPen( PS_SOLID, 0, RGB( 0xFF, 0x00, 0x00));
						CPen*	pcPenOld;

						pcPenOld = dc.SelectObject( &cPen);
						dc.MoveTo( 0, cRect.top);
						dc.LineTo( 0, cRect.bottom);
						dc.SelectObject( pcPenOld);
					}

					dc.RestoreDC( nID);
				}
			}
		}
	}
	
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}

void CMemoChip::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		CRect	cRect;
		GetClientRect( cRect);
		int		nX = ::GetSystemMetrics( SM_CXFRAME);
		int		nY = ::GetSystemMetrics( SM_CYFRAME);
		m_astMoveRgnPoints[ 0].x = cRect.right;
		m_astMoveRgnPoints[ 0].y = cRect.bottom - ( nY * 3);
		m_astMoveRgnPoints[ 1].x = cRect.right - ( nX * 3);
		m_astMoveRgnPoints[ 1].y = cRect.bottom;
		m_astMoveRgnPoints[ 2].x = cRect.right;
		m_astMoveRgnPoints[ 2].y = cRect.bottom;
		m_astMoveRgnPoints[ 3].x = cRect.right;
		m_astMoveRgnPoints[ 3].y = cRect.bottom - ( nY * 3);

		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsOwnerDraw())
			{
				SIZE	stSize;
				stSize.cx = cx;
				stSize.cy = cy;
				m_pcDxlControl->Resize( GetSafeHwnd(), &stSize);
			}
		}
	}
}

// 右下のリサイズ領域の描画をします
void CMemoChip::DrawResizeArea( CDC* pcDC, BOOL blActive)
{
	if( 0 != m_cMemoData.GetDxlID())
	{
		if( NULL != m_pcDxlControl)
		{
			if( !m_pcDxlControl->IsSupportResize())
			{
				return;
			}
		}
	}

	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		CRect	cRectResize1;
		CRect	cRect;

		GetClientRect( cRect);
		cRectResize1 = cRect;
		cRectResize1.top	= cRectResize1.bottom - ( ::GetSystemMetrics( SM_CYFRAME) * 3);
		cRectResize1.left	= cRectResize1.right - ( ::GetSystemMetrics( SM_CXFRAME) * 3);

		CRgn	cRgn;
		cRgn.CreatePolygonRgn( m_astMoveRgnPoints, MOVEAREAPOINT_MAX, ALTERNATE);
		CBrush	cBrush( GetSysColor( ( blActive) ? COLOR_3DFACE : COLOR_3DDKSHADOW));
		pcDC->FillRgn( &cRgn, &cBrush);
		COLORREF	clrNotify;

		if( m_blDelOnQuit)
		{
			clrNotify = RGB( 0xFF, 0x00, 0x00);
		}
		else
		{
			if( m_cMemoData.IsLink())
			{
				clrNotify = RGB( 0x00, 0xFF, 0x00);
			}
			else
			{
				clrNotify = GetSysColor( COLOR_3DHILIGHT);
			}
		}

		CPen	cPenHl( PS_SOLID, 0, clrNotify);
		CPen	cPenSd( PS_SOLID, 0, GetSysColor( COLOR_3DSHADOW));
		CPen*	pcPenOld;
		pcPenOld = pcDC->SelectObject( &cPenHl);
		pcDC->MoveTo( cRectResize1.right, cRectResize1.top);
		pcDC->LineTo( cRectResize1.left, cRectResize1.bottom);
		pcDC->MoveTo( cRectResize1.right, cRectResize1.top + ( cRectResize1.Height() / 2));
		pcDC->LineTo( cRectResize1.left + ( cRectResize1.Width() / 2), cRectResize1.bottom);
		pcDC->SelectObject( &cPenSd);
		pcDC->MoveTo( cRectResize1.right, cRectResize1.top + 1);
		pcDC->LineTo( cRectResize1.left + 1, cRectResize1.bottom);
		pcDC->MoveTo( cRectResize1.right, cRectResize1.top + ( cRectResize1.Height() / 2) + 1);
		pcDC->LineTo( cRectResize1.left + ( cRectResize1.Width() / 2) + 1, cRectResize1.bottom);
		pcDC->SelectObject( pcPenOld);
	}
}

void CMemoChip::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if( WA_ACTIVE == nState)m_blOnceTop = true;

	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		// アクティブ状態をDXLに通知
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsOwnerDraw())
			{
				m_pcDxlControl->Active( GetSafeHwnd(), ( WA_INACTIVE != nState));
			}
		}

		CDC*	pcDC;
		pcDC = GetDC();
		DrawResizeArea( pcDC, ( WA_INACTIVE != nState));
		ReleaseDC( pcDC);
	}
}

LRESULT CMemoChip::OnNcHitTest(CPoint point)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		if( !m_cMemoData.IsPosLock() && !m_cMemoData.IsSeeThrough())
		{
			bool	blResizeSupport = true;
			if( 0 != m_cMemoData.GetDxlID())
			{
				if( NULL != m_pcDxlControl)
				{
					if( !m_pcDxlControl->IsSupportResize())
					{
						blResizeSupport = false;
					}
				}
			}

			if( blResizeSupport)
			{
				CPoint	cPoint = point;
				CRgn	cRgn;
				cRgn.CreatePolygonRgn( m_astMoveRgnPoints, MOVEAREAPOINT_MAX, ALTERNATE);

				ScreenToClient( &cPoint);
				if( cRgn.PtInRegion( cPoint))
				{
					m_blSysResize = TRUE;
					return HTBOTTOMRIGHT;
				}

				/*
				// この計算でもいいのだけれど……
				CPoint	cPoint = point;
				ScreenToClient( &cPoint);
				int nYDiff = cPoint.y - m_astMoveRgnPoints[ 0].y;
				if( 0 < nYDiff)
				{
					int nXDiff = cPoint.x - m_astMoveRgnPoints[ 1].x;
					if( 0 < nXDiff)
					{
						if( nYDiff > ( ( GetSystemMetrics( SM_CXFRAME) * 3) - nXDiff))return HTBOTTOMRIGHT;
					}
				}*/
			}
		}
	}
	
	m_blSysResize = FALSE;
	return CWnd::OnNcHitTest(point);
}

void CMemoChip::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		// 右下のＷクリックはリサイズ
		CalcDefaultRect();
	}
	
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CMemoChip::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CWnd::OnGetMinMaxInfo(lpMMI);

	// 大きさの下限と上限を設定
	lpMMI->ptMinTrackSize.x = _ICON_WIDTH + ::GetSystemMetrics( SM_CXBORDER) * 2;
	lpMMI->ptMinTrackSize.y = _ICON_HEIGHT + ::GetSystemMetrics( SM_CYBORDER) * 2;
	lpMMI->ptMaxTrackSize.x = ::GetSystemMetrics( SM_CXSCREEN);
	lpMMI->ptMaxTrackSize.y = ::GetSystemMetrics( SM_CYSCREEN);
}

void CMemoChip::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_blOnceTop = false;
	if( EndPopupNotify())return;

	if( ( MK_CONTROL & nFlags))
	{
		if( m_cMemoData.IsPassWordLock())return;
		DoDragDrop();
	}
	else
	{
		ActiveZOrder();
		m_blMouseClickStart = TRUE;
		GetCursorPos( &m_cPntStart);
		SetCapture();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMemoChip::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	ReleaseCapture();
	if( m_blNowMove)
	{
		// 移動完了に付き、現在地を記録
		CRect	cRect;
		GetWindowRect( cRect);
		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			m_cMemoData.SetNormalRect( cRect);
		}
		else
		{
			if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
			{
				m_cMemoData.SetTitleRect( cRect);
			}
			else
			{
				if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
				{
					m_cMemoData.SetIconRect( cRect);
				}
			}
		}
	}
	else
	{
		if( m_blMouseClickStart)
		{
			if( !m_blNowZMove)
			{
				if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
				{
					if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
					{
						if( m_pcDxlControl->IsSupportClick())
						{
							POINT	stPoint;
							GetCursorPos( &stPoint);
							ScreenToClient( &stPoint);
							DWORD dwResult = m_pcDxlControl->MouseClik( GetSafeHwnd(), &stPoint);
							if( _TM_CALCRECT & dwResult)
							{
								CalcDefaultRect( FALSE);
							}
							if( _TM_REDRAW & dwResult)
							{
								if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
								{
									InvalidateRect( NULL);
								}
							}
							if( _TM_NOTIFYBLINK & dwResult)
							{
								StartPopupNotify();
							}
							if( _TM_TONORMAL & dwResult)
							{
								PostMessage( WM_COMMAND, IDM_TONORMAL, 0);
							}
							else
							{
								if( _TM_TOICON & dwResult)
								{
									PostMessage( WM_COMMAND, IDM_TOICON, 0);
								}
								else
								{
									if( _TM_TOTITLE & dwResult)
									{
										PostMessage( WM_COMMAND, IDM_TOTITLE, 0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	m_blMouseClickStart = FALSE;
	m_blNowMove = false;
	m_blNowZMove = false;
	m_cPntStart = CPoint( -1, -1);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CMemoChip::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( OnDblClk( nFlags, pcSOboe->GetLDblClkFunc()))return;
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

// ダブルクリックの処理
BOOL CMemoChip::OnDblClk( UINT nFlags, int nFunc)
{
	switch( nFunc)
	{
	case CSOboeApp::DBLCLKFUNC_EDIT:
		PostMessage( WM_COMMAND, IDM_REEDIT, 0);
		return TRUE;
	case CSOboeApp::DBLCLKFUNC_LINKEXEC:
		PostMessage( WM_COMMAND, IDM_LINKEXEC, 0);
		return TRUE;
	case CSOboeApp::DBLCLKFUNC_ICON:
		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			PostMessage( WM_COMMAND, ( nFlags & MK_SHIFT) ? IDM_TOTITLE : IDM_TOICON, 0);
		}
		else
		{
			PostMessage( WM_COMMAND, IDM_TONORMAL, 0);
		}
		return TRUE;
	case CSOboeApp::DBLCLKFUNC_SEND:
		PostMessage( WM_COMMAND, IDM_SEND, 0);
		return TRUE;
	case CSOboeApp::DBLCLKFUNC_TITLE:
		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			PostMessage( WM_COMMAND, ( nFlags & MK_SHIFT) ? IDM_TOICON : IDM_TOTITLE, 0);
		}
		else
		{
			PostMessage( WM_COMMAND, IDM_TONORMAL, 0);
		}
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

// 裏返しを開く
BOOL CMemoChip::OpenPackChip( void)
{
	if( m_blNeedPassWord)
	{
		CString	cStrTitle;
		m_cMemoData.GetTitle( cStrTitle);

		CString	cStrPassword;
		
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		cStrPassword = pcSOboe->GetGlobalPassWord();

		CPassWordCheckDialog	cPWCheckDlg( cStrTitle, this);
		do
		{
			if( IDCANCEL == cPWCheckDlg.DoModal())return FALSE;
		}while( cStrPassword != cPWCheckDlg.m_cStrPassWord);
	}

	m_blPack = FALSE;
	CalcDefaultRect();
	if( m_pcEntryData && m_pcStrResultID)
	{
		RESULTPARAM*	pstResultParam = NULL;
		if( m_blCheckOpen || m_pstResults)
		{
			CString	cSreResult;
			if( m_pstResults)
			{
				m_blBlink = false;
				CResultCheckDialog	cRsltChkDlg( this);

				cRsltChkDlg.SetResults( m_pstResults->m_cStrResult1, m_pstResults->m_cStrResult2, m_pstResults->m_blOtherResult, m_pstResults->m_cStrResult3);
				// OK以外では戻ってこないからね
				cRsltChkDlg.DoModal();
				cRsltChkDlg.GetResult( cSreResult);

				delete m_pstResults;
				m_pstResults = NULL;

				CString	cStr;
				cStr.Format( "\r\nAns: %s", cSreResult);
				AddResult( cStr);
			}
			else
			{
				m_blCheckOpen = FALSE;
				SYSTEMTIME	stSysTime;
				GetLocalTime( &stSysTime);
				COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
				cSreResult = cTime.Format( UINT( IDS_OPENRESULT));
			}

			ASSERT( !cSreResult.IsEmpty());
			pstResultParam = new RESULTPARAM;
			if( pstResultParam)
			{
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				pstResultParam->stInnerResultData.m_cStrSignature = pcSOboe->GetSignature();
				pstResultParam->stInnerResultData.m_cStrChipID = *m_pcStrResultID;
				pstResultParam->stInnerResultData.m_cStrResult = cSreResult;

				pstResultParam->cEntry			= *m_pcEntryData;

				int nIndex = pcSOboe->FindNxlID( m_pcEntryData->GetNxlID());
				if( 0 <= nIndex)
				{
					pstResultParam->pcNetExLib		= pcSOboe->GetNetExLib( nIndex);
				}
				pstResultParam->hWnd			= GetSafeHwnd();
				pstResultParam->hCancelEvent	= NULL;

				if( -1 == _beginthread( ResultSendThread, 0, pstResultParam))
				{
					delete pstResultParam;
					pstResultParam = NULL;
				}
			}
		}
	}

	return TRUE;
}

void CMemoChip::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( m_blMouseClickStart)
	{
		CPoint	cPoint;

		GetCursorPos( &cPoint);
		if( FALSE == m_blNowMove)
		{
			if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType() && m_cMemoData.IsPosLock())return;

			CSize	cSize;
			cSize = m_cPntStart - cPoint;
			// クリック位置から、実際に移動を開始していい位置までの差
			int cxDrag = ::GetSystemMetrics( SM_CXDRAG);
			int cyDrag = ::GetSystemMetrics( SM_CYDRAG);
			if( cxDrag < cSize.cx || ( cxDrag * -1) > cSize.cx ||
				cyDrag < cSize.cy || ( cyDrag * -1) > cSize.cy)
			{
				// 移動開始！
				m_blNowMove = TRUE;
			}
			else
			{
				return;
			}
		}
		else
		{
			CRect	cRect;
			m_cPntStart -= cPoint;
			GetWindowRect( cRect);
			cRect -= m_cPntStart;
			m_cPntStart = cPoint;

			cPoint = cRect.TopLeft();
			
			if( MK_SHIFT  == ( nFlags & MK_SHIFT))
			{
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				int nXShiftMoveStep = pcSOboe->GetShiftStepX();
				int nYShiftMoveStep = pcSOboe->GetShiftStepY();

				cPoint.x -= ( cPoint.x % nXShiftMoveStep);
				cPoint.y -= ( cPoint.y % nYShiftMoveStep);
				m_cPntStart.x -= ( m_cPntStart.x % nXShiftMoveStep);
				m_cPntStart.y -= ( m_cPntStart.y % nYShiftMoveStep);
			}

			SetWindowPos( NULL, cPoint.x, cPoint.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
			/*
			CRect	cRect;
			m_cPntStart -= cPoint;
			GetWindowRect( cRect);
			cRect -= m_cPntStart;
			m_cPntStart = cPoint;

			cPoint = cRect.TopLeft();
			
			if( MK_SHIFT  == ( nFlags & MK_SHIFT))
			{
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				int nXShiftMoveStep = pcSOboe->GetShiftStepX();
				int nYShiftMoveStep = pcSOboe->GetShiftStepY();

				CRect	cTmpRect;
				cTmpRect = cRect;
				cRect.left -= ( cRect.left % nXShiftMoveStep);
				cRect.right = cRect.left + cTmpRect.Width();
				cRect.top -= ( cRect.top % nYShiftMoveStep);
				cRect.bottom = cRect.top + cTmpRect.Height();
				m_cPntStart.x -= ( m_cPntStart.x % nXShiftMoveStep);
				m_cPntStart.y -= ( m_cPntStart.y % nYShiftMoveStep);
			}

			*/
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CMemoChip::OnToIcon() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
	{
		m_blFrameDraw = true;
		return;
	}

	if( m_cMemoData.IsSeeThrough())
	{
		SetWindowRgn( NULL, FALSE);
	}
	CRect	cRctFrom;
	GetWindowRect( cRctFrom);
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		m_cMemoData.SetNormalRect( cRctFrom);
	}
	else
	{
		if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
		{
			m_cMemoData.SetTitleRect( cRctFrom);
		}
	}
	// 一番最初は、現在のWindowの左上を基準とする
	if( m_cMemoData.GetIconRect().IsRectEmpty())
	{
		m_cMemoData.SetIconRect( m_cMemoData.GetNormalRect());
	}

	// アイコン化したときのサイズ算出
	CRect cRctTo = m_cMemoData.GetIconRect();
	cRctTo.right = cRctTo.left + _ICON_WIDTH + ::GetSystemMetrics( SM_CXBORDER) * 2;
	cRctTo.bottom = cRctTo.top + _ICON_HEIGHT + ::GetSystemMetrics( SM_CYBORDER) * 2;

	// 領域の正常化！
	cRctTo = ClipRectToMonitor( cRctTo);

	// リサイズ時のフレームアニメーション
	if( m_blFrameDraw)
	{
		PlaySound( "Minimize", NULL, SND_ALIAS | SND_ASYNC | SND_NODEFAULT);
		DoResizeFrameAnime( cRctTo, cRctFrom);
	}
	m_blFrameDraw = true;

	m_cMemoData.SetWindowType( CMemoData::WINDOW_TYPE_ICON);
	m_cMemoData.SetIconRect( cRctTo);
	MoveWindow( cRctTo);
	// 通常はリサイズだけで再描画が走るけど、同じサイズだった場合にはしないので、強制再描画
	if( cRctTo.Width() == cRctFrom.Width())
	{
		if( cRctTo.Height() == cRctFrom.Height())InvalidateRect( NULL, FALSE);
	}

	CreateToolTip();

	// リサイズの通知
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsOwnerDraw())
		{
			m_pcDxlControl->Zoom( GetSafeHwnd(), CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType());
		}
	}
}

void CMemoChip::OnToNormal() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		if( m_blFrameDraw)
		{
			CRect	cDisplayRect( 0, 0, ::GetSystemMetrics( SM_CXSCREEN), ::GetSystemMetrics( SM_CYSCREEN));
			CRect	cRctTo;
			GetWindowRect( cRctTo);
			DoResizeFrameAnime( cRctTo, cDisplayRect);
		}
		m_blFrameDraw = true;
		ShowWindow( SW_SHOW);
	}
	else
	{
		if( FALSE == CheckPassWord())return;
		DestroyToolTip();

		CRect	cRctFrom;
		GetWindowRect( cRctFrom);
		if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
		{
			m_cMemoData.SetIconRect( cRctFrom);
		}
		else
		{
			if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
			{
				m_cMemoData.SetTitleRect( cRctFrom);
			}
		}

		CRect cRctTo;
		cRctTo = m_cMemoData.GetNormalRect();

		// 領域の正常化！
		cRctTo = ClipRectToMonitor( cRctTo);

		// リサイズ時のフレームアニメーション
		if( m_blFrameDraw)
		{
			PlaySound( "RestoreUp", NULL, SND_ALIAS | SND_ASYNC | SND_NODEFAULT);
			DoResizeFrameAnime( cRctTo, cRctFrom);
		}
		m_blFrameDraw = true;

		m_cMemoData.SetWindowType( CMemoData::WINDOW_TYPE_NORMAL);
		m_cMemoData.SetNormalRect( cRctTo);
		MoveWindow( cRctTo);
		// 通常はリサイズだけで再描画が走るけど、同じサイズだった場合にはしないので、強制再描画
		if( cRctTo.Width() == cRctFrom.Width())
		{
			if( cRctTo.Height() == cRctFrom.Height())InvalidateRect( NULL, FALSE);
		}
		// リサイズの通知
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsOwnerDraw())
			{
				m_pcDxlControl->Zoom( GetSafeHwnd(), CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType());
			}
		}

		if( m_cMemoData.IsSeeThrough())
		{
			if( m_pcRgnSeeThrough)
			{
				HRGN	hRgn;
				hRgn = CreateRectRgn( 0, 0, 0, 0);
				CombineRgn( hRgn, hRgn, *m_pcRgnSeeThrough, RGN_OR);
				SetWindowRgn( hRgn, TRUE);
			}
		}
	}
}

void CMemoChip::OnToTitle()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
	{
		m_blFrameDraw = true;
		return;
	}
	if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
	{
		if( FALSE == CheckPassWord())return;
	}
	DestroyToolTip();

	if( m_cMemoData.IsSeeThrough())
	{
		SetWindowRgn( NULL, FALSE);
	}
	CRect	cRctFrom;
	GetWindowRect( cRctFrom);
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		m_cMemoData.SetNormalRect( cRctFrom);
	}
	else
	{
		if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
		{
			m_cMemoData.SetIconRect( cRctFrom);
		}
	}
	// 一番最初は、現在のWindowの左上を基準とする
	if( m_cMemoData.GetTitleRect().IsRectEmpty())
	{
		m_cMemoData.SetTitleRect( m_cMemoData.GetNormalRect());
	}

	// タイトル化したときのサイズ算出
	CRect cRctTo;
	CalcTitleRect( cRctTo);

	// 領域の正常化！
	cRctTo = ClipRectToMonitor( cRctTo);

	// リサイズ時のフレームアニメーション
	if( m_blFrameDraw)
	{
		PlaySound( "Minimize", NULL, SND_ALIAS | SND_ASYNC | SND_NODEFAULT);
		DoResizeFrameAnime( cRctTo, cRctFrom);
	}
	m_blFrameDraw = true;

	m_cMemoData.SetWindowType( CMemoData::WINDOW_TYPE_TITLE);
	m_cMemoData.SetTitleRect( cRctTo);
	MoveWindow( cRctTo);
	// 通常はリサイズだけで再描画が走るけど、同じサイズだった場合にはしないので、強制再描画
	if( cRctTo.Width() == cRctFrom.Width())
	{
		if( cRctTo.Height() == cRctFrom.Height())InvalidateRect( NULL, FALSE);
	}

	CreateToolTip();

	// リサイズの通知
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsOwnerDraw())
		{
			m_pcDxlControl->Zoom( GetSafeHwnd(), CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType());
		}
	}
}

// リサイズ時のアニメーション
void CMemoChip::DoResizeFrameAnime( const CRect& cRectTo, const CRect& cRectFrom)
{
	if( m_blFrameDraw)
	{
		// 下がウィンドーシュワワ効果
		int left = ( cRectTo.left - cRectFrom.left) / 10;
		int right = ( cRectTo.right - cRectFrom.right) / 10;
		int top = ( cRectTo.top - cRectFrom.top) / 10;
		int bottom = ( cRectTo.bottom - cRectFrom.bottom) / 10;
		CDC		cDC;
		cDC.CreateDC( "DISPLAY", NULL, NULL, NULL);
		CRect	cRectLast;
		CRect	cRectFrame = cRectFrom;
		SIZE	sz;
		sz.cx = 3;
		sz.cy = 3;
		cDC.DrawDragRect( cRectFrame, sz, NULL, sz);
		for( int i = 0; i < 10; i++)
		{
			cRectLast = cRectFrame;
			cRectFrame.left += left;
			cRectFrame.right += right;
			cRectFrame.top += top;
			cRectFrame.bottom += bottom;
			cDC.DrawDragRect( cRectFrame, sz, cRectLast, sz);
			Sleep( 10 + ( i * 2));	// 終わりに行くほど遅くする
		}
		cDC.DrawDragRect( cRectFrame, sz, NULL, sz);
		cDC.DeleteDC();
	}
}

void CMemoChip::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	UNUSED_ALWAYS (pWnd);

	if( 0 == IsWindowEnabled())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( NULL == pcSOboe)return;

	CMenu menu;
	CMenu* pPopup;
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		int nMenuIndex = ( pcSOboe->IsCompatibleOU() ? 2 : 0) + ( pcSOboe->IsFlatMenu() ? 1 : 0);
		VERIFY( menu.LoadMenu( IDR_FOR_NORMAL));
		pPopup = menu.GetSubMenu( nMenuIndex);
		ASSERT( pPopup != NULL);

		const CCelControl*	pcCelControl = pcSOboe->GetCelControl();
		ASSERT( pcCelControl);
		HMENU hMenu = pcCelControl->GetMenu();
		if( hMenu)
		{
			CString	cStr;
			cStr.LoadString( IDS_CELPOPUP);
			pPopup->AppendMenu( MF_SEPARATOR);
			pPopup->AppendMenu( MF_POPUP | ( CCelControl::IsUsed() ? MF_GRAYED : MF_ENABLED), ( UINT)hMenu, cStr);
		}

		BOOL blNoAlphaBlend = FALSE;
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsSupportMenu())
			{
				HMENU hMenu = m_pcDxlControl->GetDrawExMenu();
				if( hMenu)
				{
					CString	cStr;
					cStr.LoadString( IDS_DXLPOPUP);
					pPopup->AppendMenu( MF_SEPARATOR);
					pPopup->AppendMenu( MF_POPUP | ( CDxlControl::IsUsed() ? MF_GRAYED : MF_ENABLED), (UINT)hMenu, cStr);
				}
			}
			if( m_pcDxlControl->IsOwnerDraw())
			{
				pPopup->EnableMenuItem( IDM_SEETHROUGH, MF_GRAYED);
			}
			// DxlがAlphaBlendの利用を禁止している？
			blNoAlphaBlend = m_pcDxlControl->IsNoAlphaBlend();
		}

		if( IsSupportAlphaBlend() && !blNoAlphaBlend)
		{
			CMenu* pcAlphaBlend = menu.GetSubMenu( 4);
			if( pcAlphaBlend)
			{
				CString	cStr;
				cStr.Format( IDS_ALPHABLENDCAPTION, 100 - ( ( ( m_cMemoData.GetAlphaBlend()) * 100) / 255));
				/*
				cStr.LoadString( IDS_ALPHABLEND);
				switch( m_cMemoData.GetAlphaBlend())
				{
				case CMemoData::ALPHABLEND_075:
					pcAlphaBlend->CheckMenuItem( IDM_ABLEND_075, MF_BYCOMMAND | MF_CHECKED);
					break;
				case CMemoData::ALPHABLEND_050:
					pcAlphaBlend->CheckMenuItem( IDM_ABLEND_050, MF_BYCOMMAND | MF_CHECKED);
					break;
				case CMemoData::ALPHABLEND_025:
					pcAlphaBlend->CheckMenuItem( IDM_ABLEND_025, MF_BYCOMMAND | MF_CHECKED);
					break;
				case CMemoData::ALPHABLEND_000:
					pcAlphaBlend->CheckMenuItem( IDM_ABLEND_000, MF_BYCOMMAND | MF_CHECKED);
					break;
				default:
					pcAlphaBlend->CheckMenuItem( IDM_ABLEND_FREE, MF_BYCOMMAND | MF_CHECKED);
					break;
				}
				*/
				pPopup->InsertMenu( IDM_SEETHROUGH, MF_BYCOMMAND | MF_POPUP | MF_ENABLED, (UINT)pcAlphaBlend->GetSafeHmenu(), cStr);
			}
		}
	}
	else
	{
		if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
		{
			int nMenuIndex = ( pcSOboe->IsCompatibleOU() ? 2 : 0) + ( pcSOboe->IsFlatMenu() ? 1 : 0);
			VERIFY( menu.LoadMenu( IDR_FOR_ICON));
			pPopup = menu.GetSubMenu( nMenuIndex);
		}
		else
		{
			if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
			{
				int nMenuIndex = ( pcSOboe->IsCompatibleOU() ? 2 : 0) + ( pcSOboe->IsFlatMenu() ? 1 : 0);
				VERIFY( menu.LoadMenu( IDR_FOR_TITLE));
				pPopup = menu.GetSubMenu( nMenuIndex);
			}
			else
			{
				return;
			}
		}
	}
	
	if( pPopup != NULL)
	{
		if( !IsClipboardFormatAvailable( CF_TEXT))
		{
			pPopup->EnableMenuItem( IDM_PASTE_APPEND, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem( IDM_PASTE_OVERWRITE, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem( IDM_PASTE_CREATE, MF_BYCOMMAND | MF_GRAYED);
		}
		if( !IsClipboardFormatAvailable( CF_HDROP) &&
			!IsClipboardFormatAvailable( CF_TEXT))
		{
			pPopup->EnableMenuItem( IDM_PASTE_LINK, MF_BYCOMMAND | MF_GRAYED);
		}
		if( CMemoEditSheet::IsUsed())
		{
			pPopup->EnableMenuItem( IDM_SEND, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem( IDM_REEDIT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem( IDM_NEWMEMO, MF_BYCOMMAND | MF_GRAYED);
		}
		if( !m_cMemoData.IsLink())
		{
			pPopup->EnableMenuItem( IDM_LINKEXEC, MF_BYCOMMAND | MF_GRAYED);
		}
		if( m_cMemoData.IsPosLock())pPopup->CheckMenuItem( IDM_POSLOCK, MF_BYCOMMAND | MF_CHECKED);
		if( IsDelOnQuit())pPopup->CheckMenuItem( IDM_DELONQUIT, MF_BYCOMMAND | MF_CHECKED);
		if( m_cMemoData.IsSeeThrough())pPopup->CheckMenuItem( IDM_SEETHROUGH, MF_BYCOMMAND | MF_CHECKED);

		if( NULL == m_pcEntryData)
		{
			pPopup->EnableMenuItem( IDM_REPLY, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem( IDM_SENDER, MF_BYCOMMAND | MF_GRAYED);
		}
		if( m_cStrUndo.IsEmpty())
		{
			pPopup->EnableMenuItem( IDM_UNDO, MF_BYCOMMAND | MF_GRAYED);
		}
		if( !pcSOboe->IsStandbyNetwork())
		{
			pPopup->EnableMenuItem( IDM_SEND, MF_BYCOMMAND | MF_GRAYED);
		}

		switch( pcSOboe->GetLDblClkFunc())
		{
		case CSOboeApp::DBLCLKFUNC_EDIT:
			pPopup->SetDefaultItem( IDM_REEDIT);
			break;
		case CSOboeApp::DBLCLKFUNC_LINKEXEC:
			pPopup->SetDefaultItem( IDM_LINKEXEC);
			break;
		case CSOboeApp::DBLCLKFUNC_ICON:
			pPopup->SetDefaultItem( ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()) ? IDM_TOICON : IDM_TONORMAL);
			break;
		case CSOboeApp::DBLCLKFUNC_SEND:
			pPopup->SetDefaultItem( IDM_SEND);
			break;
		case CSOboeApp::DBLCLKFUNC_TITLE:
			pPopup->SetDefaultItem( ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()) ? IDM_TOTITLE : IDM_TONORMAL);
			break;
		}
		
		pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void CMemoChip::OnReedit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( TRUE == m_blBlink)
	{
		MessageBeep( MB_ICONINFORMATION);
		return;
	}

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
/*
	if( GetKeyState( VK_SHIFT) & ~1)
	{
		pcSOboe->ExEdit( !pcSOboe->IsExEdit());
	}
	else
*/
	{
		if( CResizableMemoEditDialog::IsUsed())return;
		if( FALSE == CheckPassWord())return;

		m_cOleDrop.EnableDrop( FALSE);

		{
			int nResult = IDCANCEL;

			CMemoData	cMemoData;
			BOOL		blDelOnQuit;
			BOOL		blResize = 0;
//			cMemoData = m_cMemoData;
			GetMemoData( cMemoData);
			blDelOnQuit = m_blDelOnQuit;
			if( pcSOboe->IsExEdit())
			{
				nResult = ExEditChip( cMemoData, blDelOnQuit, blResize);
			}
			else
			{
				nResult = EditChip( cMemoData, blDelOnQuit, blResize);
			}

			if( IDOK == nResult)
			{
				BOOL blNoAlphaBlend = FALSE;

				UINT	unOldDxlID;
				unOldDxlID = m_cMemoData.GetDxlID();

				m_cStrUndo = m_cMemoData.GetMemo();
				m_cMemoData = cMemoData;
				if( m_cStrUndo == m_cMemoData.GetMemo())m_cStrUndo.Empty();
				m_cMemoData.GetForeColor( m_clrInternalForeColor);
				m_cMemoData.GetBackColor( m_clrInternalBackColor);
				ActiveZOrder();

				if( unOldDxlID != m_cMemoData.GetDxlID())
				{
					if( 0 != unOldDxlID && NULL != m_pcDxlControl)
					{
						m_pcDxlControl->UnInitializeChip( GetSafeHwnd());
						delete m_pcDxlControl;
						m_pcDxlControl = NULL;

						CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
						int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
						if( 0 <= nIndex)
						{
							CDrawExLib*	pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
							if( pcDrawExLib)
							{
								pcDrawExLib->UseEnd();
							}
						}
					}
					if( 0 != m_cMemoData.GetDxlID())
					{
						CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
						int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
						if( 0 <= nIndex)
						{
							CDrawExLib*	pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
							if( pcDrawExLib)
							{
								m_pcDxlControl = new CDxlControl;
								m_pcDxlControl->SetInstance( pcDrawExLib);
								m_pcDxlControl->InitializeChip( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
								blNoAlphaBlend = m_pcDxlControl->IsNoAlphaBlend();
								pcDrawExLib->UseStart();
							}
						}
					}
				}
				else
				{
					if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
					{
						m_pcDxlControl->UpDate( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
					}
				}

				m_blDelOnQuit = ( TRUE == blDelOnQuit);
				if( blResize)CalcDefaultRect();
				if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
				{
					CRect cRctNew;
					CalcTitleRect( cRctNew);
					if( cRctNew != m_cMemoData.GetTitleRect())
					{
						m_cMemoData.SetTitleRect( cRctNew);
						MoveWindow( cRctNew);
					}
				}

				ModifyToolTip();

				// これは、タイマーの初期化だよン
				m_nNotifyDay	= 0;

				if( IsSupportAlphaBlend())
				{
					if( CMemoData::ALPHABLEND_000 == m_cMemoData.GetAlphaBlend() || blNoAlphaBlend)
					{
						ModifyStyleEx( WS_EX_LAYERED, 0, SWP_DRAWFRAME);
					}
					else
					{
						ModifyStyleEx( 0, WS_EX_LAYERED);
						SetLayeredWindowAttributes( 0, m_cMemoData.GetAlphaBlend(), LWA_ALPHA);
					}
				}

				InvalidateRect( NULL);

				// 編集後保存？
				if( pcSOboe->IsModifyAndSave())
				{
					// 保存ね
					::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_SAVEDATAS, ( LPARAM)m_unID);
				}
			}
		}

		m_cOleDrop.EnableDrop( TRUE);
	}
}

int CMemoChip::EditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize)
{
	CString	cStrCaption;
	cStrCaption.LoadString( IDS_CAPTION_EDIT);
	CMemoEditSheet	cMemoEdit( cStrCaption, this);
	
	UINT	unOldDxlID = cMemoData.GetDxlID();

	if( 0 != cMemoData.GetDxlID())
	{
		if( m_pcDxlControl)
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), cMemoData);
		}
	}
	cMemoEdit.SetMemoData( cMemoData);
	cMemoEdit.ResizeLock( cMemoData.IsPosLock());
	cMemoEdit.DelOnQuit( blDelOnQuit);

	int	nResult;
	while( TRUE)
	{
		nResult = cMemoEdit.DoModal();
		if( IDCANCEL == nResult)break;
		if( cMemoEdit.IsMemoEmpty())
		{
			CString	cStr;
			cStr.LoadString( IDS_MEMOEMPTY);
			if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON1))
			{
				nResult = IDCANCEL;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if( IDOK == nResult)
	{
		cMemoEdit.GetMemoData( cMemoData);
		blDelOnQuit = cMemoEdit.IsDelOnQuit();
		blResize = cMemoEdit.IsResize();
	}
	return nResult;
}

int CMemoChip::ExEditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize)
{
	CString	cStrCaption;
	cStrCaption.LoadString( IDS_CAPTION_EDIT);
	CResizableMemoEditDialog	cMemoEdit( cStrCaption, this);
	
	UINT	unOldDxlID = cMemoData.GetDxlID();

	if( 0 != cMemoData.GetDxlID())
	{
		if( m_pcDxlControl)
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), cMemoData);
		}
	}
	cMemoEdit.SetMemoData( cMemoData);
	cMemoEdit.ResizeLock( cMemoData.IsPosLock());
	cMemoEdit.DelOnQuit( blDelOnQuit);

	cMemoEdit.SetIcon( GetIcon( FALSE));

	int	nResult;
	while( TRUE)
	{
		nResult = cMemoEdit.DoModal();
		if( IDCANCEL == nResult)break;
		if( cMemoEdit.IsMemoEmpty())
		{
			CString	cStr;
			cStr.LoadString( IDS_MEMOEMPTY);
			if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON1))
			{
				nResult = IDCANCEL;
				break;
			}
		}
		else
		{
			break;
		}
	}
	if( IDOK == nResult)
	{
		cMemoEdit.GetMemoData( cMemoData);
		blDelOnQuit = cMemoEdit.IsDelOnQuit();
		blResize = cMemoEdit.IsResize();
	}
	return nResult;
}

int CMemoChip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	BOOL blNoAlphaBlend = FALSE;
	// 漢字は入力できないでーす
	::ImmAssociateContext( GetSafeHwnd(), NULL);
	// Dxlへの結合
	if( 0 != m_cMemoData.GetDxlID())
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
		if( 0 <= nIndex)
		{
			CDrawExLib*	pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				m_pcDxlControl = new CDxlControl;
				m_pcDxlControl->SetInstance( pcDrawExLib);
				m_pcDxlControl->InitializeChip( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
				blNoAlphaBlend = m_pcDxlControl->IsNoAlphaBlend();
				pcDrawExLib->UseStart();
			}
		}
	}

	if( m_cMemoData.IsSeeThrough())
	{
		CRect	cRect;

		cRect = m_cMemoData.GetNormalRect();
		cRect.right -= cRect.left;
		cRect.left = 0;
		cRect.bottom -= cRect.top;
		cRect.top = 0;

		cRect.right		-= ( ::GetSystemMetrics( SM_CXBORDER) * 2);
		cRect.bottom	-= ( ::GetSystemMetrics( SM_CYBORDER) * 2);

		CDC*	pcDC = GetDC();
		if( m_pcRgnSeeThrough)
		{
			delete m_pcRgnSeeThrough;
			m_pcRgnSeeThrough = NULL;
		}
		m_pcRgnSeeThrough = CreateSeeThroughRgn( pcDC, cRect);
		ReleaseDC( pcDC);

		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			HRGN	hRgn;
			hRgn = CreateRectRgn( 0, 0, 0, 0);
			CombineRgn( hRgn, hRgn, *m_pcRgnSeeThrough, RGN_OR);
			SetWindowRgn( hRgn, FALSE);
		}
	}

	ActiveZOrder();

	if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType() || CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
	{
		CreateToolTip();
	}

	if( IsSupportAlphaBlend() && !blNoAlphaBlend)
	{
		BYTE	byAlphaBlend = m_cMemoData.GetAlphaBlend();
		if( 255 > byAlphaBlend)
		{
			ModifyStyleEx( 0, WS_EX_LAYERED);
			SetLayeredWindowAttributes( 0, byAlphaBlend, LWA_ALPHA);
		}
	}

	if( 0 == m_cOleDrop.Register( this))
	{
		MessageBox( "OLE D&D Regist error");
	}

	return 0;
}

void CMemoChip::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( false == m_blSmartClose)return;

	// Dxlの開放
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		m_pcDxlControl->UnInitializeChip( GetSafeHwnd());
		delete m_pcDxlControl;
		m_pcDxlControl = NULL;

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
		if( 0 <= nIndex)
		{
			CDrawExLib*	pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				pcDrawExLib->UseEnd();
			}
		}
	}
	
	CWnd::OnClose();
}

BOOL CMemoChip::DoBlink( void)
{
	if( false != m_blBlink)
	{
		CRect	cRect;
		GetClientRect( cRect);
		CDC*	pcDc = GetDC();
		pcDc->PatBlt( cRect.left, cRect.top, cRect.Width(), cRect.Height(), DSTINVERT);
		ReleaseDC( pcDc);
	}

	return m_blBlink;
}

void CMemoChip::ActiveZOrder( void)
{
	const CWnd*	pcPos;
	switch( m_cMemoData.GetZOrder())
	{
	case CMemoData::ZORDER_TOPMOST:
		pcPos = &wndTopMost;
		break;
	case CMemoData::ZORDER_BOTTOMMOST:
		pcPos = &wndBottom;
		break;
	case CMemoData::ZORDER_NORMAL:
	default:
		SetWindowPos( &wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		pcPos = &wndTop;
		break;
	}
	SetWindowPos( pcPos, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CMemoChip::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if( TRUE == m_blSysResize)
	{
		if( GetKeyState( VK_SHIFT) & ~1)
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			lpwndpos->cx -= ( lpwndpos->cx % pcSOboe->GetShiftStepX());
			lpwndpos->cy -= ( lpwndpos->cy % pcSOboe->GetShiftStepY());
		}
	}
	if( CMemoData::ZORDER_BOTTOMMOST == m_cMemoData.GetZOrder())
	{
		if( false == m_blOnceTop)lpwndpos->hwndInsertAfter = HWND_BOTTOM;
	}

	CWnd::OnWindowPosChanging(lpwndpos);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
}

void CMemoChip::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( OnDblClk( nFlags, pcSOboe->GetMDblClkFunc()))return;
	
	CWnd::OnMButtonDblClk(nFlags, point);
}

void CMemoChip::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_blOnceTop = false;
	if( EndPopupNotify())return;

	CWnd::OnMButtonDown(nFlags, point);
}

void CMemoChip::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	m_blOnceTop = false;
	if( EndPopupNotify())return;
	if( MK_LBUTTON & nFlags)return;

	// Dragじゃないのに、Drag用のオブジェクト作のイヤじゃん？
	// だから、WM_RBUTTONDOWN後に任意時間WM_RBUTTONUPが来ないか見てるのさ！
	// 基本的に、D&D開始は200msec待ち後らしいからね
	// コード的には醜いが、致し方ないよね。
	// 1000 はね、一秒以上押したときにD&D開始を意味するよ
	BOOL	blCheck = FALSE;
	DWORD dwTick = GetTickCount();
	MSG	stMsg;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CRect	cRect;
	GetClientRect( cRect);
	POINT	stPoint;

	SetCapture();
	while( TRUE)
	{
		if( ( dwTick + 250) <= GetTickCount() || dwTick > GetTickCount())
		{
			TRACE( "T-Out\n");
			blCheck = TRUE;
			break;
		}
		if( ::PeekMessage( &stMsg, NULL, NULL, NULL, PM_REMOVE))
		{
			TRACE( "0x%X\n", stMsg.message);
			if( pcSOboe->PreTranslateMessage( &stMsg))continue;
			TranslateMessage( &stMsg);
			DispatchMessage( &stMsg);
			if( WM_MOUSEMOVE == stMsg.message)
			{
				stPoint.x = LOWORD( stMsg.lParam);
				stPoint.y = HIWORD( stMsg.lParam);
				if( !cRect.PtInRect( stPoint))
				{
					TRACE( "WM_MOUSEMOVEでClient領域外！\n");
					blCheck = TRUE;
					break;
				}
				if( GetSafeHwnd() != stMsg.hwnd)
				{
					TRACE( "WM_MOUSEMOVEでWindowハンドルが違う！\n");
					blCheck = TRUE;
					break;
				}
			}
			if( WM_RBUTTONUP == stMsg.message)
			{
				TRACE( "R-UP-0x%X\n", stMsg.message);
				blCheck = FALSE;
				break;
			}
			if( WM_NCRBUTTONUP == stMsg.message)
			{
				TRACE( "NCR-UP-0x%X\n", stMsg.message);
				blCheck = FALSE;
				break;
			}
			// 基本的に、ここがチェック対象になることはありえない
			if( WM_CONTEXTMENU == stMsg.message)
			{
				TRACE( "C-Menu-0x%X\n", stMsg.message);
				blCheck = FALSE;
				break;
			}
		}
	}
	ReleaseCapture();

//	if( ( MK_CONTROL & nFlags))
	if( blCheck)
	{
		if( m_cMemoData.IsPassWordLock())return;
		
		DWORD	dwEffects = DoDragDrop( TRUE);
		if( DROPEFFECT_MOVE == dwEffects)
		{
			::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_DELONLY, ( LPARAM)m_unID);
			return;
		}
		else
		{
			if( DROPEFFECT_NONE != dwEffects)
			{
				return;
			}
		}
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMemoChip::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( MK_LBUTTON == ( MK_LBUTTON & nFlags))
	{
		ClipCursor( NULL);
		m_blMouseClickStart = FALSE;
		m_blNowMove = FALSE;
		m_cPntStart = CPoint( -1, -1);
		ReleaseCapture();
		m_blNowZMove = true;
		SetWindowPos( &wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		return;
	}
	
	CWnd::OnRButtonUp(nFlags, point);
}

BOOL CMemoChip::CheckExtensiveNotify( const COleDateTime& cTime)
{
	m_cMemoData.GetForeColor( m_clrInternalForeColor);
	m_cMemoData.GetBackColor( m_clrInternalBackColor);

	if( FALSE == m_cMemoData.IsTimer())return FALSE;

	int	nNotifyMonth = m_cMemoData.GetNotifyMonth( cTime);
	int	nNotifyDay = m_cMemoData.GetNotifyDay( cTime);

	if( m_cMemoData.IsAfterNotify())
	{
		if( CMemoData::TIMERTYPE_DAY == m_cMemoData.GetTimerType())
		{
			int nYear = cTime.GetYear();
			if( 1 == cTime.GetMonth() && 12 == nNotifyMonth)
			{
				nYear -= 1;
			}
			COleDateTime		cTimeCheck( nYear, ( 0 == nNotifyMonth) ? cTime.GetMonth() : nNotifyMonth, nNotifyDay, m_cMemoData.GetTimerHour(), m_cMemoData.GetTimerMin(), 0);
			COleDateTimeSpan	cSpan;
			cSpan = cTime - cTimeCheck;

			bool blNotify = true;
			if( ( 0 == nNotifyMonth) && nNotifyDay != cTime.GetDay())blNotify = false;
			if( 31 >= cSpan.GetDays() && 0 < cSpan.GetTotalSeconds() && blNotify)
			{
				m_blBeforeVisible = ( FALSE != IsWindowVisible());
				ShowWindow( SW_SHOW);

				m_blBlink = true;
				m_nNotifyDay = cTime.GetDay();

				CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				m_clrInternalForeColor = pcSOboe->GetAfterForeColor();
				m_clrInternalBackColor = pcSOboe->GetAfterBackColor();
				return TRUE;
			}
		}
		else
		{
			if( CMemoData::TIMERTYPE_WEEK == m_cMemoData.GetTimerType())
			{
				if( m_cMemoData.GetWeek()& ( 0x0001 << ( cTime.GetDayOfWeek() - 1)))
				{
					if( cTime.GetHour() >= m_cMemoData.GetTimerHour())
					{
						if( cTime.GetMinute() >= m_cMemoData.GetTimerMin())
						{
							m_blBeforeVisible = ( FALSE != IsWindowVisible());
							ShowWindow( SW_SHOW);

							m_blBlink = true;
							m_nNotifyDay = cTime.GetDay();

							CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
							ASSERT( pcSOboe);

							m_clrInternalForeColor = pcSOboe->GetAfterForeColor();
							m_clrInternalBackColor = pcSOboe->GetAfterBackColor();
							return TRUE;
						}
					}
				}
			}
		}
	}

	if( m_cMemoData.GetBeforeNotify())
	{
		if( CMemoData::TIMERTYPE_DAY == m_cMemoData.GetTimerType())
		{
			int nYear = cTime.GetYear();
			if( 12 == cTime.GetMonth() && 1 == nNotifyMonth)
			{
				nYear += 1;
			}
			COleDateTime		cTimeCheck( nYear, ( ( 0 != nNotifyMonth) ? nNotifyMonth : cTime.GetMonth()), nNotifyDay, cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
			COleDateTimeSpan	cDiffTime;
			if( cTimeCheck > cTime)
			{
				cDiffTime = cTimeCheck - cTime;
				if( m_cMemoData.GetBeforeNotify() >= cDiffTime.GetDays() && 0 != cDiffTime.GetDays())
				{
					m_blBeforeVisible = ( FALSE != IsWindowVisible());
					ShowWindow( SW_SHOW);

					CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);

					m_clrInternalForeColor = pcSOboe->GetBeforeForeColor();
					m_clrInternalBackColor = pcSOboe->GetBeforeBackColor();
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

#pragma optimize( "", off )
#pragma optimize( "atp", on )	// 速度重視で最適化
void CMemoChip::CheckTimer( const COleDateTime& cTime)
{
	if( m_blBlink)
	{
		if( m_nNotifyDay != cTime.GetDay())
		{
			// 日付が変わったのでブリンクは停止しましょう
			m_nNotifyDay = 0;
			EndPopupNotify();
		}
		// さぁ、ブリンク処理ですよ
		DoBlink();
		return;
	}
	// タイマは使っていません！
	if( !m_cMemoData.IsTimer())return;
	// 通知済みです
	if( m_nNotifyDay == cTime.GetDay())return;

	// 今日は、通知してイイ日？の検査
	if( CMemoData::TIMERTYPE_EVERYDAY != m_cMemoData.GetTimerType())
	{
		if( CMemoData::TIMERTYPE_WEEK == m_cMemoData.GetTimerType())
		{
			if( !( m_cMemoData.GetWeek() & ( 0x0001 << ( cTime.GetDayOfWeek() - 1))))return;
		}
		else
		{
			if( CMemoData::TIMERTYPE_DAY == m_cMemoData.GetTimerType())
			{
				int	nNotifyMonth = m_cMemoData.GetNotifyMonth( cTime);
				int	nNotifyDay = m_cMemoData.GetNotifyDay( cTime);

				if( 0 != nNotifyMonth && nNotifyMonth != cTime.GetMonth())return;
				if( nNotifyDay != cTime.GetDay())return;
			}
			else
			{
				return;
			}
		}
	}

	// 通知時間にマッチ？
	if( m_cMemoData.GetTimerHour() == cTime.GetHour() &&
		m_cMemoData.GetTimerMin() == cTime.GetMinute())
	{
		// 通知した日を記録
		m_nNotifyDay = cTime.GetDay();

		CString	cStrWave;
		if( m_cMemoData.GetNotifySound( cStrWave))
		{
			PlaySound( "Oboe_Alarm", NULL, SND_ALIAS | SND_ASYNC | SND_NODEFAULT | SND_APPLICATION);
		}
		else
		{
			PlaySound( cStrWave, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
		}


		// 先頭でのブリンク通知開始
		StartPopupNotify();

		// タイマでの関連付けはある？
		if( m_cMemoData.IsTimerExec())
		{
			if( m_cMemoData.IsLink())PostMessage( WM_COMMAND, IDM_LINKEXEC, 0L);
		}
	}
	return;
}
#pragma optimize( "atp", off )
#pragma optimize( "", on )

#pragma optimize( "", off )
#pragma optimize( "atp", on )	// 速度重視で最適化
void CMemoChip::DxlCheckTimer( const COleDateTime& cTime)
{
	if( 0 == m_cMemoData.GetDxlID())return;
	if( NULL == m_pcDxlControl)return;

	DWORD dwResult = m_pcDxlControl->CheckTimer();
	if( _TM_CALCRECT & dwResult)
	{
		CalcDefaultRect( FALSE);
	}
	if( _TM_REDRAW & dwResult)
	{
		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			InvalidateRect( NULL);
		}
	}
	if( _TM_NOTIFYBLINK & dwResult)
	{
		StartPopupNotify();
	}
	if( _TM_TONORMAL & dwResult)
	{
		PostMessage( WM_COMMAND, IDM_TONORMAL, 0);
	}
	else
	{
		if( _TM_TOICON & dwResult)
		{
			PostMessage( WM_COMMAND, IDM_TOICON, 0);
		}
		else
		{
			if( _TM_TOTITLE & dwResult)
			{
				PostMessage( WM_COMMAND, IDM_TOTITLE, 0);
			}
		}
	}
}
#pragma optimize( "atp", off )
#pragma optimize( "", on )

BOOL CMemoChip::CheckTimerOnDayChange( const COleDateTime& cTime)
{
	BOOL blUpdate = FALSE;
	blUpdate = CheckExtensiveNotify( cTime);
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
		if( m_pcDxlControl->UpDate( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())))
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			if( !m_pcDxlControl->IsSupportResize() || pcSOboe->IsAutoResize())
			{
				CalcDefaultRect();
			}
			blUpdate = TRUE;
		}
		m_cMemoData.SetDxlExtData( NULL, 0);
	}

	if( blUpdate)InvalidateRect( NULL);

	return TRUE;
}

void CMemoChip::Pack( BOOL blNeedPassWord)
{
	m_blPack = TRUE;
	m_blNeedPassWord = blNeedPassWord;
}

void CMemoChip::CheckOpen( BOOL blCheckOpen)
{
	m_blCheckOpen = blCheckOpen;
}

void CMemoChip::SetResultID( const CString& cStrResultID)
{
	if( m_pcStrResultID)
	{
		delete m_pcStrResultID;
		m_pcStrResultID = NULL;
	}
	m_pcStrResultID = new CString;

	*m_pcStrResultID = cStrResultID;
}

BOOL CMemoChip::ReceiveNotify( void)
{
	// 受信のマーキング
	m_blReceive = TRUE;

	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
	m_nNotifyDay = cTime.GetDay();
	// ブリンク開始指定
	m_blBlink = true;

	return TRUE;
}

void CMemoChip::AddResult( const CString& cStrResult)
{
	if( cStrResult.IsEmpty())return;

	CString	cStrMemo;
	cStrMemo = m_cMemoData.GetMemo();
	cStrMemo += cStrResult;
	m_cMemoData.SetMemo( cStrMemo);

	CalcDefaultRect();
}

// 最前列への移動とブリンク通知
BOOL CMemoChip::StartPopupNotify( void)
{
	if( m_blBlink)return FALSE;

	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
	m_nNotifyDay = cTime.GetDay();
	// 通知前が非表示だったかを記録
	m_blBeforeVisible = ( FALSE != IsWindowVisible());
	// 通知のために表示を行う
	CWnd::ShowWindow( SW_SHOW);
	// ブリンク開始指定
	m_blBlink = true;

	if( m_hParentWnd)::SetForegroundWindow( m_hParentWnd);
	// 自分を先頭へ移動
	m_blOnceTop = true;
	BringWindowToTop();
	// サイズ復元
	if( CMemoData::WINDOW_TYPE_NORMAL != m_cMemoData.GetWindowType())
	{
		// アイコンを復元するが、移動フレームは描画しません
		m_blFrameDraw = false;
		SendMessage( WM_COMMAND, IDM_TONORMAL);
		m_blFrameDraw = true;
	}
	// 小片をセンタリング
	CenterWindow( GetDesktopWindow());
	// 最前列へ移動
	::SetWindowPos( GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	return TRUE;
}

BOOL CMemoChip::EndPopupNotify( void)
{
	if( m_blBlink)
	{
		if( m_blPack)
		{
			m_blBlink = false;
			if( FALSE == OpenPackChip())
			{
				m_blBlink = true;
				return TRUE;
			}
		}
		if( m_blReceive)
		{
			::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_CHECKEDRECEIVE, ( LPARAM)m_unID);
			m_blReceive = FALSE;
			m_nNotifyDay = 0;
		}
		// ブリンク終了
		m_blBlink = false;
		// 非表示は非表示へ
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		if( pcSOboe->IsHideback())
		{
			if( !m_blBeforeVisible)
			{
				if( 0 == ( GetKeyState( VK_SHIFT) & ~1))
				{
					CWnd::ShowWindow( SW_HIDE);
				}
				else
				{
					ShowWindow( SW_SHOW);
				}
			}
			m_blBeforeVisible = ( FALSE != IsWindowVisible());
		}
		// もとの場所へ
		if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
		{
			MoveWindow( m_cMemoData.GetNormalRect());
		}

		// 再描画
		InvalidateRect( NULL);

		// Ｚオーダ復元
		ActiveZOrder();

		return TRUE;
	}

	return FALSE;
}

// パスワードのチェック
BOOL CMemoChip::CheckPassWord( void)
{
	if( m_cMemoData.IsPassWordLock())
	{
		CString	cStrTitle;
		m_cMemoData.GetTitle( cStrTitle);

		CPassWordCheckDialog	cPWCheckDlg( cStrTitle, this);
		do
		{
			if( IDCANCEL == cPWCheckDlg.DoModal())return FALSE;
		}while( m_cMemoData.GetPassWord() != cPWCheckDlg.m_cStrPassWord);
	}
	return TRUE;
}

// OLEによるD&D
BOOL CMemoChip::OleDrop( LPCSTR lpcszData, const POINT& stPoint, BOOL blLink)
{
	CMenu menu;
	CMenu* pPopup;
	CPoint	cMenuPoint = stPoint;
	ClientToScreen( &cMenuPoint);

	VERIFY( menu.LoadMenu( IDR_OLEDROP));
	pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if( m_hParentWnd)::SetForegroundWindow( m_hParentWnd);
	BringWindowToTop();
	if (pPopup != NULL)
	{
		pPopup->EnableMenuItem( IDM_DD_LINKPASTE, blLink ? MF_ENABLED : MF_GRAYED);
		if( pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, cMenuPoint.x, cMenuPoint.y, this, NULL))
		{
			CString	cStr;
			cStr = lpcszData;
			MSG	stMsg;
			while( ::PeekMessage( &stMsg, GetSafeHwnd(), WM_COMMAND, WM_COMMAND, PM_REMOVE))
			{
				if( WM_COMMAND == stMsg.message)
				{
					switch( stMsg.wParam)
					{
					case IDM_DD_PASTE:
						if( FALSE != CheckPassWord())
						{
							PasteMemo( cStr);
						}
						break;
					case IDM_DD_OVERPASTE:
						if( FALSE != CheckPassWord())
						{
							OverWriteMemo( cStr);
						}
						break;
					case IDM_DD_CREATEPASTE:
						CreateMemo( cStr);
						break;
					case IDM_DD_LINKPASTE:
						if( FALSE != CheckPassWord())
						{
							PasteLink( cStr);
						}
						break;
					default:
						DispatchMessage( &stMsg);
						break;
					}
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

// 挿入貼り付け
BOOL CMemoChip::PasteMemo( const CString& cStrData)
{
	if( 0 >= cStrData.GetLength())return FALSE;

	CString	cStr;
	CString	cStrWork = cStrData;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	BOOL blTrim = pcSOboe->IsAutoTriming();
	if( 0 != ( GetKeyState( VK_CONTROL) & ~1))blTrim = !blTrim;
	if( blTrim)
	{
		cStrWork.TrimLeft();
		cStrWork.TrimRight();
	}
	
	m_cStrUndo = m_cMemoData.GetMemo();
	cStr = m_cMemoData.GetMemo() + "\r\n";
	cStr += cStrWork;
	m_cMemoData.SetMemo( cStr);
	if( m_cStrUndo == m_cMemoData.GetMemo())m_cStrUndo.Empty();

	bool	blResize = true;
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
		m_pcDxlControl->UpDate( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
		m_cMemoData.SetDxlExtData( NULL, 0);
		if( m_pcDxlControl->IsSupportResize())blResize = true;
	}

	if( blResize)
	{
		CalcDefaultRect();
	}
	InvalidateRect( NULL);

	// 編集後保存？
	if( pcSOboe->IsModifyAndSave())
	{
		// 保存ね
		::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_SAVEDATAS, ( LPARAM)m_unID);
	}

	return TRUE;
}

// 上書き貼り付け
BOOL CMemoChip::OverWriteMemo( const CString& cStrData, BOOL blNoTriming)
{
	if( 0 >= cStrData.GetLength())return FALSE;

	CString	cStrWork = cStrData;
	if( FALSE == blNoTriming)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		BOOL blTrim = pcSOboe->IsAutoTriming();
		if( 0 != ( GetKeyState( VK_CONTROL) & ~1))blTrim = !blTrim;
		if( blTrim)
		{
			cStrWork.TrimLeft();
			cStrWork.TrimRight();
		}
	}

	m_cStrUndo = m_cMemoData.GetMemo();
	m_cMemoData.SetMemo( cStrWork);
	if( m_cStrUndo == m_cMemoData.GetMemo())m_cStrUndo.Empty();

	bool	blResize = true;
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
		m_pcDxlControl->UpDate( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
		m_cMemoData.SetDxlExtData( NULL, 0);
		if( m_pcDxlControl->IsSupportResize())blResize = true;
	}

	if( blResize)
	{
		CalcDefaultRect();
	}
	InvalidateRect( NULL);

	// 編集後保存？
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe->IsModifyAndSave())
	{
		// 保存ね
		::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_SAVEDATAS, ( LPARAM)m_unID);
	}

	return TRUE;
}

// 貼り付け継承作成
BOOL CMemoChip::CreateMemo( const CString& cStrData)
{
	if( 0 >= cStrData.GetLength())return FALSE;

	CMemoData	cMemoData;

	LOGFONT	stLogFont;
	if( m_cMemoData.GetFont( &stLogFont))
	{
		cMemoData.SetFont( &stLogFont);
	}
	COLORREF	clr;
	if( m_cMemoData.GetForeColor( clr))
	{
		cMemoData.SetForeColor( clr);
	}
	if( m_cMemoData.GetBackColor( clr))
	{
		cMemoData.SetBackColor( clr);
	}
	UINT unDxlID = m_cMemoData.GetDxlID();
	if( 0 != unDxlID && m_pcDxlControl)
	{
		cMemoData.SetDxlID( unDxlID);
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
			UINT unSize = m_cMemoData.GetDxlExtData( NULL, 0);
			if( unSize)
			{
				BYTE*	pabyData;
				pabyData = new BYTE[ unSize];
				m_cMemoData.GetDxlExtData( pabyData, unSize);
				m_cMemoData.SetDxlExtData( NULL, 0);
				cMemoData.SetDxlExtData( pabyData, unSize);
				delete [] pabyData;
			}
		}
	}
	CString	cStrWork = cStrData;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	BOOL blTrim = pcSOboe->IsAutoTriming();
	if( 0 != ( GetKeyState( VK_CONTROL) & ~1))blTrim = !blTrim;
	if( blTrim)
	{
		cStrWork.TrimLeft();
		cStrWork.TrimRight();
	}
	cMemoData.SetMemo( cStrWork);

	::SendMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_CREATE, ( LPARAM)&cMemoData);

	return TRUE;
}

// 関連付け貼り付け
BOOL CMemoChip::PasteLink( const CString& cStrData)
{
	if( 0 >= cStrData.GetLength())return FALSE;

	CString	cStrFile;
	CString	cStrParam;
	int nIndex = cStrData.FindOneOf( "\r\n");
	if( 0 <= nIndex)
	{
		cStrParam = cStrData.Left( nIndex);
	}
	else
	{
		cStrParam = cStrData;
	}

	if( 0 >= cStrParam.GetLength())return FALSE;

	m_cMemoData.Link( TRUE);
	m_cMemoData.SetLinkData( cStrFile, cStrParam);

	// 編集後保存？
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe->IsModifyAndSave())
	{
		// 保存ね
		::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_SAVEDATAS, ( LPARAM)m_unID);
	}

	return TRUE;
}

void CMemoChip::OnPasteAppend() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	if( TRUE == m_blBlink)
	{
		MessageBeep( MB_ICONINFORMATION);
		return;
	}

	CString	cStr;
	if( GetClipboradString( cStr))
	{
		PasteMemo( cStr);
	}
}

void CMemoChip::OnPasteOverwrite() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;

	CString	cStr;
	if( GetClipboradString( cStr))
	{
		OverWriteMemo( cStr);
	}
}

void CMemoChip::OnPasteCreate() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	if( TRUE == m_blBlink)
	{
		MessageBeep( MB_ICONINFORMATION);
		return;
	}

	CString	cStr;
	if( GetClipboradString( cStr))
	{
		CreateMemo( cStr);
	}
}

void CMemoChip::OnPasteLink() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	if( TRUE == m_blBlink)
	{
		MessageBeep( MB_ICONINFORMATION);
		return;
	}

	CString	cStr;
	if( !GetClipboradString( cStr))
	{
		if( !GetClipboradLink( cStr))
		{
			return;
		}
	}
	PasteLink( cStr);
}

// クリップボードからの文字取得
BOOL CMemoChip::GetClipboradString( CString& cStr)
{
	BOOL	blResult = FALSE;

	if( OpenClipboard())
	{
		bool	blResize = false;
		HANDLE hMem = GetClipboardData( CF_TEXT);
		if( NULL != hMem)
		{
			LPSTR lpszData = ( LPSTR)GlobalLock( hMem);
			if( lpszData)
			{
				cStr = lpszData;
				GlobalUnlock( hMem);
				blResult = TRUE;
			}
		}
		CloseClipboard();
	}

	return blResult;
}

// クリップボードからのファイルリンク取得
BOOL CMemoChip::GetClipboradLink( CString& cStr)
{
	BOOL	blResult = FALSE;

	if( OpenClipboard())
	{
		bool	blResize = false;
		HANDLE hMem = GetClipboardData( CF_HDROP);
		if( NULL != hMem)
		{
			char	szPath[ _MAX_PATH];
			DragQueryFile( ( HDROP)hMem, 0, szPath, _MAX_PATH);
			cStr = szPath;
			blResult = TRUE;
		}
		CloseClipboard();
	}

	return blResult;
}

void CMemoChip::OnDelete() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( m_blBlink)
	{
		MessageBeep( MB_ICONHAND);
		return;
	}

	SHORT	stGetKey = ( GetKeyState( VK_SHIFT) & ~1);

	CString	cStrMsg;
	m_cMemoData.GetTitle( cStrMsg);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	WPARAM	wpDelFlag = pcSOboe->IsLoggingDelChip() ? CMainFrame::NMFM_DELSAVELOG : CMainFrame::NMFM_DELSAVE;
	CString	cStr;
	if( stGetKey)
	{
		cStr.Format( IDS_NOSAVEDEL, (LPCSTR)cStrMsg);
		if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON1))return;
		wpDelFlag = CMainFrame::NMFM_DELONLY;
	}
	if( m_cMemoData.IsPassWordLock() && pcSOboe->IsLoggingDelChip())
	{
		cStr.Format( IDS_PWLOCKDEL, (LPCSTR)cStrMsg);
		int nResult = MessageBox( cStr, NULL, MB_YESNOCANCEL | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
		if( IDCANCEL == nResult)return;
		if( IDNO == nResult)wpDelFlag = CMainFrame::NMFM_DELSAVE;
		if( IDYES == nResult)
		{
			if( FALSE == CheckPassWord())return;
			wpDelFlag = CMainFrame::NMFM_DELLOG;
		}
	}
	else
	{
		if( !pcSOboe->IsNoCheckDelete() && !stGetKey)
		{
			cStr.Format( IDS_CHECKDEL, (LPCSTR)cStrMsg);
			if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))return;
		}
	}

	::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), wpDelFlag, ( LPARAM)m_unID);
}

void CMemoChip::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CWnd::PostNcDestroy();

	delete this;
}

void CMemoChip::OnNewmemo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CMemoData cMemoData;

	LOGFONT	stLogFont;
	if( m_cMemoData.GetFont( &stLogFont))
	{
		cMemoData.SetFont( &stLogFont);
	}
	COLORREF	clr;
	if( m_cMemoData.GetForeColor( clr))
	{
		cMemoData.SetForeColor( clr);
	}
	if( m_cMemoData.GetBackColor( clr))
	{
		cMemoData.SetBackColor( clr);
	}
	cMemoData.SetIconIndex( m_cMemoData.GetIconIndex());

	UINT unDxlID = m_cMemoData.GetDxlID();
	if( 0 != unDxlID && m_pcDxlControl)
	{
		cMemoData.SetDxlID( unDxlID);
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
			UINT unSize = m_cMemoData.GetDxlExtData( NULL, 0);
			if( unSize)
			{
				BYTE*	pabyData;
				pabyData = new BYTE[ unSize];
				m_cMemoData.GetDxlExtData( pabyData, unSize);
				m_cMemoData.SetDxlExtData( NULL, 0);
				cMemoData.SetDxlExtData( pabyData, unSize);
				delete [] pabyData;
			}
		}
	}

	// 削除は送信先で行われる
	::SendMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_CREATEEDIT, ( LPARAM)&cMemoData);
}

BOOL CMemoChip::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( this->GetSafeHwnd() == this->GetFocus()->GetSafeHwnd())
	{
		if( ( ( pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) &&	// キーを押した場合
			( pMsg->wParam == VK_F10 && ( GetKeyState( VK_SHIFT) & ~1) != 0)) ||		// Shift+F10 キー
			pMsg->message == WM_CONTEXTMENU)											// またはナチュラル キーボードのキー
		{
			EndPopupNotify();

			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}

		if( WM_KEYDOWN == pMsg->message)
		{
			switch( pMsg->wParam)
			{
			case 'D': // 直接編集
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					CDirectEdit* pcDirectWnd;
					pcDirectWnd = new CDirectEdit( m_cMemoData.GetLogFont(), m_cMemoData.GetForeColor(), m_cMemoData.GetBackColor());
					pcDirectWnd->Create( this);

					CString	cStrText;
					if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
					{
						m_pcDxlControl->GetMemoString( cStrText, m_cMemoData);
					}
					if( cStrText.IsEmpty())
					{
						cStrText = m_cMemoData.GetMemo();
					}

					pcDirectWnd->SetWindowText( cStrText);

					pcDirectWnd->SetFocus();

					return TRUE;
				}
				break;
			case 'Z':	// アンドゥー
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( !m_cStrUndo.IsEmpty())
					{
						PostMessage( WM_COMMAND, IDM_UNDO, 0L);
						return TRUE;
					}
				}
				break;
			case 'C':		// コピー
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_COPY, 0L);
					return TRUE;
				}
				break;
			case 'V':	// 貼り付け
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( 0 != ( GetKeyState( VK_SHIFT) & ~1))
					{
						PostMessage( WM_COMMAND, IDM_PASTE_LINK, 0L);
						return TRUE;
					}
					else
					{
						PostMessage( WM_COMMAND, IDM_PASTE_APPEND, 0L);
						return TRUE;
					}
				}
				break;
			case VK_DELETE:	// 削除
				PostMessage( WM_COMMAND, IDM_DELETE, 0L);
				return TRUE;
			case 'E':		// 編集
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_REEDIT, 0L);
					return TRUE;
				}
				break;
			case 'R':
			case 'P':	// アイコン化あるいは復元
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
					{
						PostMessage( WM_COMMAND, IDM_TOICON, 0L);
					}
					else
					{
						PostMessage( WM_COMMAND, IDM_TONORMAL, 0L);
					}
					return TRUE;
				}
				break;
			case 'T':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
					{
						PostMessage( WM_COMMAND, IDM_TOTITLE, 0L);
					}
					else
					{
						PostMessage( WM_COMMAND, IDM_TONORMAL, 0L);
					}
					return TRUE;
				}
				break;
			case 'W':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
					{
						PostMessage( WM_COMMAND, IDM_SEETHROUGH, 0L);
						return TRUE;
					}
				}
				break;
			case 'G':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					if( m_cMemoData.IsLink())
					{
						PostMessage( WM_COMMAND, IDM_LINKEXEC, 0L);
						return TRUE;
					}
				}
				break;
			case 'N':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_NEWMEMO, 0L);
					return TRUE;
				}
				break;
			case 'S':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_SEND, 0L);
					return TRUE;
				}
				break;
			case 'F':
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_REPLY, 0L);
					return TRUE;
				}
				break;
#ifdef	_KEYCONTROL_ENABLE_
			case VK_LEFT:
			case VK_RIGHT:
			case VK_UP:
			case VK_DOWN:
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					CPoint	cPoint( 0, 0);
					int		nOffset;
					nOffset = ( 0 != ( GetKeyState( VK_SHIFT) & ~1)) ? 10 : 1;
					if( VK_LEFT == pMsg->wParam)cPoint.Offset( -1 * nOffset, 0);
					if( VK_RIGHT == pMsg->wParam)cPoint.Offset( nOffset, 0);
					if( VK_UP == pMsg->wParam)cPoint.Offset( 0, -1 * nOffset);
					if( VK_DOWN == pMsg->wParam)cPoint.Offset( 0, nOffset);

					CRect	cRect;
					GetWindowRect( cRect);
					cRect.OffsetRect( cPoint);
					// 領域の正常化！
					cRect = ClipRectToMonitor( cRect);

					if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
					{
						m_cMemoData.SetNormalRect( cRect);
					}
					else
					{
						if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
						{
							m_cMemoData.SetTitleRect( cRect);
						}
						else
						{
							if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
							{
								m_cMemoData.SetIconRect( cRect);
							}
						}
					}
					MoveWindow( cRect);

					return TRUE;
				}
				break;
			case VK_TAB:
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), ( 0 != ( GetKeyState( VK_SHIFT) & ~1)) ? CMainFrame::NMFM_PREVCHIP : CMainFrame::NMFM_NEXTCHIP, ( LPARAM)m_unID);
					return TRUE;
				}
				break;
#endif	//_KEYCONTROL_ENABLE_
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CMemoChip::OnCopy() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	if( TRUE == m_blBlink)
	{
		MessageBeep( MB_ICONINFORMATION);
		return;
	}

	if( OpenClipboard())
	{
		if( EmptyClipboard())
		{
			CString	cStrText;
			cStrText.Empty();
			if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
			{
				m_pcDxlControl->GetMemoString( cStrText, m_cMemoData);
			}
			if( cStrText.IsEmpty())
			{
				cStrText = m_cMemoData.GetMemo();
			}

			DWORD dwLength = cStrText.GetLength() + 1;
			HGLOBAL hGMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT, dwLength);
			LPSTR lpszData = ( LPSTR)GlobalLock( hGMem);
			lstrcpy( lpszData, cStrText);
			GlobalUnlock( hGMem);

			SetClipboardData( CF_TEXT, hGMem);

		}
		CloseClipboard();
	}
}

void CMemoChip::OnDxlMenu( UINT nID)
{
	if( !CDxlControl::IsUsed())
	{
		if( _DXL_MENUTOP <= nID && _DXL_MENUBOTTOM >= nID)
		{
			if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
			{
				if( m_pcDxlControl->IsSupportMenu())
				{
					DWORD	dwResult;
					dwResult = m_pcDxlControl->DoDrawExMenu( GetSafeHwnd(), nID);
					if( _TM_CALCRECT & dwResult)
					{
						CalcDefaultRect();
					}
					if( _TM_REDRAW & dwResult)
					{
						InvalidateRect( NULL);
					}
				}
			}
		}
	}
	return;
}

void CMemoChip::OnCelMenu( UINT nID)
{
	if( CCelControl::IsUsed())return;
	if( FALSE == CheckPassWord())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	const CCelControl*	pcCelControl = pcSOboe->GetCelControl();
	ASSERT( pcCelControl);

	CMemoData	cMemoData;
	cMemoData = m_cMemoData;
	DWORD dwResult = pcCelControl->DoCel( GetSafeHwnd(), nID, cMemoData);
	if( _CELCTL_ERROR & dwResult)return;
	m_cMemoData = cMemoData;
	m_cMemoData.GetForeColor( m_clrInternalForeColor);
	m_cMemoData.GetBackColor( m_clrInternalBackColor);

	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsSupportNativeData())
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
		m_pcDxlControl->UpDate( GetSafeHwnd(), *this, ( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType()));
		if( m_pcDxlControl->IsSupportNativeData())m_cMemoData.SetDxlExtData( NULL, 0);
	}

	if( _CELCTL_RESIZE & dwResult)
	{
		CalcDefaultRect();
	}
	else
	{
		if( _CELCTL_MOVE & dwResult)
		{
			MoveWindow( cMemoData.GetNormalRect());
		}
	}
	if( _CELCTL_REDRAW & dwResult)
	{
		InvalidateRect( NULL);
	}
}

void CMemoChip::OnceHide( void)
{
	m_blShowBeforeOndeHide = CWnd::ShowWindow( SW_HIDE);
}

void CMemoChip::RestoreOnceHide( void)
{
	if( m_blShowBeforeOndeHide)
	{
		m_blBeforeVisible = TRUE;
		CWnd::ShowWindow( SW_SHOWNOACTIVATE);
	}
}

BOOL CMemoChip::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	return FALSE;//CWnd::OnEraseBkgnd(pDC);
}

void CMemoChip::OnLinkexec() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == m_cMemoData.IsLink())return;
	if( FALSE == CheckPassWord())return;

	CString	cStrFile;
	CString	cStrParam;
	if( FALSE == m_cMemoData.GetLinkData( cStrFile, cStrParam))return;
	if( cStrFile.IsEmpty())
	{
		if( !cStrParam.IsEmpty())
		{
			cStrFile = cStrParam;
		}
		else
		{
			return;
		}
	}
	if( 32 >= ( int)ShellExecute( GetSafeHwnd(), "open", cStrFile, cStrParam, NULL, SW_SHOWNORMAL))
	{
		MessageBeep( MB_ICONHAND);
		return;
	}
}

void CMemoChip::OnDelonquit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	DelOnQuit( !IsDelOnQuit());
}

BOOL CMemoChip::DelOnQuit( BOOL blDelOnQuit)
{
	BOOL blResult = m_blDelOnQuit;
	m_blDelOnQuit = ( FALSE != blDelOnQuit);

	if( ::IsWindow( GetSafeHwnd()))
	{
		if( ( FALSE != blResult) != m_blDelOnQuit)
		{
			if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
			{
				CDC* pcDC = GetDC();
				DrawResizeArea( pcDC, ( this == GetActiveWindow()));
				ReleaseDC( pcDC);
			}
			else
			{
				InvalidateRect( NULL, FALSE);
			}
		}
	}

	return blResult;
}

void CMemoChip::OnMemoexport() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	m_cOleDrop.Revoke();

	char	szFolder[ _MAX_PATH];
	GetCurrentDirectory( _MAX_PATH, szFolder);
	CString	cStrFolder = szFolder;

	CString	cStrData;
	m_cMemoData.GetTitle( cStrData);
	if( -1 != cStrData.FindOneOf( "\\:./|<>?*"))
	{
		GetWindowText( cStrData);
	}

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe)
	{
		CString	cStr;
		pcSOboe->GetExportFolder( cStr);
		if( !cStr.IsEmpty())
		{
			SetCurrentDirectory( cStr);
		}
	}
	
	CString	cStr;
	cStr.LoadString( IDS_EXPORTFILTER);
	CFileDialog	cFileDlg( FALSE, "mrd", cStrData, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, cStr, this);

	if( IDOK == cFileDlg.DoModal())
	{
		CFile	cFile;

		if( cFile.Open( cFileDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
		{
			CString cStrVer;
			cStrVer.LoadString( IDS_VERHEADER);
			int	nVersion = VER_CUR;
			cFile.Write( cStrVer, cStrVer.GetLength());
			cFile.Write( &nVersion, sizeof( int));

			CArchive	cArchive( &cFile, CArchive::store);
			DWORD	dwCount = 1;
			cArchive << dwCount;
			cArchive.WriteObject( this);

			cArchive.Close();
			cFile.Close();
		}
	}
	GetCurrentDirectory( _MAX_PATH, szFolder);
	if( pcSOboe)
	{
		pcSOboe->SetExportFolder( szFolder);
	}
	
	SetCurrentDirectory( cStrFolder);

	m_cOleDrop.Register( this);
}

void CMemoChip::OnTextsave() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;
	m_cOleDrop.Revoke();

	char	szFolder[ _MAX_PATH];
	GetCurrentDirectory( _MAX_PATH, szFolder);
	CString	cStrFolder = szFolder;

	CString	cStrData;
	m_cMemoData.GetTitle( cStrData);
	if( -1 != cStrData.FindOneOf( "\\:./|<>?*"))
	{
		GetWindowText( cStrData);
	}
	
	CString	cStr;
	cStr.LoadString( IDS_TEXTFILTER);
	CFileDialog	cFileDlg( FALSE, "txt", cStrData, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, cStr, this);

	if( IDOK == cFileDlg.DoModal())
	{
		CFile	cFile;

		if( cFile.Open( cFileDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
		{
			cFile.Write( m_cMemoData.GetMemo(), m_cMemoData.GetMemoLength());
			cFile.Close();
		}
	}
	SetCurrentDirectory( cStrFolder);

	m_cOleDrop.Register( this);
}

void CMemoChip::OnCopyMemodata() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( FALSE == CheckPassWord())return;

	if( OpenClipboard())
	{
		if( EmptyClipboard())
		{
			if( 0 != m_cMemoData.GetDxlID())
			{
				if( m_pcDxlControl)
				{
					m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
				}
			}

			int nSize = m_cMemoData.CalcDDDataSize();
			HGLOBAL hGMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT, nSize);
			DDDATA* lpstDDData = ( DDDATA*)GlobalLock( hGMem);

			m_cMemoData.GetDDData( lpstDDData, nSize);

			GlobalUnlock( hGMem);

			SetClipboardData( CMemoData::GetClipboardFormat(), hGMem);

		}
		CloseClipboard();
	}
}

// OLE D&D
DWORD CMemoChip::DoDragDrop( BOOL blMoveEnable)
{
	CRect	cRect;
	GetWindowRect( cRect);

	if( 0 != m_cMemoData.GetDxlID())
	{
		if( m_pcDxlControl)
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
	}

	m_cOleDrop.EnableDrop( FALSE);

	COleDataSource*	pcOleFata;
	pcOleFata = new COleDataSource;

	HGLOBAL hGText = NULL;
	CString	cStrDrag;
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsDataExchange())
		{
			m_pcDxlControl->GetMemoString( cStrDrag, m_cMemoData);
		}
	}
	if( cStrDrag.IsEmpty())
	{
		cStrDrag = m_cMemoData.GetMemo();
	}
	if( !cStrDrag.IsEmpty())
	{			
		int nSize = cStrDrag.GetLength() + 1;
		hGText = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT, nSize);
		if( hGText)
		{
			LPSTR lpsz = ( LPSTR)GlobalLock( hGText);
			if( lpsz)
			{
				lstrcpy( lpsz, cStrDrag);
			}
			GlobalUnlock( hGText);
			pcOleFata->CacheGlobalData( CF_TEXT, hGText);
		}
	}

	HGLOBAL hGMemoData = NULL;
	int nSize = m_cMemoData.CalcDDDataSize();
	hGMemoData = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT, nSize);
	if( hGMemoData)
	{
		DDDATA* lpstDDData = ( DDDATA*)GlobalLock( hGMemoData);
		m_cMemoData.GetDDData( lpstDDData, nSize);
		GlobalUnlock( hGMemoData);
		pcOleFata->CacheGlobalData( CMemoData::GetClipboardFormat(), hGMemoData);
	}

	DWORD dwEffects = ( hGText || hGMemoData) ? pcOleFata->DoDragDrop( blMoveEnable ? ( DROPEFFECT_COPY | DROPEFFECT_MOVE) : DROPEFFECT_COPY, cRect) : DROPEFFECT_NONE ;
	delete pcOleFata;

	GlobalFree( hGMemoData);
	GlobalFree( hGText);

	m_cOleDrop.EnableDrop( TRUE);

	return dwEffects;
}

void CMemoChip::OnPoslock() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_cMemoData.PosLock( !m_cMemoData.IsPosLock());
}

// シースルー用のリージョン作成
CRgn* CMemoChip::CreateSeeThroughRgn( CDC* pcDC, const CRect& cRctClient)
{
	BeginWaitCursor(); // 砂時計カーソルを表示します。

	CRgn* pcRgnSeeThrough = new CRgn;

	CRect	cRect = cRctClient;

	CDC	cDC;
	cDC.CreateCompatibleDC( pcDC);
	CBitmap	cBmp;
	cBmp.CreateCompatibleBitmap( pcDC, cRect.Width(), cRect.Height());
	cDC.SelectObject( &cBmp);

	{
		CString	cStrText;
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsDataExchange())
			{
				m_pcDxlControl->GetMemoString( cStrText, m_cMemoData);
			}
		}
		if( cStrText.IsEmpty())
		{
			cStrText = m_cMemoData.GetMemo();
		}
		// 通常表示形態での描画
		CFont	cFont;
		CFont*	pcFontOld;
		LOGFONT	stLogFont;
		m_cMemoData.GetFont( &stLogFont);
//		stLogFont.lfQuality = DRAFT_QUALITY;
		cFont.CreateFontIndirect( &stLogFont);
		pcFontOld = cDC.SelectObject( &cFont);

		TEXTMETRIC	tm;
		cDC.GetTextMetrics( &tm);
		int nMaxHeight = tm.tmHeight;

		cDC.BeginPath();

		cDC.SetBkMode( TRANSPARENT);

		int		nLength = cStrText.GetLength();
		char	chChar;
		CString	cStrWork;
		CSize	cSize;
		cRect.top		+= ::GetSystemMetrics( SM_CYFRAME);
		cRect.left		+= ::GetSystemMetrics( SM_CXFRAME);
//		cRect.top		+= GetSystemMetrics( SM_CYBORDER);
//		cRect.left		+= GetSystemMetrics( SM_CXBORDER);
		int		nDrawPosX = cRect.left;
		int		nDrawPosY = cRect.top;
		int		nCharCount = 0;
		for( int nIndex = 0; nIndex < nLength; nIndex++)
		{
			chChar = cStrText[ nIndex];
			if( _ismbblead( chChar))
			{
				if( _ismbbtrail( cStrText[ nIndex + 1]))
				{
					cStrWork = chChar;
					cStrWork += cStrText[ nIndex + 1];
					nIndex++;
					cSize = cDC.GetTextExtent( cStrWork);
					if( cRect.Width() < ( nDrawPosX + cSize.cx))
					{
						nDrawPosX = cRect.left;
						nDrawPosY += nMaxHeight;
					}
					cDC.TextOut( nDrawPosX, nDrawPosY, cStrWork);
					nDrawPosX += cSize.cx;
					nCharCount += 2;
					continue;
				}
			}
			if( '\n' == chChar)
			{
				nDrawPosX = cRect.left;
				nDrawPosY += nMaxHeight;
				nCharCount = 0;
				continue;
			}
			if( '\r' == chChar)continue;
			if( '\t' == chChar)
			{
				const int nTab = 8;
				int nTabCount = ( ( ( nCharCount / nTab) + 1) * nTab) - nCharCount;
				TRACE( "CHAR= %d:", nCharCount);
				TRACE( "TAB = %d\n", nTabCount);
				nDrawPosX += ( nTabCount * tm.tmAveCharWidth);
				nCharCount += nTabCount;
				continue;
			}

			cStrWork = chChar;
			cSize = cDC.GetTextExtent( cStrWork);
			if( cRect.Width() < ( nDrawPosX + cSize.cx))
			{
				nDrawPosX = cRect.left;
				nDrawPosY += nMaxHeight;
			}
			cDC.TextOut( nDrawPosX, nDrawPosY, cStrWork);
			nDrawPosX += cSize.cx;
			nCharCount++;
		}
		cDC.EndPath();
		pcRgnSeeThrough->CreateFromPath( &cDC);
	}

	cDC.DeleteDC();
	cBmp.DeleteObject();

	EndWaitCursor(); // 砂時計カーソルを表示します。

	return pcRgnSeeThrough;
}

void CMemoChip::OnSeetrough() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
	{
		if( m_pcDxlControl->IsOwnerDraw())return;
	}

	m_cMemoData.SeeThrough( !m_cMemoData.IsSeeThrough());
	if( m_cMemoData.IsSeeThrough())
	{
		CRect	cRect;
		GetClientRect( cRect);

		CDC*	pcDC = GetDC();
		if( m_pcRgnSeeThrough)
		{
			delete m_pcRgnSeeThrough;
			m_pcRgnSeeThrough = NULL;
		}
		m_pcRgnSeeThrough = CreateSeeThroughRgn( pcDC, cRect);
		ReleaseDC( pcDC);

		HRGN	hRgn;
		hRgn = CreateRectRgn( 0, 0, 0, 0);
		CombineRgn( hRgn, hRgn, *m_pcRgnSeeThrough, RGN_OR);
		SetWindowRgn( hRgn, TRUE);
	}
	else
	{
		SetWindowRgn( NULL, TRUE);
	}
	InvalidateRect( NULL, TRUE);
}

void CMemoChip::OnHide() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ShowWindow( SW_HIDE);
}

BOOL CMemoChip::ShowWindow( int nCmdShow)
{
	m_cMemoData.Show( ( SW_HIDE == nCmdShow) ? FALSE : TRUE);

	return CWnd::ShowWindow( nCmdShow);
}

// ツールチップ
void CMemoChip::CreateToolTip( void)
{
	if( m_pcToolTip)
	{
		CString	cStr;

		if( m_cMemoData.IsPassWordLock() || CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType() || 0 >= m_pcToolTip->GetMaxTipWidth())
		{
			m_cMemoData.GetTitle( cStr);
		}
		else
		{
			if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
			{
				m_pcDxlControl->GetMemoString( cStr, m_cMemoData);
			}
			if( cStr.IsEmpty())
			{
				cStr = m_cMemoData.GetMemo();
				cStr = cStr.Left( TOOLTIPSTR_MAX);
				cStr.Replace( '\t', ' ');
			}
		}

		m_pcToolTip->AddTool( this, cStr);
	}
}

void CMemoChip::ModifyToolTip( LPCSTR lpcsz)
{
	if( m_pcToolTip)
	{
		CString	cStr;
		if( NULL == lpcsz)
		{
			if( m_cMemoData.IsPassWordLock() || CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType() || 0 >= m_pcToolTip->GetMaxTipWidth())
			{
				m_cMemoData.GetTitle( cStr);
			}
			else
			{
				if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
				{
					m_pcDxlControl->GetMemoString( cStr, m_cMemoData);
				}
				if( cStr.IsEmpty())
				{
					cStr = m_cMemoData.GetMemo();
					cStr = cStr.Left( TOOLTIPSTR_MAX);
				}
			}
		}
		m_pcToolTip->UpdateTipText( ( NULL == lpcsz) ? cStr : lpcsz, this);
	}
}

void CMemoChip::DestroyToolTip( void)
{
	if( m_pcToolTip)
	{
		m_pcToolTip->DelTool( this);
	}
}

void CMemoChip::OnUndo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( m_cStrUndo.IsEmpty())return;
	CString	cStr = m_cStrUndo;
	OverWriteMemo( cStr, FALSE);
}

void CMemoChip::OnSend() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CMemoSendSheet::IsUsed())return;
	if( FALSE == CheckPassWord())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( !pcSOboe->IsStandbyNetwork())return;

	m_cOleDrop.Revoke();
	
	if( 0 != m_cMemoData.GetDxlID())
	{
		if( m_pcDxlControl)
		{
			m_pcDxlControl->GetSaveData( GetSafeHwnd(), m_cMemoData);
		}
	}

	CMemoSendSheet	cMemoSend( IDS_CAPTION_SEND, this);

	cMemoSend.SetMemoData( m_cMemoData);
	while( TRUE)
	{
		if( IDOK == cMemoSend.DoModal())
		{
			if( cMemoSend.IsMemoEmpty())
			{
				CString	cStr;
				cStr.LoadString( IDS_MEMOEMPTY);
				if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
				{
					break;
				}
				cMemoSend.SetActivePage( 0);
				continue;
			}
			if( cMemoSend.IsDistributeEmpty())
			{
				CString	cStr;
				cStr.LoadString( IDS_DISTRIBUTEEMPTY);
				if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
				{
					break;
				}
				cMemoSend.SetActivePage( 3);
				continue;
			}
			// すわ！送信
			{
				INNERSENDDATA	stSendData;
				stSendData.m_cMemoData = m_cMemoData;
				cMemoSend.GetSendData( stSendData);

				if( stSendData.m_blPack && ( stSendData.m_blCheckOpen || stSendData.m_blChackResult))
				{
					// 回答を受け取る為のID自分自身に設定
					SetContextID( stSendData.m_cStrResultID);
				}

				CSendWorkerDialog*	pcSendWorker;
				pcSendWorker = new CSendWorkerDialog( this);

				if( pcSendWorker)
				{
					pcSendWorker->SetSendData( stSendData);
					int nCount = cMemoSend.GetEntryCount();
					for( int nIndex = 0; nIndex < nCount; nIndex++)
					{
						pcSendWorker->AddSendEntry( cMemoSend.GetEntryData( nIndex));
					}
					if( pcSendWorker->Create( CSendWorkerDialog::IDD))
					{
						if( cMemoSend.IsDeleteAfterSend())
						{
							if( !stSendData.m_blPack || !( stSendData.m_blCheckOpen || stSendData.m_blChackResult))
							{
								::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_DELSAVE, ( LPARAM)m_unID);
							}
						}
					}
				}
			}
		}
		break;
	}

	m_cOleDrop.Register( this);
}

BOOL CMemoChip::SetEntryData( const CEntryData* pcEntryData)
{
	if( m_pcEntryData)
	{
		delete m_pcEntryData;
		m_pcEntryData = NULL;
	}
	if( NULL == pcEntryData)return FALSE;

	m_pcEntryData = new CEntryData;

	*m_pcEntryData = *pcEntryData;

	return TRUE;
}

const CEntryData* CMemoChip::GetEntryData( void) const
{
	return m_pcEntryData;
}

BOOL CMemoChip::GetMemoData( CMemoData& cMemoData) const
{
	cMemoData = m_cMemoData;

	if( ::IsWindow( GetSafeHwnd()))
	{
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( m_pcDxlControl->IsSupportNativeData())
			{
				m_pcDxlControl->GetSaveData( GetSafeHwnd(), cMemoData);
			}
		}
	}

	return TRUE;
}

BOOL CMemoChip::SetResults( const INNERRESULTS& stResults)
{
	if( !stResults.m_cStrResult1.IsEmpty() && !stResults.m_cStrResult2.IsEmpty())
	{
		m_pstResults = new INNERRESULTS;
		if( m_pstResults)
		{
			m_pstResults->m_cStrResult1		= stResults.m_cStrResult1;
			m_pstResults->m_cStrResult2		= stResults.m_cStrResult2;
			m_pstResults->m_blOtherResult	= stResults.m_blOtherResult;
				m_pstResults->m_cStrResult3	= stResults.m_cStrResult3;
			return TRUE;
		}
	}
	return FALSE;
}

void CMemoChip::OnSender() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( NULL == m_pcEntryData)return;
	if( FALSE == CheckPassWord())return;

	CString	cStrExInfo;
	CString	cStrNxlName;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	int nIndex = pcSOboe->FindNxlID( m_pcEntryData->GetNxlID());
	if( 0 <= nIndex)
	{
		CNetExLib*	pcNetExLib = pcSOboe->GetNetExLib( nIndex);
		if( pcNetExLib)
		{
			pcNetExLib->GetEntryInfo( cStrExInfo, *m_pcEntryData);
			cStrNxlName = pcNetExLib->GetLibName();
		}
	}
	CSenderInfoDialog	cSenderInfo( this);

	cSenderInfo.m_cStrEntryName = m_pcEntryData->GetEntryName();
	cSenderInfo.m_cStrReceiveTime = m_pcReceiveTime->Format( "%H:%M:%S");
	cSenderInfo.m_cStrNxlName = cStrNxlName;
	cSenderInfo.m_cStrExInfo = cStrExInfo;

	cSenderInfo.DoModal();
}

UINT CMemoChip::m_unURL = RegisterClipboardFormat( "UniformResourceLocator");

BOOL CMemoChip::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	if( dropEffect & DROPEFFECT_COPY)
	{
		STGMEDIUM		stG;
		if( pDataObject->IsDataAvailable( CF_TEXT))
		{
			pDataObject->GetData( CF_TEXT, &stG);
			if( 1 == stG.tymed)
			{
				LPCSTR lpsz = ( LPCSTR)GlobalLock( stG.hGlobal);
				OleDrop( lpsz, point, FALSE);
				GlobalUnlock( stG.hGlobal);
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if( dropEffect & DROPEFFECT_LINK)
		{
			STGMEDIUM		stG;
			if( pDataObject->IsDataAvailable( m_unURL))
			{
				pDataObject->GetData( m_unURL, &stG);
				LPCSTR lpsz = ( LPCSTR)GlobalLock( stG.hGlobal);
				OleDrop( lpsz, point, TRUE);
				GlobalUnlock( stG.hGlobal);
			}
			else
			{
				if( pDataObject->IsDataAvailable( CF_HDROP))
				{
					pDataObject->GetData( CF_HDROP, &stG);
					char	szData[ 4096];
					DragQueryFile( ( HDROP)stG.hGlobal, 0, szData, 4096);
					OleDrop( szData, point, TRUE);
				}
				else
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

DROPEFFECT CMemoChip::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return OnDragOver( pDataObject, dwKeyState, point);
}

DROPEFFECT CMemoChip::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	STGMEDIUM		stG;
	if( pDataObject->IsDataAvailable( m_unURL))
	{
		pDataObject->GetData( m_unURL, &stG);
		if( 1 == stG.tymed)return DROPEFFECT_LINK;
	}
	if( pDataObject->IsDataAvailable( CF_HDROP))
	{
		pDataObject->GetData( CF_HDROP, &stG);
		if( 1 == stG.tymed)return DROPEFFECT_LINK;
	}
	if( pDataObject->IsDataAvailable( CF_TEXT))
	{
		pDataObject->GetData( CF_TEXT, &stG);
		if( 1 == stG.tymed)return DROPEFFECT_COPY;
	}

	return DROPEFFECT_NONE;
}

void CMemoChip::OnReply() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( NULL == m_pcEntryData)return;
	if( CMemoSendSheet::IsUsed())return;
	if( FALSE == CheckPassWord())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( !pcSOboe->IsStandbyNetwork())return;

	::PostMessage( m_hParentWnd, CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_SENDREPLY, ( LPARAM)m_unID);
}

BOOL CMemoChip::ArrangeIcon( const POINT& stPoint)
{
	CRect	cRect;

	cRect.left		= stPoint.x;
	cRect.top		= stPoint.y;
	cRect.right		= cRect.left + _ICON_WIDTH + ( ::GetSystemMetrics( SM_CXBORDER) * 2);
	cRect.bottom	= cRect.top + _ICON_HEIGHT + ( ::GetSystemMetrics( SM_CYBORDER) * 2);

	// 領域の正常化！
	cRect = ClipRectToMonitor( cRect);

	m_cMemoData.SetIconRect( cRect);
	if( CMemoData::WINDOW_TYPE_ICON == m_cMemoData.GetWindowType())
	{
		MoveWindow( cRect, TRUE);
	}

	return TRUE;
}

BOOL CMemoChip::ArrangeTitle( const RECT& stRect)
{
	CRect	cRect;

	cRect.left = stRect.left;
	cRect.top = stRect.top;
	if( ::IsWindow( GetSafeHwnd()) && CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
	{
		CRect	cRectNow;
		GetWindowRect( cRectNow);
		cRect.right = cRect.left + cRectNow.Width();
		cRect.bottom = cRect.top + cRectNow.Height();
	}
	else
	{
		cRect.right = cRect.left + m_cMemoData.GetTitleRect().Width();
		cRect.bottom = cRect.top + m_cMemoData.GetTitleRect().Height();
	}

	// 領域の正常化！
	cRect = ClipRectToMonitor( cRect);

	m_cMemoData.SetTitleRect( cRect);
	if( CMemoData::WINDOW_TYPE_TITLE == m_cMemoData.GetWindowType())
	{
		MoveWindow( cRect, TRUE);
	}

	return TRUE;
}

BOOL CMemoChip::ArrangeChip( const RECT& stRect)
{
	if( m_cMemoData.IsPosLock())return TRUE;

	// 領域の正常化！
	CRect	cRect = stRect;
	cRect.right = cRect.left + max( _ICON_WIDTH + ( ::GetSystemMetrics( SM_CXBORDER) * 2), cRect.Width());
	cRect.bottom = cRect.top + max( _ICON_HEIGHT + ( ::GetSystemMetrics( SM_CYBORDER) * 2), cRect.Height());
	cRect = ClipRectToMonitor( cRect);

	m_cMemoData.SetNormalRect( cRect);
	if( CMemoData::WINDOW_TYPE_NORMAL == m_cMemoData.GetWindowType())
	{
		MoveWindow( cRect, TRUE);
	}

	return TRUE;
}

void CMemoChip::SetReceiveTime( const COleDateTime& cReceiveTime)
{
	if( NULL == m_pcReceiveTime)
	{
		m_pcReceiveTime = new COleDateTime;
	}
	*m_pcReceiveTime = cReceiveTime;
}

// Window領域を見えるところにするやつね
const CRect CMemoChip::ClipRectToMonitor( const CRect& cRctSrc)
{
	int	nWidth	= max( cRctSrc.Width(), _ICON_WIDTH);
	int	nHeight	= max( cRctSrc.Height(), _ICON_HEIGHT);

	// 表示位置の調整
	RECT	stWorkArea;

	HMONITOR	hMonitor = MonitorFromRect( cRctSrc, MONITOR_DEFAULTTONEAREST);
	MONITORINFO	stMonitorInfo;
	stMonitorInfo.cbSize = sizeof( MONITORINFO);
	if( GetMonitorInfo( hMonitor, &stMonitorInfo))
	{
		stWorkArea = stMonitorInfo.rcWork;
	}
	else
	{
		SystemParametersInfo( SPI_GETWORKAREA, 0, &stWorkArea, 0);
	}

	int nXMin = stWorkArea.left - ( nWidth - _ICON_WIDTH);
	int nXMax = stWorkArea.right - _ICON_WIDTH;
	int nYMin = stWorkArea.top - ( nHeight - _ICON_HEIGHT);
	int nYMax = stWorkArea.bottom - _ICON_HEIGHT;

	int nXPos = max( min( nXMax, cRctSrc.left), nXMin);
	int nYPos = max( min( nYMax, cRctSrc.top), nYMin);

	CRect cRctDist = cRctSrc;
	cRctDist.left = nXPos;
	cRctDist.right = nXPos + nWidth;
	cRctDist.top = nYPos;
	cRctDist.bottom = nYPos + nHeight;

	return cRctDist;
}

void CMemoChip::OnAlphaBlend( UINT nID)
{
	if( IsSupportAlphaBlend())
	{
		// DxlがAlphaBlendを禁止している
		if( 0 != m_cMemoData.GetDxlID() && NULL != m_pcDxlControl)
		{
			if( TRUE == m_pcDxlControl->IsNoAlphaBlend())return;
		}

		BYTE	byAlpha = 255;
		switch( nID)
		{
		case IDM_ABLEND_075:
			byAlpha = CMemoData::ALPHABLEND_075;
			break;
		case IDM_ABLEND_050:
			byAlpha = CMemoData::ALPHABLEND_050;
			break;
		case IDM_ABLEND_025:
			byAlpha = CMemoData::ALPHABLEND_025;
			break;
		case IDM_ABLEND_FREE:
#ifdef _ALPHABLEND
			{
				if( !CAlphaBlendDialog::IsUsed())
				{
					byAlpha = m_cMemoData.GetAlphaBlend();

					CAlphaBlendDialog	cABDlg( this);
					cABDlg.SetAlphaBlend( byAlpha);
					if( IDOK == cABDlg.DoModal())
					{
						byAlpha = cABDlg.GetAlphaBlend();
						if( 255 > byAlpha)break;
					}
				}
				else
				{
					MessageBeep( -1);
					HWND hWnd = CAlphaBlendDialog::GetUseWnd();
					if( NULL != hWnd)
					{
						::SetForegroundWindow( hWnd);
					}
					return;
				}
			}
#endif
		case IDM_ABLEND_000:
		default:
			m_cMemoData.SetAlphaBlend( CMemoData::ALPHABLEND_000);
			ModifyStyleEx( WS_EX_LAYERED, 0, SWP_DRAWFRAME);
			return;
		}

		m_cMemoData.SetAlphaBlend( byAlpha);
		ModifyStyleEx( 0, WS_EX_LAYERED);
		SetLayeredWindowAttributes( 0, byAlpha, LWA_ALPHA);
	}
}

BOOL CMemoChip::SetLayeredWindowAttributes( COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	if( m_lpfnSetLayeredWindowAttributes)
	{
		if( NULL != GetSafeHwnd() && IsWindow( GetSafeHwnd()))
		{
			return m_lpfnSetLayeredWindowAttributes( GetSafeHwnd(), crKey, bAlpha, dwFlags);
		}
	}
	return FALSE;
}

BOOL CMemoChip::IsSupportAlphaBlend( void)
{
	return ( NULL != m_lpfnSetLayeredWindowAttributes);
}
