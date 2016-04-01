// BaseEditPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "BaseEditPage.h"
#include "DrawExLib.h"
#include "MainFrm.h"
#include "MemoEditSheet.h"
#include "baseeditpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseEditPage プロパティ ページ

IMPLEMENT_DYNCREATE(CBaseEditPage, CPropertyPage)

CBaseEditPage::CBaseEditPage() : CPropertyPage(CBaseEditPage::IDD)
{
	//{{AFX_DATA_INIT(CBaseEditPage)
	m_cStrMemo = _T("");
	m_cStrTitle = _T("");
	m_blResize = FALSE;
	m_blDelOnQuit = FALSE;
	//}}AFX_DATA_INIT

	m_blLogFont = FALSE;
	m_blForeColor = FALSE;
	m_blBackColor = FALSE;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe)
	{
		pcSOboe->GetDefaultFont( &m_stLogFont);
		m_clrFore = pcSOboe->GetDefaultForeColor();
		m_clrBack = pcSOboe->GetDefaultBackColor();
		m_nIconIndex = pcSOboe->GetDefaultIconIndex();
		m_blResize = pcSOboe->IsAutoResize();
	}

	m_unDxlID = 0;
	m_unDxlExtDataSize = 0;
	m_pabyDxlExtData = NULL;
	m_blResizeLock = FALSE;

	m_pcIcon = NULL;
}

CBaseEditPage::~CBaseEditPage()
{
	if( m_pabyDxlExtData)
	{
		delete [] m_pabyDxlExtData;
		m_pabyDxlExtData = NULL;
	}
	m_unDxlExtDataSize = 0;
}

void CBaseEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseEditPage)
	DDX_Control(pDX, IDC_ICONSEL, m_cBtnIconSel);
	DDX_Control(pDX, IDC_CALCRESIZE, m_cBtnResize);
	DDX_Control(pDX, IDC_DXLEDITHELPER, m_cBtnEditHelper);
	DDX_Control(pDX, IDC_MEMOEDIT, m_cEdtMemo);
	DDX_Text(pDX, IDC_MEMOEDIT, m_cStrMemo);
	DDX_Text(pDX, IDC_TITLE, m_cStrTitle);
	DDX_Check(pDX, IDC_CALCRESIZE, m_blResize);
	DDX_Check(pDX, IDC_DELONQUIT, m_blDelOnQuit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBaseEditPage)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DXLEDITHELPER, OnDxledithelper)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_ICONSEL, OnIconsel)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE( IDM_TEMPLET0MENU, IDM_TEMPLET9MENU, OnTemplteMenu)
	ON_MESSAGE( WM_USER + 100, OnSetFocusDelay)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseEditPage メッセージ ハンドラ

// WM_INITDIALOGでうまくフォーカスを設定できなかったので、
// デレイで処理することに……
// 本当はRegisterWindowMessageを使おうかとも考えたけど、
// なんか、無駄っぽいのでヤメヤメ。
LRESULT CBaseEditPage::OnSetFocusDelay( WPARAM wParam, LPARAM lParam)
{
	if( ( HWND)wParam == m_cEdtMemo.GetSafeHwnd())
	{
		m_cEdtMemo.SetFocus();
		return 0;
	}
	else
	{
		return DefWindowProc( WM_USER + 100, wParam, lParam);
	}
}

void CBaseEditPage::OnFont() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CFontDialog	cFontDlg( &m_stLogFont, CF_EFFECTS | CF_SCREENFONTS, NULL, this);

	cFontDlg.m_cf.rgbColors = m_clrFore;

	if( IDOK == cFontDlg.DoModal())
	{
		m_clrFore = cFontDlg.GetColor();
		m_blForeColor = TRUE;
		m_blLogFont = TRUE;

		// 編集領域へフォントを設定
		m_cFntEdit.DeleteObject();
		m_cFntEdit.CreateFontIndirect( &m_stLogFont);
		m_cEdtMemo.SetFont( &m_cFntEdit);
		m_cEdtMemo.SetMargins( 0, 0);
	}
}

