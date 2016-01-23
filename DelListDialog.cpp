// DelListDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "MainFrm.h"
#include "MemoChip.h"
#include "MemoChipList.h"
#include "DelListDialog.h"
#include "PassWordCheckDialog.h"
#include "PrintChipDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelListDialog ダイアログ


CDelListDialog::CDelListDialog( CString cStrFilePath, CWnd* pParent /*=NULL*/)
	: CDialog(CDelListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDelListDialog)
	m_blReverse = FALSE;
	//}}AFX_DATA_INIT
	m_cStrDustFilePath = cStrFilePath;
	m_clrFore = GetSysColor( COLOR_WINDOWTEXT);
	m_clrBack = GetSysColor( COLOR_WINDOW);
	m_cBrshBack.CreateSolidBrush( m_clrBack);

	CSOboeApp*	pcSOboeApp = ( CSOboeApp*)AfxGetApp();
	if( pcSOboeApp)
	{
		m_blReverse = pcSOboeApp->IsDelListReverse();
	}
}

CDelListDialog::~CDelListDialog()
{
	CSOboeApp*	pcSOboeApp = ( CSOboeApp*)AfxGetApp();
	if( pcSOboeApp)
	{
		pcSOboeApp->DelListReverse( m_blReverse);
	}

	RemoveList();
}

void CDelListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelListDialog)
	DDX_Control(pDX, IDC_DELETE, m_cBtnDelete);
	DDX_Control(pDX, IDC_RESTORE, m_cBtnRestore);
	DDX_Control(pDX, IDC_REVERSE, m_cBtnReverse);
	DDX_Control(pDX, IDC_SAVETEXT, m_cBtnSaveText);
	DDX_Control(pDX, IDC_ITEMUP, m_cBtnItemUp);
	DDX_Control(pDX, IDC_ITEMDOWN, m_cBtnItemDown);
	DDX_Control(pDX, IDC_PREVIEW, m_cEdtPreview);
	DDX_Control(pDX, IDC_DELLIST, m_cLstMemoDatas);
	DDX_Check(pDX, IDC_REVERSE, m_blReverse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelListDialog, CDialog)
	//{{AFX_MSG_MAP(CDelListDialog)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_DELLIST, OnSelchangeDellist)
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_ITEMUP, OnItemup)
	ON_BN_CLICKED(IDC_ITEMDOWN, OnItemdown)
	ON_BN_CLICKED(IDC_SAVETEXT, OnSavetext)
	ON_BN_CLICKED(IDC_RELOAD, OnReload)
	ON_BN_CLICKED(IDC_RESTORE, OnRestore)
	ON_COMMAND(IDM_COPYRESTORE, OnCopyRestore)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_COMMAND(IDM_LINKEXEC, OnLinkexec)
	ON_WM_HELPINFO()
	ON_LBN_DBLCLK(IDC_DELLIST, OnRestore)
	ON_COMMAND(IDM_PRINTCHIP, OnPrintchip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelListDialog メッセージ ハンドラ

CSemaphore	CDelListDialog::sm_cSmpDoDisp;

// 使用中？
BOOL CDelListDialog::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoDisp);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

int CDelListDialog::DoModal() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	int nResult = IDCANCEL;
	CSingleLock	cSlk( &sm_cSmpDoDisp);
	if( cSlk.Lock( 0))
	{
		nResult = CDialog::DoModal();
	}
	return nResult;
}

