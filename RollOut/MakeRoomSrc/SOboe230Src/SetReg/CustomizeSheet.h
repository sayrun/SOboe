// CustomizeSheet.h : �w�b�_�[ �t�@�C��
//
// ���̃N���X�̓J�X�^�� ���[�_�� �v���p�e�B �V�[�g���`���܂��B 
// CCustomizeSheet.
 
#ifndef __CUSTOMIZESHEET_H__
#define __CUSTOMIZESHEET_H__

#include "InterfacePage.h"
#include "NetworkPage.h"
#include "DesignPage.h"
#include "CustomExtPage.h"
#include "NetworkExtPage.h"
#include "ExtDllPage.h"
#include "SecurityPage.h"
#include "TempletPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

class CProfileData;

class CCustomizeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCustomizeSheet)

// �\�z
public:
	CCustomizeSheet( BOOL blSpec, CWnd* pWndParent = NULL);

// �A�g���r���[�g
public:
	CDesignPage		m_cDesign;
	CNetworkPage	m_cNetWork;
	CInterfacePage	m_cInterface;
	CCustomExtPage	m_cCustomExt;
	CNetworkExtPage	m_cNetworkExt;
	CExtDllPage		m_cExtDll;
	CSecurityPage	m_cSecurity;
	CTempletPage	m_cTemplet;

// �I�y���[�V����
public:
	void SetProfileData( CProfileData* pcProfileData);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CCustomizeSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustomizeSheet();

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CCustomizeSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CUSTOMIZESHEET_H__
