// TimerEditPage.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "TimerEditPage.h"
#include "MemoData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimerEditPage �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CTimerEditPage, CPropertyPage)

CTimerEditPage::CTimerEditPage() : CPropertyPage(CTimerEditPage::IDD)
{
	//{{AFX_DATA_INIT(CTimerEditPage)
	m_nMin = 0;
	m_nHour = 0;
	m_nMonth = 0;
	m_nDay = 0;
	m_blTimerEnable = FALSE;
	m_nTimerType = 0;
	m_nNotifyShift = 0;
	m_blAfterNotify = FALSE;
	m_nBeforeNotify = 0;
	m_cStrNotifySound = _T("");
	//}}AFX_DATA_INIT
	m_nWeek = 0;
}

CTimerEditPage::~CTimerEditPage()
{
}

void CTimerEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerEditPage)
	DDX_Control(pDX, IDC_WAVEPLAY, m_cBtnNotifySoundPlay);
	DDX_Control(pDX, IDC_WAVSEL, m_cBtnNotifySoundSel);
	DDX_Control(pDX, IDC_WAVEFILE, m_cEdtNotifySound);
	DDX_Control(pDX, IDC_AFTERNOTIFY, m_cBtnAfterNotify);
	DDX_Control(pDX, IDC_BEFORSPIN, m_cSpnBeforeNotify);
	DDX_Control(pDX, IDC_BEFORENOTIFY, m_cEdtBeforeNotify);
	DDX_Control(pDX, IDC_SPNMONTH, m_cSpnMonth);
	DDX_Control(pDX, IDC_SPNMIN, m_cSpnMin);
	DDX_Control(pDX, IDC_SPNHOUR, m_cSpnHour);
	DDX_Control(pDX, IDC_SPNDAY, m_cSpnDay);
	DDX_Control(pDX, IDC_NOTIFYSHIFT, m_cCmbNotifyShift);
	DDX_Control(pDX, IDC_TIMERTYPE, m_cCmbTimerType);
	DDX_Control(pDX, IDC_DAY, m_cEdtDay);
	DDX_Control(pDX, IDC_MONTH, m_cEdtMonth);
	DDX_Control(pDX, IDC_MIN, m_cEdtMin);
	DDX_Control(pDX, IDC_HOUR, m_cEdtHour);
	DDX_Control(pDX, IDC_WEEKLIST, m_cLstDayOfWeek);
	DDX_Text(pDX, IDC_MIN, m_nMin);
	DDX_Text(pDX, IDC_HOUR, m_nHour);
	DDX_Text(pDX, IDC_MONTH, m_nMonth);
	DDX_Text(pDX, IDC_DAY, m_nDay);
	DDX_Check(pDX, IDC_TIMERENABLE, m_blTimerEnable);
	DDX_CBIndex(pDX, IDC_TIMERTYPE, m_nTimerType);
	DDX_CBIndex(pDX, IDC_NOTIFYSHIFT, m_nNotifyShift);
	DDX_Check(pDX, IDC_AFTERNOTIFY, m_blAfterNotify);
	DDX_Text(pDX, IDC_BEFORENOTIFY, m_nBeforeNotify);
	DDX_Text(pDX, IDC_WAVEFILE, m_cStrNotifySound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimerEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTimerEditPage)
	ON_BN_CLICKED(IDC_TIMERENABLE, OnTimerenable)
	ON_CBN_SELCHANGE(IDC_TIMERTYPE, OnSelchangeTimertype)
	ON_BN_CLICKED(IDC_WAVSEL, OnWavsel)
	ON_BN_CLICKED(IDC_WAVEPLAY, OnWaveplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimerEditPage ���b�Z�[�W �n���h��

BOOL CTimerEditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString	cWeek;
	cWeek.LoadString( IDS_WEEKDATA);
	CString	cStrWork;
	int nPos;
	for( int nIndex = 0; nIndex < 7; nIndex++)
	{
		nPos = cWeek.Find( '|');
		if( 0 < nPos)
		{
			cStrWork = cWeek.Left( nPos);
			cWeek = cWeek.Mid( nPos + 1);
		}
		m_cLstDayOfWeek.AddString( cStrWork);
	}
	for( int nIndex = 0; nIndex < 7; nIndex++)
	{
		m_cLstDayOfWeek.SetSel( nIndex, ( 0 != ( m_nWeek & ( 0x0001 << nIndex))));
	}

	// �R���g���[���͈̔�
	m_cSpnMonth.SetRange( 0, 12);
	m_cSpnDay.SetRange( 1, 31);
	m_cSpnHour.SetRange( 0, 23);
	m_cSpnMin.SetRange( 0, 59);
	m_cSpnBeforeNotify.SetRange( 0, 10);

	// IME�̋֎~
	::ImmAssociateContext( m_cEdtMonth.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtDay.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtHour.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtMin.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtBeforeNotify.GetSafeHwnd(), NULL);

	ControlEnable();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CTimerEditPage::ControlEnable( void)
{
	// �󋵂Ŋe�R���g���[���̗L��������ݒ肷��
	m_cEdtMonth.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_DAY == m_nTimerType));
	m_cEdtDay.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_DAY == m_nTimerType));
	m_cCmbNotifyShift.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_DAY == m_nTimerType));
	m_cEdtBeforeNotify.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_DAY == m_nTimerType));
	m_cBtnAfterNotify.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_DAY == m_nTimerType || CMemoData::TIMERTYPE_WEEK == m_nTimerType));
	m_cLstDayOfWeek.EnableWindow( m_blTimerEnable && ( CMemoData::TIMERTYPE_WEEK == m_nTimerType));
	m_cEdtHour.EnableWindow( m_blTimerEnable);
	m_cEdtMin.EnableWindow( m_blTimerEnable);
	m_cCmbTimerType.EnableWindow( m_blTimerEnable);
	m_cEdtNotifySound.EnableWindow( m_blTimerEnable);
	m_cBtnNotifySoundSel.EnableWindow( m_blTimerEnable);
	m_cBtnNotifySoundPlay.EnableWindow( m_blTimerEnable);
}

