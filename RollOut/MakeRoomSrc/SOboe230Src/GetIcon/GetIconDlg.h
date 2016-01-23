#ifndef	__GETICONDLG_H__
#define __GETICONDLG_H__

#include "TreeCtrl.h"

#define ID_TIMEOUT 1025

int GetIconDlg_DoModal( HWND hWnd);

class CDialog
{
public:
	CDialog( HWND hWndParent){ m_hWndParent = hWndParent;}

	HWND GetSafeHwnd( void) const;

	void ScreenToClient( LPPOINT lpPoint);
	HWND SetCapture( void);

	void CenterWindow( HWND hWndAlternateOwner = NULL );

protected:
	HWND m_hWnd;
	HWND m_hWndParent;


	HWND GetDlgItem( int nIDDlgItem);
	LRESULT SendDlgItemMessage( int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam);
	int MessageBox( LPCTSTR lpText, LPCTSTR lpCaption = NULL, UINT uType = MB_OK);

	void OnCancel( void);
	void OnOK( void);
};

class CLoginDialog : public CDialog
{
public:
	CLoginDialog( LPCTSTR lpcszID = NULL, LPCTSTR lpcszPW = NULL, HWND hWndParent = NULL);
	~CLoginDialog( void);
	int DoModal( void);

	LPCTSTR GetID( void) const;
	LPCTSTR GetPW( void) const;

private:
	TCHAR m_szID[ 128];	// 可変にしようと思ったけど、フツーこんなに長くないだろうし
	TCHAR m_szPW[ 128];

	void OnInitDialog( void);
	BOOL OnCommand( WORD wID, WORD wNotifyCode);

	static BOOL LoginDialogProc( HWND hDlg, UINT unMessage, WPARAM wParam ,LPARAM lParam);
};

class CGetIconDialog : public CDialog
{
public:
	CGetIconDialog( HWND hWndParent);
	~CGetIconDialog( void);
	int DoModal( void);

protected:
	MSXML2::IXMLDOMDocumentPtr m_piDocument;
	MSXML2::IXMLHTTPRequestPtr m_piHttpReq;

	int m_nIconDataCount;

	CTreeCtrl m_cTrExtIcon;
	BOOL m_blDragging;
	HTREEITEM m_hitemDrag;
	HTREEITEM m_hitemDrop;
	HIMAGELIST m_hImageList;

	BOOL m_blModify;

	BOOL ListUpIcon( void);
	BOOL SetTree( HTREEITEM hParent, MSXML2::IXMLDOMElementPtr& piRoot, MSXML2::IXMLDOMElementPtr& piGroup, const int* const lpcanIconIndex, BOOL* const lpablIconsCheck);
	BOOL GetTree( HTREEITEM hParent, MSXML2::IXMLDOMElementPtr& piRoot, MSXML2::IXMLDOMElementPtr& piParent);

	void CleanupList( void);
	BOOL SaveList( void);

	BOOL SaveTreeItem( HTREEITEM hItem, int nParent, int& nGroupIndex, int& nIconIndex, LPTSTR lpszWork, int nWorkSize);

	void OnInitDialog( void);
	void OnDestroy( void);
	BOOL OnCommand( WORD wID, WORD wNotifyCode);
	void OnMouseMove( UINT nFlags, POINT stPoint);
	BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	BOOL OnButtonUp( UINT nFlags, POINT stPoint);
	void OnContextMenu( HWND hWnd, POINT stPoint);
	BOOL OnStartDownLoad();
	void OnUserMessage( WPARAM wParam, LPARAM lParam);
	void OnTimer( UINT unTimerID);

	static LPCTSTR m_slpszDefaultURL;

	static BOOL GetIconDlgProc( HWND hDlg, UINT unMessage, WPARAM wParam, LPARAM lParam);
};

#include "GetIconDlg.inl"

#endif	//__GETICONDLG_H__
