#if !defined(AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_)
#define AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SecurityPage.h : �w�b�_�[ �t�@�C��
//

#define	MIN_PASSWORD	4

class CProfileData;

/////////////////////////////////////////////////////////////////////////////
// CSecurityPage �_�C�A���O

class CSecurityPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSecurityPage)

// �R���X�g���N�V����
public:
	CSecurityPage();
	~CSecurityPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSecurityPage)
	enum { IDD = IDD_SECURITY };
	CString	m_cStrOldPW;
	CString	m_cStrNewPW;
	CString	m_cStrCheckPW;
	BOOL	m_blReceptionTimer;
	BOOL	m_blReceptionLink;
	BOOL	m_blReceptionDxl;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CSecurityPage)
	public:
	virtual void OnOK();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSecurityPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SECURITYPAGE_H__904DCC92_827F_11D2_9205_004005469C82__INCLUDED_)
