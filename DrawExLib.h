#ifndef	__DRAWEXLIB_H__
#define	__DRAWEXLIB_H__

#include "DrawEx_IF.h"

class CDrawExLib : public CObject
{
public:
	CDrawExLib();
	~CDrawExLib();

	const CString& GetPath( void) const;
	BOOL SetPath( LPCSTR lpcsz);

	HINSTANCE GetInstance( void) const;
	const CString& GetMenuString( void) const;
	BOOL SetMenuString( LPCSTR lpcsz);
	UINT GetDataVersion( void) const;

	UINT GetDxlID( void) const;
	UINT GetDrawType( void) const;
	UINT GetDxlStyle( void) const;

	BOOL LoadDxl( void);
	BOOL FreeDxl( void);

	BOOL IsFunctionAssign( void) const;
	BOOL IsSupportEditHelper( void) const;
	BOOL IsSupportExSend( void) const;
	BOOL IsSupportNativeData( void) const;

	const int UseStart( void);
	const int UseEnd( void);
	int GetUseCount( void) const;

	BOOL DoEditHelper( HWND hWnd, EDITDATA* pstEditData) const;
	BOOL About( HWND hWnd) const;
	BOOL ExchangeSendString( EDITDATA* pstEditData) const;
protected:
	// 機能確認
	typedef	BOOL ( *DRAWEXSPECIFICATION)( DXLSPECIFICATION* pDxlSpecification);
	// Copyright変更用
	typedef	BOOL ( *DRAWEXABOUT)( HWND hWnd);
	// 送信する文字列
	typedef	int ( *EXCHANGESENDSTRING)( HGLOBAL* hgMem);
	// 特殊編集：入力ヘルパー
	typedef	int ( *EDITHELPER)( HWND hDlgWnd, EDITDATA* pstEditData);
	// 以降のインターフェイスは、シン覚え書で利用
	// 送信する文字列：拡張
	typedef	int ( *EXCHANGESENDSTRINGEX)( EDITDATA* pstEditData);

	UINT		m_unDxlDataVersion;		// 処理可能な最新バージョン
	CString*	m_pcStrMenuString;		// デフォルトの処理名称
	UINT		m_unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	UINT		m_unDrawType;			// dxlの処理内容
	UINT		m_unStyle;				// dxlのスタイル

	HINSTANCE	m_hInstance;			// 
	CString*	m_pcStrDxlPath;			// ファイル名

	int			m_nUseCount;
};

#include "DrawExLib.inl"
#endif	//__DRAWEXLIB_H__