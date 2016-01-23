#if !defined(AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_)
#define AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InterfacePage.h : �w�b�_�[ �t�@�C��
//

class CProfileData;
/////////////////////////////////////////////////////////////////////////////
// CInterfacePage �_�C�A���O

class CInterfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInterfacePage)

// �R���X�g���N�V����
public:
	CInterfacePage();
	~CInterfacePage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CInterfacePage)
	enum { IDD = IDD_INTERFACE };
	CHotKeyCtrl	m_cHotKeyCtrl;
	CButton	m_cBtnCEdit;
	CButton	m_cBtnCExec;
	CButton	m_cBtnCSmall;
	CButton	m_cBtnCTitle;
	CButton	m_cBtnCSend;
	int		m_nLDblClkFunc;
	int		m_nMDblClkFunc;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CInterfacePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	BOOL Is3Button( void);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CInterfacePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_INTERFACEPAGE_H__029A50D3_80FE_11D2_9202_004005469C82__INCLUDED_)
