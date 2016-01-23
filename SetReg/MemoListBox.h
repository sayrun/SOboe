#if !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoListBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox ウィンドウ
#include "MemoChip.h"
#include "OleDrop.h"

#define	_MAX_TEMPLET	10

class CMemoListBox : public CListBox
{
// コンストラクション
public:
	CMemoListBox();

	void StartDD( void);
	void EndDD( void);

	BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
// アトリビュート
public:

// オペレーション
public:
	int AddMemoChip( const CMemoChip* pcMemoChip);
	int InsertMemoChip( int nIndex, const CMemoChip* pcMemoChip);
	const CMemoChip* GetMemoChip( int nIndex);
	int DeleteMemoChip( int nIndex);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMemoListBox();

	DECLARE_DYNAMIC( CMemoListBox);

	// 生成されたメッセージ マップ関数
protected:
	COleDrop	m_cOleDrop;			// OLE D&D用

	//{{AFX_MSG(CMemoListBox)
	afx_msg void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMOLISTBOX_H__471CB20C_2D92_11D2_90A8_00804C15184E__INCLUDED_)
