#if !defined(AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_)
#define AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomExtPage.h : �w�b�_�[ �t�@�C��
//

class CProfileData;

/////////////////////////////////////////////////////////////////////////////
// CCustomExtPage �_�C�A���O

class CCustomExtPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomExtPage)

// �R���X�g���N�V����
public:
	CCustomExtPage();
	~CCustomExtPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CCustomExtPage)
	enum { IDD = IDD_CUSTOMEXT };
	BOOL	m_blFlatMenu;
	BOOL	m_blHideStart;
	BOOL	m_blLoggingDelChip;
	BOOL	m_blAutoResize;
	BOOL	m_blHideback;
	BOOL	m_blAutoTriming;
	BOOL	m_blNoNotifyVersion;
	BOOL	m_blNoCheckDelete;
	BOOL	m_blCompatibleOU;
	BOOL	m_blModifyAndSave;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CCustomExtPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CCustomExtPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CUSTOMEXTPAGE_H__B5C1B724_8118_11D2_9203_004005469C82__INCLUDED_)
