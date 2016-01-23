// AboutDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog ダイアログ


CAboutDialog::CAboutDialog( int nWinType, CWnd* pParent)
	: CDialog( ( CAboutDialog::WINTYPE_98 == nWinType) ? CAboutDialog::IDD98 : CAboutDialog::IDDNT, pParent)
{
	m_lpGetFreeSystemResource = NULL;
	m_nWinType = nWinType;
	if( WINTYPE_98 == m_nWinType)
	{
	//{{AFX_DATA_INIT(CAboutDialog)
	m_cStrGDI = _T("");
	m_cStrSYSTEM = _T("");
	m_cStrUSER = _T("");
	//}}AFX_DATA_INIT
	}
}


void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAboutDialog)
	DDX_Control(pDX, IDC_PHYSICAL, m_cPgbPhysical);		
	DDX_Text(pDX, IDC_PHYSICALPER, m_cStrPhysical);
	if( WINTYPE_98 == m_nWinType)
	{
		DDX_Control(pDX, IDC_USER, m_cPgbUSER);
		DDX_Control(pDX, IDC_SYSTEM, m_cPgbSYSTEM);
		DDX_Control(pDX, IDC_GDI, m_cPgbGDI);
		DDX_Text(pDX, IDC_GDIPER, m_cStrGDI);
		DDX_Text(pDX, IDC_SYSTEMPER, m_cStrSYSTEM);
		DDX_Text(pDX, IDC_USERPER, m_cStrUSER);
	}
	else
	{
		DDX_Control(pDX, IDC_PAGEFILE, m_cPgbPagefile);		
		DDX_Text(pDX, IDC_PAGEFILEPER, m_cStrPagefile);
	}
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	ON_BN_CLICKED(IDC_GOSUPORT, OnGosuport)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog メッセージ ハンドラ

void CAboutDialog::OnGosuport() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ShellExecute( GetSafeHwnd(), "open", "http://www2.famille.ne.jp/~chiba/software/", NULL, NULL, SW_NORMAL);
	EndDialog( IDOK);
}

BOOL CAboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cPgbPhysical.SetRange( 0, 100);

	if( WINTYPE_98 == m_nWinType)
	{
		m_cPgbUSER.SetRange( 0, 100);
		m_cPgbGDI.SetRange( 0, 100);
		m_cPgbSYSTEM.SetRange( 0, 100);

		// リソースチェックを含むDLLを明示的にリンクします。
		m_hInst = LoadLibrary( "RSRC32.DLL");
		// リンクが正常になされた？
		if( NULL != m_hInst)
		{
			// リソースチェックのDLLから、目的の関数ポインタを取得します。
			m_lpGetFreeSystemResource = ( GetFreeSystemResource)GetProcAddress( m_hInst, "_MyGetFreeSystemResources32@4");
		}
	}
	else
	{
		m_cPgbPagefile.SetRange( 0, 100);
	}

	CheckResources();
	SetTimer( 4096, 500, NULL);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CAboutDialog::CheckResources( void)
{
	MEMORYSTATUS	stMemStat;
	stMemStat.dwLength = sizeof( MEMORYSTATUS);
	GlobalMemoryStatus( &stMemStat);
	UINT	unTotalPhys = stMemStat.dwTotalPhys;
	UINT	unAvailPhys = stMemStat.dwAvailPhys;
	unAvailPhys /= ( 1024 * 1024);
	unTotalPhys /= ( 1024 * 1024);
	UINT unData = ( ( unAvailPhys) * 100) / unTotalPhys;
	m_cPgbPhysical.SetPos( unData);

	if( WINTYPE_98 == m_nWinType)
	{
		m_cStrPhysical.Format( IDS_RESOURCEINFO, unData);
		// 取得成功？
		if( NULL != m_lpGetFreeSystemResource)
		{
			// システム・リソースの【残り％】取得
			UINT unSystem = m_lpGetFreeSystemResource( 0);
			// ＧＤＩ・リソースの【残り％】取得
			UINT unGDI = m_lpGetFreeSystemResource( 1);
			// ＵＳＥＲ・リソースの【残り％】取得
			UINT unUSER = m_lpGetFreeSystemResource( 2);

			m_cPgbSYSTEM.SetPos( ( int)unSystem);
			m_cPgbGDI.SetPos( ( int)unGDI);
			m_cPgbUSER.SetPos( ( int)unUSER);

			m_cStrSYSTEM.Format( IDS_RESOURCEINFO, unSystem);
			m_cStrGDI.Format( IDS_RESOURCEINFO, unGDI);
			m_cStrUSER.Format( IDS_RESOURCEINFO, unUSER);
		}
	}
	else
	{
		m_cStrPhysical.Format( IDS_MEMORYINFO, ( unAvailPhys), unTotalPhys);

		UINT unTotalPageFile = stMemStat.dwTotalPageFile;
		UINT unAvailPageFile = stMemStat.dwAvailPageFile;
		unAvailPageFile /= ( 1024 * 1024);
		unTotalPageFile /= ( 1024 * 1024);
		m_cStrPagefile.Format( IDS_MEMORYINFO, ( unAvailPageFile), unTotalPageFile);
		unData = ( ( unAvailPageFile) * 100) / unTotalPageFile;
		m_cPgbPagefile.SetPos( unData);
	}
	UpdateData( FALSE);
	return TRUE;
}

void CAboutDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CheckResources();
	
	CDialog::OnTimer(nIDEvent);
}

void CAboutDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	KillTimer( 4096);

	if( WINTYPE_98 == m_nWinType)
	{
		if( m_hInst)
		{
			FreeLibrary( m_hInst);
			m_hInst = NULL;
		}
	}
}