void CBaseEditPage::OnColor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog	cColorDlg( m_clrBack, 0, this);
	
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->GetUserColor( cColorDlg.m_cc.lpCustColors);
	if( IDOK == cColorDlg.DoModal())
	{
		m_clrBack = cColorDlg.GetColor();
		m_blBackColor = TRUE;

		pcSOboe->SetUserColor( cColorDlg.GetSavedCustomColors());

		// 編集領域のブラシ作成
		m_cBrhEdit.DeleteObject();
		CDC*	pcDC;
		pcDC = GetDC();
		m_cBrhEdit.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
		ReleaseDC( pcDC);
		m_cEdtMemo.InvalidateRect( NULL);
	}
}

HBRUSH CBaseEditPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: この位置で DC のアトリビュートを変更してください
	HBRUSH hbr;
	if( ( CTLCOLOR_EDIT == nCtlColor || CTLCOLOR_MSGBOX == nCtlColor) && ( pWnd->GetSafeHwnd() == m_cEdtMemo.GetSafeHwnd()))
	{
		hbr = m_cBrhEdit;
		pDC->SetBkColor( m_clrBack);
		pDC->SetTextColor( m_clrFore);
	}
	else
	{
		// TODO: この位置で DC のアトリビュートを変更してください
		hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

BOOL CBaseEditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( IsWindow( m_cEdtMemo.GetSafeHwnd()) && !pcSOboe->IsAutoScroll())
	{
		DWORD dwStyle = GetWindowLong( m_cEdtMemo.GetSafeHwnd(), GWL_STYLE);
		DWORD dwStyleEx = GetWindowLong( m_cEdtMemo.GetSafeHwnd(), GWL_EXSTYLE);
		CRect	cRect;
		CString	cStr;
		m_cEdtMemo.GetWindowText(cStr);
		m_cEdtMemo.GetWindowRect( cRect);
		char	szClass[ 512];
		GetClassName( m_cEdtMemo.GetSafeHwnd(), szClass, 512);
		ScreenToClient( cRect);
		dwStyle ^= ( ES_AUTOHSCROLL | WS_HSCROLL);
		m_cEdtMemo.DestroyWindow();
		m_cEdtMemo.CreateEx( dwStyleEx, szClass, NULL, dwStyle, cRect.left, cRect.top, cRect.Width(), cRect.Height(), GetSafeHwnd(), ( HMENU)IDC_MEMOEDIT, NULL);
		m_cEdtMemo.SetWindowPos( &wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOSIZE);
		m_cEdtMemo.SetWindowText(cStr);
	}

	const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
	if( pcIconImgs)
	{
		CDC *pcDC = m_cBtnIconSel.GetDC();
		if( pcDC)
		{
			CDC	cMemDC;
			if( cMemDC.CreateCompatibleDC( pcDC))
			{
				m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
				cMemDC.SelectObject( &m_cBmpIcon);
				pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

				cMemDC.DeleteDC();

				m_cBtnIconSel.SetBitmap( m_cBmpIcon);
			}
			m_cBtnIconSel.ReleaseDC( pcDC);
		}
	}

	// 編集エリア用のブラシ作成
	m_cBrhEdit.DeleteObject();
	CDC*	pcDc;
	pcDc = GetDC();
	m_cBrhEdit.CreateSolidBrush( pcDc->GetNearestColor( m_clrBack));
	ReleaseDC( pcDc);
	
	// デフォルトのフォントの設定
	m_cFntEdit.DeleteObject();
	m_cFntEdit.CreateFontIndirect( &m_stLogFont);
	m_cEdtMemo.SetFont( &m_cFntEdit);
	m_cEdtMemo.SetMargins( 0, 0);

	m_cBtnResize.EnableWindow( !m_blResizeLock);

	m_unOldDxlID = m_unDxlID;

	if( IsWindow( m_cEdtMemo.GetSafeHwnd()) && !pcSOboe->IsAutoScroll())
	{
		int nLine = m_cEdtMemo.GetLineCount();
		m_cEdtMemo.LineScroll( nLine);
		//m_cEdtMemo.SetFocus();
		PostMessage( WM_USER + 100, ( WPARAM)m_cEdtMemo.GetSafeHwnd(), 0);
		return FALSE;
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CBaseEditPage::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	BOOL	blEnable = FALSE;
	if( 0 != m_unDxlID)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		int nIndex = pcSOboe->FindDxlID( m_unDxlID);
		if( 0 <= nIndex)
		{
			const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				if( pcDrawExLib->IsSupportEditHelper())
				{
					blEnable = TRUE;
				}
			}
		}
	}
	m_cBtnEditHelper.EnableWindow( blEnable);

	m_cEdtMemo.PostMessage( EM_SETSEL, -1, -1);
	m_cEdtMemo.PostMessage( EM_SCROLLCARET, 0, 0);

	return CPropertyPage::OnSetActive();
}

