#if !defined(AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_)
#define AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecurityPage.h : ヘッダー ファイル
//

#define	MIN_PASSWORD	4

class CProfileData;

/////////////////////////////////////////////////////////////////////////////
// CSecurityPage ダイアログ

class CSecurityPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSecurityPage)

// コンストラクション
public:
	CSecurityPage();
	~CSecurityPage();

// ダイアログ データ
	//{{AFX_DATA(CSecurityPage)
	enum { IDD = IDD_SECURITY };
	CString	m_cStrOldPW;
	CString	m_cStrNewPW;
	CString	m_cStrCheckPW;
	BOOL	m_blReceptionTimer;
	BOOL	m_blReceptionLink;
	BOOL	m_blReceptionDxl;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CSecurityPage)
	public:
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSecurityPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_)
