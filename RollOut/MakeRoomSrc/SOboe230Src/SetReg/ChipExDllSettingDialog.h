#if !defined(AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_)
#define AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChipExDllSettingDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CChipExDllSettingDialog ダイアログ

class CChipExDllSettingDialog : public CDialog
{
// コンストラクション
public:
	CChipExDllSettingDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CChipExDllSettingDialog)
	enum { IDD = IDD_CHIPEXDLL };
	CButton	m_cBtnSetSeparator;
	CEdit	m_cEdtMenuText;
	CButton	m_cBtnConfig;
	CButton	m_cBtnAbout;
	CString	m_cStrChipExDllName;
	CString	m_cStrChipExMenuText;
	BOOL	m_blSetSepa;
	//}}AFX_DATA
	int	m_nMenuIndex;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CChipExDllSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL GetExtendDll( CString& cStrFile);

	typedef struct tagSPECIFICATION
	{
		UINT	unLowerStructVersion;	// 処理可能な最古バージョン
		UINT	unUpperStructVersion;	// 処理可能な最新バージョン
		char	szMenuString[ 50];		// デフォルトのメニュー名称
	}SPECIFICATION;
	typedef	BOOL ( *CHIPEXSPECIFICATION)( SPECIFICATION* pSpecification);
	typedef	BOOL ( *CHIPEXCONFIGURE)( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
	typedef	BOOL ( *CHIPEXABOUT)( UINT unStructVersion, HWND hWnd, UINT unMenuNum);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChipExDllSettingDialog)
	afx_msg void OnDllsel();
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

#endif // !defined(AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_)
