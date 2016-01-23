#ifndef	__DATALISTNODE_H__
#define	__DATALISTNODE_H__

#include "MemoChip.h"

class CMemoListNode
{
public:
	enum
	{
		DATA_DISPLAY,
		DATA_DELETE
	};

	CMemoListNode( const CMemoChip* pcMemo = NULL, int nDataType = DATA_DISPLAY);
	~CMemoListNode();

	BOOL SetData( const CMemoChip* pcMemo, int nDataType);
	BOOL GetData( CMemoData& cMemo);

	const CString& GetTitle( void) const;
	const CString& GetMemo( void) const;
	const CString& GetDate( void) const;
	const int GetIconIndex( void) const;
	COLORREF GetForeColor( void) const;
	COLORREF GetBackColor( void) const;
	const HWND GetWndHandle( void) const;

	BOOL IsHide() const;
	BOOL IsLink() const;
	BOOL IsDelOnQuit() const;
	BOOL IsSeeThrough() const;
	BOOL IsPassWordLock() const;

protected:
	CString		m_cStrTitle;
	CString		m_cStrMemo;
	CString		m_cStrDate;
	int			m_nIconIndex;
	COLORREF	m_clrFore;
	COLORREF	m_clrBack;
	LOGFONT		m_stLogFont;
	bool		m_blLink;
	bool		m_blSeeThrough;
	bool		m_blPassWordLock;
	bool		m_blShow;

	UINT		m_unDxlID;
	int			m_nDxlSaveDataSize;
	BYTE*		m_pabyDxlSaveData;


	HWND		m_hWnd;
	bool		m_blDelOnQuit;
};

#include "MemoListNode.inl"
#endif	//__DATALISTNODE_H__
