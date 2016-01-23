// DataListDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "DataListDialog.h"
#include "MemoChipList.h"
#include "PrintChipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataListDialog ダイアログ


CDataListDialog::CDataListDialog( CMemoChipList* pcMemoChipList, CWnd* pParent /*=NULL*/)
	: CDialog(CDataListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataListDialog)
	m_blReverse = FALSE;
	m_blFilter = FALSE;
	m_blCentering = FALSE;
	//}}AFX_DATA_INIT
	m_pcMemoChipList = pcMemoChipList;
	m_nFilter = -1;
	m_clrFore = GetSysColor( COLOR_WINDOWTEXT);
	m_clrBack = GetSysColor( COLOR_WINDOW);
	m_cBrshBack.CreateSolidBrush( m_clrBack);

	CSOboeApp*	pcSOboeApp = ( CSOboeApp*)AfxGetApp();
	if( pcSOboeApp)
	{
		m_nFilterIcon = pcSOboeApp->GetDataListFilter();
		m_blReverse = pcSOboeApp->IsDataListReverse();
	}

	m_pcIcon = NULL;
}

CDataListDialog::~CDataListDialog()
{
	CSOboeApp*	pcSOboeApp = ( CSOboeApp*)AfxGetApp();
	if( pcSOboeApp)
	{
		pcSOboeApp->SetDataListFilter( m_nFilterIcon);
		pcSOboeApp->DataListReverse( m_blReverse);
	}
}

void CDataListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataListDialog)
	DDX_Control(pDX, IDC_REVERSE, m_cBtnReverse);
	DDX_Control(pDX, IDC_FILTER, m_cBtnFilter);
	DDX_Control(pDX, IDOK, m_cBtnOK);
	DDX_Control(pDX, IDC_DELETE, m_cBtnDelete);
	DDX_Control(pDX, IDC_ITEMUP, m_cBtnItemUp);
	DDX_Control(pDX, IDC_ITEMDOWN, m_cBtnItemDown);
	DDX_Control(pDX, IDC_PREVIEW, m_cEdtPreview);
	DDX_Control(pDX, IDC_DATALIST, m_cLstMemoDatas);
	DDX_Control(pDX, IDC_ICONFILTER, m_cBtnIconFilter);
	DDX_Check(pDX, IDC_REVERSE, m_blReverse);
	DDX_Check(pDX, IDC_FILTER, m_blFilter);
	DDX_Check(pDX, IDC_CENTER, m_blCentering);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataListDialog, CDialog)
	//{{AFX_MSG_MAP(CDataListDialog)
	ON_LBN_SELCHANGE(IDC_DATALIST, OnSelchangeDatalist)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_FILTER, OnFilter)
	ON_CBN_SELCHANGE(IDC_ICONFILTER, OnSelchangeIconfilter)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ITEMDOWN, OnItemdown)
	ON_BN_CLICKED(IDC_ITEMUP, OnItemup)
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_LINKEXEC, OnLinkexec)
	ON_COMMAND(IDM_SEETHROUGH, OnSeethrough)
	ON_COMMAND(IDM_DELONQUIT, OnDelonquit)
	ON_WM_HELPINFO()
	ON_COMMAND(IDM_PRINTCHIP, OnPrintchip)
	ON_COMMAND(IDC_SAVETEXT, OnSavetext)
	ON_BN_CLICKED(IDC_ICONFILTER, OnIconfilter)
	ON_LBN_DBLCLK(IDC_DATALIST, OnOK)
	//}}AFX_MSG_MAP
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataListDialog メッセージ ハンドラ

void CDataListDialog::SetListData( int nFilterIcon, BOOL blReverse)
{
	CSingleLock cSglk( &m_pcMemoChipList->m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nTopIndex = m_cLstMemoDatas.GetTopIndex();
		int	nSelIndex = m_cLstMemoDatas.GetCaretIndex();
		m_cLstMemoDatas.SetRedraw( FALSE);
		m_cLstMemoDatas.ResetContent();
		m_cIndexArray.RemoveAll();

		CMemoChip*	pcMemoChip;
		m_nMemoCount = m_pcMemoChipList->GetCount();
		for( int nIndex = 0; nIndex < m_nMemoCount; nIndex++)
		{
			pcMemoChip = m_pcMemoChipList->GetAt( m_pcMemoChipList->FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( -1 != nFilterIcon)
				{
					const CMemoData* pcMemoData;
					pcMemoData = pcMemoChip->AccessMemoData();
					if( nFilterIcon != pcMemoData->GetIconIndex())continue;
				}
				if( blReverse)
				{
					m_cLstMemoDatas.InsertMemoChip( 0, *pcMemoChip);
					m_cIndexArray.InsertAt( 0, nIndex);
				}
				else
				{
					m_cLstMemoDatas.AddMemoChip( *pcMemoChip);
					m_cIndexArray.Add( nIndex);
				}
			}
		}
		cSglk.Unlock();

		m_cLstMemoDatas.SetTopIndex( nTopIndex);
		m_cLstMemoDatas.SetCaretIndex( nSelIndex, TRUE);
		m_cLstMemoDatas.SetSel( nSelIndex, TRUE);
		OnSelchangeDatalist();
	
		m_cLstMemoDatas.SetRedraw( TRUE);
	}
}

