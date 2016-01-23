// SecurityPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "SecurityPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecurityPage プロパティ ページ

IMPLEMENT_DYNCREATE(CSecurityPage, CPropertyPage)

CSecurityPage::CSecurityPage() : CPropertyPage(CSecurityPage::IDD)
{
	//{{AFX_DATA_INIT(CSecurityPage)
	m_cStrOldPW = _T("");
	m_cStrNewPW = _T("");
	m_cStrCheckPW = _T("");
	m_blReceptionTimer = FALSE;
	m_blReceptionLink = FALSE;
	m_blReceptionDxl = FALSE;
	//}}AFX_DATA_INIT
}

CSecurityPage::~CSecurityPage()
{
}

void CSecurityPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecurityPage)
	DDX_Text(pDX, IDC_OLDPASSWORD, m_cStrOldPW);
	DDX_Text(pDX, IDC_NEWPASSWORD, m_cStrNewPW);
	DDX_Text(pDX, IDC_CHECKPASSWORD, m_cStrCheckPW);
	DDX_Check(pDX, IDC_RECEPTIONTIMER, m_blReceptionTimer);
	DDX_Check(pDX, IDC_RECEPTIONLINK, m_blReceptionLink);
	DDX_Check(pDX, IDC_RECEPTIONDXL, m_blReceptionDxl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecurityPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSecurityPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecurityPage メッセージ ハンドラ

BOOL CSecurityPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_blReceptionTimer = m_pcProfileDatas->m_blReceptionTimer;
	m_blReceptionLink = m_pcProfileDatas->m_blReceptionLink;
	m_blReceptionDxl = m_pcProfileDatas->m_blReceptionDxl;

	m_cStrNewPW = m_pcProfileDatas->m_cStrGlobalPassWord;
	m_cStrCheckPW = m_pcProfileDatas->m_cStrGlobalPassWord;

	UpdateData( FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSecurityPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	m_pcProfileDatas->m_blReceptionTimer = m_blReceptionTimer;
	m_pcProfileDatas->m_blReceptionLink = m_blReceptionLink;
	m_pcProfileDatas->m_blReceptionDxl = m_blReceptionDxl;
	
	CPropertyPage::OnOK();
}

BOOL CSecurityPage::OnKillActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	UpdateData();
	if( m_cStrNewPW != m_pcProfileDatas->m_cStrGlobalPassWord)
	{
		CString	cStrErr;
		if( m_cStrOldPW == m_pcProfileDatas->m_cStrGlobalPassWord)
		{
			if( m_cStrNewPW == m_cStrCheckPW)
			{
				if( 0 < m_cStrNewPW.GetLength())
				{
					if( MIN_PASSWORD > m_cStrNewPW.GetLength())
					{
						cStrErr.LoadString( IDS_VERYSHORTPW);
						MessageBox( cStrErr, NULL, MB_ICONHAND);
						return 0;
					}
				}
				m_pcProfileDatas->m_cStrGlobalPassWord = m_cStrNewPW;
			}
			else
			{
				cStrErr.LoadString( IDS_NOTEQUPW);
				MessageBox( cStrErr, NULL, MB_ICONHAND);
				return 0;
			}
		}
		else
		{
			cStrErr.LoadString( IDS_MISMATCHOLDPW);
			MessageBox( cStrErr, NULL, MB_ICONHAND);
			return 0;
		}
	}
	
	return CPropertyPage::OnKillActive();
}
