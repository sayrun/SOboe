#if !defined(AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_)
#define AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempletPage.h : ヘッダー ファイル
//
#include "MemoListBox.h"

class CProfileData;
/////////////////////////////////////////////////////////////////////////////
// CTempletPage ダイアログ

class CTempletPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTempletPage)

// コンストラクション
public:
	CTempletPage();
	~CTempletPage();

// ダイアログ データ
	//{{AFX_DATA(CTempletPage)
	enum { IDD = IDD_TEMPLET };
	CButton	m_cBtnDownItem;
	CButton	m_cBtnUpItem;
	CButton	m_cBtnTemletDel;
	CMemoListBox	m_cLstTemplet;
	BOOL	m_blEnableTemplet;
	int		m_nLstTemplet;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CTempletPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CString	m_cStrTempletFile;

	void LoadTempletData( void);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTempletPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnabletemplet();
	afx_msg void OnTempletdel();
	afx_msg void OnSelchangeTempletlist();
	afx_msg void OnDownitem();
	afx_msg void OnUpitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL			m_blSpecSet;
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_)
