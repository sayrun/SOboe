#ifndef	__DXLCONTROL_H__
#define	__DXLCONTROL_H__

#include "DrawEx_IF.h"
#include "DrawExLib.h"

class CMemoChip;

class CDxlControl : public CObject
{
public:
	CDxlControl( void);
	~CDxlControl( void);

	BOOL SetInstance( const CDrawExLib* cDrawExLib);
	HINSTANCE GetInstance( void) const;

	const CDxlControl& operator=( CDxlControl& cStrSrc);

	BOOL InitializeChip( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom);
	BOOL UnInitializeChip( HWND hWnd);

	BOOL CalcDefaultRect( HWND hWnd, SIZE* pSize, const CMemoData& cMemoData);
	BOOL DrawChip( HWND hWnd, HDC hDC, const CMemoData& cMemoData);
	DWORD MouseClik( HWND hWnd, POINT* pstPoint);
	BOOL Resize( HWND hWnd, SIZE* pstSize);
	BOOL Zoom( HWND hWnd, BOOL blZoom);
	BOOL UpDate( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom);
	BOOL GetMemoString( CString& cStr, const CMemoData& cMemoData);
	BOOL DoEditHelper( HWND hWnd, HGLOBAL* phGloba);
	HMENU GetDrawExMenu( void);
	BOOL DoDrawExMenu( HWND hWnd, UINT unMenuID);
	BOOL GetSaveData( HWND hWnd, CMemoData& cMemoData);
	int GetTimerSpan( void) const;
	DWORD DoTimer( void) const;
	BOOL Active( HWND hWnd, BOOL blActive);

	// 形態
	BOOL IsDataExchange() const;
	BOOL IsOwnerDraw() const;

	// サポートメニュー
	BOOL IsSupportResize() const;
	BOOL IsSupportClick() const;
	BOOL IsSupportMenu() const;
	BOOL IsSupportNativeData() const;
	BOOL IsSupportTimer() const;
	BOOL IsNeedString() const;
	BOOL IsNoAlphaBlend() const;

	DWORD CheckTimer( void);

	// メニュー使用中？
	static BOOL IsUsed();
protected:
	// 初期化処理
	typedef	BOOL ( *INITIALIZECHIP)( DWORD* pdwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
	// 終了処理
	typedef	BOOL ( *UNINITIALIZECHIP)( DWORD dwDxlWord, HWND hWnd);
	// 大きさ再計算
	typedef	BOOL ( *CALCDEFAULTRECT)( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize, LPCSTR lpcszMemoData);
	// 表示
	typedef	BOOL ( *DRAWCHIP)( DWORD dwDxlWord, HWND hWnd, HDC hDC, LPCSTR lpcszMemoData);
	// マウスクリック
	typedef	BOOL ( *MOUSECLIK)( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
	// サイズ変更
	typedef	BOOL ( *RESIZE)( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
	// 小片化／アイコン化に呼ばれる：_DT_OWNERDRAW指定時のみ処理
	typedef	void ( *ZOOM)( DWORD dwDxlWord, HWND hWnd, BOOL blZoom);
	// データ更新：編集により発生
	typedef	BOOL ( *UPDATE)( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstGelIfData);
	// 描画する文字列：_DT_EXCHANGE指定時のみ処理
	typedef	int ( *GETMEMOSTRING)( DWORD dwDxlWord, LPSTR lpszData, int nSize, LPCSTR lpcszMemoData);
	// ポップアップメニュー
	typedef	HMENU ( *GETDRAWEXMENU)( DWORD dwDxlWord);
	// ポップアップメニュー
	typedef	DWORD ( *DODRAWEXMENU)( DWORD dwDxlWord, HWND hWnd, UINT unMenuID);
	// データ保存時の編集
	typedef BOOL ( *GETSAVEDATA)( DWORD dwDxlWord, HWND hWnd, HGLOBAL* phGMem);
	// タイマ周期取得
	typedef int ( *GETTIMERSPAN)( DWORD dwDxlWord);
	// タイマ処理
	typedef DWORD ( *DOTIMER)( DWORD dwDxlWord);

	// 以降のインターフェイスは、シン覚え書で利用
	// マウスクリック：拡張
	typedef DWORD ( *MOUSECLIKEX)( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
	// 小片のアクティブ：新規
	typedef void ( *ACTIVE)( DWORD dwDxlWord, HWND hWnd, BOOL blActive);

protected:
	int m_nTimerSpan;
	HINSTANCE			m_hInstance;
	DWORD				m_dwDxlWord;
	UINT				m_unDrawType;
	UINT				m_unStyle;
	UINT				m_unIFDataVer;

	static CSemaphore	sm_cSmpDoMenu;

	// 初期化処理
	INITIALIZECHIP m_lpfInitializeChip;
	// 終了処理
	UNINITIALIZECHIP m_lpfUnInitializeChip;
	// 大きさ再計算
	CALCDEFAULTRECT m_lpfCalcDefaultRect;
	// 表示
	DRAWCHIP m_lpfDrawChip;
	// マウスクリック
	MOUSECLIK m_lpfMouseClik;
	// サイズ変更
	RESIZE m_lpfResize;
	// 小片化／アイコン化
	ZOOM m_lpfZoom;
	// データ更新：編集により発生
	UPDATE m_lpfUpdate;
	// 描画する文字列：_DT_EXCHANGE指定時のみ処理
	GETMEMOSTRING m_lpfGetMemoString;
	// ポップアップメニュー
	GETDRAWEXMENU m_lpfGetDrawExMenu;
	// ポップアップメニュー
	DODRAWEXMENU m_lpfDoDrawExMenu;
	// データ保存時の編集
	GETSAVEDATA m_lpfGetSaveData;
	// タイマ周期取得
	GETTIMERSPAN m_lpfnGetTimerSpan;
	// タイマ処理
	DOTIMER m_lpfnDoTimer;

	// マウスクリック：拡張
	MOUSECLIKEX m_lpfMouseClikEx;
	// 小片のアクティブ：新規
	ACTIVE m_lpfActive;
};

#include "DxlControl.inl"
#endif	//__DXLCONTROL_H__
