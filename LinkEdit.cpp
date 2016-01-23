// LinkEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "LinkEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkEdit ダイアログ


CLinkEdit::CLinkEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkEdit)
	m_cStrFile = _T("");
	m_blLink = FALSE;
	m_cStrParameters = _T("");
	m_blTimerExec = FALSE;
	//}}AFX_DATA_INIT
}


void CLinkEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkEdit)
	DDX_Control(pDX, IDC_TIMEREXEC, m_cBtnTimerExec);
	DDX_Control(pDX, IDC_PARAMETERSSEL, m_cBtnParametersSel);
	DDX_Control(pDX, IDC_PARAMETERS, m_cEdtParameters);
	DDX_Control(pDX, IDC_FILESEL, m_cBtnFileSel);
	DDX_Control(pDX, IDC_FILE, m_cEdtFile);
	DDX_Text(pDX, IDC_FILE, m_cStrFile);
	DDX_Check(pDX, IDC_LINK, m_blLink);
	DDX_Text(pDX, IDC_PARAMETERS, m_cStrParameters);
	DDX_Check(pDX, IDC_TIMEREXEC, m_blTimerExec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkEdit, CDialog)
	//{{AFX_MSG_MAP(CLinkEdit)
	ON_BN_CLICKED(IDC_FILESEL, OnFilesel)
	ON_BN_CLICKED(IDC_PARAMETERSSEL, OnParameterssel)
	ON_BN_CLICKED(IDC_LINK, OnLink)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkEdit メッセージ ハンドラ

void CLinkEdit::OnFilesel() 
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

void CLinkEdit::OnParameterssel() 
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

int CLinkEdit::DoFileSel( CString& cStrFile, LPCSTR lpcszFilter)
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

BOOL CLinkEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cBtnTimerExec.EnableWindow( m_blLink);
	m_cBtnParametersSel.EnableWindow( m_blLink);
	m_cEdtParameters.EnableWindow( m_blLink);
	m_cBtnFileSel.EnableWindow( m_blLink);
	m_cEdtFile.EnableWindow( m_blLink);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CLinkEdit::OnLink() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cBtnTimerExec.EnableWindow( m_blLink);
		m_cBtnParametersSel.EnableWindow( m_blLink);
		m_cEdtParameters.EnableWindow( m_blLink);
		m_cBtnFileSel.EnableWindow( m_blLink);
		m_cEdtFile.EnableWindow( m_blLink);
	}
}

BOOL CLinkEdit::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_LINK:
	case IDC_TIMEREXEC:
	case IDC_FILE:
	case IDC_PARAMETERS:
	case IDC_FILESEL:
	case IDC_PARAMETERSSEL:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}
