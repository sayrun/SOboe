#ifndef	__MEMODATA_H__
#define	__MEMODATA_H__

#include "DDData.h"

#define	_MAX_TITLE_LEN	63
#define	_CODE_BASE		16

#define	_SCHEMA_ID_TYPE1	0x094A	//1.29�ȑO
#define	_SCHEMA_ID_TYPE2	0x094B	//1.40
#define	_SCHEMA_ID_TYPE3	0x094C	//1.60
#define	_SCHEMA_ID			0x2000	//2.00

class CMemoData : public CObject
{
public:	// ���\�b�h
	CMemoData();
	CMemoData( const CMemoData& cMemoData);
	~CMemoData();

	// �\���f�[�^
	void SetTitle( const CString& cStrTitle);
	BOOL GetTitle( CString& cStrTitle) const;
	void SetMemo( const CString& cStrMemo);
	int GetMemoLength( void) const;
	const CString& GetMemo( void) const;

	// �\���X�^�C��
	void SetFont( const LOGFONT* pstLogFont = NULL);
	BOOL GetFont( LOGFONT* const stLogFont) const;
	void SetForeColor( const COLORREF& clrFore = CLR_INVALID);
	BOOL GetForeColor( COLORREF& clrFore) const;
	void SetBackColor( const COLORREF& clrBack = CLR_INVALID);
	BOOL GetBackColor( COLORREF& clrBack) const;
	void SetIconIndex( const int nIconIndex);
	int GetIconIndex( void) const;

	// �֘A�t��
	BOOL Link( const BOOL blLinkEnable = FALSE);
	BOOL IsLink( void) const;
	// �^�C�}�ł̊֘A�t��
	BOOL TimerExec( const BOOL blTimerExec = FALSE);
	BOOL IsTimerExec( void) const;
	// �����N���I
	BOOL SetLinkData( const CString& cStrFile, const CString& cStrParameters);
	BOOL GetLinkData( CString& cStrFile, CString& cStrParameters) const;

	// �p�X���[�h���b�N
	BOOL PassWordLock( const BOOL blPassWordLock = FALSE);
	BOOL IsPassWordLock( void) const;
	BOOL SetPassWord( const CString& cStrPassWord);
	const CString& GetPassWord( void) const;

	// �^�C�}
	BOOL Timer( BOOL blTimer = FALSE);
	BOOL IsTimer( void) const;

	// �^�C�}���
	enum
	{
		TIMERTYPE_EVERYDAY,		// �����ʒm
		TIMERTYPE_WEEK,			// �j���w��
		TIMERTYPE_DAY			// ���t�w��
	};
	int SetTimerType( int nType = TIMERTYPE_EVERYDAY);
	int GetTimerType( void) const;

	// �ʒm���̈ړ�
	enum
	{
		NOTIFYSHIFT_NOMOVE,				// �ړ�����
		NOTIFYSHIFT_TOSAT,				// �y�j����
		NOTIFYSHIFT_TOFRI,				// ���j����
		NOTIFYSHIFT_TOMON_FROMSUN,		// ���j���� ���j��
		NOTIFYSHIFT_TOMON_FROMSATSUN	// ���j���� �y�j���^���j��
	};
	int SetNotifyShift( int nShift = NOTIFYSHIFT_NOMOVE);
	int GetNotifyShift( void) const;

	// �ʒm��
	int SetTimerMonth( int nMonth);
	int GetTimerMonth( void) const;
	int GetNotifyMonth( const CTime& cTimeNow) const;	// �ʒm���ړ����邢�͖����ɂ��ړ�

	// �ʒm��
	int SetTimerDay( int nDay);
	int GetTimerDay( void) const;
	int GetNotifyDay( const CTime& cTimeNow) const;		// �ʒm���ړ��ɂ��ړ�

	// �j���̒ʒm
	enum
	{
		WEEK_DISABLE = 0,
		WEEK_SUN = 0x00000001,
		WEEK_MON = 0x00000002,
		WEEK_TUE = 0x00000004,
		WEEK_WED = 0x00000008,
		WEEK_THU = 0x00000010,
		WEEK_FRI = 0x00000010,
		WEEK_SAT = 0x00000020,
		WEEK_ALL = ( WEEK_SUN | WEEK_MON | WEEK_TUE | WEEK_WED | WEEK_THU | WEEK_FRI | WEEK_SAT)
	};
	int SetWeek( int nWeek);
	int GetWeek( void) const;

	// �ʒm��
	int SetTimerHour( int nHour);
	int GetTimerHour( void) const;
	// �ʒm��
	int SetTimerMin( int nMin);
	int GetTimerMin( void) const;

	int SetBeforeNotify( int nBefore = 0);
	int GetBeforeNotify( void) const;

	BOOL AfterNotify( BOOL blNotify);
	BOOL IsAfterNotify( void) const;

	BOOL SetNotifySound( const CString& cStrNotifySound);
	BOOL GetNotifySound( CString& cStrNotifySound) const;

	// �`��g��
	UINT SetDxlID( const UINT unDxlID);
	UINT GetDxlID( void) const;
	BOOL SetDxlExtData( const BYTE* pabyDxlExtData, const UINT unSize);
	UINT GetDxlExtData( BYTE* pabyDxlExtData = NULL, UINT unSize = 0) const;

	// �R�s�[�����
	const CMemoData& operator=( const CMemoData& cMemoData);

