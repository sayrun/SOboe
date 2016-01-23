#pragma once



// CIconSelectMenu コマンド ターゲット

class CIconSelectMenu : public CMenu
{
public:
	CIconSelectMenu();
	virtual ~CIconSelectMenu();

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);

	BOOL AppendMenu( CDC* pcDC, UINT nItemID, LPCTSTR lpcszItem, HBITMAP hBitmap);

	BOOL DestroyMenu();
private:
	void DeleteMyItem( HMENU hMenu);

protected:
	struct MENUITEM
	{
		HBITMAP hBitmap;
		int nHeight;
		int nWidth;
		char szItemName[ 1];
	};
};


