// ExtendEditPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "ExtendEditPage.h"
#include "MemoEditSheet.h"
#include "DrawExLib.h"
#include ".\extendeditpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtendEditPage プロパティ ページ

IMPLEMENT_DYNCREATE(CExtendEditPage, CPropertyPage)

CExtendEditPage::CExtendEditPage() : CPropertyPage(CExtendEditPage::IDD)
{
	//{{AFX_DATA_INIT(CExtendEditPage)
	m_blLink = FALSE;
	m_blTimerExec = FALSE;
	m_cStrParameters = _T("");
	m_cStrFile = _T("");
	m_blPassWordEnable = FALSE;
	m_cStrPassWord1st = _T("");
	m_cStrPassWord2nd = _T("");
	m_nZOrder = 0;
	m_nDxlSel = 0;
	//}}AFX_DATA_INIT
	m_unDxlID = 0;
}

CExtendEditPage::~CExtendEditPage()
{
}

void CExtendEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtendEditPage)
	DDX_Control(pDX, IDC_DXLSEL, m_cCmbDxlSel);
	DDX_Control(pDX, IDC_PASSWORD2ND, m_cEdtPassWord2nd);
	DDX_Control(pDX, IDC_PASSWORD1ST, m_cEdtPassWord1st);
	DDX_Control(pDX, IDC_TIMEREXEC, m_cBtnTimerExec);
	DDX_Control(pDX, IDC_PARAMETERSSEL, m_cBtnParametersSel);
	DDX_Control(pDX, IDC_FILESEL, m_cBtnFileSel);
	DDX_Control(pDX, IDC_PARAMETERS, m_cEdtParameters);
	DDX_Control(pDX, IDC_FILE, m_cEdtFile);
	DDX_Check(pDX, IDC_LINK, m_blLink);
	DDX_Check(pDX, IDC_TIMEREXEC, m_blTimerExec);
	DDX_Text(pDX, IDC_PARAMETERS, m_cStrParameters);
	DDX_Text(pDX, IDC_FILE, m_cStrFile);
	DDX_Check(pDX, IDC_PASSWORDENABLE, m_blPassWordEnable);
	DDX_Text(pDX, IDC_PASSWORD1ST, m_cStrPassWord1st);
	DDX_Text(pDX, IDC_PASSWORD2ND, m_cStrPassWord2nd);
	DDX_CBIndex(pDX, IDC_ZODER, m_nZOrder);
	DDX_CBIndex(pDX, IDC_DXLSEL, m_nDxlSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtendEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CExtendEditPage)
	ON_BN_CLICKED(IDC_LINK, OnLink)
	ON_BN_CLICKED(IDC_PASSWORDENABLE, OnPasswordenable)
	ON_BN_CLICKED(IDC_PARAMETERSSEL, OnParameterssel)
	ON_BN_CLICKED(IDC_FILESEL, OnFilesel)
	ON_CBN_SELCHANGE(IDC_DXLSEL, OnSelchangeDxlsel)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_SETDEFAULT, OnSetdefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtendEditPage メッセージ ハンドラ

void CExtendEditPage::OnLink() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cEdtFile.EnableWindow( m_blLink);
		m_cBtnFileSel.EnableWindow( m_blLink);
		m_cEdtParameters.EnableWindow( m_blLink);
		m_cBtnParametersSel.EnableWindow( m_blLink);
		m_cBtnTimerExec.EnableWindow( m_blLink);
	}
}

BOOL CExtendEditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cEdtFile.EnableWindow( m_blLink);
	m_cBtnFileSel.EnableWindow( m_blLink);
	m_cEdtParameters.EnableWindow( m_blLink);
	m_cBtnParametersSel.EnableWindow( m_blLink);
	m_cBtnTimerExec.EnableWindow( m_blLink);

	m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
	m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);

	m_cCmbDxlSel.SetItemData( m_cCmbDxlSel.AddString( "未使用"), 0);
	int	nPos = -1;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	int nMaxEntry = pcSOboe->GetDxlMaxEntry();
	for( int nIndex = 0; nIndex < nMaxEntry; nIndex++)
	{
		const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
		if( pcDrawExLib)
		{
			if( pcDrawExLib->IsFunctionAssign())
			{
				nPos = m_cCmbDxlSel.AddString( pcDrawExLib->GetMenuString());
				m_cCmbDxlSel.SetItemData( nPos, pcDrawExLib->GetDxlID());
				if( pcDrawExLib->GetDxlID() == m_unDxlID)m_nDxlSel = nPos;
			}
		}
	}
	if( 1 >= m_cCmbDxlSel.GetCount())
	{
		m_cCmbDxlSel.EnableWindow( FALSE);
	}
	m_cCmbDxlSel.SetCurSel( m_nDxlSel);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CExtendEditPage::OnPasswordenable() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
		m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);
	}
}

