// NetworkExtPage.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "NetworkExtPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkExtPage �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CNetworkExtPage, CPropertyPage)

CNetworkExtPage::CNetworkExtPage() : CPropertyPage(CNetworkExtPage::IDD)
{
	//{{AFX_DATA_INIT(CNetworkExtPage)
	m_blPackSend = FALSE;
	m_blNotifyOpen = FALSE;
	m_blHideReceive = FALSE;
	m_blHideSubNotify = FALSE;
	m_blLoggingSendChip = FALSE;
	m_blFullLoggingSendChip = FALSE;
	m_blLoggingReceiveChip = FALSE;
	m_blFullLoggingReceiveChip = FALSE;
	m_blCheckDelOnQuit = FALSE;
	m_blReceiveAndSave = FALSE;
	m_blDeleteAfterSend = FALSE;
	m_blDelOnQuit2Rcv = FALSE;
	//}}AFX_DATA_INIT
}

CNetworkExtPage::~CNetworkExtPage()
{
}

void CNetworkExtPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkExtPage)
	DDX_Control(pDX, IDC_SENDOPENDEFAULT, m_cBtnNotifyOpen);
	DDX_Control(pDX, IDC_RECEIVELOGFULL, m_cBtnFullLoggingReceiveChip);
	DDX_Control(pDX, IDC_SENDLOGFULL, m_cBtnFullLoggingSendChip);
	DDX_Check(pDX, IDC_SENDPACKDEFAULT, m_blPackSend);
	DDX_Check(pDX, IDC_SENDOPENDEFAULT, m_blNotifyOpen);
	DDX_Check(pDX, IDC_RECEIVECHIPHIDE, m_blHideReceive);
	DDX_Check(pDX, IDC_INFOHIDE, m_blHideSubNotify);
	DDX_Check(pDX, IDC_SENDLOG, m_blLoggingSendChip);
	DDX_Check(pDX, IDC_SENDLOGFULL, m_blFullLoggingSendChip);
	DDX_Check(pDX, IDC_RECEIVELOG, m_blLoggingReceiveChip);
	DDX_Check(pDX, IDC_RECEIVELOGFULL, m_blFullLoggingReceiveChip);
	DDX_Check(pDX, IDC_DELONQUITDEFAULT, m_blCheckDelOnQuit);
	DDX_Check(pDX, IDC_RCSAVE, m_blReceiveAndSave);
	DDX_Check(pDX, IDC_DELETEAFTERSEND, m_blDeleteAfterSend);
	DDX_Check(pDX, IDC_DELONQUIT2RCV, m_blDelOnQuit2Rcv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkExtPage, CPropertyPage)
	//{{AFX_MSG_MAP(CNetworkExtPage)
	ON_BN_CLICKED(IDC_SENDLOG, OnSendlog)
	ON_BN_CLICKED(IDC_RECEIVELOG, OnReceivelog)
	ON_BN_CLICKED(IDC_SENDPACKDEFAULT, OnSendpackdefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkExtPage ���b�Z�[�W �n���h��

BOOL CNetworkExtPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_blPackSend = m_pcProfileDatas->m_blPackSend;
	m_blNotifyOpen = m_pcProfileDatas->m_blNotifyOpen;
	m_blHideReceive = m_pcProfileDatas->m_blHideReceive;
	m_blHideSubNotify = !m_pcProfileDatas->m_blSubNotify;
	m_blLoggingSendChip = m_pcProfileDatas->m_blLoggingSendChip;
	m_blFullLoggingSendChip = m_pcProfileDatas->m_blFullLoggingSendChip;
	m_blLoggingReceiveChip = m_pcProfileDatas->m_blLoggingReceiveChip;
	m_blFullLoggingReceiveChip = m_pcProfileDatas->m_blFullLoggingReceiveChip;
	m_blCheckDelOnQuit = m_pcProfileDatas->m_blCheckDelOnQuit;
	m_blReceiveAndSave = m_pcProfileDatas->m_blReceiveAndSave;
	m_blDeleteAfterSend = m_pcProfileDatas->m_blDeleteAfterSend;
	m_blDelOnQuit2Rcv = m_pcProfileDatas->m_blDelOnQuit2Rcv;

	m_cBtnFullLoggingSendChip.EnableWindow( m_blLoggingSendChip);
	m_cBtnFullLoggingReceiveChip.EnableWindow( m_blLoggingReceiveChip);
	m_cBtnNotifyOpen.EnableWindow( m_blPackSend);

	UpdateData( FALSE);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CNetworkExtPage::OnOK() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_pcProfileDatas->m_blPackSend = m_blPackSend;
	m_pcProfileDatas->m_blNotifyOpen = m_blNotifyOpen;
	m_pcProfileDatas->m_blHideReceive = m_blHideReceive;
	m_pcProfileDatas->m_blSubNotify = !m_blHideSubNotify;
	m_pcProfileDatas->m_blLoggingSendChip = m_blLoggingSendChip;
	m_pcProfileDatas->m_blFullLoggingSendChip = m_blFullLoggingSendChip;
	m_pcProfileDatas->m_blLoggingReceiveChip = m_blLoggingReceiveChip;
	m_pcProfileDatas->m_blFullLoggingReceiveChip = m_blFullLoggingReceiveChip;
	m_pcProfileDatas->m_blCheckDelOnQuit = m_blCheckDelOnQuit;
	m_pcProfileDatas->m_blReceiveAndSave = m_blReceiveAndSave;
	m_pcProfileDatas->m_blDeleteAfterSend = m_blDeleteAfterSend;
	m_pcProfileDatas->m_blDelOnQuit2Rcv = m_blDelOnQuit2Rcv;
	
	CPropertyPage::OnOK();
}

void CNetworkExtPage::OnSendlog() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData();
	m_cBtnFullLoggingSendChip.EnableWindow( m_blLoggingSendChip);
}

void CNetworkExtPage::OnReceivelog() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData();
	m_cBtnFullLoggingReceiveChip.EnableWindow( m_blLoggingReceiveChip);
}

void CNetworkExtPage::OnSendpackdefault() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData();
	m_cBtnNotifyOpen.EnableWindow( m_blPackSend);
}