void CBaseEditPage::OnDxledithelper() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 0 != m_unDxlID)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		int nIndex = pcSOboe->FindDxlID( m_unDxlID);
		if( 0 <= nIndex)
		{
			const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				if( pcDrawExLib->IsSupportEditHelper())
				{
					DxlEditSupport( pcDrawExLib);
				}
			}
		}
	}
}

BOOL CBaseEditPage::DxlEditSupport( const CDrawExLib* pcDrawExLib)
{
	if( m_unDxlID != m_unOldDxlID)
	{
		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		m_unDxlExtDataSize = 0;
		m_unOldDxlID = m_unDxlID;
	}

	UpdateData();
	EDITDATA	stEdit;
	stEdit.m_nSelStart = 0;
	stEdit.m_nSelEnd = 0;
	stEdit.m_hEdit = NULL;
	if( !m_cStrMemo.IsEmpty())
	{
		m_cEdtMemo.GetSel( stEdit.m_nSelStart, stEdit.m_nSelEnd);
		stEdit.m_hEdit = GlobalAlloc( GHND, m_cStrMemo.GetLength() + 1);
		LPSTR lpsz = ( LPSTR)GlobalLock( stEdit.m_hEdit);
		lstrcpy( lpsz, m_cStrMemo);
		GlobalUnlock( stEdit.m_hEdit);
	}
	if( pcDrawExLib->IsSupportNativeData())
	{
		stEdit.m_nSize = m_unDxlExtDataSize;
		if( stEdit.m_nSize)
		{
			stEdit.m_hSaveData = GlobalAlloc( GHND, stEdit.m_nSize);
			LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEdit.m_hSaveData);
			CopyMemory( lpbyData, m_pabyDxlExtData, m_unDxlExtDataSize);
			GlobalUnlock( stEdit.m_hSaveData);
		}
		else
		{
			stEdit.m_hSaveData = NULL;
		}
	}
	else
	{
		stEdit.m_nSize = 0;
		stEdit.m_hSaveData = NULL;
	}
	if( IDOK == pcDrawExLib->DoEditHelper( GetSafeHwnd(), &stEdit))
	{
		LPSTR lpsz = ( LPSTR)GlobalLock( stEdit.m_hEdit);
		m_cStrMemo = lpsz;
		GlobalUnlock( stEdit.m_hEdit);
		UpdateData( FALSE);
		m_cEdtMemo.SetSel( stEdit.m_nSelStart, stEdit.m_nSelEnd);

		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		if( pcDrawExLib->IsSupportNativeData())
		{
			m_unDxlExtDataSize = stEdit.m_nSize;
			if( m_unDxlExtDataSize)
			{
				LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEdit.m_hSaveData);

				m_pabyDxlExtData = new BYTE [ m_unDxlExtDataSize];
				CopyMemory( m_pabyDxlExtData, lpbyData, m_unDxlExtDataSize);
				GlobalUnlock( stEdit.m_hSaveData);
			}
		}
	}
	if( stEdit.m_hEdit)GlobalFree( stEdit.m_hEdit);
	if( stEdit.m_hSaveData)GlobalFree( stEdit.m_hSaveData);
	return TRUE;
}

void CBaseEditPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( UpdateData())
	{
		// 描画拡張データに矛盾が有れば削除する
		if( m_unOldDxlID != m_unDxlID)
		{
			if( m_pabyDxlExtData)
			{
				delete [] m_pabyDxlExtData;
				m_pabyDxlExtData = NULL;
			}
			m_unDxlExtDataSize = 0;
			m_unOldDxlID = m_unDxlID;
		}
		m_cStrMemo4IF = m_cStrMemo;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		BOOL blTrim = pcSOboe->IsAutoTriming();
		if( 0 != ( GetKeyState( VK_CONTROL) & ~1))blTrim = !blTrim;
		if( blTrim)
		{
			m_cStrMemo4IF.TrimLeft();
			m_cStrMemo4IF.TrimRight();
		}
	}

	CPropertyPage::OnOK();
}

