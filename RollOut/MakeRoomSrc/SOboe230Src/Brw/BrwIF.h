#ifndef	__BACKUPCHIP_READ_WRITE_H__
#define	__BACKUPCHIP_READ_WRITE_H__

#define	_BRW_IF_DATA_200_VER		0x00000200
#define	_BRW_IF_DATA_300_VER		0x00000300
#define	_BRW_IF_DATA_CUR_VER		0x00000400

#pragma pack( push, 1)
typedef struct tagBRWIFDATA
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	HGLOBAL			m_hachszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル

	int				m_nZOder;				// Ｚオーダー

	HGLOBAL			m_hachszPassWord;		// パスワード

	BOOL			m_blShellExec;			// 関連付け使用の有無
	BOOL			m_blTimerExec;			// 関連付けのタイマでの起動
	HGLOBAL			m_hachszFileName;		// 関連付けのファイル名称
	HGLOBAL			m_hachszProgName;		// 関連付けのプログラム名称

	BOOL			m_blTimerEnable;		// タイマ使用の有無
	int				m_nHour;				// タイマの時
	int				m_nMin;					// タイマの分
	int				m_nTimerType;			// タイマタイプ 0/1/2 = 毎日/曜日/月日
	int				m_nMonth;				// タイマの月
	int				m_nDay;					// タイマの日
	int				m_nWeekDay;				// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	int				m_nNotifyShift;			// 通知日が日曜日だった場合のシフト
	int				m_nBeforeNotify;		// ｎ日前からの通知
	BOOL			m_blExpireTimer;		// 経過後のタイマー
	HGLOBAL			m_hachszSoundFile;		// サウンドファイル

	BOOL			m_blMinSize;			// 小片の表示状態 ０通常／０以外アイコン
	BOOL			m_blShow;				// 表示状態
	int				m_nIconIndex;			// アイコン種別
	RECT			m_stRctChip;			// 小片サイズ
	POINT			m_stPntIcon;			// アイコン位置
	BOOL			m_blChipPosLock;		// 位置サイズ固定

	int				m_nDeleteYear;			// 削除の年
	int				m_nDeleteMonth;			// 削除の月
	int				m_nDeleteDay;			// 削除の日

	BOOL			m_blSeeTrough;			// シースルーフラグ:1.60
	UINT			m_unDXLID;				// DxlのユニークなID:1.60
	int				m_nDxlSaveDataSize;		// dxlの固有データのサイズ:1.60
	HGLOBAL			m_hDxlSaveData;			// dxlの固有データ:1.60
}BRWIFDATA;

#pragma pack( pop)

#ifdef	__cplusplus
extern "C" 
{
#endif

__declspec( dllexport ) DWORD WINAPI GetBrwVersion( int* pnOldStruct, int* pnNewStruct);

__declspec( dllexport ) void WINAPI InitializeBrw( void);
__declspec( dllexport ) void WINAPI UnInitializeBrw( void);
__declspec( dllexport ) int WINAPI CreateChipGroup( void);
__declspec( dllexport ) int WINAPI ReleaseChipGroup( int nChipGroupID);
__declspec( dllexport ) int WINAPI ReadChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI WriteChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI GetChipCount( int nChipGroupID);
__declspec( dllexport ) int WINAPI GetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI SetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI AddChipData( int nChipGroupID, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI InsertAfterChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI InsertBeforeChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData);
__declspec( dllexport ) int WINAPI RemoveChipData( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord);
__declspec( dllexport ) int WINAPI FreeChipData( BRWIFDATA* pBrwData);

__declspec( dllexport ) int WINAPI GetChipTitle( int nChipGroupID, int nDataIndex, LPSTR lpszTitle);
__declspec( dllexport ) int WINAPI GetChipMemo( int nChipGroupID, int nDataIndex, LPSTR lpszData, int nSize, LPCSTR lpcszPassWord);
__declspec( dllexport ) int WINAPI GetChipFontColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor);
__declspec( dllexport ) int WINAPI GetChipBackColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor);
__declspec( dllexport ) int WINAPI GetChipIcon( int nChipGroupID, int nDataIndex);
__declspec( dllexport ) int WINAPI GetChipFont( int nChipGroupID, int nDataIndex, LOGFONT* pstLogFont);
__declspec( dllexport ) int WINAPI IsPwLockChip( int nChipGroupID, int nDataIndex);
__declspec( dllexport ) int WINAPI TestChipPw( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord);
__declspec( dllexport ) int WINAPI IsLink( int nChipGroupID, int nDataIndex);

__declspec( dllexport ) int WINAPI MoveChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex);
__declspec( dllexport ) int WINAPI SwapChipData( int nDstChipGroupID, int nDstDataIndex, int nSrcFromChipGroupID, int nSrcFromDataIndex);
__declspec( dllexport ) int WINAPI CopyChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex);

__declspec( dllexport ) int WINAPI DrawIconHelper( HDC hDC, int nLeft, int nTop, int nIconIndex, COLORREF clrFont, COLORREF clrBack);
__declspec( dllexport ) int WINAPI GetMaxIconIndex( void);
__declspec( dllexport ) HBITMAP WINAPI GetIconImage( void);

__declspec( dllexport ) int WINAPI ExportChipGroup( int nChipGroupID, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI ExportChipData( int nChipGroupID, int nDataIndex, LPCSTR lpcszFileName);
__declspec( dllexport ) int WINAPI ImportChipGroup( int nChipGroupID, LPCSTR lpcszFileName, BOOL blAppend);

#ifdef	__cplusplus
}
#endif
#endif	//__BACKUPCHIP_READ_WRITE_H__
