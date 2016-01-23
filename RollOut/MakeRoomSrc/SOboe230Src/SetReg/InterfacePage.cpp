// InterfacePage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "InterfacePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterfacePage プロパティ ページ

IMPLEMENT_DYNCREATE(CInterfacePage, CPropertyPage)

CInterfacePage::CInterfacePage() : CPropertyPage(CInterfacePage::IDD)
{
	//{{AFX_DATA_INIT(CInterfacePage)
	m_nLDblClkFunc = -1;
	m_nMDblClkFunc = -1;
	//}}AFX_DATA_INIT
}

CInterfacePage::~CInterfacePage()
{
}

void CInterfacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterfacePage)
	DDX_Control(pDX, IDC_HOTKEYLIST, m_cHotKeyCtrl);
	DDX_Control(pDX, IDC_CREEDIT, m_cBtnCEdit);
	DDX_Radio(pDX, IDC_LREEDIT, m_nLDblClkFunc);
	DDX_Radio(pDX, IDC_CREEDIT, m_nMDblClkFunc);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CEXEC, m_cBtnCExec);
	DDX_Control(pDX, IDC_CSMALLBIG, m_cBtnCSmall);
	DDX_Control(pDX, IDC_CTITLE, m_cBtnCTitle);
	DDX_Control(pDX, IDC_CSEND, m_cBtnCSend);
}


BEGIN_MESSAGE_MAP(CInterfacePage, CPropertyPage)
	//{{AFX_MSG_MAP(CInterfacePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CInterfacePage::Is3Button( void)
{
	if( 3 <= GetSystemMetrics( SM_CMOUSEBUTTONS))return TRUE;

	OSVERSIONINFO	stOSVersion;
	stOSVersion.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);
	BOOL blResult = GetVersionEx( &stOSVersion);
	if( VER_PLATFORM_WIN32_NT == stOSVersion.dwPlatformId || blResult == 0)
	{
		if( GetSystemMetrics( 75/*SM_MOUSEWHEELPRESENT*/))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	UINT	uiMsh_MsgMouseWheel;
	UINT	uiMsh_Msg3DSupport;
	UINT	uiMsh_MsgScrollLines;
	BOOL	f3DSupport;
	int		iScrollLines;

	HwndMSWheel( &uiMsh_MsgMouseWheel, &uiMsh_Msg3DSupport, &uiMsh_MsgScrollLines, &f3DSupport, &iScrollLines);

	return f3DSupport;
}

/////////////////////////////////////////////////////////////////////////////
// CInterfacePage メッセージ ハンドラ

BOOL CInterfacePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if( !Is3Button())
	{
		m_nMDblClkFunc = -1;
		m_cBtnCEdit.EnableWindow( FALSE);
		m_cBtnCExec.EnableWindow( FALSE);
		m_cBtnCSmall.EnableWindow( FALSE);
		m_cBtnCTitle.EnableWindow( FALSE);
		m_cBtnCSend.EnableWindow( FALSE);
	}

	m_nLDblClkFunc = m_pcProfileDatas->m_nLDblClkFunc;
	m_nMDblClkFunc = m_pcProfileDatas->m_nMDblClkFunc;

	WORD wData = HIWORD( m_pcProfileDatas->m_dwHotKey);
	WORD wSetData = 0;
	if( MOD_ALT & wData)wSetData |= HOTKEYF_ALT;
	if( MOD_CONTROL & wData)wSetData |= HOTKEYF_CONTROL;
	if( MOD_SHIFT & wData)wSetData |= HOTKEYF_SHIFT;
	m_cHotKeyCtrl.SetRules( HKCOMB_NONE, HOTKEYF_ALT | HOTKEYF_SHIFT);
	m_cHotKeyCtrl.SetHotKey( LOWORD( m_pcProfileDatas->m_dwHotKey), wSetData);

	UpdateData( FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CInterfacePage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	m_pcProfileDatas->m_nLDblClkFunc = m_nLDblClkFunc;
	m_pcProfileDatas->m_nMDblClkFunc = m_nMDblClkFunc;

	WORD wVKey = 0;
	WORD wData = 0;
	WORD wSetData = 0;
	m_cHotKeyCtrl.GetHotKey( wVKey, wSetData);
	if( 0 != wVKey)
	{
		if( HOTKEYF_ALT & wSetData)wData |= MOD_ALT;
		if( HOTKEYF_CONTROL & wSetData)wData |= MOD_CONTROL;
		if( HOTKEYF_SHIFT & wSetData)wData |= MOD_SHIFT;
	}
	m_pcProfileDatas->m_dwHotKey = MAKELONG( wVKey, wData);
	
	CPropertyPage::OnOK();
}
