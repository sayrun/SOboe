#ifndef	__OBOE_NXL_IF_H__
#define	__OBOE_NXL_IF_H__

// 構造体のバージョン
#define	_NXL_IF_DATA_VER		0x00000100

// 受信通知RegisterWindowMessage用
typedef int (CALLBACK* RECEIVEDATA)( const LPVOID lpvEntry, int nEntrySize, const LPVOID lpvData, int nSize);

#define	_NXL_SEND					0x0000000000000001
#define	_NXL_RECEIVE				0x0000000000000002
#define	_NXL_SENDRECEIVE			( _NXL_SEND | _NXL_RECEIVE)
#define	_NXL_ENTRYGROUP				0x0000000000000004
#define	_NXL_SUPPORTRESOLUTION		0x0000000000000008
#define	_NXL_RESERVED				0x0000000000000010
#define	_NXL_SUPPORTORIGINALICON	0x0000000000000020
#define	_NXL_SUPPORTINPUTENTRY		0x0000000000000040
#define	_NXL_SUPPORTBULKSEND		0x0000000000000080

#pragma pack( push, __NetExLib, 1)
typedef struct tagNXLSPECIFICATION
{
	UINT	unLowerStructVersion;		// 処理可能な最古バージョン
	UINT	unUpperStructVersion;		// 処理可能な最新バージョン
	char	szProtcolString[ 50];		// 処理名称
	UINT	unNxlID;					// プロトコル固有ID T.Chiba発行による
	UINT	unNxlStyle;					// Nxlの処理内容
}NXLSPECIFICATION;

#define	_SENDDATA_VER100		0x0100
// 送信データテーブル
// 文字列データは、以下のように取出す
// タイトルの例
//	int nOffset = pstSendDataEx->m_nszMemoTitle;
//	char* pszMemoTitle = NULL;
//	if( 0 <= nOffset)
//	{
//		pszMemoTitle = ( ( char*)pstSendDataEx)[ nOffset];
//	}
// ちなみに、各データの順番は問わない
typedef	struct tagSENDDATA
{
	// フォーマットコード
	char			szHeader[ 9];		// "OboeData\0"
	// 構造体情報
	int				nSize;
	UINT			unStructVersion;

	// バージョン差異通知
	UINT			m_unVersion;		// バージョン番号
	int				m_nszVersion;		// バージョン名称:offset from *SENDDATA

	// 基本設定のデータ
	int				m_nszMemoTitle;		// タイトル:offset from *SENDDATA
	int				m_nszMemoData;		// 内容:offset from *SENDDATA

	// 装飾情報
	int				m_nstLogFont;		// フォント構造体(LOGFONT):offset from *SENDDATA
	COLORREF		m_clrForeColor;		// 文字色
	COLORREF		m_clrBackColor;		// 背景色
	int				m_nIconIndex;		// アイコン種別

	// 関連付け
	BOOL			m_blShellExec;		// 関連付けの有無
	int				m_nszProgName;		// 関連付けプログラム:offset from *SENDDATA
	int				m_nszFileName;		// 関連付けファイル:offset from *SENDDATA

	// タイマ関連
	BOOL			m_blTimerEnable;	// タイマの有無
	int				m_nHour;			// 時間
	int				m_nMin;				// 分
	int				m_nTimerType;		// タイマの種別
	int				m_nMonth;			// 通知月
	int				m_nDay;				// 通知日
	int				m_nNotifyShift;		// 通知日が日曜日だった場合のシフト:1.40
	int				m_nBeforeNotify;	// ｎ日前からの通知:1.40
	BOOL			m_blExpireTimer;	// 経過後のタイマー:1.40
	int				m_nszWaveFile;		// サウンドファイル:1.40:offset from *SENDDATA
	int				m_nWeekDay;			// 通知曜日

	// 送信のデータ
	BOOL			m_blSendMaster;		// 署名の有無
	int				m_nAppendPos;		// 署名追加位置
	int				m_nszSignature;		// 署名:offset from *SENDDATA
	BOOL			m_blSendTime;		// 送信時間
	BOOL			m_blSendResize;		// 送信後のリサイズ
	RECT			m_stRctNormal;		// 表示サイズ
	BOOL			m_blSendCenter;		// センタリング受信:1.40
	BOOL			m_blPack;			// 裏返し送信:1.40
	BOOL			m_blPassWord;		// パスワード要求:1.40
	BOOL			m_blCheckOpen;		// 開封確認:1.40
	BOOL			m_blDelOnQuit;		// 終了時の削除
	int				m_nszFrom;			// 送信元:offset from *SENDDATA
	int				m_nszTo;			// 送信先一覧:offset from *SENDDATA
										// 送信先一覧は、アドレスのみで、","カンマで区切る事

	// ネットワーク決議
	BOOL			m_blCheckYesNo;		// はい／いいえ確認:1.40
	BOOL			m_blOtherResult;	// その他の解:1.42
	int				m_nszResult1;		// 選択肢１:offset from *SENDDATA
	int				m_nszResult2;		// 選択肢１:offset from *SENDDATA
	int				m_nszResult3;		// その他の解デフォルト:offset from *SENDDATA
	int				m_nszCHIPID;		// 応答コード（文字列）:offset from *SENDDATA
	int				m_nResultTime;		// 応答有効時間：－１は無期限

	// 小片の作成時間
	int				m_nCreateYear;		// 作成の年:1.40
	int				m_nCreateMonth;		// 作成の月:1.40
	int				m_nCreateDay;		// 作成の日:1.40
	int				m_nZapDays;			// 削除期日:1.40；使用しない

	// 描画拡張
	UINT			m_unDxlID;			// DxlのID
	int				m_nszDxlDummy;		// Dxl未使用時の表示文字:offset from *SENDDATA
	int				m_nDxlDataSize;		// dxlの拡張情報サイズ
	int				m_nabyDxlData;		// dxlの拡張情報:offset from *SENDDATA

	// この後ろに可変データが格納される
}SENDDATA;

