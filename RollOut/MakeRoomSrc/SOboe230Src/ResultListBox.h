#if !defined(AFX_SENDLISTBOX_H__A90B5701_FE7E_11D0_8131_004005469C82__INCLUDED_)
#define AFX_SENDLISTBOX_H__A90B5701_FE7E_11D0_8131_004005469C82__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ResultListBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CResultListBox ウィンドウ

class CResultListBox : public CListBox
{
// コンストラクション
public:
	CResultListBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CResultListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CResultListBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CResultListBox)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SENDLISTBOX_H__A90B5701_FE7E_11D0_8131_004005469C82__INCLUDED_)
