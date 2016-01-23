// NetworkPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "NetworkPage.h"
#include "ProfileData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CNetworkPage, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CNetworkPage プロパティ ページ

CNetworkPage::CNetworkPage() : CPropertyPage(CNetworkPage::IDD)
{
	//{{AFX_DATA_INIT(CNetworkPage)
	m_blEnableNetwork = FALSE;
	m_cStrSignature = _T("");
	m_nResumeDelay = 0;
	//}}AFX_DATA_INIT
}

CNetworkPage::~CNetworkPage()
{
}

void CNetworkPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPage)
	DDX_Control(pDX, IDC_DELAYSPIN, m_cSpnDelay);
	DDX_Control(pDX, IDC_SIGNATURE, m_cEdtSignature);
	DDX_Control(pDX, IDC_READYDELAY, m_cEdtReadyDelay);
	DDX_Control(pDX, IDC_NXLCONFIG, m_cBtnConfig);
	DDX_Control(pDX, IDC_NXLABOUT, m_cBtnAbout);
	DDX_Control(pDX, IDC_NXLINSTALL, m_cBtnInst);
	DDX_Control(pDX, IDC_NXLUNINSTALL, m_cBtnUninst);
	DDX_Control(pDX, IDC_NXLLIST, m_cLstNxl);
	DDX_Check(pDX, IDC_ENABLENETWORK, m_blEnableNetwork);
	DDX_Text(pDX, IDC_SIGNATURE, m_cStrSignature);
	DDX_Text(pDX, IDC_READYDELAY, m_nResumeDelay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNetworkPage)
	ON_LBN_SELCHANGE(IDC_NXLLIST, OnSelchangeNxllist)
	ON_BN_CLICKED(IDC_NXLABOUT, OnNxlabout)
	ON_BN_CLICKED(IDC_NXLCONFIG, OnNxlconfig)
	ON_LBN_DBLCLK(IDC_NXLLIST, OnDblclkNxllist)
	ON_BN_CLICKED(IDC_NXLINSTALL, OnNxlinstall)
	ON_BN_CLICKED(IDC_NXLUNINSTALL, OnNxluninstall)
	ON_BN_CLICKED(IDC_ENABLENETWORK, OnEnablenetwork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CNetworkPage::OnInitDialog() 
{
	m_blEnableNetwork = m_pcProfileDatas->m_blEnableNetwork;
	m_cStrSignature = m_pcProfileDatas->m_cStrSignature;
	m_nResumeDelay = m_pcProfileDatas->m_nResumeDelay;

	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString	cStrMenuString;
	UINT	unNxlID;
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( CheckNxl( m_pcProfileDatas->m_cStrNxlPath[ nIndex], cStrMenuString, unNxlID))
		{
			m_cStrNxlPath[ nIndex] = m_pcProfileDatas->m_cStrNxlPath[ nIndex];
			m_cStrNxlMenu[ nIndex] = cStrMenuString;
			m_unNxlID[ nIndex] = unNxlID;
		}
		else
		{
			m_cStrNxlPath[ nIndex].Empty();
			m_cStrNxlMenu[ nIndex].LoadString( IDS_INVALID);
			m_unNxlID[ nIndex] = 0;
		}
		m_cLstNxl.AddString( m_cStrNxlMenu[ nIndex]);
	}
	m_nDefaultNxl = m_pcProfileDatas->m_nDefaultNxl;
	
	m_cBtnInst.EnableWindow( FALSE);
	m_cBtnUninst.EnableWindow( FALSE);
	m_cBtnAbout.EnableWindow( FALSE);
	m_cBtnConfig.EnableWindow( FALSE);

	if( !m_blEnableNetwork)
	{
		m_cLstNxl.EnableWindow( FALSE);
		m_cEdtReadyDelay.EnableWindow( FALSE);
		m_cSpnDelay.EnableWindow( FALSE);
		m_cEdtSignature.EnableWindow( FALSE);
	}

	m_cSpnDelay.SetRange( 0, 60);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CNetworkPage::CheckNxl( const CString cStrPath, CString& cStrMenuString, UINT& unNxlID)
{
	if( cStrPath.IsEmpty())return FALSE;

	HINSTANCE hInstance = LoadLibrary( cStrPath);
	if( NULL == hInstance)
	{
		char	szWork[ 128];
		wsprintf( szWork, "Nxlを正しく読み込めませんでした。\nErrorCode[%d]", GetLastError());
		::MessageBox( NULL, szWork, "LoadLibraryの失敗", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	// 機能確認
	PNETEXSPECIFICATION	lpfNetExSpecification;
	lpfNetExSpecification = ( PNETEXSPECIFICATION)GetProcAddress( hInstance, "NetExSpecification");
	if( NULL != lpfNetExSpecification)
	{
		NXLSPECIFICATION	stNxlSpec;
		if( lpfNetExSpecification( &stNxlSpec))
		{
			cStrMenuString = stNxlSpec.szProtcolString;
			unNxlID = stNxlSpec.unNxlID;
			/*
			m_unNxlID = stNxlSpec.unNxlID;
			m_unVersion = stNxlSpec.unLowerStructVersion;
			m_unNxlStyle = stNxlSpec.unNxlStyle;
			*/

			return TRUE;
		}
#ifdef	_DEBUG
		else
		{
			::MessageBox( NULL, "NetExSpecification呼び出しの失敗", NULL, MB_OK);
		}
#endif
	}
#ifdef	_DEBUG
	else
	{
		::MessageBox( NULL, "GetProcAddressの失敗", "NetExSpecification", MB_OK);
	}
#endif
	return FALSE;
}

void CNetworkPage::OnSelchangeNxllist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		BOOL blNxlEnable = !m_cStrNxlPath[ nSel].IsEmpty();
		m_cBtnInst.EnableWindow( !blNxlEnable);
		m_cBtnUninst.EnableWindow( blNxlEnable);
		m_cBtnAbout.EnableWindow( blNxlEnable);
		m_cBtnConfig.EnableWindow( blNxlEnable);
	}
}

void CNetworkPage::OnDblclkNxllist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		if( !m_cStrNxlPath[ nSel].IsEmpty())
		{
			OnNxlconfig();
		}
		else
		{
			OnNxlinstall();
		}
	}	
}

