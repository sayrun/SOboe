#ifndef	__MEMOSENDSHEET_H__
#define	__MEMOSENDSHEET_H__

#include "MemoEditSheet.h"
#include "SendEditPage.h"
#include "NetExLib.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoSendSheet

class CMemoSendSheet : public CMemoEditSheet
{
	DECLARE_DYNAMIC(CMemoSendSheet)
	// コンストラクション
public:
	CMemoSendSheet( UINT nIDCaption, CWnd* pParentWnd = NULL, BOOL blSendDirect = FALSE, UINT iSelectPage = 0);
	CMemoSendSheet( LPCTSTR pszCaption, CWnd* pParentWnd = NULL, BOOL blSendDirect = FALSE, UINT iSelectPage = 0);

	BOOL GetSendData( INNERSENDDATA& stInnerSendData);

	int GetEntryCount( void);
	const CEntryData* GetEntryData( int nIndex);
	BOOL AddEntryData( const CEntryData* pcEntryData);

	BOOL IsDistributeEmpty( void);
	BOOL IsDeleteAfterSend( void);

	// Dxl-ID転送用のメソッド
	BOOL ForwardDxlID( UINT unDxlID);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoSendSheet)
	public:
	//}}AFX_VIRTUAL

protected:
	CSendEditPage		m_cSendEditPage;
	BOOL				m_blSendDirect;

	//{{AFX_MSG(CMemoSendSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "MemoSendSheet.inl"

#endif	//__MEMOSENDSHEET_H__