void CTimerEditPage::OnOK() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_nWeek = 0;
	for( int nIndex = 0; nIndex < 7; nIndex++)
	{
		if( m_cLstDayOfWeek.GetSel( nIndex))
		{
			m_nWeek |= ( 0x0001 << nIndex);
		}
	}
	
	CPropertyPage::OnOK();
}

void CTimerEditPage::OnTimerenable() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		ControlEnable();
	}
}

void CTimerEditPage::OnSelchangeTimertype() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( UpdateData())
	{
		ControlEnable();
	}
}

void CTimerEditPage::OnWavsel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData();

	char	szCurrentFolder[ _MAX_PATH];
	if( GetCurrentDirectory( _MAX_PATH, szCurrentFolder))
	{
		CString	cStrFile;
		WIN32_FIND_DATA		stFindData;
		HANDLE hFind = FindFirstFile( m_cStrNotifySound, &stFindData);
		if( INVALID_HANDLE_VALUE != hFind)
		{
			cStrFile = m_cStrNotifySound;
			FindClose( hFind);
		}

		CFileDialog	cFileDlg( TRUE, NULL, cStrFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, "Wave File(wav)|*.wav||", this);

		if( IDOK == cFileDlg.DoModal())
		{
			m_cStrNotifySound = cFileDlg.GetPathName();
			UpdateData( FALSE);
		}

		SetCurrentDirectory( szCurrentFolder);
	}
}

void CTimerEditPage::OnWaveplay() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if( !m_cStrNotifySound.IsEmpty())
	{
		BeginWaitCursor();
		PlaySound( m_cStrNotifySound, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
		EndWaitCursor();
	}
}
