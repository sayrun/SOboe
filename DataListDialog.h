#if !defined(AFX_DATALISTDIALOG_H__471CB20B_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_DATALISTDIALOG_H__471CB20B_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DataListDialog.h : �w�b�_�[ �t�@�C��
//
#include "MemoListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDataListDialog �_�C�A���O
#include "MemoChipList.h"

class CDataListDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CDataListDialog( CMemoChipList* pcMemoChipList, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDataListDialog();

// �_�C�A���O �f�[�^
	int m_nFilterIcon;
	//{{AFX_DATA(CDataListDialog)
	enum { IDD = IDD_DATASEL };
	CButton	m_cBtnReverse;
	CButton	m_cBtnFilter;
	CButton	m_cBtnOK;
	CButton	m_cBtnDelete;
	CButton	m_cBtnItemUp;
	CButton	m_cBtnItemDown;
	CButton	m_cBtnIconFilter;
	CEdit	m_cEdtPreview;
	CMemoListBox	m_cLstMemoDatas;
	BOOL	m_blReverse;
	BOOL	m_blFilter;
	BOOL	m_blCentering;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDataListDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// �C���v�������e�[�V����
protected:
	void SetListData( int nFilterIcon, BOOL blReverse = FALSE);

	BOOL MemoItemUp( int Pos);
	BOOL MemoItemDown( int Pos);

	COLORREF	m_clrFore;
	COLORREF	m_clrBack;
	CBrush		m_cBrshBack;
	int			m_nFilter;
	CBitmap		m_cBmpIcon;

	int			m_nMemoCount;

	CIconSelectMenu* m_pcIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDataListDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDatalist();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnReverse();
	afx_msg void OnFilter();
	afx_msg void OnSelchangeIconfilter();
	virtual void OnOK();
	afx_msg void OnDelete();
	afx_msg void OnItemdown();
	afx_msg void OnItemup();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLinkexec();
	afx_msg void OnSeethrough();
	afx_msg void OnDelonquit();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnPrintchip();
	afx_msg void OnSavetext();
	afx_msg void OnIconfilter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CMemoChipList*	m_pcMemoChipList;
	CWordArray		m_cIndexArray;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DATALISTDIALOG_H__471CB20B_2D92_11D2_90A8_00804C15184E__INCLUDED_)
