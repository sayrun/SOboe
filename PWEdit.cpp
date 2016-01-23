// PWEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "PWEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPWEdit ダイアログ


CPWEdit::CPWEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CPWEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPWEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CPWEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPWEdit)
	DDX_Control(pDX, IDC_PASSWORD1ST, m_cEdtPassWord1st);
	DDX_Control(pDX, IDC_PASSWORD2ND, m_cEdtPassWord2nd);
	DDX_Check(pDX, IDC_PASSWORDENABLE, m_blPassWordEnable);
	DDX_Text(pDX, IDC_PASSWORD1ST, m_cStrPassWord1st);
	DDX_Text(pDX, IDC_PASSWORD2ND, m_cStrPassWord2nd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPWEdit, CDialog)
	//{{AFX_MSG_MAP(CPWEdit)
	ON_BN_CLICKED(IDC_PASSWORDENABLE, OnPasswordenable)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPWEdit メッセージ ハンドラ

BOOL CPWEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
	m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CPWEdit::OnPasswordenable() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
		m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);
	}
}

void CPWEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if( UpdateData())
	{
		if( m_blPassWordEnable)
		{
			if( _MINLEN_PW > m_cStrPassWord1st.GetLength() || m_cStrPassWord1st != m_cStrPassWord2nd)
			{
				MessageBox( "設定されたパスワードが異常です", NULL, MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	
	CDialog::OnOK();
}

BOOL CPWEdit::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_PASSWORDENABLE:
	case IDC_PASSWORD1ST:
	case IDC_PASSWORD2ND:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}
