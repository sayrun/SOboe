#if !defined(AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_)
#define AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DxlEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDxlEdit ダイアログ

class CDxlEdit : public CDialog
{
// コンストラクション
public:
	UINT m_unDxlID;
	CDxlEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDxlEdit)
	enum { IDD = IDD_DRAWEXDLL };
	CEdit	m_cEdtMenuText;
	CButton	m_cBtnDllAbt;
	CButton	m_cBtnDllCnfg;
	CButton	m_cBtnOK;
	CString	m_cStrMenuText;
	CString	m_cStrDxlPath;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDxlEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL GetExtendDll( CString& cStr);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDxlEdit)
	afx_msg void OnDxlsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChipexdllabout();
	afx_msg void OnChipexdllconfig();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_)
