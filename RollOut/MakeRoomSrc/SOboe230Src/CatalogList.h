#ifndef	__CATALOGLIST_H__
#define	__CATALOGLIST_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CatalogList.h : ヘッダー ファイル
//

#include "MemoData.h"

struct MENUITEM
{
	SIZE		stSize;
	int			nIndex;
	UINT		unItemID;
	COLORREF	clrFore;
	COLORREF	clrBack;
	char		lfFaceName[ LF_FACESIZE]; 
	CString		cStrTitle;
};

class CCatalogList : public CList <MENUITEM*, MENUITEM*&>
{
public:
	CCatalogList() : CList< MENUITEM*, MENUITEM*&>( 5){};
	~CCatalogList();

	BOOL AppendData( HDC hWorkDC, UINT nItemID, const CMemoData* pccMemoData);
};
#endif	//__CATALOGLIST_H__
