// CustomizeSheet.h : ヘッダー ファイル
//
// このクラスはカスタム モーダル プロパティ シートを定義します。 
// CCustomizeSheet.
 
#ifndef __CUSTOMIZESHEET_H__
#define __CUSTOMIZESHEET_H__

#include "InterfacePage.h"
#include "NetworkPage.h"
#include "DesignPage.h"
#include "CustomExtPage.h"
#include "NetworkExtPage.h"
#include "ExtDllPage.h"
#include "SecurityPage.h"
#include "TempletPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

class CProfileData;

class CCustomizeSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCustomizeSheet)

// 構築
public:
	CCustomizeSheet( BOOL blSpec, CWnd* pWndParent = NULL);

// アトリビュート
public:
	CDesignPage		m_cDesign;
	CNetworkPage	m_cNetWork;
	CInterfacePage	m_cInterface;
	CCustomExtPage	m_cCustomExt;
	CNetworkExtPage	m_cNetworkExt;
	CExtDllPage		m_cExtDll;
	CSecurityPage	m_cSecurity;
	CTempletPage	m_cTemplet;

// オペレーション
public:
	void SetProfileData( CProfileData* pcProfileData);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCustomizeSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCustomizeSheet();

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCustomizeSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CUSTOMIZESHEET_H__