BOOL CBaseEditPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( WM_KEYDOWN == pMsg->message)
	{
		if( m_cEdtMemo == *GetFocus())
		{
			switch( pMsg->wParam)
			{
			case 'T':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_TIME);
					return TRUE;
				}
				break;
			case 'D':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_DATE);
					return TRUE;
				}
				break;
			case 'Q':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDC_DXLEDITHELPER, 0L);
				}
				break;
			case 'W':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_DAYOFWEEK);
					return TRUE;
				}
				break;
			case 'S':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					DWORD dwStyle = GetWindowLong( m_cEdtMemo.GetSafeHwnd(), GWL_STYLE);
					DWORD dwStyleEx = GetWindowLong( m_cEdtMemo.GetSafeHwnd(), GWL_EXSTYLE);
					CRect	cRect;
					CString	cStr;
					int	nStartSel, nEndSel;
					m_cEdtMemo.GetSel( nStartSel, nEndSel);
					m_cEdtMemo.GetWindowText(cStr);
					m_cEdtMemo.GetWindowRect( cRect);
					char	szClass[ 512];
					GetClassName( m_cEdtMemo.GetSafeHwnd(), szClass, 512);
					ScreenToClient( cRect);
					m_cEdtMemo.DestroyWindow();

					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);
					if( ES_AUTOHSCROLL & dwStyle)
					{
						dwStyle ^= ( ES_AUTOHSCROLL | WS_HSCROLL);
						pcSOboe->AutoScroll( FALSE);
					}
					else
					{
						dwStyle |= ( ES_AUTOHSCROLL | WS_HSCROLL);
						pcSOboe->AutoScroll( TRUE);
					}
					m_cEdtMemo.CreateEx( dwStyleEx, szClass, cStr, dwStyle, cRect.left, cRect.top, cRect.Width(), cRect.Height(), GetSafeHwnd(), ( HMENU)IDC_MEMOEDIT, NULL);
					m_cEdtMemo.SetFont( &m_cFntEdit);
					m_cEdtMemo.SetFocus();
					m_cEdtMemo.SetSel( nStartSel, nEndSel);
					return TRUE;
				}
				break;
			case VK_RETURN:
			case VK_ESCAPE:
				if( 0 == ( GetAsyncKeyState( VK_SHIFT) & ~1))break;
				GetParent()->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CBaseEditPage::InsertTime( int nType)
{
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	CString	cStrSendTime;
	switch( nType)
	{
	case _INSERT_TIME:
		cStrSendTime = cTime.Format( "%H:%M:%S");
		break;
	case _INSERT_DATE:
		cStrSendTime = cTime.Format( "%y/%m/%d");
		break;
	case _INSERT_DAYOFWEEK:
		cStrSendTime = cTime.Format( "(%a)");
		break;
	}

	m_cEdtMemo.ReplaceSel( cStrSendTime, TRUE);
}

void CBaseEditPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	int nCount = pcSOboe->GetTempletCount();
	if( 0 < nCount)
	{
		CMenu	cTempletMemu;
		if( cTempletMemu.CreatePopupMenu())
		{
			CString		cStr;
			CString		cStrMenu;

			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				if( pcSOboe->GetTempletName( nIndex, cStr))
				{
					cStrMenu.Format( "(&%d) %s", nIndex, cStr);
					cTempletMemu.AppendMenu( MF_BYCOMMAND, IDM_TEMPLET0MENU + nIndex, cStrMenu);
				}
			}
			cTempletMemu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
		}
	}
}

