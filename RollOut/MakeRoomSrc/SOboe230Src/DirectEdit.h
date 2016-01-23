#pragma once


// CDirectEdit

class CDirectEdit : public CEdit
{
	DECLARE_DYNAMIC(CDirectEdit)

public:
	CDirectEdit( const LOGFONT* pFont, COLORREF clrFore, COLORREF clrBack);
	virtual ~CDirectEdit();

	enum
	{
		ID_DIRECTEDIT = 1022
	};

private:
	CFont m_cFont;
	COLORREF m_clrFore;
	COLORREF m_clrBack;
	CBrush m_cBrushBack;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create( CWnd* pParentWnd, UINT nID = ID_DIRECTEDIT);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


