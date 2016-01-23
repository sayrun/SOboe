#if !defined(AFX_RESIZABLEMEMOEDITDIALOG_H__CA05B113_0838_11D3_92FC_004005469C82__INCLUDED_)
#define AFX_RESIZABLEMEMOEDITDIALOG_H__CA05B113_0838_11D3_92FC_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizableMemoEditDialog.h : �w�b�_�[ �t�@�C��
//
#define	EDITDLG_TOOLBAR	25

/////////////////////////////////////////////////////////////////////////////
// CResizableMemoEditDialog �_�C�A���O
#include "MemoData.h"
#include "SpEdit.h"

class CResizableMemoEditDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CResizableMemoEditDialog( const CString cStrCaption, CWnd* pcParent = NULL, const CMemoData* pcMemoData = NULL);
	~CResizableMemoEditDialog( void);

// �I�y���[�V����
public:
	void DelOnQuit( BOOL blDelOnQuit);
	BOOL IsDelOnQuit( void);
	BOOL IsResize( void);
	void ResizeLock( BOOL blResizeLock);
	BOOL IsMemoEmpty( void);
	void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData);
	// �g�p���H
	static BOOL IsUsed();
	static HWND GetUseWnd();

	void SetIcon( HICON hIcon);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CResizableMemoEditDialog)
	enum { IDD = IDD_RESIZABLEMEMOEDIT };
	CStatic	m_cTitleTitle;
	CEdit	m_cTitleEdit;
	CSpEdit	m_cMainEdit;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CResizableMemoEditDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// �C���v�������e�[�V����
protected:
	CString			m_cStrCaption;
	CToolBarCtrl	m_cToolBar;
	CBitmap			m_cExtIcons;
	int				m_nExtIconTop;
	CIconSelectMenu* m_pcIcon;

	HICON			m_hIcon;

	HWND			m_hWndParent;

	enum
	{
		_INSERT_TIME,
		_INSERT_DATE,
		_INSERT_DAYOFWEEK
	};
	void InsertTime( int nType);

	static TBBUTTON m_sastTButton[ EDITDLG_TOOLBAR];

	CMemoData		m_cMemoData;
	COLORREF		m_clrBack;
	COLORREF		m_clrFore;
	CBrush			m_cBrush;
	CFont			m_cFntEdit;

	BOOL			m_blResizeLock;
	BOOL			m_blResize;
	BOOL			m_blDelOnQuit;

	static CSemaphore	sm_cSmpDoEdit;
	static HWND			sm_hWndParent;

	void ByeBye( void);

	void Formating( void);

	BOOL OnTBHelpInfo(HELPINFO* pHelpInfo);
	BOOL UseTemplet( int nTempletIndex);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CResizableMemoEditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnOk();
	afx_msg void OnNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFont();
	afx_msg void OnFontColor();
	afx_msg void OnColor();
	afx_msg void OnTemplet();
	afx_msg void OnZOrder();
	afx_msg void OnDxlsel();
	afx_msg void OnDxledithelper();
	afx_msg void OnCalcresize();
	afx_msg void OnDelonquit();
	afx_msg void OnLink();
	afx_msg void OnTimer();
	afx_msg void OnPwset();
	afx_msg void OnInsertdate();
	afx_msg void OnInserttime();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnIconsel();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg LRESULT OnSetFocusDelay( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#include "ResizableMemoEditDialog.inl"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RESIZABLEMEMOEDITDIALOG_H__CA05B113_0838_11D3_92FC_004005469C82__INCLUDED_)
