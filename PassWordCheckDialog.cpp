// PassWordCheckDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "PassWordCheckDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassWordCheckDialog ダイアログ


CPassWordCheckDialog::CPassWordCheckDialog( const CString& cMemoTitle, CWnd* pParent /*=NULL*/)
	: CDialog(CPassWordCheckDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPassWordCheckDialog)
	m_cStrMemoTitle = cMemoTitle;
	m_cStrPassWord = _T("");
	//}}AFX_DATA_INIT
}


void CPassWordCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPassWordCheckDialog)
	DDX_Text(pDX, IDC_TITLE, m_cStrMemoTitle);
	DDX_Text(pDX, IDC_PWINPUT, m_cStrPassWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPassWordCheckDialog, CDialog)
	//{{AFX_MSG_MAP(CPassWordCheckDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassWordCheckDialog メッセージ ハンドラ
