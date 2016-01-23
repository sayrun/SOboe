#if !defined(AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_)
#define AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempletPage.h : �w�b�_�[ �t�@�C��
//
#include "MemoListBox.h"

class CProfileData;
/////////////////////////////////////////////////////////////////////////////
// CTempletPage �_�C�A���O

class CTempletPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTempletPage)

// �R���X�g���N�V����
public:
	CTempletPage();
	~CTempletPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTempletPage)
	enum { IDD = IDD_TEMPLET };
	CButton	m_cBtnDownItem;
	CButton	m_cBtnUpItem;
	CButton	m_cBtnTemletDel;
	CMemoListBox	m_cLstTemplet;
	BOOL	m_blEnableTemplet;
	int		m_nLstTemplet;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CTempletPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	CString	m_cStrTempletFile;

	void LoadTempletData( void);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTempletPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnabletemplet();
	afx_msg void OnTempletdel();
	afx_msg void OnSelchangeTempletlist();
	afx_msg void OnDownitem();
	afx_msg void OnUpitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL			m_blSpecSet;
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TEMPLETPAGE_H__56996021_BA9E_11D2_9269_004005469C82__INCLUDED_)
