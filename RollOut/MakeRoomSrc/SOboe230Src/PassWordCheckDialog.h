#if !defined(AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PassWordCheckDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPassWordCheckDialog ダイアログ

class CPassWordCheckDialog : public CDialog
{
// コンストラクション
public:
	CPassWordCheckDialog( const CString& cMemoTitle, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPassWordCheckDialog)
	enum { IDD = IDD_PWCHECK };
	CString	m_cStrMemoTitle;
	CString	m_cStrPassWord;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPassWordCheckDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPassWordCheckDialog)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_)
