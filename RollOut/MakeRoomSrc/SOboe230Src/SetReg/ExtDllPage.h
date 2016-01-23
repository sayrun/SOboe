#if !defined(AFX_EXTDLLPAGE_H__904DCC91_827F_11D2_9205_004005469C82__INCLUDED_)
#define AFX_EXTDLLPAGE_H__904DCC91_827F_11D2_9205_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtDllPage.h : ヘッダー ファイル
//

class CProfileData;
/////////////////////////////////////////////////////////////////////////////
// CExtDllPage ダイアログ

#define	_ARGDLL_TYPE_COUNT	4

class CExtDllPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CExtDllPage)

// コンストラクション
public:
	CExtDllPage();
	~CExtDllPage();

	BOOL	m_blSpecSet;

// ダイアログ データ
	//{{AFX_DATA(CExtDllPage)
	enum { IDD = IDD_EXTDLL };
	CButton	m_cBtnArgDllSel;
	CEdit	m_cEdtArgDllPath;
	CButton	m_cBtnArgDllConfig;
	CButton	m_cBtnArgDllAbout;
	CButton	m_cBtnDrawExDelete;
	CButton	m_cBtnDrawExEdit;
	CButton	m_cBtnChipExDllDelete;
	CButton	m_cBtnChipExDllEdit;
	CListBox	m_cLstDrawEx;
	CListBox	m_cLstChipExDll;
	int		m_nChipExDllList;
	int		m_nDrawExDll;
	int		m_nArgDll;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CExtDllPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CString		m_cStrCelPath[ _CEL_MAX];
	CString		m_cStrCelMenuString[ _CEL_MAX];
	CString		m_cStrMenuSeparator;
	CString		m_cStrDxlPath[ _DRAWEXLIB_MAX];
	CString		m_cStrDxlMenuString[ _DRAWEXLIB_MAX];
	UINT		m_unDxlID[ _DRAWEXLIB_MAX];
	UINT		m_unDefaultDxlID;

	CString		m_cStrArrangeDll[ _ARGDLL_TYPE_COUNT];
	int			m_nArgDllOld;

	typedef struct tagSPECIFICATION
	{
		UINT	unLowerStructVersion; // 処理可能な最古バージョン
		UINT	unUpperStructVersion; // 処理可能な最新バージョン
	}SPECIFICATION;

	typedef	BOOL ( *ARRANGESTRUCTVERSION)( SPECIFICATION*);
	typedef	BOOL ( *ARRANGEDLG)( UINT, HWND);

	BOOL GetExtendDll( CString & cStrFile);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExtDllPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeChipexdlllist();
	afx_msg void OnChipexdlledit();
	afx_msg void OnChipexdlldelete();
	afx_msg void OnSelchangeDrawexlist();
	afx_msg void OnDrawexedit();
	afx_msg void OnDrawexdelete();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdrawex();
	afx_msg void OnDowndrawex();
	afx_msg void OnSelchangeArrangedll();
	afx_msg void OnArrangedllSel();
	afx_msg void OnArrangedllConfig();
	afx_msg void OnArrangedllAbout();
	afx_msg void OnChangeArrangedllPath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
	afx_msg void OnSetdefaultdxl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXTDLLPAGE_H__904DCC91_827F_11D2_9205_004005469C82__INCLUDED_)