void CNetworkPage::OnNxlabout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		if( !m_cStrNxlPath[ nSel].IsEmpty())
		{
			HINSTANCE hInstance = LoadLibrary( m_cStrNxlPath[ nSel]);
			if( NULL == hInstance)
			{
#ifdef	_DEBUG
				char	szWork[ 128];
				wsprintf( szWork, "EC=%d", GetLastError());
				::MessageBox( NULL, szWork, "LoadLibraryの失敗", MB_OK);
#endif
				return;
			}

			// 機能確認
			PNETEXABOUT	lpfNetExAbout;
			lpfNetExAbout = ( PNETEXABOUT)GetProcAddress( hInstance, "NetExAbout");
			if( NULL != lpfNetExAbout)
			{
				lpfNetExAbout( m_hWnd);
			}
#ifdef	_DEBUG
			else
			{
				::MessageBox( NULL, "GetProcAddressの失敗", "NetExAbout", MB_OK);
			}
#endif
		}
	}
}

void CNetworkPage::OnNxlconfig() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		if( !m_cStrNxlPath[ nSel].IsEmpty())
		{
			HINSTANCE hInstance = LoadLibrary( m_cStrNxlPath[ nSel]);
			if( NULL == hInstance)
			{
#ifdef	_DEBUG
				char	szWork[ 128];
				wsprintf( szWork, "EC=%d", GetLastError());
				::MessageBox( NULL, szWork, "LoadLibraryの失敗", MB_OK);
#endif
				return;
			}

			// 機能確認
			PNETEXCONFIGURE	lpfNetExConfigure;
			lpfNetExConfigure = ( PNETEXCONFIGURE)GetProcAddress( hInstance, "NetExConfigure");
			if( NULL != lpfNetExConfigure)
			{
				lpfNetExConfigure( m_hWnd);
			}
#ifdef	_DEBUG
			else
			{
				::MessageBox( NULL, "GetProcAddressの失敗", "NetExConfigure", MB_OK);
			}
#endif
		}
	}
}

