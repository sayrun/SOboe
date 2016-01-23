#if !defined(AFX_EXTENDEDITPAGE_H__741DB809_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_EXTENDEDITPAGE_H__741DB809_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExtendEditPage.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CExtendEditPage ダイアログ

#define	_MINLEN_PW	4	// パスワードの最低長

class CExtendEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CExtendEditPage)

// コンストラクション
public:
	CExtendEditPage();
	~CExtendEditPage();

// ダイアログ データ
	UINT	m_unDxlID;
	//{{AFX_DATA(CExtendEditPage)
	enum { IDD = IDD_EXTENDPAGE };
	CComboBox	m_cCmbDxlSel;
	CEdit	m_cEdtPassWord2nd;
	CEdit	m_cEdtPassWord1st;
	CButton	m_cBtnTimerExec;
	CButton	m_cBtnParametersSel;
	CButton	m_cBtnFileSel;
	CEdit	m_cEdtParameters;
	CEdit	m_cEdtFile;
	BOOL	m_blLink;
	BOOL	m_blTimerExec;
	CString	m_cStrParameters;
	CString	m_cStrFile;
	BOOL	m_blPassWordEnable;
	CString	m_cStrPassWord1st;
	CString	m_cStrPassWord2nd;
	int		m_nZOrder;
	int		m_nDxlSel;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CExtendEditPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	int DoFileSel( CString& cStrFile, LPCSTR lpcszFilter);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExtendEditPage)
	afx_msg void OnLink();
	virtual BOOL OnInitDialog();
	afx_msg void OnPasswordenable();
	afx_msg void OnParameterssel();
	afx_msg void OnFilesel();
	afx_msg void OnSelchangeDxlsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXTENDEDITPAGE_H__741DB809_2743_11D2_90A4_00804C15184E__INCLUDED_)
