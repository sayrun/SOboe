// ResultCheckDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "ResultCheckDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultCheckDialog �_�C�A���O


CResultCheckDialog::CResultCheckDialog(CWnd* pParent /*= NULL*/)
	: CDialog(CResultCheckDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultCheckDialog)
	m_cStrResult3 = _T("");
	m_nResultSel = -1;
	//}}AFX_DATA_INIT
}

void CResultCheckDialog::SetResults( const CString& cStrResult1, const CString& cStrResult2, BOOL blResult3, const CString& cStrResult3)
{
	if( !cStrResult1.IsEmpty())
	{
		m_cStrResult1 = cStrResult1;
	}
	else
	{
		m_cStrResult1 = _T( "Yes");
	}

	if( !cStrResult2.IsEmpty())
	{
		m_cStrResult2 = cStrResult2;
	}
	else
	{
		m_cStrResult2 = _T( "No");
	}

	m_blResult3 = blResult3;

	if( !cStrResult3.IsEmpty())
	{
		m_cStrResult3 = cStrResult3;
	}
}

BOOL CResultCheckDialog::GetResult( CString& cStrResult)
{
	switch( m_nResultSel)
	{
	case 0:
		cStrResult = m_cStrResult1;
		break;
	case 1:
		cStrResult = m_cStrResult2;
		break;
	case 2:
		if( !m_cStrResult3.IsEmpty())
		{
			cStrResult = m_cStrResult3;
			break;
		}
	default:
		cStrResult = _T( "Reject");
		return FALSE;
	}
	return TRUE;
}

void CResultCheckDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultCheckDialog)
	DDX_Control(pDX, IDOK, m_cBtnOK);
	DDX_Control(pDX, IDC_RESULT3EDIT, m_cEdtResult3);
	DDX_Control(pDX, IDC_RESULT1, m_cBtnResult1);
	DDX_Control(pDX, IDC_RESULT2, m_cBtnResult2);
	DDX_Control(pDX, IDC_RESULT3, m_cBtnResult3);
	DDX_Text(pDX, IDC_RESULT3EDIT, m_cStrResult3);
	DDV_MaxChars(pDX, m_cStrResult3, 20);
	DDX_Radio(pDX, IDC_RESULT1, m_nResultSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultCheckDialog, CDialog)
	//{{AFX_MSG_MAP(CResultCheckDialog)
	ON_BN_CLICKED(IDC_RESULT1, OnResultChange)
	ON_BN_CLICKED(IDC_RESULT2, OnResultChange)
	ON_BN_CLICKED(IDC_RESULT3, OnResultChange)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultCheckDialog ���b�Z�[�W �n���h��

BOOL CResultCheckDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_cBtnResult1.SetWindowText( m_cStrResult1);
	m_cBtnResult2.SetWindowText( m_cStrResult2);

	m_cBtnResult3.EnableWindow( m_blResult3);
	m_cEdtResult3.EnableWindow( m_blResult3);

	m_cBtnOK.EnableWindow( FALSE);
	m_cEdtResult3.EnableWindow( FALSE);

	CWnd*	pcWnd = GetParent();
	if( pcWnd)
	{
		CRect	cRectParent;
		pcWnd->GetWindowRect( cRectParent);
		CRect	cRect;
		GetWindowRect( cRect);

		int nHeight = cRect.Height();
		cRect.top		= cRectParent.bottom;
		cRect.bottom	= cRect.top + nHeight;
		if( ( cRect.bottom <= GetSystemMetrics( SM_CYFULLSCREEN)) && ( cRect.right <= GetSystemMetrics( SM_CXFULLSCREEN)))
		{
			MoveWindow( cRect);
		}
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CResultCheckDialog::OnResultChange() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		m_cBtnOK.EnableWindow( ( 0 <= m_nResultSel && 2 >= m_nResultSel));
		m_cEdtResult3.EnableWindow( 2 == m_nResultSel);
	}
}

void CResultCheckDialog::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		if( 2 == m_nResultSel)
		{
			if( m_cStrResult3.IsEmpty())
			{
				MessageBox( "�𓚂������͂ł�", NULL, MB_OK | MB_ICONINFORMATION);
				return;
			}
			else
			{
				CString	cStr;
				cStr.Format( "�𓚂�\n�u%s�v\n�𑗂�܂����H", m_cStrResult3);
				if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2))return;
			}
		}
		CDialog::OnOK();
	}
	else
	{
		m_cBtnOK.EnableWindow( FALSE);
	}
}

void CResultCheckDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	return;
}

BOOL CResultCheckDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_RESULT1:
	case IDC_RESULT2:
	case IDC_RESULT3:
	case IDC_RESULT3EDIT:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}
