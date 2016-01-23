// ChipExDllSettingDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ChipExDllSettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChipExDllSettingDialog ダイアログ


CChipExDllSettingDialog::CChipExDllSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChipExDllSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChipExDllSettingDialog)
	m_cStrChipExDllName = _T("");
	m_cStrChipExMenuText = _T("");
	m_blSetSepa = FALSE;
	//}}AFX_DATA_INIT
}


void CChipExDllSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChipExDllSettingDialog)
	DDX_Control(pDX, IDC_CHIPEXSETSEPA, m_cBtnSetSeparator);
	DDX_Control(pDX, IDC_CHIPEXDLLMENUTEXT, m_cEdtMenuText);
	DDX_Control(pDX, IDC_CHIPEXDLLCONFIG, m_cBtnConfig);
	DDX_Control(pDX, IDC_CHIPEXDLLABOUT, m_cBtnAbout);
	DDX_Text(pDX, IDC_CHIPEXDLLNAME, m_cStrChipExDllName);
	DDX_Text(pDX, IDC_CHIPEXDLLMENUTEXT, m_cStrChipExMenuText);
	DDV_MaxChars(pDX, m_cStrChipExMenuText, 50);
	DDX_Check(pDX, IDC_CHIPEXSETSEPA, m_blSetSepa);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChipExDllSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CChipExDllSettingDialog)
	ON_BN_CLICKED(IDC_CHIPEXDLLSEL, OnDllsel)
	ON_BN_CLICKED(IDC_CHIPEXDLLABOUT, OnChipexdllabout)
	ON_BN_CLICKED(IDC_CHIPEXDLLCONFIG, OnChipexdllconfig)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChipExDllSettingDialog メッセージ ハンドラ

void CChipExDllSettingDialog::OnDllsel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	CString	cStrFile;
	cStrFile = m_cStrChipExDllName;
	if( GetExtendDll( cStrFile))
	{
		m_cStrChipExDllName = cStrFile;
		m_cBtnAbout.EnableWindow( !cStrFile.IsEmpty());
		m_cBtnConfig.EnableWindow( !cStrFile.IsEmpty());
		m_cEdtMenuText.EnableWindow( !cStrFile.IsEmpty());
		m_cBtnSetSeparator.EnableWindow( !cStrFile.IsEmpty() && ( 0 != m_nMenuIndex));

		if( !cStrFile.IsEmpty())
		{
			HINSTANCE	hInst = NULL;
			hInst = LoadLibrary( m_cStrChipExDllName);
			// リンクが正常になされた？
			if( NULL != hInst)
			{
				CHIPEXSPECIFICATION	lpChipExSpecification;
				// ヴァージョンの確認
				lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( hInst, "ChipExSpecification");
				if( lpChipExSpecification)
				{
					SPECIFICATION	stSpecification;
					lpChipExSpecification( &stSpecification);
					m_cStrChipExMenuText = stSpecification.szMenuString;
				}
			}
		}
		else
		{
			m_cStrChipExMenuText.Empty();
		}

		UpdateData( FALSE);
	}
}

BOOL CChipExDllSettingDialog::GetExtendDll( CString& cStrFile)
{
	DWORD dwLen = GetCurrentDirectory( 0, NULL);
	char*	pszFolder;
	dwLen++;
	pszFolder = new char[ dwLen];
	GetCurrentDirectory( dwLen, pszFolder);
	CString	cStrFolder = pszFolder;
	delete pszFolder;
	pszFolder = NULL;

	CString	cStrData;
	cStrData = cStrFile;
	CFileDialog	cFileDlg( TRUE, "cel", cStrData, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "Chip Ex Dll(*.cel)|*.cel||", this);

	int nResult = cFileDlg.DoModal();
	if( IDOK == nResult)
	{
		cStrFile = cFileDlg.GetPathName();
	}
	SetCurrentDirectory( cStrFolder);

	return ( IDOK == nResult);
}

