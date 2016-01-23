#if !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoListBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox ウィンドウ
#include "MemoListNode.h"

class CMemoListBox : public CListBox
{
// コンストラクション
public:
	CMemoListBox();

// アトリビュート
public:

// オペレーション
public:
	int AddMemoChip( const CMemoChip& cMemoChip, BOOL blDelData = FALSE);
	int InsertMemoChip( int nIndex, const CMemoChip& cMemoChip, BOOL blDelData = FALSE);
	int GetPreviewData( int nIndex, CString& cStrMemo, COLORREF& clrFore, COLORREF& clrBack);
	int GetPrintData( int nIndex, CMemoData& cMemoData);
	HWND GetWndHandle( int nIndex);

	BOOL IsLink( int nIndex);
	BOOL IsSeeThrough( int nIndex);
	BOOL IsDelOnQuit( int nIndex);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMemoListBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMemoListBox)
	afx_msg void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