BOOL CDelListDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	LoadDelData();
	SetListData( m_blReverse);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe->IsCompatibleOU())
	{
		CString	cStr;
		cStr.LoadString( IDS_STOCKLIST);
		SetWindowText( cStr);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDelListDialog::RemoveList( void)
{
	CMemoChip*	pcMemoChip;
	while( m_cLstDelMemoChip.GetCount())
	{
		pcMemoChip = m_cLstDelMemoChip.GetHead();
		m_cLstDelMemoChip.RemoveHead();
		if( pcMemoChip)
		{
			delete pcMemoChip;
			pcMemoChip = NULL;
		}
	}
}

void CDelListDialog::LoadDelData( void)
{
	RemoveList();

	try
	{
		CFile	cLoadFile;
		if( cLoadFile.Open( m_cStrDustFilePath, CFile::modeRead | CFile::typeBinary))
		{
			CMemoChip*	pcMemoChip;
			CArchive	cArchive( &cLoadFile, CArchive::load);
			while( TRUE)
			{
				pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
				if( NULL == pcMemoChip)break;
				m_cLstDelMemoChip.AddTail( pcMemoChip);
			}
		}
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
}

void CDelListDialog::SaveDelData( void)
{
	try
	{
		CFile	cSaveFile;
		if( cSaveFile.Open( m_cStrDustFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			CMemoChip*	pcMemoChip;
			CArchive	cArchive( &cSaveFile, CArchive::store);

			int nMemoCount = m_cLstDelMemoChip.GetCount();
			for( int nIndex = 0; nIndex < nMemoCount; nIndex++)
			{
				pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nIndex));
				if( pcMemoChip)
				{
					cArchive.WriteObject( pcMemoChip);
				}
			}
			cArchive.Flush();
		}
		cSaveFile.Flush();
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
}

void CDelListDialog::SetListData( BOOL blReverse)
{
	CSingleLock cSglk( &m_cLstDelMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nTopIndex = m_cLstMemoDatas.GetTopIndex();
		int	nSelIndex = m_cLstMemoDatas.GetCaretIndex();

		m_cLstMemoDatas.SetRedraw( FALSE);
		m_cLstMemoDatas.ResetContent();

		CMemoChip*	pcMemoChip;
		int nMemoCount = m_cLstDelMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nMemoCount; nIndex++)
		{
			pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( blReverse)
				{
					m_cLstMemoDatas.InsertMemoChip( 0, *pcMemoChip, TRUE);
				}
				else
				{
					m_cLstMemoDatas.AddMemoChip( *pcMemoChip, TRUE);
				}
			}
		}
		cSglk.Unlock();
	
		m_cLstMemoDatas.SetTopIndex( nTopIndex);
		m_cLstMemoDatas.SetCaretIndex( nSelIndex, TRUE);
		m_cLstMemoDatas.SetSel( nSelIndex, TRUE);
		OnSelchangeDellist();

		m_cLstMemoDatas.SetRedraw( TRUE);
	}
}

HBRUSH CDelListDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDelListDialog::OnSelchangeDellist() 
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
	m_cBtnSaveText.EnableWindow( blSingle && !blNoData);
	m_cBtnReverse.EnableWindow( !blNoData);
	m_cBtnRestore.EnableWindow( !blNoData);
	m_cBtnDelete.EnableWindow( !blNoData);
}

void CDelListDialog::OnReverse() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		SetListData( m_blReverse);
	}
}

void CDelListDialog::OnItemup() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
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
			SaveDelData();
			m_cLstMemoDatas.SetSel( nCaret, FALSE);
			m_cLstMemoDatas.SetSel( nCaret - 1);
			m_cLstMemoDatas.SetCaretIndex( nCaret - 1);
			SetListData( m_blReverse);
		}
	}
}

void CDelListDialog::OnItemdown() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
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
			SaveDelData();
			m_cLstMemoDatas.SetSel( nCaret, FALSE);
			m_cLstMemoDatas.SetSel( nCaret + 1);
			m_cLstMemoDatas.SetCaretIndex( nCaret + 1);
			SetListData( m_blReverse);
		}
	}
}

BOOL CDelListDialog::MemoItemUp( int nPos)
{
	if( 0 >= nPos)return FALSE;

	CSingleLock cSglk( &m_cLstDelMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nMax = m_cLstDelMemoChip.GetCount();
		if( nMax <= nPos)return FALSE;

		POSITION	Pos = m_cLstDelMemoChip.FindIndex( nPos);
		CMemoChip*	pcMemoChip = m_cLstDelMemoChip.GetAt( Pos);
		m_cLstDelMemoChip.RemoveAt( Pos);
		m_cLstDelMemoChip.InsertBefore( m_cLstDelMemoChip.FindIndex( nPos - 1), pcMemoChip);

		return TRUE;
	}
	return FALSE;
}

BOOL CDelListDialog::MemoItemDown( int nPos)
{
	CSingleLock cSglk( &m_cLstDelMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		int nMax = m_cLstDelMemoChip.GetCount();
		if( nMax <= nPos)return FALSE;

		POSITION	Pos = m_cLstDelMemoChip.FindIndex( nPos);
		CMemoChip*	pcMemoChip = m_cLstDelMemoChip.GetAt( Pos);
		m_cLstDelMemoChip.RemoveAt( Pos);
		m_cLstDelMemoChip.InsertAfter( m_cLstDelMemoChip.FindIndex( nPos), pcMemoChip);

		return TRUE;
	}
	return FALSE;
}

