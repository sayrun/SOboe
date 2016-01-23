// SOboe.h : SOBOE アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_SOBOE_H__386BA42D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_SOBOE_H__386BA42D_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル
#define	DWORD_PTR	DWORD
#define	ULONG_PTR	ULONG
#include <ddeml.h>

/////////////////////////////////////////////////////////////////////////////
// CSOboeApp:
// このクラスの動作の定義に関しては SOboe.cpp ファイルを参照してください。
//
#include "DrawExLib.h"
#include "ChipExLib.h"
#include "NetExLib.h"
#include "CelControl.h"
#include "IconImgList.h"

#define	VER_120			0x00000200
#define	VER_129			0x00000250
#define	VER_141			0x00000300
#define	VER_142			0x00000350
#define	VER_145			0x00000400
#define	VER_160			0x00000600
#define	VER_CUR			0x00000700

#define	VERSION_CODE	0x00023000

#define	_DRAWEXLIB_MAX	15
#define	_CHIPEXLIB_MAX	10
#define	_NETEXLIB_MAX	10
#define	_COMMAND_MAX	15
#define	_USERCOLOR_MAX	16

#define MAX_FREEIMPORT	7
#define MAX_REJECTCOUNT	50

class CSOboeApp : public CWinApp
{
public:
	CSOboeApp();

	// 固定データ取得
	void GetDefaultFont( LOGFONT* stLogFont) const;
	COLORREF GetDefaultForeColor( void) const;
	COLORREF GetDefaultBackColor( void) const;
	int GetDefaultIconIndex( void) const;
	COLORREF GetBeforeForeColor( void) const;
	COLORREF GetBeforeBackColor( void) const;
	COLORREF GetAfterForeColor( void) const;
	COLORREF GetAfterBackColor( void) const;
	const CString& GetSignature( void) const;
	const CString& GetEntryName( void) const;

	// 描画拡張
	int GetDxlMaxEntry( void) const;
	int	GetDxlCount( void) const;
	int FindDxlID( UINT unDxlID) const;
	CDrawExLib* GetDrawExLib( int nIndex);
	UINT GetDefaultDxlID( void) const;
	

	// 小片拡張
	int GetCelMaxCount( void);
	CChipExLib* GetChipExLib( int nIndex);

	// 通信拡張
	int GetNxlMaxEntry( void) const;
	int GetNxlCount( void) const;
	int FindNxlID( UINT unNxlID) const;
	CNetExLib* GetNetExLib( int nIndex);
	CNetExLib* GetDefaultNetExLib( void);
	BOOL OpenNetwork( HWND hWnd);
	BOOL StartNetwork( BOOL blAbsence = FALSE);
	BOOL EndNetwork( void);
	BOOL CloseNetwork( HWND hWnd);
	void SuspendReceive( BOOL blSuspend = TRUE);
	BOOL NotifyAbsence( BOOL blAbsence);

	// 保存時のファイル
	BOOL SetSaveFilePath( const CString& cStrFile);
	const CString& GetSaveFilePath( void) const;
	const CString& GetDelLogFilePath( void) const;
	const CString& GetSendLogFilePath( void) const;
	const CString& GetReceiveLogFilePath( void) const;
	CString GetStockFilePath( void) const;
	BOOL SetStockFilePath( LPCSTR cStrStockFilePath);

	// グローバルパスワード
	const CString& GetGlobalPassWord( void) const;

	// for DDE
	BOOL OnDdeExecut( HDDEDATA hData);
	HDDEDATA OnDdeRequest( UINT wFmt, HSZ hszItem) const;
	BOOL OnDdePoke( UINT wFmt, HSZ hszItem, HDDEDATA hData);
	HSZ GetAppName( void) const;
	HSZ GetAppTopic( void) const;
	UINT GetOboeMemoFormat( void) const;
	BOOL CreateMemoFromBinData( const LPBYTE pabyData, DWORD dwSize, BOOL blCalcRect, BOOL DelOnQuit, BOOL blEdit);

	// for NET
	int ReceiveData( const LPVOID lpvEntry, int nEntrySize, const LPVOID lpvData, int nDataSize);

	enum
	{
		DBLCLKFUNC_EDIT,
		DBLCLKFUNC_LINKEXEC,
		DBLCLKFUNC_ICON,
		DBLCLKFUNC_SEND,
		DBLCLKFUNC_TITLE
	};
	int GetLDblClkFunc( void) const;
	int GetMDblClkFunc( void) const;

	BOOL IsAutoResize( void) const;
	BOOL IsAutoTriming( void) const;
	BOOL IsAutoIMEOpen( void) const;
	BOOL IsAutoScroll( void) const;
	void AutoScroll( BOOL blAutoScroll);

	BOOL PopupReceive( BOOL blPopupReceive);
	BOOL IsPopupReceive( void) const;
		BOOL IsHideReceive( void) const;
		BOOL IsSubNotify( void) const;

