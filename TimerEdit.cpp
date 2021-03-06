// TimerEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "TimerEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimerEdit ダイアログ


CTimerEdit::CTimerEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CTimerEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimerEdit)
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


void CTimerEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerEdit)
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


BEGIN_MESSAGE_MAP(CTimerEdit, CDialog)
	//{{AFX_MSG_MAP(CTimerEdit)
	ON_BN_CLICKED(IDC_TIMERENABLE, OnTimerenable)
	ON_CBN_SELCHANGE(IDC_TIMERTYPE, OnSelchangeTimertype)
	ON_BN_CLICKED(IDC_WAVSEL, OnWavsel)
	ON_BN_CLICKED(IDC_WAVEPLAY, OnWaveplay)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimerEdit メッセージ ハンドラ

BOOL CTimerEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
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

	// コントロールの範囲
	m_cSpnMonth.SetRange( 0, 12);
	m_cSpnDay.SetRange( 1, 31);
	m_cSpnHour.SetRange( 0, 23);
	m_cSpnMin.SetRange( 0, 59);
	m_cSpnBeforeNotify.SetRange( 0, 10);

	// IMEの禁止
	::ImmAssociateContext( m_cEdtMonth.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtDay.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtHour.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtMin.GetSafeHwnd(), NULL);
	::ImmAssociateContext( m_cEdtBeforeNotify.GetSafeHwnd(), NULL);

	ControlEnable();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CTimerEdit::ControlEnable( void)
{
	// 状況で各コントロールの有功無効を設定する
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

void CTimerEdit::OnTimerenable() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		ControlEnable();
	}
}

void CTimerEdit::OnSelchangeTimertype() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		ControlEnable();
	}
}

void CTimerEdit::OnWavsel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
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

void CTimerEdit::OnWaveplay() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_cStrNotifySound.IsEmpty())
	{
		BeginWaitCursor();
		PlaySound( m_cStrNotifySound, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
		EndWaitCursor();
	}
}

void CTimerEdit::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	m_nWeek = 0;
	for( int nIndex = 0; nIndex < 7; nIndex++)
	{
		if( m_cLstDayOfWeek.GetSel( nIndex))
		{
			m_nWeek |= ( 0x0001 << nIndex);
		}
	}
	
	CDialog::OnOK();
}

BOOL CTimerEdit::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_TIMERENABLE:
	case IDC_HOUR:
	case IDC_SPNHOUR:
	case IDC_MIN:
	case IDC_SPNMIN:
	case IDC_MONTH:
	case IDC_SPNMONTH:
	case IDC_DAY:
	case IDC_SPNDAY:
	case IDC_WEEKLIST:
	case IDC_TIMERTYPE:
	case IDC_BEFORENOTIFY:
	case IDC_BEFORSPIN:
	case IDC_AFTERNOTIFY:
	case IDC_NOTIFYSHIFT:
	case IDC_WAVEFILE:
	case IDC_WAVSEL:
	case IDC_WAVEPLAY:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}

	
	return CDialog::OnHelpInfo(pHelpInfo);
}