	// �쐬���t
	void SetCreateDate( void);
	void GetCreateDate( int& nYear, int& nMonth, int& nDay) const;

	// �\���`��
	enum
	{
		WINDOW_TYPE_NORMAL,
		WINDOW_TYPE_TITLE,
		WINDOW_TYPE_ICON
	};
	int SetWindowType( int nType = WINDOW_TYPE_NORMAL);
	int GetWindowType( void) const;

	BOOL Show( BOOL blShow);
	BOOL IsShow( void) const;

	// �y�I�[�_
	enum
	{
		ZORDER_NORMAL,
		ZORDER_TOPMOST,
		ZORDER_BOTTOMMOST
	};
	int SetZOrder( int nZOrder = ZORDER_NORMAL);
	int GetZOrder( void) const;

	// �ʒu�T�C�Y�Œ�
	BOOL PosLock( BOOL blLock);
	BOOL IsPosLock( void) const;
	// �����̂�
	BOOL SeeThrough( BOOL SeeThrough);
	BOOL IsSeeThrough( void) const;

	CRect SetNormalRect( const CRect& cRctNormal);
	const CRect& GetNormalRect( void) const;
	CRect SetTitleRect( const CRect& cRctTitle);
	const CRect& GetTitleRect( void) const;
	CRect SetIconRect( const CRect& cRctIcon);
	const CRect& GetIconRect( void) const;

	// D&D�Ƃ�ClipBoard�p�̃f�[�^����
	int	CalcDDDataSize( void);
	int GetDDData( DDDATA* stDDData, int nSize);
	static CMemoData* CreateMemoFromBinData( BYTE* pabyData, int nSize);

	static const UINT GetClipboardFormat( void);

	// �V���A���C�Y����
	void Store( CArchive& ar);
	void Restore( CArchive& ar, UINT unVersion);

protected:	// ���\�b�h
	// �ۑ����ɁA����Ƃ����u�Í����v���h�L���{�����߂Ɏg�p
	void Encode( CString& cStrDst, const CString& cStrSrc);
	void Decode( CString& cStrDst, const CString& cStrSrc);
	static CString sm_cStrData;

	BOOL CopyMember( const CMemoData& cMemoData);

	// �ʒm���̓����ϊ�
	enum
	{
		GET_MONTH,
		GET_DAY
	};
	int ExchangeNotifyDay( const CTime& cTimeNow, const int nGetType) const;

	// ���o�[�W��������ǂݏo��
	void RestoreFromOldFormat( CArchive& ar, UINT unVersion);

protected:	// �����o�[�ϐ�
	CRect		m_cRctNormal;	// �ʏ�
	CRect		m_cRctTitle;	// �^�C�g��
	CRect		m_cRctIcon;		// �A�C�R���F�ʒu
	int			m_nWindowType;	// �`��

	BOOL		m_blShow;		// �\���^��\��
	int			m_nZOrder;		// �y�I�[�_
	BOOL		m_blPosLock;	// �ʒu�T�C�Y�Œ�
	BOOL		m_blSeeThrough;	// �V�[�X���[


	CString		m_cStrTitle;	// �^�C�g��
	CString		m_cStrMemo;		// �\�����e

	LOGFONT*	m_pstLogFont;	// �\���t�H���g
	COLORREF	m_clrForeColor;	// �����F
	COLORREF	m_clrBackColor;	// �w�i�F

	int			m_nIconIndex; // �A�C�R�����

	// �֘A�t��
	BOOL		m_blLink;			// �֘A�t���L������
	CString		m_cStrFile;			// �t�@�C��
	CString		m_cStrParameters;	// �p�����[�^
	BOOL		m_blTimerExec;		// �^�C�}�ŋN��

	BOOL		m_blPassWordLock;	// �p�X���[�h�̗L������
	CString		m_cStrPassWord;		// �p�X���[�h

	// �`��g��
	UINT		m_unDxlID;			// �`��g��ID
	UINT		m_unDxlExtDataSize;	// DXL�Ɋւ������ۑ��f�[�^�T�C�Y
	BYTE*		m_pabyDxlExtData;	// DXL�Ɋւ������ۑ��f�[�^

	// �^�C�}
	BOOL		m_blTimer;			// �^�C�}�ʒm�L��
	int			m_nTimerType;		// �^�C�}���
	int			m_nNotifyShift;		// �ʒm���̈ړ�����
	int			m_nMonth;			// �ʒm��
	int			m_nDay;				// �ʒm��
	int			m_nHour;			// �ʒm��
	int			m_nMin;				// �ʒm��
	int			m_nWeek;			// �ʒm�j��
	int			m_nBeforeNotify;	// ���O�ʒm
	BOOL		m_blAfterNotify;	// �o�ߒʒm
	CString		m_cStrNotifySound;	// �ʒm�T�E���h�t�@�C��

	union CREATEDATE
	{
		UINT		m_unCreateDate;	// �ۑ����̃A�N�Z�X�p
		struct			
		{
			UINT	m_btCreateDay:6;	// bit:0000000000000000-0000000000xxxxxx
			UINT	m_btCreateMonth:5;	// bit:0000000000000000-00000xxxxx000000
			UINT	m_btCreateYear:12;	// bit:000000000xxxxxxx-xxxxx00000000000
		};
	}	m_uniCreateDate;			// �쐬���t

	static UINT	m_unOboeChip;
};

#include "MemoData.inl"

#endif	//__MEMODATA_H__