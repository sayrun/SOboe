#if !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoChip.h : ヘッダー ファイル
//

#include "MemoData.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoChip ウィンドウ

#define	MOVEAREAPOINT_MAX	4

class CMemoChip : public CWnd
{
// コンストラクション
public:
	CMemoChip();

// アトリビュート
public:

// オペレーション
public:
	const CMemoChip& operator=( const CMemoChip& cMemoChip);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CMemoChip)
	public:
	protected:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMemoChip();

	// データへのアクセス
	void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData) const;
	const CMemoData* AccessMemoData( void) const;

	void Serialize( CArchive& ar );

	DECLARE_SERIAL( CMemoChip);
	// プライベートなメソッド
protected:
	CMemoData	m_cMemoData;
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMemoChip)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "MemoChip.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