BOOL CExtendEditPage::OnKillActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( UpdateData())
	{
		if( m_blPassWordEnable)
		{
			if( _MINLEN_PW > m_cStrPassWord1st.GetLength() || m_cStrPassWord1st != m_cStrPassWord2nd)
			{
				MessageBox( "設定されたパスワードが異常です", NULL, MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
		}
		CMemoEditSheet* pcParent = ( CMemoEditSheet*)GetParent();
		if( pcParent)
		{
			pcParent->ForwardDxlID( m_unDxlID);
		}
	}
	
	return CPropertyPage::OnKillActive();
}

void CExtendEditPage::OnFilesel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	CString	cStrFile;
	cStrFile = m_cStrFile;
	if( IDOK == DoFileSel( cStrFile, "File(*.exe)|*.exe||"))
	{
		m_cStrFile = cStrFile;
		UpdateData( FALSE);
	}
}

void CExtendEditPage::OnParameterssel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	CString	cStrFile;
	cStrFile = m_cStrParameters;
	if( IDOK == DoFileSel( cStrFile, "File(all)|*.*||"))
	{
		m_cStrParameters = cStrFile;
		UpdateData( FALSE);
	}
}

int CExtendEditPage::DoFileSel( CString& cStrFile, LPCSTR lpcszFilter)
{
	int	nResult = IDCANCEL;

	char	szCurrentFolder[ _MAX_PATH];
	if( GetCurrentDirectory( _MAX_PATH, szCurrentFolder))
	{
		CString	cStrFileWork;
		WIN32_FIND_DATA		stFindData;
		HANDLE hFind = FindFirstFile( cStrFile, &stFindData);
		if( INVALID_HANDLE_VALUE != hFind)
		{
			cStrFileWork = cStrFile;
			FindClose( hFind);
		}
		CFileDialog	cFileDlg( TRUE, NULL, cStrFileWork, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, lpcszFilter, this);

		nResult = cFileDlg.DoModal();
		if( IDOK == nResult)
		{
			cStrFile = cFileDlg.GetPathName();
		}

		SetCurrentDirectory( szCurrentFolder);
	}

	return nResult;
}

void CExtendEditPage::OnSelchangeDxlsel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_unDxlID = ( 0 < m_nDxlSel) ? ( UINT)m_cCmbDxlSel.GetItemData( m_nDxlSel) : 0;
	}
}

void CExtendEditPage::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	if( m_cCmbDxlSel.GetSafeHwnd() == pWnd->GetSafeHwnd())
	{
		int nSelIndex = m_cCmbDxlSel.GetCurSel();
		if( 0 <= nSelIndex)
		{
			CMenu cMenu;
			if( cMenu.LoadMenu( IDR_DLGMENU))
			{
				CMenu* pcPopup = cMenu.GetSubMenu( 2);

				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				if( pcSOboe->GetDefaultDxlID() == m_cCmbDxlSel.GetItemData( nSelIndex))
				{
					pcPopup->CheckMenuItem( IDM_SETDEFAULT, MF_CHECKED | MF_BYCOMMAND);
				}
				pcPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}
	}
}

void CExtendEditPage::OnSetdefault()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	int nSelIndex = m_cCmbDxlSel.GetCurSel();
	if( 0 <= nSelIndex)
	{
		UINT unDxlID = m_cCmbDxlSel.GetItemData( nSelIndex);

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		pcSOboe->SetDefaultDxlID( unDxlID);
	}
}
