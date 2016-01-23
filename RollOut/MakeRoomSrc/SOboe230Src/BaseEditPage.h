#if !defined(AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BaseEditPage.h : �w�b�_�[ �t�@�C��
//

#include "SpEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseEditPage �_�C�A���O

class CBaseEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBaseEditPage)

// �R���X�g���N�V����
public:
	CBaseEditPage();
	~CBaseEditPage();

// �_�C�A���O �f�[�^
	BOOL		m_blForeColor;	// �����F�̗L����
	COLORREF	m_clrFore;
	BOOL		m_blBackColor;	// �w�i�F�̗L����
	COLORREF	m_clrBack;
	BOOL		m_blLogFont;	// �t�H���g�̗L����
	LOGFONT		m_stLogFont;
	UINT		m_unDxlID;			// �`��g��ID
	UINT		m_unDxlExtDataSize;	// DXL�Ɋւ������ۑ��f�[�^�T�C�Y
	BYTE*		m_pabyDxlExtData;	// DXL�Ɋւ������ۑ��f�[�^
	BOOL		m_blResizeLock;
	CString		m_cStrMemo4IF;
	int			m_nIconIndex;
	//{{AFX_DATA(CBaseEditPage)
	enum { IDD = IDD_BASEPAGE };
	CButton	m_cBtnIconSel;
	CButton	m_cBtnResize;
	CButton	m_cBtnEditHelper;
	CSpEdit	m_cEdtMemo;
	CString	m_cStrMemo;
	CString	m_cStrTitle;
	BOOL	m_blResize;
	BOOL	m_blDelOnQuit;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CBaseEditPage)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CBaseEditPage)
	afx_msg void OnFont();
	afx_msg void OnColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDxledithelper();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnIconsel();
	//}}AFX_MSG
	afx_msg void OnTemplteMenu( UINT nID);
	afx_msg LRESULT OnSetFocusDelay( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// ��Ɨp�f�[�^
protected:
	UINT	m_unOldDxlID;
	CBrush	m_cBrhEdit;
	CFont	m_cFntEdit;
	CBitmap m_cBmpIcon;
	CIconSelectMenu* m_pcIcon;
protected:
	BOOL DxlEditSupport( const CDrawExLib* pcDrawExLib);
	enum
	{
		_INSERT_TIME,
		_INSERT_DATE,
		_INSERT_DAYOFWEEK
	};
	void InsertTime( int nType);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_)
