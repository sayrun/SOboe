#if !defined(AFX_RESULTCHECKDIALOG_H__31583092_51C4_11D2_A0EE_00C04FCA10B7__INCLUDED_)
#define AFX_RESULTCHECKDIALOG_H__31583092_51C4_11D2_A0EE_00C04FCA10B7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ResultCheckDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CResultCheckDialog ダイアログ

class CResultCheckDialog : public CDialog
{
// コンストラクション
public:
	CResultCheckDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

	void SetResults( const CString& cStrResult1, const CString& cStrResult2, BOOL blResult3, const CString& cStrResult3);
	BOOL GetResult( CString& cStrResult);

// ダイアログ データ
	//{{AFX_DATA(CResultCheckDialog)
	enum { IDD = IDD_RESULTCHECK };
	CButton	m_cBtnOK;
	CEdit	m_cEdtResult3;
	CButton	m_cBtnResult1;
	CButton	m_cBtnResult2;
	CButton	m_cBtnResult3;
	CString	m_cStrResult3;
	int		m_nResultSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CResultCheckDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CResultCheckDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnResultChange();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString	m_cStrResult1;
	CString	m_cStrResult2;
	BOOL	m_blResult3;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RESULTCHECKDIALOG_H__31583092_51C4_11D2_A0EE_00C04FCA10B7__INCLUDED_)
