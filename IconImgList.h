#ifndef __ICONIMGLIST_H__
#define __ICONIMGLIST_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IconImgList.h : ヘッダー ファイル
//
#include "IconSelectMenu.h"

class CIconImgList
{
public:
	CIconImgList( void);
	~CIconImgList( void);
	BOOL Draw( int nID, HDC hDC, LPCRECT lpcRect) const;

	BOOL LoadExtIconData( void);
	void ReleaseExtIconData( void);

	int GetDefaultCount( void) const;

	int GetExtendStartIndex( void) const;
	int GetExtendCount( void) const;
	int GetEnableCount( void) const;
	BOOL IsEnable( int nIndex) const;
	int GetParentGroup( int nIndex) const;

	CIconSelectMenu* CreateIconSelectMenu( CWnd* pcWnd, BOOL blForce) const;

	int TrackPopupMenu( int x, int y, CWnd* pcWnd, BOOL blForce = FALSE) const;

	enum { COUNT_DEFAULTICON = 14};

protected:
	BOOL LoadIcons( MSXML2::IXMLDOMDocumentPtr& piDocument);
	BOOL LoadGroups( MSXML2::IXMLDOMDocumentPtr& piDocument);
	BOOL PickUpGroups( MSXML2::IXMLDOMElementPtr& piGroup, int& nParent);
	long GetGroupCount( MSXML2::IXMLDOMElementPtr piGroups);


	HMENU CreatePopupMenu( CWnd* pcWnd, BOOL blForce) const;
	BOOL DestroyMenu( HMENU hMenu) const;
	int InsertIcons( HMENU hMenu, int nID, CDC& cDCWork, BOOL blForce) const;
	int InsertIconsEx( HMENU hMenu, int nID, CDC& cDCWork, BOOL blForce) const;

	static int __cdecl IconCompare(const void *elem1, const void *elem2);

protected:
	HBITMAP m_hBmpDefault;
	HBITMAP m_hBmpExtend;
	int m_nCountExtend;
	int m_nEnableCount;

	struct ICONSTATUS
	{
		int nID;
		BOOL blEnable;
		int nParent;
		int nPosition;
		CString cStrIconName;
	};
	ICONSTATUS* m_astIconStatus;
	ICONSTATUS** m_astIconStatus4Menu;

	struct GROUPDATA
	{
		int nParent;
		BOOL blEnable;
		char szName[1];
	};
	int m_nGroupCount;
	GROUPDATA** m_astGroupData;
};

#endif //__ICONIMGLIST_H__
