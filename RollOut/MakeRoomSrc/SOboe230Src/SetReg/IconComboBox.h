// IconComboBox.h : ヘッダー ファイル
//
#ifndef	__ICONCOMBOBOX_H__
#define	__ICONCOMBOBOX_H__

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox ウィンドウ

class CIconComboBox : public CComboBox
{
// コンストラクション
public:
	CIconComboBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CIconComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CIconComboBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CIconComboBox)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	//__ICONCOMBOBOX_H__
