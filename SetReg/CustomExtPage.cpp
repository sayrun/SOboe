// CustomExtPage.cpp : インプリメンテーション ファイル
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
// CCustomExtPage プロパティ ページ

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
// CCustomExtPage メッセージ ハンドラ

BOOL CCustomExtPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
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
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CCustomExtPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
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
