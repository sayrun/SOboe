#if !defined(AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_)
#define AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ChipExDllSettingDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CChipExDllSettingDialog �_�C�A���O

class CChipExDllSettingDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CChipExDllSettingDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
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


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CChipExDllSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	BOOL GetExtendDll( CString& cStrFile);

	typedef struct tagSPECIFICATION
	{
		UINT	unLowerStructVersion;	// �����\�ȍŌÃo�[�W����
		UINT	unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
		char	szMenuString[ 50];		// �f�t�H���g�̃��j���[����
	}SPECIFICATION;
	typedef	BOOL ( *CHIPEXSPECIFICATION)( SPECIFICATION* pSpecification);
	typedef	BOOL ( *CHIPEXCONFIGURE)( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
	typedef	BOOL ( *CHIPEXABOUT)( UINT unStructVersion, HWND hWnd, UINT unMenuNum);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CHIPEXDLLSETTINGDIALOG_H__3FB0220B_620B_11D1_8FDC_00804C15184E__INCLUDED_)
