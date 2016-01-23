#if !defined(AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_)
#define AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintChipDialog.h : ヘッダー ファイル
//

class CMemoData;
/////////////////////////////////////////////////////////////////////////////
// CPrintChipDialog ダイアログ

class CPrintChipDialog : public CDialog
{
// コンストラクション
public:
	CPrintChipDialog( HDC& hPrinterDC, RECT& stRctMargin, CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CPrintChipDialog( void);

	BOOL AddPrintChip( const CMemoData* pcMemoData);

// ダイアログ データ
	//{{AFX_DATA(CPrintChipDialog)
	enum { IDD = IDD_PRINTCHIPDLG };
	CStatic	m_cStcTitle;
	CButton	m_cBtnCancel;
	CProgressCtrl	m_cPrgrsPrintChip;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPrintChipDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CList <CMemoData*, CMemoData*&>	m_cLstMemoData;

	HANDLE		m_hCancelEvent;
	HDC			m_hPrinterDC;
	RECT		m_stRctMargin;

	static UINT	sm_unNotifyEnd;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPrintChipDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LONG OnNotifyEnd( UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	enum
	{
		RESULT_SUCCESS,
		RESULT_ERROR,
		RESULT_ABORT
	};

	friend void PrintChipThread( void* lpvData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

void PrintChipThread( void* lpvData);

#endif // !defined(AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_)
