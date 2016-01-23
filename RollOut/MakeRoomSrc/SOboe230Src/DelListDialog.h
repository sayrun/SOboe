#if !defined(AFX_DELLISTDIALOG_H__D4B3B905_4392_11D2_90BD_00804C15184E__INCLUDED_)
#define AFX_DELLISTDIALOG_H__D4B3B905_4392_11D2_90BD_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DelListDialog.h : �w�b�_�[ �t�@�C��
//

#include "MemoChipList.h"
#include "MemoListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CDelListDialog �_�C�A���O

class CDelListDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CDelListDialog( CString cStrFilePath, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDelListDialog();

	static BOOL IsUsed();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDelListDialog)
	enum { IDD = IDD_DELLIST };
	CButton	m_cBtnDelete;
	CButton	m_cBtnRestore;
	CButton	m_cBtnReverse;
	CButton	m_cBtnSaveText;
	CButton	m_cBtnItemUp;
	CButton	m_cBtnItemDown;
	CEdit	m_cEdtPreview;
	CMemoListBox	m_cLstMemoDatas;
	BOOL	m_blReverse;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDelListDialog)
	public:
	virtual int DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void LoadDelData( void);
	void SaveDelData( void);

	void RemoveList( void);
	void SetListData( BOOL blReverse);

	BOOL MemoItemUp( int nPos);
	BOOL MemoItemDown( int nPos);

	BOOL Restore( int nIndex);
	BOOL Delete( int nIndex, BOOL blNoneCheck = FALSE);

	// �p�X���[�h�̃`�F�b�N
	BOOL CheckPassWord( const CMemoChip* pcMemoChip);

protected:
	CString	m_cStrDustFilePath;
	static CSemaphore	sm_cSmpDoDisp;
	CMemoChipList	m_cLstDelMemoChip;

	COLORREF	m_clrFore;
	COLORREF	m_clrBack;
	CBrush		m_cBrshBack;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDelListDialog)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeDellist();
	afx_msg void OnReverse();
	afx_msg void OnItemup();
	afx_msg void OnItemdown();
	afx_msg void OnSavetext();
	afx_msg void OnReload();
	afx_msg void OnRestore();
	afx_msg void OnCopyRestore();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDelete();
	afx_msg void OnLinkexec();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnPrintchip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DELLISTDIALOG_H__D4B3B905_4392_11D2_90BD_00804C15184E__INCLUDED_)