void CBaseEditPage::OnTemplteMenu( UINT nID)
{
	if( IDM_TEMPLET0MENU > nID || IDM_TEMPLET9MENU < nID)return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CString	cStrTempletMsg;
	cStrTempletMsg.LoadString( IDS_TEMPLETMSG);
	if( IDOK != MessageBox( cStrTempletMsg, NULL, MB_OKCANCEL | MB_ICONQUESTION))return;

	int nIndex = nID - IDM_TEMPLET0MENU;
	if( nIndex < pcSOboe->GetTempletCount())
	{
		const CMemoData* pcMemoData = pcSOboe->GetTemplet( nIndex);
		ASSERT( pcMemoData);
		if( pcMemoData)
		{
			CMemoEditSheet*	pcParent = ( CMemoEditSheet*)GetParent();
			pcParent->SetMemoData( *pcMemoData);
			{
				// 編集エリア用のブラシ作成
				m_cBrhEdit.DeleteObject();
				CDC*	pcDc;
				pcDc = GetDC();
				m_cBrhEdit.CreateSolidBrush( pcDc->GetNearestColor( m_clrBack));
				ReleaseDC( pcDc);
				
				// デフォルトのフォントの設定
				m_cFntEdit.DeleteObject();
				m_cFntEdit.CreateFontIndirect( &m_stLogFont);
				m_cEdtMemo.SetFont( &m_cFntEdit);
				m_cEdtMemo.SetMargins( 0, 0);

				m_cBtnResize.EnableWindow( !m_blResizeLock);

				m_unOldDxlID = m_unDxlID;

				BOOL	blEnable = FALSE;
				if( 0 != m_unDxlID)
				{
					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					int nIndex = pcSOboe->FindDxlID( m_unDxlID);
					if( 0 <= nIndex)
					{
						const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
						if( pcDrawExLib)
						{
							if( pcDrawExLib->IsSupportEditHelper())
							{
								blEnable = TRUE;
							}
						}
					}
				}
				m_cBtnEditHelper.EnableWindow( blEnable);

				m_cEdtMemo.PostMessage( EM_SETSEL, -1, -1);
				m_cEdtMemo.PostMessage( EM_SCROLLCARET, 0, 0);
			}
			UpdateData( FALSE);
		}
	}
}

void CBaseEditPage::OnIconsel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//m_cBtnIconSel.SetCheck( 1);
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	const CIconImgList* pcIconImgs = pcSOboe->GetIconList();

	if( pcIconImgs)
	{
		CRect cRect;
		m_cBtnIconSel.GetWindowRect( cRect);

		m_pcIcon = pcIconImgs->CreateIconSelectMenu( this, FALSE);

		UINT unMenuCMD = m_pcIcon->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this);
		if( NULL != m_pcIcon)
		{
			delete m_pcIcon;
			m_pcIcon = NULL;
		}
		/*
		UINT unMenuCMD = pcIconImgs->TrackPopupMenu( cRect.left, cRect.bottom, this);
		*/

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nIconIndex = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CDC *pcDC = m_cBtnIconSel.GetDC();
			if( pcDC)
			{
				CDC	cMemDC;
				if( cMemDC.CreateCompatibleDC( pcDC))
				{
					if( NULL == m_cBmpIcon.GetSafeHandle())
					{
						m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					}
					cMemDC.SelectObject( &m_cBmpIcon);
					pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));
					cMemDC.DeleteDC();

					m_cBtnIconSel.SetBitmap( m_cBmpIcon);
				}
				ReleaseDC( pcDC);
			}
		}
		/*
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, NULL, 0, 0, PM_REMOVE));
		}*/
	}
	//m_cBtnIconSel.SetCheck( 0);

	/*
	m_cBtnIconSel.SetCheck( 1);
	CIconSelectmenu	cPopIcon;
	if( cPopIcon.CreatePopupMenu())
	{
		UINT	unMenuCMD;

		CRect cRect;
		m_cBtnIconSel.GetWindowRect( cRect);

		unMenuCMD = cPopIcon.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nIconIndex = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
			if( pcIconImgs)
			{
				CDC *pcDC = m_cBtnIconSel.GetDC();
				if( pcDC)
				{
					CDC	cMemDC;
					if( cMemDC.CreateCompatibleDC( pcDC))
					{
						if( NULL == m_cBmpIcon.GetSafeHandle())
						{
							m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
						}
						cMemDC.SelectObject( &m_cBmpIcon);
						pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

						m_cBtnIconSel.SetBitmap( m_cBmpIcon);

						cMemDC.DeleteDC();
					}
					ReleaseDC( pcDC);
				}
			}
		}
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, GetSafeHwnd(), 0, 0, PM_REMOVE));
		}
	}
	m_cBtnIconSel.SetCheck( 0);
	*/
}

void CBaseEditPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( NULL != m_pcIcon)
	{
		m_pcIcon->DrawItem( lpDrawItemStruct);

		return;
	}

	CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CBaseEditPage::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( NULL != m_pcIcon)
	{
		m_pcIcon->MeasureItem( lpMeasureItemStruct);

		return;
	}

	CPropertyPage::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