void CDelListDialog::OnSavetext() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 0 > m_cLstMemoDatas.GetCurSel())return;

	int nCaret = m_cLstMemoDatas.GetCaretIndex();
	if( 0 <= nCaret)
	{
		int nIndex = ( m_blReverse) ? ( m_cLstMemoDatas.GetCount() - ( nCaret + 1)) : nCaret;

		CMemoChip*	pcMemoChip;
		pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nIndex));
		if( FALSE == CheckPassWord( pcMemoChip))return;

		const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();

		CString		cStrMemo = pcMemoData->GetMemo();
		if( !cStrMemo.IsEmpty())
		{
			char	szFolder[ _MAX_PATH];
			GetCurrentDirectory( _MAX_PATH, szFolder);
			{
				CString	cStrData;
				if( pcMemoData->GetTitle( cStrData))
				{
					if( -1 != cStrData.FindOneOf( "\\:./|<>?*"))
					{
						cStrData.Empty();
					}
				}
				
				CFileDialog	cFileDlg( FALSE, "txt", cStrData, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Text File(*.txt)|*.txt||", this);

				if( IDOK == cFileDlg.DoModal())
				{
					CFile	cFile;
					if( cFile.Open( cFileDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate))
					{
						cFile.Write( cStrMemo, cStrMemo.GetLength());
						cFile.Close();
					}
				}
			}
			SetCurrentDirectory( szFolder);
		}
	}
}

void CDelListDialog::OnReload() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	LoadDelData();
	SetListData( m_blReverse);
}

void CDelListDialog::OnRestore() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 0 >= m_cLstMemoDatas.GetSelCount())return;

	if( UpdateData())
	{
		int	nIndex;
		int	nMasterCount = m_cLstDelMemoChip.GetCount();
		for( int nMasterIndex = ( nMasterCount - 1); nMasterIndex >= 0; nMasterIndex--)
		{
			nIndex = ( m_blReverse) ? ( nMasterCount - ( nMasterIndex + 1)) : nMasterIndex;
			if( m_cLstMemoDatas.GetSel( nIndex))
			{
				if( Restore( nMasterIndex))
				{
					Delete( nMasterIndex, TRUE);
				}
			}
		}
		SaveDelData();
		SetListData( m_blReverse);
	}
}

void CDelListDialog::OnCopyRestore() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 0 >= m_cLstMemoDatas.GetSelCount())return;

	int	nIndex;
	for( int nMasterIndex = ( m_cLstDelMemoChip.GetCount() - 1); nMasterIndex >= 0; nMasterIndex--)
	{
		nIndex = ( m_blReverse) ? ( m_cLstDelMemoChip.GetCount() - ( nMasterIndex + 1)) : nMasterIndex;
		if( m_cLstMemoDatas.GetSel( nIndex))
		{
			Restore( nMasterIndex);
		}
	}
}

BOOL CDelListDialog::Restore( int nIndex)
{
	if( 0 <= nIndex)
	{
		int nMasterIndex = nIndex;
		CMemoChip*	pcMemoChip;
		pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nMasterIndex));
		if( FALSE == CheckPassWord( pcMemoChip))return FALSE;

		const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();

		if( pcMemoData->IsPassWordLock())
		{
			CString	cStrTitle;
			pcMemoData->GetTitle( cStrTitle);

			CPassWordCheckDialog	cPWCheckDlg( cStrTitle, this);
			do
			{
				if( IDCANCEL == cPWCheckDlg.DoModal())return FALSE;
			}while( pcMemoData->GetPassWord() != cPWCheckDlg.m_cStrPassWord);
		}

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		CMainFrame* pcMainFrm = ( CMainFrame*)pcSOboe->m_pMainWnd;
		pcMainFrm->CreateChipFromMemoData( *pcMemoData, FALSE);
		return TRUE;
	}
	return FALSE;
}

void CDelListDialog::OnDelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 0 >= m_cLstMemoDatas.GetSelCount())return;

	int	nIndex;
	int	nMasterCount = m_cLstDelMemoChip.GetCount();
	for( int nMasterIndex = ( nMasterCount - 1); nMasterIndex >= 0; nMasterIndex--)
	{
		nIndex = ( m_blReverse) ? ( nMasterCount - ( nMasterIndex + 1)) : nMasterIndex;
		if( m_cLstMemoDatas.GetSel( nIndex))
		{
			Delete( nMasterIndex);
		}
	}
	SaveDelData();
	SetListData( m_blReverse);
}

BOOL CDelListDialog::Delete( int nIndex, BOOL blNoneCheck)
{
	if( 0 <= nIndex)
	{
		int nMasterIndex = nIndex;
		CMemoChip*	pcMemoChip;
		pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nMasterIndex));
		if( FALSE == CheckPassWord( pcMemoChip))return FALSE;

		if( pcMemoChip)
		{
			if( FALSE == blNoneCheck)
			{
				const CMemoData*	pcMemoData = pcMemoChip->AccessMemoData();
				if( pcMemoData->IsPassWordLock())
				{
					CString	cStrTitle;
					pcMemoData->GetTitle( cStrTitle);

					CPassWordCheckDialog	cPWCheckDlg( cStrTitle, this);
					do
					{
						if( IDCANCEL == cPWCheckDlg.DoModal())return FALSE;
					}while( pcMemoData->GetPassWord() != cPWCheckDlg.m_cStrPassWord);
				}
			}

			m_cLstDelMemoChip.RemoveAt( m_cLstDelMemoChip.FindIndex( nMasterIndex));
			delete pcMemoChip;
			pcMemoChip = NULL;
		}
		return TRUE;
	}
	return FALSE;
}


void CDelListDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
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
				int nIndex = ( m_blReverse) ? ( m_cLstMemoDatas.GetCount() - ( nSel + 1)) : nSel;

				CMemoChip*	pcMemoChip;
				const CMemoData*	pcMemoData;
				pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nIndex));
				pcMemoData = ( NULL != pcMemoChip) ? pcMemoChip->AccessMemoData() : NULL;
				if( pcMemoData)
				{
					CMenu menu;
					VERIFY( menu.LoadMenu( IDR_DLGMENU));

					CMenu* pPopup = menu.GetSubMenu(1);
					ASSERT(pPopup != NULL);

					if (pPopup != NULL)
					{
						BOOL blShingle = ( 1 >= m_cLstMemoDatas.GetSelCount());
						pPopup->EnableMenuItem( IDC_SAVETEXT, MF_BYCOMMAND | ( blShingle ? MF_ENABLED : MF_GRAYED));
						pPopup->EnableMenuItem( IDM_LINKEXEC, MF_BYCOMMAND | ( ( blShingle && ( TRUE == pcMemoData->IsLink())) ? MF_ENABLED : MF_GRAYED));
						pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
					}
				}
			}
		}
	}
}

void CDelListDialog::OnLinkexec() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( 1 <= m_cLstMemoDatas.GetSelCount())
	{
		int	nSel = m_cLstMemoDatas.GetCurSel();
		if( LB_ERR != nSel)
		{
			int nIndex = ( m_blReverse) ? ( m_cLstMemoDatas.GetCount() - ( nSel + 1)) : nSel;

			CMemoChip*	pcMemoChip;
			const CMemoData*	pcMemoData;
			pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nIndex));
			if( FALSE == CheckPassWord( pcMemoChip))return;

			pcMemoData = pcMemoChip->AccessMemoData();
			if( NULL == pcMemoData)return;

			CString	cStrFile;
			CString	cStrParam;
			if( FALSE == pcMemoData->GetLinkData( cStrFile, cStrParam))return;
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
	}
}

// パスワードのチェック
BOOL CDelListDialog::CheckPassWord( const CMemoChip* pcMemoChip)
{
	if( NULL == pcMemoChip)return FALSE;
	const CMemoData* pcMemoData = pcMemoChip->AccessMemoData();
	if( NULL == pcMemoData)return FALSE;

	if( pcMemoData->IsPassWordLock())
	{
		CString	cStrTitle;
		pcMemoData->GetTitle( cStrTitle);

		CPassWordCheckDialog	cPWCheckDlg( cStrTitle, this);
		do
		{
			if( IDCANCEL == cPWCheckDlg.DoModal())return FALSE;
		}while( pcMemoData->GetPassWord() != cPWCheckDlg.m_cStrPassWord);
	}
	return TRUE;
}

BOOL CDelListDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_DELLIST:
	case IDC_PREVIEW:
	case IDC_RELOAD:
	case IDC_RESTORE:
	case IDC_SAVETEXT:
	case IDC_DELETE:
	case IDC_ITEMUP:
	case IDC_ITEMDOWN:
	case IDC_REVERSE:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CDelListDialog::PreTranslateMessage(MSG* pMsg) 
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

void CDelListDialog::OnPrintchip() 
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
				int	nIndex;
				const CMemoChip*	pcMemoChip;
				const CMemoData*	pcMemoData;
				int	nMasterCount = m_cLstDelMemoChip.GetCount();
				for( int nMasterIndex = ( nMasterCount - 1); nMasterIndex >= 0; nMasterIndex--)
				{
					nIndex = ( m_blReverse) ? ( nMasterCount - ( nMasterIndex + 1)) : nMasterIndex;
					if( m_cLstMemoDatas.GetSel( nIndex))
					{
						pcMemoChip = m_cLstDelMemoChip.GetAt( m_cLstDelMemoChip.FindIndex( nMasterIndex));
						if( pcMemoChip)
						{
							pcMemoData = pcMemoChip->AccessMemoData();
							pcPrintChipDialog->AddPrintChip( pcMemoData);
						}
					}
				}
				pcPrintChipDialog->Create( CPrintChipDialog::IDD);
			}
		}
	}
}
