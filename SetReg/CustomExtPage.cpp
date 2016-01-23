// CustomExtPage.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "CustomExtPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomExtPage �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CCustomExtPage, CPropertyPage)

CCustomExtPage::CCustomExtPage() : CPropertyPage(CCustomExtPage::IDD)
{
	//{{AFX_DATA_INIT(CCustomExtPage)
	m_blFlatMenu = FALSE;
	m_blHideStart = FALSE;
	m_blLoggingDelChip = FALSE;
	m_blAutoResize = FALSE;
	m_blHideback = FALSE;
	m_blAutoTriming = FALSE;
	m_blNoNotifyVersion = FALSE;
	m_blNoCheckDelete = FALSE;
	m_blCompatibleOU = FALSE;
	m_blModifyAndSave = FALSE;
	//}}AFX_DATA_INIT
}

CCustomExtPage::~CCustomExtPage()
{
}

void CCustomExtPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomExtPage)
	DDX_Check(pDX, IDC_MENUFLAT, m_blFlatMenu);
	DDX_Check(pDX, IDC_HIDESTART, m_blHideStart);
	DDX_Check(pDX, IDC_DELLOG, m_blLoggingDelChip);
	DDX_Check(pDX, IDC_CALCDEFAULT, m_blAutoResize);
	DDX_Check(pDX, IDC_HIDEBACK, m_blHideback);
	DDX_Check(pDX, IDC_AUTOTRIMMING, m_blAutoTriming);
	DDX_Check(pDX, IDC_VERUNNOTIFY, m_blNoNotifyVersion);
	DDX_Check(pDX, IDC_NOCHECKDELETE, m_blNoCheckDelete);
	DDX_Check(pDX, IDC_COMPATIBLEOU, m_blCompatibleOU);
	DDX_Check(pDX, IDC_MFSAVE, m_blModifyAndSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomExtPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCustomExtPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomExtPage ���b�Z�[�W �n���h��

BOOL CCustomExtPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_blFlatMenu = m_pcProfileDatas->m_blFlatMenu;
	m_blHideStart = m_pcProfileDatas->m_blHideStart;
	m_blLoggingDelChip = m_pcProfileDatas->m_blLoggingDelChip;
	m_blAutoResize = m_pcProfileDatas->m_blAutoResize;
	m_blHideback = m_pcProfileDatas->m_blHideback;
	m_blAutoTriming = m_pcProfileDatas->m_blAutoTriming;
	m_blNoNotifyVersion = !m_pcProfileDatas->m_blNotifyVersion;
	m_blNoCheckDelete = m_pcProfileDatas->m_blNoCheckDelete;
	m_blCompatibleOU = m_pcProfileDatas->m_blCompatibleOU;
	m_blModifyAndSave = m_pcProfileDatas->m_blModifyAndSave;

	UpdateData( FALSE);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CCustomExtPage::OnOK() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_pcProfileDatas->m_blFlatMenu = m_blFlatMenu;
	m_pcProfileDatas->m_blHideStart = m_blHideStart;
	m_pcProfileDatas->m_blLoggingDelChip = m_blLoggingDelChip;
	m_pcProfileDatas->m_blAutoResize = m_blAutoResize;
	m_pcProfileDatas->m_blHideback = m_blHideback;
	m_pcProfileDatas->m_blAutoTriming = m_blAutoTriming;
	m_pcProfileDatas->m_blNotifyVersion = !m_blNoNotifyVersion;
	m_pcProfileDatas->m_blNoCheckDelete = m_blNoCheckDelete;
	m_pcProfileDatas->m_blCompatibleOU = m_blCompatibleOU;
	m_pcProfileDatas->m_blModifyAndSave = m_blModifyAndSave;
	
	CPropertyPage::OnOK();
}