BOOL CDataListDialog::MemoItemUp( int nPos)
{
	if( 0 >= nPos)return FALSE;

	CSingleLock cSglk( &m_pcMemoChipList->m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nMax = m_pcMemoChipList->GetCount();
		if( nMax <= nPos)return FALSE;

		POSITION	Pos = m_pcMemoChipList->FindIndex( nPos);
		CMemoChip*	pcMemoChip = m_pcMemoChipList->GetAt( Pos);
		m_pcMemoChipList->RemoveAt( Pos);
		m_pcMemoChipList->InsertBefore( m_pcMemoChipList->FindIndex( nPos - 1), pcMemoChip);

		return TRUE;
	}
	return FALSE;
}

BOOL CDataListDialog::MemoItemDown( int nPos)
{
	CSingleLock cSglk( &m_pcMemoChipList->m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nMax = m_pcMemoChipList->GetCount();
		if( nMax <= nPos)return FALSE;

		POSITION	Pos = m_pcMemoChipList->FindIndex( nPos);
		CMemoChip*	pcMemoChip = m_pcMemoChipList->GetAt( Pos);
		m_pcMemoChipList->RemoveAt( Pos);
		m_pcMemoChipList->InsertAfter( m_pcMemoChipList->FindIndex( nPos), pcMemoChip);

		return TRUE;
	}
	return FALSE;
}


BOOL CDataListDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cBtnIconFilter.EnableWindow( FALSE);
	SetListData( m_nFilter, m_blReverse);

	SetTimer( 9801, 200, NULL);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
	if( pcIconImgs)
	{
		CDC *pcDC = m_cBtnIconFilter.GetDC();
		if( pcDC)
		{
			CDC	cMemDC;
			if( cMemDC.CreateCompatibleDC( pcDC))
			{
				m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
				cMemDC.SelectObject( &m_cBmpIcon);
				pcIconImgs->Draw( m_nFilterIcon, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

				m_cBtnIconFilter.SetBitmap( m_cBmpIcon);

				cMemDC.DeleteDC();
			}
			m_cBtnIconFilter.ReleaseDC( pcDC);
		}
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDataListDialog::OnSelchangeDatalist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nCaret = m_cLstMemoDatas.GetCaretIndex();

	CString		cStr;
	COLORREF	clrBack = m_clrBack;
	if( 0 <= nCaret)
	{
		if( m_cLstMemoDatas.GetPreviewData( nCaret, cStr, m_clrFore, m_clrBack))
		{
			if( clrBack != m_clrBack)
			{
				m_cBrshBack.DeleteObject();

				CDC*	pcDC;
				pcDC = m_cEdtPreview.GetDC();
				m_cBrshBack.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
				m_cEdtPreview.ReleaseDC( pcDC);
			}
		}
		m_cEdtPreview.SetWindowText( cStr);
		m_cEdtPreview.InvalidateRect( NULL);
	}

	BOOL blSingle = ( 1 == m_cLstMemoDatas.GetSelCount());
	BOOL blNoData = ( 0 >= m_cLstMemoDatas.GetCount());
	m_cBtnItemDown.EnableWindow( blSingle && !blNoData && !( nCaret >= m_cLstMemoDatas.GetCount() - 1));
	m_cBtnItemUp.EnableWindow( blSingle && !blNoData && !( nCaret == 0));
	m_cBtnDelete.EnableWindow( blSingle && !blNoData);
	m_cBtnOK.EnableWindow( !blNoData);
	m_cBtnReverse.EnableWindow( !blNoData);
	m_cBtnFilter.EnableWindow( ( 0 != m_nMemoCount));
}

HBRUSH CDataListDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: この位置で DC のアトリビュートを変更してください
	HBRUSH hbr;

	if( m_cEdtPreview.GetSafeHwnd() == pWnd->GetSafeHwnd())
	{
		hbr = m_cBrshBack;
		pDC->SetBkColor( m_clrBack);
		pDC->SetTextColor( m_clrFore);
	}
	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CDataListDialog::OnReverse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		SetListData( m_nFilter, m_blReverse);
	}
	
}