	BOOL IsAppendSignature( void) const;
	BOOL AppendSignature( BOOL blSignature);
	BOOL IsAppendTime( void) const;
	BOOL AppendTime( BOOL blTime);
		int GetAppendPos( void) const;
		int SetAppendPos( int nPos);
	BOOL IsPackSend( void) const;
		BOOL IsNotifyOpen( void) const;

	BOOL IsDataListReverse( void) const;
	BOOL DataListReverse( BOOL blReverse);
	int GetDataListFilter( void) const;
	int SetDataListFilter( int nFilter);

	BOOL IsDelListReverse( void) const;
	BOOL DelListReverse( BOOL blReverse);

	UINT GetHKModifiers( void) const;
	UINT GetHKVirtKey( void) const;

	int GetResumeDelay( void) const;

	const CString& ArrangeNormalDLL( void) const;
	const CString& ArrangeTitleDLL( void) const;
	const CString& ArrangeIconDLL( void) const;
	const CString& ArrangeAllDLL( void) const;

	const CCelControl* GetCelControl( void) const;

	int GetShiftStepX( void);
	int GetShiftStepY( void);

	BOOL IsEnableNetwork( void) const;
		BOOL IsStandbyNetwork( void) const;
	BOOL IsFlatMenu( void) const;
	BOOL IsHideStart( void) const;
	BOOL IsLoggingDelChip( void) const;
	BOOL IsHideback( void) const;
	BOOL IsNofityVersion( void) const;
	BOOL IsNoCheckDelete( void) const;
	BOOL IsDeleteAfterSend( void) const;
	BOOL IsDelOnQuit2Rcv( void) const;

	BOOL IsLoggingSendChip( void) const;
	BOOL IsFullLoggingSendChip( void) const;
	BOOL IsLoggingReceiveChip( void) const;
	BOOL IsFullLoggingReceiveChip( void) const;

	BOOL IsCheckDelOnQuit( void) const;

	BOOL IsReceptionLink( void) const;
	BOOL IsReceptionTimer( void) const;
	BOOL IsReceptionDxl( void) const;

	BOOL IsEnableTemplet( void) const;
		BOOL FreeTemplet( void);
		BOOL LoadTemplet( void);
		int GetTempletCount( void) const;
		BOOL GetTempletName( int nIndex, CString& cStr) const;
		const CMemoData* GetTemplet( int nIndex) const;

	BOOL IsEnableSpMenu() const;

	BOOL GetUserColor( COLORREF* paclrUserColors);
	BOOL SetUserColor( const COLORREF* paclrUserColors);

	BOOL IsCompatibleOU( void) const;

	BOOL GetExportFolder( CString& cStrFolder);
	BOOL SetExportFolder( LPCSTR lpcszFolder);

	BOOL IsMakeBackupFile( void) const;

	BOOL EnableNxlGroup( void) const;

	int GetGenerationCount( void) const;

	BOOL IsExEdit( void) const;
	void ExEdit( BOOL blExEdit);
		BOOL IsBigButton( void) const;
		void BigButton( BOOL blBigButton);
		const CRect& GetEditWindowRect( void) const;
		void SetEditWindowRect( const CRect& cRect);

	BOOL IsSendDlgAutoClose( void);
	void SendDlgAutoClose( BOOL blSendDlgAutoClose);

	DWORD GetDelayTime( void) const;

	// 使用中？
	static BOOL IsCustomizing( void);
	int DoCustomize( CWnd* pcParent);

	static BOOL IsUpdating( void);
	BOOL IsExistUpdateDLL();
	int DoUpdateIcon( CWnd* pcParent);


	// 印刷のカラーorモノクロ指定
	BOOL IsColorPrint( void) const;
	void ColorPrint( BOOL blColor);

	// カタログ表示
	BOOL IsEnableCatalogView( void) const;

	// 受信と保存
	BOOL IsReceiveAndSave( void) const;
	// 編集と保存
	BOOL IsModifyAndSave( void) const;

	// 送信リストの展開
	BOOL IsExpandSendTree( void) const;

	// アイコン情報
	const CIconImgList* GetIconList( void) const;
	BOOL UpdateIconList( void);

	// 
	void DecrimentImportCount( void);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSOboeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSOboeApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CList < CMemoData*, CMemoData*&>	m_cLstSeeds;
	// 固定情報
	LOGFONT		m_stLOGFONT;
	COLORREF	m_clrForeColor;
	COLORREF	m_clrBackColor;
	int			m_nIconIndex;
	CDrawExLib	m_acDrawExlib[ _DRAWEXLIB_MAX];
	UINT		m_unDefaultDxlID;
	CChipExLib	m_acChipExlib[ _CHIPEXLIB_MAX];
	CNetExLib	m_acNetExlib[ _NETEXLIB_MAX];
	int			m_nDefaultNxl;
	CCelControl	m_cCelControl;

	CIconImgList* m_pcIconImgs;

	COLORREF	m_clrBeforeFore;
	COLORREF	m_clrBeforeBack;
	COLORREF	m_clrAfterFore;
	COLORREF	m_clrAfterBack;
	CString		m_cStrSignature;
	CString		m_cStrEntryName;

