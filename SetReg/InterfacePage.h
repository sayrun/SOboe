#if !defined(AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_)
#define AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfacePage.h : ヘッダー ファイル
//

class CProfileData;
/////////////////////////////////////////////////////////////////////////////
// CInterfacePage ダイアログ

class CInterfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInterfacePage)

// コンストラクション
public:
	CInterfacePage();
	~CInterfacePage();

// ダイアログ データ
	//{{AFX_DATA(CInterfacePage)
	enum { IDD = IDD_INTERFACE };
	CHotKeyCtrl	m_cHotKeyCtrl;
	CButton	m_cBtnCEdit;
	CButton	m_cBtnCExec;
	CButton	m_cBtnCSmall;
	CButton	m_cBtnCTitle;
	CButton	m_cBtnCSend;
	int		m_nLDblClkFunc;
	int		m_nMDblClkFunc;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CInterfacePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL Is3Button( void);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CInterfacePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_)