void CDataListDialog::OnFilter() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cBtnIconFilter.EnableWindow( m_blFilter);
		m_cBtnItemDown.EnableWindow( !m_blFilter);
		m_cBtnItemUp.EnableWindow( !m_blFilter);
		m_cBtnDelete.EnableWindow( !m_blFilter);
		int		nOldFilter = m_nFilter;
		if( FALSE == m_blFilter)
		{
			m_nFilter = -1;
		}
		else
		{
			m_nFilter = m_nFilterIcon;
		}

		if( nOldFilter != m_nFilter)
		{
			SetListData( m_nFilter, m_blReverse);
		}
	}	
}

void CDataListDialog::OnSelchangeIconfilter() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		if( FALSE != m_blFilter)
		{
			int	nOldFilter = m_nFilter;
			m_nFilter = m_nFilterIcon;
			if( nOldFilter != m_nFilter)
			{
				SetListData( m_nFilter, m_blReverse);
			}
		}
	}
}

void CDataListDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData();
	int nMax = m_cLstMemoDatas.GetCount();
	for( int nIndex = 0; nIndex < nMax; nIndex++)
	{
		if( m_cLstMemoDatas.GetSel( nIndex))
		{
			HWND hWnd = m_cLstMemoDatas.GetWndHandle( nIndex);
			if( NULL != hWnd)
			{
				CWnd*	pcWnd;
				pcWnd = CWnd::FromHandle( hWnd);
				if( m_blCentering)
				{
					pcWnd->CenterWindow();
				}
				pcWnd->SendMessage( WM_COMMAND, IDM_TONORMAL, 0L);
				pcWnd->SetActiveWindow();
				pcWnd->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	}

	CDialog::OnOK();
}

void CDataListDialog::OnDelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nMax = m_cLstMemoDatas.GetCount();
	for( int nIndex = 0; nIndex < nMax; nIndex++)
	{
		if( m_cLstMemoDatas.GetSel( nIndex))
		{
			HWND hWnd = m_cLstMemoDatas.GetWndHandle( nIndex);
			if( NULL != hWnd)
			{
				::PostMessage( hWnd, WM_COMMAND, IDM_DELETE, 0L);
			}
		}
	}	
}

void CDataListDialog::OnItemdown() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_blFilter)return;

	int nCaret = m_cLstMemoDatas.GetCaretIndex();
	if( 0 <= nCaret)
	{
		BOOL blResult;
		if( !m_blReverse)
		{
			blResult = MemoItemDown( nCaret);
		}
		else
		{
			int nMax = m_cLstMemoDatas.GetCount();
			blResult = MemoItemUp( nMax - ( nCaret + 1));
		}

		if( blResult)
		{
			m_cLstMemoDatas.SetSel( nCaret + 1);
			m_cLstMemoDatas.SetCaretIndex( nCaret + 1);
			SetListData( m_nFilter, m_blReverse);
		}
	}
}

void CDataListDialog::OnItemup() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_blFilter)return;

	int nCaret = m_cLstMemoDatas.GetCaretIndex();
	if( 0 <= nCaret)
	{
		BOOL blResult;
		if( !m_blReverse)
		{
			blResult = MemoItemUp( nCaret);
		}
		else
		{
			int nMax = m_cLstMemoDatas.GetCount();
			blResult = MemoItemDown( nMax - ( nCaret + 1));
		}

		if( blResult)
		{
			m_cLstMemoDatas.SetSel( nCaret - 1);
			m_cLstMemoDatas.SetCaretIndex( nCaret - 1);
			SetListData( m_nFilter, m_blReverse);
		}
	}
}

void CDataListDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	int	nCount = m_nMemoCount;

	CSingleLock cSglk( &m_pcMemoChipList->m_cCriticalSection);
	if( cSglk.Lock( 0))
	{
		nCount = m_pcMemoChipList->GetCount();
		cSglk.Unlock();
	}

	if( m_nMemoCount != nCount)
	{
		SetListData( m_nFilter, m_blReverse);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDataListDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNUSED_ALWAYS( pWnd);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( 1 <= m_cLstMemoDatas.GetSelCount())
	{
		int	nSel = m_cLstMemoDatas.GetCurSel();
		if( LB_ERR != nSel)
		{
			CRect	cRect;
			m_cLstMemoDatas.GetWindowRect( cRect);
			if( cRect.PtInRect(point))
			{
				if( IsWindow( m_cLstMemoDatas.GetWndHandle( nSel)))
				{
					CMenu menu;
					VERIFY( menu.LoadMenu( IDR_DLGMENU));

					CMenu* pPopup = menu.GetSubMenu(0);
					ASSERT(pPopup != NULL);

					if( pPopup != NULL)
					{
						pPopup->EnableMenuItem( IDM_LINKEXEC, ( ( 1 >= m_cLstMemoDatas.GetSelCount()) && ( TRUE == m_cLstMemoDatas.IsLink( nSel))) ? MF_ENABLED : MF_GRAYED);
						pPopup->EnableMenuItem( IDM_SEETHROUGH, ( ( 1 >= m_cLstMemoDatas.GetSelCount()) && ( TRUE == m_cLstMemoDatas.IsSeeThrough( nSel))) ? MF_ENABLED : MF_GRAYED);
						pPopup->EnableMenuItem( IDC_DELETE, ( 1 >= m_cLstMemoDatas.GetSelCount()) ? MF_ENABLED : MF_GRAYED);
						pPopup->CheckMenuItem( IDM_DELONQUIT, m_cLstMemoDatas.IsDelOnQuit( nSel) ? MF_CHECKED : MF_UNCHECKED);
						pPopup->EnableMenuItem( IDM_DELONQUIT, ( 1 >= m_cLstMemoDatas.GetSelCount()) ? MF_ENABLED : MF_GRAYED);
						pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
					}
				}
			}
		}
	}
}

void CDataListDialog::OnLinkexec() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int	nSel = m_cLstMemoDatas.GetCurSel();
	HWND hWnd = m_cLstMemoDatas.GetWndHandle( nSel);
	if( IsWindow( hWnd))
	{
		::PostMessage( hWnd, WM_COMMAND, IDM_LINKEXEC, 0L);
	}
	CDialog::OnOK();
}

void CDataListDialog::OnSeethrough() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int	nSel = m_cLstMemoDatas.GetCurSel();
	HWND hWnd = m_cLstMemoDatas.GetWndHandle( nSel);
	if( IsWindow( hWnd))
	{
		::PostMessage( hWnd, WM_COMMAND, IDM_SEETHROUGH, 0L);
	}
	CDialog::OnOK();
}

void CDataListDialog::OnDelonquit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int	nSel = m_cLstMemoDatas.GetCurSel();
	HWND hWnd = m_cLstMemoDatas.GetWndHandle( nSel);
	if( IsWindow( hWnd))
	{
		CSingleLock cSglk( &m_pcMemoChipList->m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			int nPos = m_cIndexArray.GetAt( nSel);
			if( m_pcMemoChipList->GetCount() > nPos)
			{
				POSITION	Pos = m_pcMemoChipList->FindIndex( nPos);
				CMemoChip*	pcMemoChip = m_pcMemoChipList->GetAt( Pos);
				if( pcMemoChip)
				{
					pcMemoChip->DelOnQuit( !pcMemoChip->IsDelOnQuit());
					m_cLstMemoDatas.InsertMemoChip( nSel, *pcMemoChip);
					m_cLstMemoDatas.DeleteString( nSel + 1);
					m_cLstMemoDatas.SetSel( nSel);
					m_cLstMemoDatas.SetCurSel( nSel);
				}
			}
		}
	}
}