	CString		m_cStrSaveFilePath;
	CString		m_cStrStockFilePath;
	CString		m_cStrDelLogFilePath;
	CString		m_cStrSendLogFilePath;
	CString		m_cStrReceiveLogFilePath;
	CString		m_cStrExportFolder;

	CString		m_cStrGlobalPassWord;

	// for DDE
	DWORD		m_dwIdInst;
	HSZ			m_hszAppName;
	HSZ			m_hszTopic;
	BOOL		m_blSetWarehouse;

	static struct COMMAND
	{
		LPCSTR	lpcsz;
		UINT	unCommand;
	}			stCommands[ _COMMAND_MAX];

	static UINT	m_unOboeFmt;

	CCriticalSection	m_cCrsRcvMemo;

	BOOL		m_blAutoResize;
	BOOL		m_blAutoTriming;
	BOOL		m_blAutoIMEOpen;
	BOOL		m_blAutoScroll;

	BOOL		m_blPopupReceive;
		BOOL		m_blHideReceive;
		BOOL		m_blSubNotify;

	BOOL		m_blAppendSignature;
	BOOL		m_blAppendTime;
		BOOL		m_nAppendPos;
	BOOL		m_blPackSend;
		BOOL		m_blNotifyOpen;

	CString		m_cStrCompName;
	CString		m_cStrUserName;

	int			m_nLDblClkFunc;
	int			m_nMDblClkFunc;

	BOOL		m_blDataListReverse;
	int			m_nDataListFilter;
	BOOL		m_blDelListReverse;

	DWORD		m_dwHotKey;

	int			m_nResumeDelay;

	CString		m_cStrArrangeNormalDLL;
	CString		m_cStrArrangeTitleDLL;
	CString		m_cStrArrangeIconDLL;
	CString		m_cStrArrangeAllDLL;

	int			m_nShiftStepY;
	int			m_nShiftStepX;

	BOOL		m_blStandbyNetwork;
	BOOL		m_blEnableNetwork;

	BOOL		m_blFlatMenu;
	BOOL		m_blHideStart;
	BOOL		m_blLoggingDelChip;
	BOOL		m_blHideback;
	BOOL		m_blNoCheckDelete;
	BOOL		m_blLoggingSendChip;
	BOOL		m_blFullLoggingSendChip;
	BOOL		m_blLoggingReceiveChip;
	BOOL		m_blFullLoggingReceiveChip;
	BOOL		m_blCheckDelOnQuit;
	BOOL		m_blDelOnQuit2Rcv;

	BOOL		m_blNotifyVersion;

	BOOL		m_blReceptionLink;
	BOOL		m_blReceptionTimer;
	BOOL		m_blReceptionDxl;

	BOOL		m_blEnableTemplet;
	CString		m_cStrTempletFile;

	BOOL		m_blInitialized;

	BOOL		m_blCompatibleOU;

	HINSTANCE	m_hNetInitDll;

	BOOL		m_blMakeBackupFile;

	BOOL		m_blEnableNxlGroup;

	int			m_nGenerationCount;

	BOOL		m_blSuspendReceive;

	BOOL		m_blEnableSpMenu;

	BOOL		m_blExEdit;
	BOOL		m_blBigButton;
	CRect		m_cRctEditWindow;

	DWORD		m_dwInitalDeray;

	BOOL		m_blSendDlgAutoClose;

	BOOL		m_blColorPrint;

	BOOL		m_blCatalog;

	BOOL		m_blAbsence;

	BOOL		m_blReceiveAndSave;
	BOOL		m_blModifyAndSave;
	BOOL		m_blExpandSendTree;

	// 色選択ダイアログのカスタム色
	// CColorDialogのカスタム色16
	COLORREF	m_clrCustomColor[ _USERCOLOR_MAX];

	int			m_nImportCount;

	BOOL		m_blDeleteAfterSend;

protected:
	BOOL LoadDrawExLibs( void);
	BOOL FreeDrawExLibs( void);
	BOOL LoadNetExLibs( void);
	BOOL FreeNetExLibs( void);

	BOOL LoadProfile( BOOL blAllLoad = TRUE);
	BOOL SaveProfile( void);
	BOOL InitializeProfile( void);

	void GetProfileName( CString& cStr, BOOL blMasterFile = FALSE);
	BOOL IsMultiUserMode( void);

	typedef int ( INITIALIZEWINSOCK)( WORD* pwVersion);
	typedef int ( TERMINATEWINSOCK)( void);


#define	_CUSTOMIZEDLL_VER	0x0200
	typedef	UINT ( *GETSETREGVERSION)( void);
	typedef	int ( *DOCUSTOMIZE)( BOOL, HWND);
	static CSemaphore sm_cSmpDoCustomize;

	typedef int ( *LPDOUPDATEICON)( HWND hWnd);
	static CSemaphore sm_cSmpDoUpdating;
};

HDDEDATA CALLBACK DdeCallback( UINT wType, UINT wFmt, HCONV hConv, HSZ hszTopic, HSZ hszItem, HDDEDATA hData, DWORD lData1, DWORD lData2);

#include "SOboe.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SOBOE_H__386BA42D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
