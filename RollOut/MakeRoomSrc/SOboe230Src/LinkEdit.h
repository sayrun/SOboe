#if !defined(AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_)
#define AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CLinkEdit ダイアログ

class CLinkEdit : public CDialog
{
// コンストラクション
public:
	CLinkEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CLinkEdit)
	enum { IDD = IDD_LINK };
	CButton	m_cBtnTimerExec;
	CButton	m_cBtnParametersSel;
	CEdit	m_cEdtParameters;
	CButton	m_cBtnFileSel;
	CEdit	m_cEdtFile;
	CString	m_cStrFile;
	BOOL	m_blLink;
	CString	m_cStrParameters;
	BOOL	m_blTimerExec;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CLinkEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	int DoFileSel( CString& cStrFile, LPCSTR lpcszFilter);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CLinkEdit)
	afx_msg void OnFilesel();
	afx_msg void OnParameterssel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLink();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_)