BOOL CDataListDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_ICONFILTER:
	case IDC_FILTER:
	case IDC_DATALIST:
	case IDC_PREVIEW:
	case IDC_ITEMUP:
	case IDC_ITEMDOWN:
	case IDC_DELETE:
	case IDC_REVERSE:
	case IDC_CENTER:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CDataListDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( WM_KEYDOWN == pMsg->message)
	{
		switch( pMsg->wParam)
		{
		case VK_DELETE:
			PostMessage( WM_COMMAND, IDC_DELETE, 0L);
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDataListDialog::OnPrintchip() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( 0 >= m_cLstMemoDatas.GetSelCount())return;

	CPageSetupDialog	cPrnDlg( PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS, this);
	cPrnDlg.m_psd.rtMargin.left = 2000;		//20 * 100;
	cPrnDlg.m_psd.rtMargin.top = 1000;		//10 * 100;
	cPrnDlg.m_psd.rtMargin.right = 1000;	//10 * 100;
	cPrnDlg.m_psd.rtMargin.bottom = 1000;	//10 * 100;

	if( IDOK == cPrnDlg.DoModal())
	{
		HDC hPrinterDC = cPrnDlg.CreatePrinterDC();
		if( NULL!= hPrinterDC)
		{
			CPrintChipDialog*	pcPrintChipDialog = new CPrintChipDialog( hPrinterDC, cPrnDlg.m_psd.rtMargin, this);
			if( pcPrintChipDialog)
			{
				CMemoData	cMemoData;
				int	nMasterCount = m_cLstMemoDatas.GetCount();
				for( int nIndex = 0; nIndex < nMasterCount; nIndex++)
				{
					if( m_cLstMemoDatas.GetSel( nIndex))
					{
						m_cLstMemoDatas.GetPrintData( nIndex, cMemoData);
						pcPrintChipDialog->AddPrintChip( &cMemoData);
					}
				}

				pcPrintChipDialog->Create( CPrintChipDialog::IDD);
			}
		}
	}
}

void CDataListDialog::OnSavetext() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( 0 < m_cLstMemoDatas.GetSelCount())
	{
		CFileDialog	cFileDlg( FALSE, "txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text File(*.txt)|*.txt||", this);

		if( IDOK == cFileDlg.DoModal())
		{
			CFile cFile;

			if( cFile.Open( cFileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
			{
				CString cStr;
				CMemoData cMemoData;

				LPCSTR lpcszTitle = "Title : ";
				LPCSTR lpcszCRLF = "\r\n";
				LPCSTR lpcszSepa = "\r\n-------------------------------\r\n";

				int nCount = m_cLstMemoDatas.GetCount();
				for( int nIndex = 0; nIndex < nCount; nIndex++)
				{
					if( 0 < m_cLstMemoDatas.GetSel( nIndex))
					{
						if( m_cLstMemoDatas.GetPrintData( nIndex, cMemoData))
						{
							cFile.Write( lpcszTitle, lstrlen( lpcszTitle));
							cMemoData.GetTitle( cStr);
							cStr += lpcszCRLF;
							cFile.Write( cStr, cStr.GetLength());

							cStr = cMemoData.GetMemo();
							cStr += lpcszSepa;
							cFile.Write( cStr, cStr.GetLength());
						}
					}
				}
				cFile.Close();
			}
		}
	}
}

void CDataListDialog::OnIconfilter() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_cBtnIconFilter.SetCheck( 1);
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	const CIconImgList* pcIconImgs = pcSOboe->GetIconList();

	if( pcIconImgs)
	{
		CRect cRect;
		m_cBtnIconFilter.GetWindowRect( cRect);

		m_pcIcon = pcIconImgs->CreateIconSelectMenu( this, FALSE);

		UINT unMenuCMD = m_pcIcon->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this);
		if( NULL != m_pcIcon)
		{
			delete m_pcIcon;
			m_pcIcon = NULL;
		}
		/*
		UINT unMenuCMD = pcIconImgs->TrackPopupMenu( cRect.left, cRect.bottom, this, TRUE);
		*/

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nFilterIcon = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CDC *pcDC = m_cBtnIconFilter.GetDC();
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
					pcIconImgs->Draw( m_nFilterIcon, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

					m_cBtnIconFilter.SetBitmap( m_cBmpIcon);

					cMemDC.DeleteDC();
				}
				ReleaseDC( pcDC);
			}

			m_nFilter = m_nFilterIcon;
			SetListData( m_nFilter, m_blReverse);
		}
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, GetSafeHwnd(), 0, 0, PM_REMOVE));
		}
	}
	m_cBtnIconFilter.SetCheck( 0);

	/*
	m_cBtnIconFilter.SetCheck( 1);

	CIconSelectmenu	cPopIcon;
	if( cPopIcon.CreatePopupMenu())
	{
		UINT	unMenuCMD;

		CRect	cRect;
		m_cBtnIconFilter.GetWindowRect( cRect);

		unMenuCMD = cPopIcon.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nFilterIcon = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
			if( pcIconImgs)
			{
				CDC *pcDC = m_cBtnIconFilter.GetDC();
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
						pcIconImgs->Draw( m_nFilterIcon, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

						m_cBtnIconFilter.SetBitmap( m_cBmpIcon);

						cMemDC.DeleteDC();
					}
					ReleaseDC( pcDC);
				}
			}

			m_nFilter = m_nFilterIcon;
			SetListData( m_nFilter, m_blReverse);
		}
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, GetSafeHwnd(), 0, 0, PM_REMOVE));
		}
	}
	m_cBtnIconFilter.SetCheck( 0);*/
}

void CDataListDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( NULL != m_pcIcon)
	{
		m_pcIcon->DrawItem( lpDrawItemStruct);

		return;
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDataListDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( NULL != m_pcIcon)
	{
		m_pcIcon->MeasureItem( lpMeasureItemStruct);

		return;
	}

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
