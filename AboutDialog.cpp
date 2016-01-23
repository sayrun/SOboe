// AboutDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog �_�C�A���O


CAboutDialog::CAboutDialog( int nWinType, CWnd* pParent)
	: CDialog( ( CAboutDialog::WINTYPE_98 == nWinType) ? CAboutDialog::IDD98 : CAboutDialog::IDDNT, pParent)
{
	m_lpGetFreeSystemResource = NULL;
	m_nWinType = nWinType;
	if( WINTYPE_98 == m_nWinType)
	{
	//{{AFX_DATA_INIT(CAboutDialog)
	m_cStrGDI = _T("");
	m_cStrSYSTEM = _T("");
	m_cStrUSER = _T("");
	//}}AFX_DATA_INIT
	}
}


void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAboutDialog)
	DDX_Control(pDX, IDC_PHYSICAL, m_cPgbPhysical);		
	DDX_Text(pDX, IDC_PHYSICALPER, m_cStrPhysical);
	if( WINTYPE_98 == m_nWinType)
	{
		DDX_Control(pDX, IDC_USER, m_cPgbUSER);
		DDX_Control(pDX, IDC_SYSTEM, m_cPgbSYSTEM);
		DDX_Control(pDX, IDC_GDI, m_cPgbGDI);
		DDX_Text(pDX, IDC_GDIPER, m_cStrGDI);
		DDX_Text(pDX, IDC_SYSTEMPER, m_cStrSYSTEM);
		DDX_Text(pDX, IDC_USERPER, m_cStrUSER);
	}
	else
	{
		DDX_Control(pDX, IDC_PAGEFILE, m_cPgbPagefile);		
		DDX_Text(pDX, IDC_PAGEFILEPER, m_cStrPagefile);
	}
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	ON_BN_CLICKED(IDC_GOSUPORT, OnGosuport)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog ���b�Z�[�W �n���h��

void CAboutDialog::OnGosuport() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	ShellExecute( GetSafeHwnd(), "open", "http://www2.famille.ne.jp/~chiba/software/", NULL, NULL, SW_NORMAL);
	EndDialog( IDOK);
}

BOOL CAboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_cPgbPhysical.SetRange( 0, 100);

	if( WINTYPE_98 == m_nWinType)
	{
		m_cPgbUSER.SetRange( 0, 100);
		m_cPgbGDI.SetRange( 0, 100);
		m_cPgbSYSTEM.SetRange( 0, 100);

		// ���\�[�X�`�F�b�N���܂�DLL�𖾎��I�Ƀ����N���܂��B
		m_hInst = LoadLibrary( "RSRC32.DLL");
		// �����N������ɂȂ��ꂽ�H
		if( NULL != m_hInst)
		{
			// ���\�[�X�`�F�b�N��DLL����A�ړI�̊֐��|�C���^���擾���܂��B
			m_lpGetFreeSystemResource = ( GetFreeSystemResource)GetProcAddress( m_hInst, "_MyGetFreeSystemResources32@4");
		}
	}
	else
	{
		m_cPgbPagefile.SetRange( 0, 100);
	}

	CheckResources();
	SetTimer( 4096, 500, NULL);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

BOOL CAboutDialog::CheckResources( void)
{
	MEMORYSTATUS	stMemStat;
	stMemStat.dwLength = sizeof( MEMORYSTATUS);
	GlobalMemoryStatus( &stMemStat);
	UINT	unTotalPhys = stMemStat.dwTotalPhys;
	UINT	unAvailPhys = stMemStat.dwAvailPhys;
	unAvailPhys /= ( 1024 * 1024);
	unTotalPhys /= ( 1024 * 1024);
	UINT unData = ( ( unAvailPhys) * 100) / unTotalPhys;
	m_cPgbPhysical.SetPos( unData);

	if( WINTYPE_98 == m_nWinType)
	{
		m_cStrPhysical.Format( IDS_RESOURCEINFO, unData);
		// �擾�����H
		if( NULL != m_lpGetFreeSystemResource)
		{
			// �V�X�e���E���\�[�X�́y�c�聓�z�擾
			UINT unSystem = m_lpGetFreeSystemResource( 0);
			// �f�c�h�E���\�[�X�́y�c�聓�z�擾
			UINT unGDI = m_lpGetFreeSystemResource( 1);
			// �t�r�d�q�E���\�[�X�́y�c�聓�z�擾
			UINT unUSER = m_lpGetFreeSystemResource( 2);

			m_cPgbSYSTEM.SetPos( ( int)unSystem);
			m_cPgbGDI.SetPos( ( int)unGDI);
			m_cPgbUSER.SetPos( ( int)unUSER);

			m_cStrSYSTEM.Format( IDS_RESOURCEINFO, unSystem);
			m_cStrGDI.Format( IDS_RESOURCEINFO, unGDI);
			m_cStrUSER.Format( IDS_RESOURCEINFO, unUSER);
		}
	}
	else
	{
		m_cStrPhysical.Format( IDS_MEMORYINFO, ( unAvailPhys), unTotalPhys);

		UINT unTotalPageFile = stMemStat.dwTotalPageFile;
		UINT unAvailPageFile = stMemStat.dwAvailPageFile;
		unAvailPageFile /= ( 1024 * 1024);
		unTotalPageFile /= ( 1024 * 1024);
		m_cStrPagefile.Format( IDS_MEMORYINFO, ( unAvailPageFile), unTotalPageFile);
		unData = ( ( unAvailPageFile) * 100) / unTotalPageFile;
		m_cPgbPagefile.SetPos( unData);
	}
	UpdateData( FALSE);
	return TRUE;
}

void CAboutDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CheckResources();
	
	CDialog::OnTimer(nIDEvent);
}

void CAboutDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	KillTimer( 4096);

	if( WINTYPE_98 == m_nWinType)
	{
		if( m_hInst)
		{
			FreeLibrary( m_hInst);
			m_hInst = NULL;
		}
	}
}
