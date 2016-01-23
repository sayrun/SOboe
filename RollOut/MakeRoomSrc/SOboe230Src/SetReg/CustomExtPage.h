#if !defined(AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_)
#define AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomExtPage.h : ヘッダー ファイル
//

class CProfileData;

/////////////////////////////////////////////////////////////////////////////
// CCustomExtPage ダイアログ

class CCustomExtPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomExtPage)

// コンストラクション
public:
	CCustomExtPage();
	~CCustomExtPage();

// ダイアログ データ
	//{{AFX_DATA(CCustomExtPage)
	enum { IDD = IDD_CUSTOMEXT };
	BOOL	m_blFlatMenu;
	BOOL	m_blHideStart;
	BOOL	m_blLoggingDelChip;
	BOOL	m_blAutoResize;
	BOOL	m_blHideback;
	BOOL	m_blAutoTriming;
	BOOL	m_blNoNotifyVersion;
	BOOL	m_blNoCheckDelete;
	BOOL	m_blCompatibleOU;
	BOOL	m_blModifyAndSave;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CCustomExtPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCustomExtPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_)