BOOL CChipExDllSettingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cBtnAbout.EnableWindow( !m_cStrChipExDllName.IsEmpty());
	m_cBtnConfig.EnableWindow( !m_cStrChipExDllName.IsEmpty());
	m_cEdtMenuText.EnableWindow( !m_cStrChipExDllName.IsEmpty());

	m_cBtnSetSeparator.EnableWindow( !m_cStrChipExDllName.IsEmpty() && ( 0 != m_nMenuIndex));
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CChipExDllSettingDialog::OnChipexdllabout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_cStrChipExDllName.IsEmpty())
	{
		MessageBeep( MB_ICONHAND);
		return;
	}

	HINSTANCE	hInst = NULL;
	hInst = LoadLibrary( m_cStrChipExDllName);
	// リンクが正常になされた？
	if( NULL != hInst)
	{
		CHIPEXABOUT	lpChipExAbout;
		// リソースチェックのDLLから、目的の関数ポインタを取得します。
		lpChipExAbout = ( CHIPEXABOUT)GetProcAddress( hInst, "ChipExAbout");
		if( lpChipExAbout)
		{
			lpChipExAbout( ( UINT)0x0100, m_hWnd, m_nMenuIndex);
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_MISSMATCHEXTENDED);
			MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
		FreeLibrary( hInst);
	}
	else
	{
		CString	cStr;
		cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
		MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
	}
}

void CChipExDllSettingDialog::OnChipexdllconfig() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_cStrChipExDllName.IsEmpty())
	{
		MessageBeep( MB_ICONHAND);
		return;
	}

	HINSTANCE	hInst = NULL;
	hInst = LoadLibrary( m_cStrChipExDllName);
	// リンクが正常になされた？
	if( NULL != hInst)
	{
		CHIPEXSPECIFICATION	lpChipExSpecification;
		// ヴァージョンの確認
		lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( hInst, "ChipExSpecification");
		if( lpChipExSpecification)
		{
			SPECIFICATION	stSpecification;
			lpChipExSpecification( &stSpecification);
			if( ( UINT)0x0100 <= stSpecification.unUpperStructVersion && ( UINT)0x0100 >= stSpecification.unLowerStructVersion)
			{
				CHIPEXCONFIGURE	lpChipExConfig;
				// リソースチェックのDLLから、目的の関数ポインタを取得します。
				lpChipExConfig = ( CHIPEXCONFIGURE)GetProcAddress( hInst, "ChipExConfigure");
				if( lpChipExConfig)
				{
					lpChipExConfig( ( UINT)0x0100, m_hWnd, m_nMenuIndex);
				}
				else
				{
					CString	cStr;
					cStr.LoadString( IDS_MISSMATCHEXTENDED);
					MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
				}
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MISSMATCHEXTENEDEDVERSION);
				MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
			}
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_MISSMATCHEXTENDED);
			MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
		FreeLibrary( hInst);
	}
	else
	{
		CString	cStr;
		cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
		MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
	}
}

void CChipExDllSettingDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if( UpdateData())
	{		
		if( m_cStrChipExMenuText.IsEmpty())
		{
			m_cStrChipExDllName.Empty();
			UpdateData( FALSE);
		}
		else
		{
			if( m_cStrChipExDllName.IsEmpty())
			{
				m_cStrChipExMenuText.Empty();
				UpdateData( FALSE);
			}
		}
	
		CDialog::OnOK();
	}
}

BOOL CChipExDllSettingDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_CHIPEXDLLMENUTEXT:
	case IDC_CHIPEXDLLNAME:
	case IDC_CHIPEXDLLCONFIG:
	case IDC_CHIPEXDLLABOUT:
	case IDC_CHIPEXDLLSEL:
	case IDC_CHIPEXSETSEPA:
		::WinHelp( m_hWnd, "SetReg.hlp", HELP_CONTEXTPOPUP, pHelpInfo->iCtrlId | 0x60000);
		return FALSE;
	default:
		return CDialog::OnHelpInfo(pHelpInfo);
	}
}