void CNetworkPage::OnNxlinstall() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		if( m_cStrNxlPath[ nSel].IsEmpty())
		{
			CFileDialog	cFileDlg( TRUE, "nxl", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "Net Ex Dll(*.nxl)|*.nxl||", this);

			if( IDOK == cFileDlg.DoModal())
			{
				CString	cStrPath;
				CString	cStrMenu;
				UINT	unNxlID;
				cStrPath = cFileDlg.GetPathName();
				if( CheckNxl( cStrPath, cStrMenu, unNxlID))
				{
					for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
					{
						if( 0 < m_unNxlID[ nIndex])
						{
							if( unNxlID == m_unNxlID[ nIndex])
							{
								MessageBox( "二重定義はできない", NULL, MB_ICONHAND);
								return;
							}
						}
					}
					m_cStrNxlPath[ nSel] = cStrPath;
					m_cStrNxlMenu[ nSel] = cStrMenu;
					m_unNxlID[ nSel] = unNxlID;
					m_cLstNxl.InsertString( nSel, m_cStrNxlMenu[ nSel]);
					m_cLstNxl.DeleteString( nSel + 1);
				}
			}
		}
	}
}

void CNetworkPage::OnNxluninstall() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel = m_cLstNxl.GetCurSel();
	if( 0 <= nSel && _NETEXLIB_MAX > nSel)
	{
		if( !m_cStrNxlPath[ nSel].IsEmpty())
		{
			if( IDYES == MessageBox( "まことにアンインストールするのかえ？", NULL, MB_ICONQUESTION | MB_YESNO))
			{
				m_cStrNxlPath[ nSel].Empty();
				m_cStrNxlMenu[ nSel].LoadString( IDS_INVALID);
				m_unNxlID[ nSel] = 0;
				m_cLstNxl.InsertString( nSel, m_cStrNxlMenu[ nSel]);
				m_cLstNxl.DeleteString( nSel + 1);
			}
		}
	}	
}

void CNetworkPage::OnEnablenetwork() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( !m_blEnableNetwork)
	{
		m_cBtnInst.EnableWindow( FALSE);
		m_cBtnUninst.EnableWindow( FALSE);
		m_cBtnAbout.EnableWindow( FALSE);
		m_cBtnConfig.EnableWindow( FALSE);

		m_cLstNxl.EnableWindow( FALSE);
		m_cLstNxl.SetCurSel( -1);
		m_cEdtReadyDelay.EnableWindow( FALSE);
		m_cSpnDelay.EnableWindow( FALSE);
		m_cEdtSignature.EnableWindow( FALSE);
	}
	else
	{
		m_cLstNxl.EnableWindow( TRUE);
		m_cEdtReadyDelay.EnableWindow( TRUE);
		m_cSpnDelay.EnableWindow( TRUE);
		m_cEdtSignature.EnableWindow( TRUE);
	}
}

void CNetworkPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		m_pcProfileDatas->m_cStrNxlPath[ nIndex] = m_cStrNxlPath[ nIndex];
	}
	m_pcProfileDatas->m_nDefaultNxl = m_nDefaultNxl;
	
	m_pcProfileDatas->m_nResumeDelay = m_nResumeDelay;
	m_pcProfileDatas->m_blEnableNetwork = m_blEnableNetwork;
	m_pcProfileDatas->m_cStrSignature = m_cStrSignature;
	CPropertyPage::OnOK();
}
