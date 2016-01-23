#if !defined(AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoEditSheet.h : ヘッダー ファイル
//

#include "BaseEditPage.h"
#include "ExtendEditPage.h"
#include "TimerEditPage.h"
#include "MemoData.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoEditSheet

class CMemoEditSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMemoEditSheet)

// コンストラクション
public:
	CMemoEditSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMemoEditSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoEditSheet)
	public:
	virtual int DoModal();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMemoEditSheet();

	// 編集内容の設定と取得
	virtual void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData);
	// リサイズ情報の取得
	BOOL IsResize( void) const;
	// 終了時の破棄フラグ
	void DelOnQuit( BOOL blDelOnQuit);
	BOOL IsDelOnQuit( void) const;
	// データ整合性テスト：内容
	BOOL IsMemoEmpty( void) const;
	// Dxl-ID転送用のメソッド
	virtual BOOL ForwardDxlID( UINT unDxlID);

	// リサイズの禁止
	void ResizeLock( BOOL blResizeLock);

	// 使用中？
	static BOOL IsUsed();
	static HWND GetUseWnd();

protected:
	// メンバー変数
	CBaseEditPage		m_cBasePage;
	CExtendEditPage		m_cExtPage;
	CTimerEditPage		m_cTimerPage;

	HWND				m_hWndParent;
	HICON m_hIcon;

	static CSemaphore	sm_cSmpDoEdit;
	static HWND			sm_hWndParent;

	// メンバー関数

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMemoEditSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "MemoEditSheet.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMOEDITSHEET_H__741DB80B_2743_11D2_90A4_00804C15184E__INCLUDED_)