typedef	struct tagRESULTDATA
{
	// フォーマットコード
	char			szHeader[ 9];		// "OboeRslt\0"
	// 構造体情報
	int				nSize;
	UINT			unStructVersion;

	int				m_nszSignature;
	int				m_nszCHIPID;		// 応答コード（文字列）:offset from *RESULTDATA
	int				m_nszResult;		// お返事だよン：offset from *RESULTDATA

	// この後ろに可変データが格納される
}RESULTDATA;

typedef struct tagIMAGEDATA
{
	SIZE		stSize;
	int			nCount;
	HBITMAP		hImage;
	COLORREF	clrMask;
}IMAGEDATA;

#define	_STATUS_NONE	0
#define	_STATUS_NORMAL	1
#define	_STATUS_ABSENCE	2

// エントリデータ
#define	_ENTRYSTRUCT_VER100	0x0100
typedef struct tagENTRYDATA
{
	int		nSize;
	UINT	unStructVersion;
	UINT	unNxlID;
	int		nEntryStat;
	int		nParentGroupCount;
	int		nOffsetParentGroup;
	int		nOffsetEntryName;
}ENTRYDATA;

#define	_GROUPSTRUCT_VER100	0x0100
typedef struct tagGROUPDATA
{
	int		nSize;
	UINT	unStructVersion;
	int		nParentGroup;
	int		nGroupStatus;
	int		nOffsetGroupName;
}GROUPDATA;

#pragma pack( pop, __NetExLib)

#define	_SEND_SUCCESS	0		// 送信は無事に完了した
#define	_SEND_ABSENCE	1		// 送信したが、相手は留守だった！
#define	_SEND_FORCEDEND	2		// nxlが開放され、処理を中断した
#define	_SEND_ERROR		3		// 通信中にエラーが発生した
#define	_SEND_TIMEOUT	4		// 処理がタイムアウトした
#define	_SEND_CANCEL	5		// ユーザによりキャンセルされ、処理を中断した
#define	_SEND_USER		100		// これ以上が各Nxlのエラーコードとする：一応正の数範囲にしてちょ

#define	_NXLSTATUS_ACTIVE			0x000000001
#define	_NXLSTATUS_READY_RECEIVE	0x000000002
#define	_NXLSTATUS_READY_SEND		0x000000004

#ifdef	__cplusplus
extern "C"
{
#endif

// 機能確認
__declspec( dllexport ) BOOL NetExSpecification( NXLSPECIFICATION* pNxlSpecification);
// 設定変更用
__declspec( dllexport ) BOOL NetExConfigure( HWND hWnd);
// Copyright表示用
__declspec( dllexport ) BOOL NetExAbout( HWND hWnd);
// 初期化
__declspec( dllexport ) BOOL InitializeNetEx( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData);
// 終了
__declspec( dllexport ) BOOL UnInitializeNetEx( HWND hWnd);
// 状態の確認
__declspec( dllexport ) DWORD GetNxlStatus( void);
// エントリ開始
__declspec( dllexport ) BOOL StartEntry( void);
// エントリ開始：留守通知付き
__declspec( dllexport ) BOOL StartEntryEx( BOOL blAbsence);
// 留守
__declspec( dllexport ) BOOL AbsenceEntry( BOOL blAbsence);
// エントリ終了
__declspec( dllexport ) BOOL ExitEntry( void);
// エントリリスト更新の中断、取得の開始のため
__declspec( dllexport ) BOOL LockEntryList( void);
// エントリリスト更新の再開
__declspec( dllexport ) BOOL UnlockEntryList( void);
// エントリーイメージの取得
__declspec( dllexport ) BOOL GetEntryImage( IMAGEDATA* lpImageData);
// グループ数を取得
__declspec( dllexport ) int GetGroupCount( void);
// グループ情報を取得
__declspec( dllexport ) int GetGroupData( int nIndex, GROUPDATA* pstGroupData, int nSize);
// リストに存在するエントリ個数取得
__declspec( dllexport ) int GetEntryCount( void);
// エントリ情報の取得
__declspec( dllexport ) int GetEntryData( int nIndex, ENTRYDATA* pstEntry, int nSize);
// エントリ情報の比較
__declspec( dllexport ) int CompareEntryData( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize);
// エントリ情報の表示
__declspec( dllexport ) int GetEntryInfo( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize);
// エントリーの直接取得
__declspec( dllexport ) int InputEntry( HWND hWnd, HGLOBAL* hEntry);
// データ送信
__declspec( dllexport ) int SendMemoData( const ENTRYDATA* pstEntry, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
// データ送信（一括
__declspec( dllexport ) int SendMemoDataInBluk( const ENTRYDATA** pstEntry, int* panResult, int nCount, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
// 応答送信
__declspec( dllexport ) int SendResultData( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent);
// 固有エラーメッセージ
__declspec( dllexport ) int GetErrorMsg( int nEcode, char* pszErrorMsg, int nSize);

#ifdef	__cplusplus
}
#endif

#endif	__OBOE_NXL_IF_H__