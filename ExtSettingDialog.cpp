// ExtSettingDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "ExtSettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtSettingDialog ダイアログ


CExtSettingDialog::CExtSettingDialog( BOOL blEnableExt, CWnd* pParent /*=NULL*/)
	: CDialog(CExtSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtSettingDialog)
	m_cStrResult1.Empty();
	m_cStrResult2.Empty();
	m_blNotifyOpen = FALSE;
	m_blNotifyResult = FALSE;
	m_blOtherResult = FALSE;
	m_blNeedPassWord = FALSE;
	//}}AFX_DATA_INIT
	m_blEnableExt = blEnableExt;
}


void CExtSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtSettingDialog)
	DDX_Control(pDX, IDC_OTHERRESULT, m_cBtnOtherResult);
	DDX_Control(pDX, IDC_OPENPASSWORD, m_cBtnNeedPassWord);
	DDX_Control(pDX, IDC_NOTIFYRESULT, m_cBtnNotifyResult);
	DDX_Control(pDX, IDC_NOTIFYOPEN, m_cBtnNotifyOpen);
	DDX_Control(pDX, IDC_EDTRESULT2, m_cEdtResult2);
	DDX_Control(pDX, IDC_EDTRESULT1, m_cEdtResult1);
	DDX_Text(pDX, IDC_EDTRESULT1, m_cStrResult1);
	DDV_MaxChars(pDX, m_cStrResult1, 20);
	DDX_Text(pDX, IDC_EDTRESULT2, m_cStrResult2);
	DDV_MaxChars(pDX, m_cStrResult2, 20);
	DDX_Check(pDX, IDC_NOTIFYOPEN, m_blNotifyOpen);
	DDX_Check(pDX, IDC_NOTIFYRESULT, m_blNotifyResult);
	DDX_Check(pDX, IDC_OTHERRESULT, m_blOtherResult);
	DDX_Check(pDX, IDC_OPENPASSWORD, m_blNeedPassWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CExtSettingDialog)
	ON_BN_CLICKED(IDC_NOTIFYOPEN, OnSettingCheck)
	ON_BN_CLICKED(IDC_NOTIFYRESULT, OnSettingCheck)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtSettingDialog メッセージ ハンドラ

BOOL CExtSettingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	if( m_blEnableExt)
	{
		if( m_blNotifyResult && m_blNotifyOpen)m_blNotifyResult = FALSE;

		m_cBtnNotifyResult.EnableWindow( !m_blNotifyOpen);
		m_cEdtResult1.EnableWindow( m_blNotifyResult);
		m_cEdtResult2.EnableWindow( m_blNotifyResult);
		m_cBtnOtherResult.EnableWindow( m_blNotifyResult);
		m_cBtnNotifyOpen.EnableWindow( !m_blNotifyResult);
	}
	else
	{
		m_cBtnNotifyResult.EnableWindow( FALSE);
		m_cEdtResult1.EnableWindow( FALSE);
		m_cEdtResult2.EnableWindow( FALSE);
		m_cBtnOtherResult.EnableWindow( FALSE);
		m_cBtnNotifyOpen.EnableWindow( FALSE);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CExtSettingDialog::OnSettingCheck() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_blEnableExt)
	{
		if( UpdateData())
		{
			m_cBtnNotifyResult.EnableWindow( !m_blNotifyOpen);
			m_cEdtResult1.EnableWindow( m_blNotifyResult);
			m_cEdtResult2.EnableWindow( m_blNotifyResult);
			m_cBtnOtherResult.EnableWindow( m_blNotifyResult);
			m_cBtnNotifyOpen.EnableWindow( !m_blNotifyResult);
		}	
	}
}

BOOL CExtSettingDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_OPENPASSWORD:
	case IDC_NOTIFYOPEN:
	case IDC_NOTIFYRESULT:
	case IDC_OTHERRESULT:
	case IDC_EDTRESULT1:
	case IDC_EDTRESULT2:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}
