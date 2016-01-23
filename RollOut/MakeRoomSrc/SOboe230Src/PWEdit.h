#if !defined(AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_)
#define AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PWEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPWEdit ダイアログ

#define	_MINLEN_PW	4	// パスワードの最低長

class CPWEdit : public CDialog
{
// コンストラクション
public:
	CPWEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPWEdit)
	enum { IDD = IDD_PASSWORD };
	CEdit	m_cEdtPassWord1st;
	CEdit	m_cEdtPassWord2nd;
	CString	m_cStrPassWord1st;
	CString	m_cStrPassWord2nd;
	BOOL	m_blPassWordEnable;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPWEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPWEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPasswordenable();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_)
