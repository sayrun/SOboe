#ifndef	__CELCONTROL_H__
#define	__CELCONTROL_H__


#define	_CEL_MAX				10

#define	_CEL_MENUTOP			11000
#define	_CEL_MENU0				11000
#define	_CEL_MENU1				11001
#define	_CEL_MENU2				11002
#define	_CEL_MENU3				11003
#define	_CEL_MENU4				11004
#define	_CEL_MENU5				11005
#define	_CEL_MENU6				11006
#define	_CEL_MENU7				11007
#define	_CEL_MENU8				11008
#define	_CEL_MENU9				11009
#define	_CEL_MENUBOTTOM			11009

#define	_CELCTL_ERROR			0x80000000
#define	_CELCTL_NOP				0x00000000
#define	_CELCTL_REDRAW			0x00000001
#define	_CELCTL_MOVE			0x00000002
#define	_CELCTL_RESIZE			0x00000004

class CMemoData;

class CCelControl
{
public:
	CCelControl( void);

	BOOL SetSelData( int nIndex, LPCSTR lpcszPath, LPCSTR lpcszMenu = NULL);
	BOOL SetMenuSeparator( LPCSTR lpcsz);

	HMENU GetMenu( void) const;

	static BOOL IsUsed();

	DWORD DoCel( HWND hWnd, UINT unMenuID, CMemoData& cMemoData) const;

protected:
	BOOL	m_blValid;
	CString	m_acStrMenu[ _CEL_MAX];
	CString	m_acStrPath[ _CEL_MAX];
	CString	m_cStrSeparator;

	static CSemaphore	sm_cSmpDoCel;

	typedef struct tagSPECIFICATION
	{
		UINT	unLowerStructVersion;	// 処理可能な最古バージョン
		UINT	unUpperStructVersion;	// 処理可能な最新バージョン
		char	szMenuString[ 50];		// デフォルトのメニュー名称
	}SPECIFICATION;


// 構造体のバージョン
#define	_IF_DATA_VER			0x00000100

// 変更フラグ
#define	_MODIFY_MEMO			0x00000001
#define	_MODIFY_COLOR			0x00000002
#define	_MODIFY_FONT			0x00000004
#define	_MODIFY_SHELL			0x00000008
#define	_MODIFY_TIMER			0x00000010
#define	_MODIFY_ICON			0x00000020
#define	_MODIFY_PASSWORD		0x00000040
#define	_MODIFY_ZODER			0x00000080
#define	_MODIFY_RECTCHIP		0x00000100
#define	_MODIFY_POINTICON		0x00000200
#define	_MODIFY_AUTORESIZE		0x00000400

	typedef struct tagIFDATA
	{
		int				m_nSize;				// この構造体のサイズ
		int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

		UINT			m_unModify;				// DLL側での変更フラグ

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

		int				m_nIconIndex;			// アイコン種別

		RECT			m_stRctChip;			// 小片サイズ
		POINT			m_stPntIcon;			// アイコン位置
	}IFDATA;
	typedef	BOOL ( *CHIPEXSPECIFICATION)( SPECIFICATION* pSpecification);
	typedef	BOOL ( *CHIPEXPROC)( HWND hWnd, UINT unMenuNum, IFDATA* pstIFData);

	BOOL MakeIFData( IFDATA& stIfData, CMemoData& cMemoData) const;
	BOOL DeleteIFData( IFDATA& stIFData) const;
	DWORD ModifyIFData( CMemoData& cMemoData, IFDATA& stIFData) const;
};

#endif	//__CELCONTROL_H__