#ifndef	__DRAWEXLIB_H__
#define	__DRAWEXLIB_H__

#include "DrawEx_IF.h"

class CDrawExLib : public CObject
{
public:
	CDrawExLib();
	~CDrawExLib();

	const CString& GetPath( void);
	BOOL SetPath( LPCSTR lpcsz);

	HINSTANCE GetInstance( void) const;
	const CString& GetMenuString( void);
	BOOL SetMenuString( LPCSTR lpcsz);
	UINT GetDataVersion( void) const;

	UINT GetDxlID( void) const;
	UINT GetDrawType( void) const;
	UINT GetUnsupportMenu( void) const;
	const CString& GetDefaultMenuString( void);

	BOOL LoadDxl( HWND hWnd);
	BOOL FreeDxl( void);

	BOOL IsFunctionAssign( void);

	BOOL AboutDrawExLib( HWND hWnd);
	BOOL Configure( HWND hWnd);

	const int UseStart( void);
	const int UseEnd( void);
	int GetUseCount( void) const;
protected:
	void DispErrorMsg( HWND hWnd, DWORD dwCode);

	// 機能確認
	typedef	BOOL ( *DRAWEXSPECIFICATION)( DXLSPECIFICATION* pDxlSpecification);
	// Copyright変更用
	typedef	BOOL ( *DRAWEXABOUT)( HWND hWnd);
	// 設定変更用
	typedef BOOL ( *DRAWEXCONFIGURE)( HWND hWnd);

	UINT		m_unDxlDataVersion;		// 処理可能な最新バージョン
	CString*	m_pcStrMenuString;		// デフォルトの処理名称
	UINT		m_unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	UINT		m_unDrawType;			// dxlの処理内容
	UINT		m_unUnsupportMenu;		// サポートしない機能

	HINSTANCE	m_hInstance;			// 
	CString*	m_pcStrDxlPath;			// ファイル名
	CString		m_cStrDxlMenu;			// デフォルトのメニュ

	int			m_nUseCount;
};

#include "DrawExLib.inl"
#endif	//__DRAWEXLIB_H__