// DxlEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "setreg.h"
#include "DxlEdit.h"
#include "DrawExLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDxlEdit �_�C�A���O


CDxlEdit::CDxlEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDxlEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDxlEdit)
	m_cStrMenuText = _T("");
	m_cStrDxlPath = _T("");
	//}}AFX_DATA_INIT
}


void CDxlEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxlEdit)
	DDX_Control(pDX, IDC_DRAWEXDLLMENUTEXT, m_cEdtMenuText);
	DDX_Control(pDX, IDC_DRAWEXDLLABOUT, m_cBtnDllAbt);
	DDX_Control(pDX, IDC_DRAWEXDLLCONFIG, m_cBtnDllCnfg);
	DDX_Control(pDX, IDOK, m_cBtnOK);
	DDX_Text(pDX, IDC_DRAWEXDLLMENUTEXT, m_cStrMenuText);
	DDV_MaxChars(pDX, m_cStrMenuText, 50);
	DDX_Text(pDX, IDC_DRAWEXDLLNAME, m_cStrDxlPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDxlEdit, CDialog)
	//{{AFX_MSG_MAP(CDxlEdit)
	ON_BN_CLICKED(IDC_DRAWEXDLLSEL, OnDxlsel)
	ON_BN_CLICKED(IDC_DRAWEXDLLABOUT, OnChipexdllabout)
	ON_BN_CLICKED(IDC_DRAWEXDLLCONFIG, OnChipexdllconfig)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxlEdit ���b�Z�[�W �n���h��

void CDxlEdit::OnDxlsel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData();

	CString	cStrFile;
	cStrFile = m_cStrDxlPath;
	if( GetExtendDll( cStrFile))
	{
		if( !cStrFile.IsEmpty())
		{
			CDrawExLib	cDrawExLib;
			cDrawExLib.SetPath( cStrFile);
			if( !cDrawExLib.LoadDxl( m_hWnd))
			{
				m_cStrMenuText	= cDrawExLib.GetDefaultMenuString();
				m_cStrDxlPath	= cStrFile;
				m_unDxlID		= cDrawExLib.GetDxlID();
				cDrawExLib.FreeDxl();

				m_cBtnOK.EnableWindow( TRUE);
				m_cEdtMenuText.EnableWindow( TRUE);
				m_cBtnDllAbt.EnableWindow( TRUE);
				m_cBtnDllCnfg.EnableWindow( TRUE);
			}
			else
			{
				m_cBtnOK.EnableWindow( FALSE);
				m_cEdtMenuText.EnableWindow( FALSE);
				m_cBtnDllAbt.EnableWindow( FALSE);
				m_cBtnDllCnfg.EnableWindow( FALSE);
			}
		}
		else
		{
			m_cStrMenuText.Empty();
		}

		UpdateData( FALSE);
	}
}

BOOL CDxlEdit::GetExtendDll(CString & cStrFile)
{
	DWORD dwLen = GetCurrentDirectory( 0, NULL);
	char*	pszFolder;
	dwLen++;
	pszFolder = new char[ dwLen];
	GetCurrentDirectory( dwLen, pszFolder);
	CString	cStrFolder = pszFolder;
	delete pszFolder;
	pszFolder = NULL;

	CString	cStrData;
	cStrData = cStrFile;
	CFileDialog	cFileDlg( TRUE, "dxl", cStrData, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "�`��g�����C�u�����B(*.dxl)|*.dxl||", this);

	int nResult = cFileDlg.DoModal();
	if( IDOK == nResult)
	{
		cStrFile = cFileDlg.GetPathName();
	}
	SetCurrentDirectory( cStrFolder);

	return ( IDOK == nResult);
}

BOOL CDxlEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_cBtnOK.EnableWindow( FALSE);
	m_cEdtMenuText.EnableWindow( FALSE);
	m_cBtnDllAbt.EnableWindow( FALSE);
	m_cBtnDllCnfg.EnableWindow( FALSE);
	if( !m_cStrDxlPath.IsEmpty())
	{
		CDrawExLib	cDrawExLib;
		cDrawExLib.SetPath( m_cStrDxlPath);
		if( !cDrawExLib.LoadDxl( m_hWnd))
		{
			m_unDxlID		= cDrawExLib.GetDxlID();
			cDrawExLib.FreeDxl();
			m_cBtnOK.EnableWindow( TRUE);
			m_cEdtMenuText.EnableWindow( TRUE);
			m_cBtnDllAbt.EnableWindow( TRUE);
			m_cBtnDllCnfg.EnableWindow( TRUE);
		}
	}
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CDxlEdit::OnChipexdllabout() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( !m_cStrDxlPath.IsEmpty())
	{
		CDrawExLib	cDrawExLib;
		cDrawExLib.SetPath( m_cStrDxlPath);
		if( !cDrawExLib.LoadDxl( m_hWnd))
		{
			cDrawExLib.AboutDrawExLib( m_hWnd);
			cDrawExLib.FreeDxl();
		}
	}
}

void CDxlEdit::OnChipexdllconfig() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( !m_cStrDxlPath.IsEmpty())
	{
		CDrawExLib	cDrawExLib;
		cDrawExLib.SetPath( m_cStrDxlPath);
		if( !cDrawExLib.LoadDxl( m_hWnd))
		{
			cDrawExLib.Configure( m_hWnd);
			cDrawExLib.FreeDxl();
		}
	}
}

void CDxlEdit::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData();
	if( m_cStrMenuText.IsEmpty())
	{
		MessageBox( "���j���[�̕����񂪐ݒ肳��Ă��܂���", "Dxl", MB_OK);
		return;
	}
	
	CDialog::OnOK();
}

BOOL CDxlEdit::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_DRAWEXDLLMENUTEXT:
	case IDC_DRAWEXDLLNAME:
	case IDC_DRAWEXDLLCONFIG:
	case IDC_DRAWEXDLLABOUT:
	case IDC_DRAWEXDLLSEL:
		::WinHelp( m_hWnd, "SetReg.hlp", HELP_CONTEXTPOPUP, pHelpInfo->iCtrlId | 0x60000);
//		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return FALSE;
	default:
		return CDialog::OnHelpInfo(pHelpInfo);
	}
}
