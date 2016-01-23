// CustomizeSheet.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "CustomizeSheet.h"
#include "ProfileData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

IMPLEMENT_DYNAMIC(CCustomizeSheet, CPropertySheet)

CCustomizeSheet::CCustomizeSheet( BOOL blSpec, CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	// この位置にプロパティ ページをすべて追加してください。
	// ここに追加する順序が画面上に表示される順序になります。
	// デフォルトでは最初のプロパティ ページがアクティブに
	// なります。他のプロパティ ページをアクティブにするた
	// めには、SetActivePage() 関数を呼び出してください。 
	// 
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_cExtDll.m_blSpecSet = blSpec;
	m_cDesign.m_blSpecSet = blSpec;
	m_cTemplet.m_blSpecSet = blSpec;

	AddPage(&m_cDesign);
	if( m_cExtDll.m_blSpecSet)
	{
		AddPage(&m_cNetWork);
	}
	AddPage(&m_cInterface);
	AddPage(&m_cCustomExt);
	AddPage(&m_cNetworkExt);
	AddPage(&m_cExtDll);
	AddPage(&m_cSecurity);
	AddPage(&m_cTemplet);
}

CCustomizeSheet::~CCustomizeSheet()
{
}


BEGIN_MESSAGE_MAP(CCustomizeSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCustomizeSheet)
	ON_WM_NCCREATE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet メッセージ ハンドラ
void CCustomizeSheet::SetProfileData( CProfileData* pcProfileData)
{
	m_cNetWork.m_pcProfileDatas = pcProfileData;
	m_cDesign.m_pcProfileDatas = pcProfileData;
	m_cInterface.m_pcProfileDatas = pcProfileData;
	m_cCustomExt.m_pcProfileDatas = pcProfileData;
	m_cNetworkExt.m_pcProfileDatas = pcProfileData;
	m_cExtDll.m_pcProfileDatas = pcProfileData;
	m_cSecurity.m_pcProfileDatas = pcProfileData;
	m_cTemplet.m_pcProfileDatas = pcProfileData;
}

BOOL CCustomizeSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_DESIGNTYPE:
	case IDC_FONT:
	case IDC_COLOR:
	case IDC_SAMPLE:
	case IDC_ICONLIST:
	case IDC_IMEAUTO:
	case IDC_DATAFILE:
	case IDC_DATAFILESEL:
	case IDC_GENERATIONCOUNT:
	case IDC_GENERATIONCOUNTSPIN:
		// 拡張機能
	case IDC_MENUFLAT:
	case IDC_HIDESTART:
	case IDC_DELLOG:
	case IDC_CALCDEFAULT:
	case IDC_HIDEBACK:
	case IDC_NOCHECKDELETE:
	case IDC_COMPATIBLEOU:
	case IDC_AUTOTRIMMING:
	case IDC_VERUNNOTIFY:
	case IDC_MFSAVE:
		// ネットワーク関連
	case IDC_ENABLENETWORK:
	case IDC_SIGNATURE:
	case IDC_READYDELAY:
	case IDC_DELAYSPIN:
	case IDC_NXLLIST:
	case IDC_NXLINSTALL:
	case IDC_NXLUNINSTALL:
	case IDC_NXLCONFIG:
	case IDC_NXLABOUT:
		// セキュリティ
	case IDC_OLDPASSWORD:
	case IDC_NEWPASSWORD:
	case IDC_CHECKPASSWORD:
	case IDC_ALLPASSWORD:
	case IDC_RECEPTIONLINK:
	case IDC_RECEPTIONTIMER:
	case IDC_RECEPTIONDXL:
		// 拡張DLL
	case IDC_ARRANGEDLL_SEL:
	case IDC_ARRANGEDLL_CONFIG:
	case IDC_ARRANGEDLL_ABOUT:
	case IDC_ARRANGEDLL_PATH:
	case IDC_ARRANGEDLL:
	case IDC_CHIPEXDLLLIST:
	case IDC_CHIPEXDLLDELETE:
	case IDC_CHIPEXDLLEDIT:
	case IDC_DRAWEXLIST:
	case IDC_DRAWEXDELETE:
	case IDC_DRAWEXEDIT:
		// マウスとキー
	case IDC_LREEDIT:
	case IDC_LEXEC:
	case IDC_LSMALLBIG:
	case IDC_LTITLE:
	case IDC_LSEND:
	case IDC_CREEDIT:
	case IDC_CEXEC:
	case IDC_CSMALLBIG:
	case IDC_CTITLE:
	case IDC_CSEND:
	case IDC_HOTKEYLIST:
		// 送信の設定
	case IDC_SENDLOG:
	case IDC_SENDLOGFULL:
	case IDC_RECEIVELOG:
	case IDC_RECEIVELOGFULL:
	case IDC_SENDPACKDEFAULT:
	case IDC_SENDOPENDEFAULT:
	case IDC_DELONQUITDEFAULT:
	case IDC_RECEIVECHIPHIDE:
	case IDC_INFOHIDE:
	case IDC_RCSAVE:
	case IDC_DELETEAFTERSEND:
		// テンプレート
	case IDC_ENABLETEMPLET:
	case IDC_TEMPLETLIST:
	case IDC_UPITEM:
	case IDC_DOWNITEM:
	case IDC_TEMPLETDEL:
		::WinHelp( m_hWnd, "SetReg.hlp", HELP_CONTEXTPOPUP, pHelpInfo->iCtrlId | 0x60000);
		return FALSE;
	default:
		return CPropertySheet::OnHelpInfo(pHelpInfo);
	}
	
	return CPropertySheet::OnHelpInfo(pHelpInfo);
}

BOOL CCustomizeSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: この位置に固有のコードを追加してください
	ModifyStyleEx( 0, WS_EX_CONTEXTHELP | WS_EX_APPWINDOW, 0);
	
	return bResult;
}
