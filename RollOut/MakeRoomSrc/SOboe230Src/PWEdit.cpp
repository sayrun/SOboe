// PWEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "PWEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPWEdit �_�C�A���O


CPWEdit::CPWEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CPWEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPWEdit)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CPWEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPWEdit)
	DDX_Control(pDX, IDC_PASSWORD1ST, m_cEdtPassWord1st);
	DDX_Control(pDX, IDC_PASSWORD2ND, m_cEdtPassWord2nd);
	DDX_Check(pDX, IDC_PASSWORDENABLE, m_blPassWordEnable);
	DDX_Text(pDX, IDC_PASSWORD1ST, m_cStrPassWord1st);
	DDX_Text(pDX, IDC_PASSWORD2ND, m_cStrPassWord2nd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPWEdit, CDialog)
	//{{AFX_MSG_MAP(CPWEdit)
	ON_BN_CLICKED(IDC_PASSWORDENABLE, OnPasswordenable)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPWEdit ���b�Z�[�W �n���h��

BOOL CPWEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
	m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CPWEdit::OnPasswordenable() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		m_cEdtPassWord1st.EnableWindow( m_blPassWordEnable);
		m_cEdtPassWord2nd.EnableWindow( m_blPassWordEnable);
	}
}

void CPWEdit::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		if( m_blPassWordEnable)
		{
			if( _MINLEN_PW > m_cStrPassWord1st.GetLength() || m_cStrPassWord1st != m_cStrPassWord2nd)
			{
				MessageBox( "�ݒ肳�ꂽ�p�X���[�h���ُ�ł�", NULL, MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	
	CDialog::OnOK();
}

BOOL CPWEdit::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_PASSWORDENABLE:
	case IDC_PASSWORD1ST:
	case IDC_PASSWORD2ND:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}
