#ifndef	_DDDATA_H__
#define	_DDDATA_H__

typedef	struct tagDDDATA
{
	int			m_nSize;

	char		m_szDataFmt[ 20];	// "OboeGakiChipData100\0"

	RECT		m_stRctNormal;
	RECT		m_stRctTitle;
	RECT		m_stRctIcon;
	int			m_nWindowType;

	BOOL		m_blShow;
	int			m_nZOrder;
	BOOL		m_blPosLock;	// �ʒu�T�C�Y�Œ�
	BOOL		m_blSeeThrough;	// �V�[�X���[

	int			m_nszTitle;
	int			m_nszMemo;

	int			m_nstLogFont;
	COLORREF	m_clrForeColor;
	COLORREF	m_clrBackColor;

	int			m_nIconIndex;


	BOOL		m_blLink;
	int			m_nszFile;
	int			m_nszParam;
	BOOL		m_blTimerExec;

	BOOL		m_blPassWordLock;
	int			m_nszPassWord;

	UINT		m_unDxlID;
	UINT		m_unDxlExtDataSize;
	int			m_nabyDxlExtData;

	BOOL		m_blTimer;			// �^�C�}�ʒm�L��
	int			m_nTimerType;			// �^�C�}���
	int			m_nNotifyShift;		// �ʒm���̈ړ�����
	int			m_nMonth;				// �ʒm��
	int			m_nDay;				// �ʒm��
	int			m_nHour;				// �ʒm��
	int			m_nMin;				// �ʒm��
	int			m_nWeek;				// �ʒm�j��
	int			m_nBeforeNotify;		// ���O�ʒm
	BOOL		m_blAfterNotify;		// �o�ߒʒm
	int			m_nszNotifySound;	// �ʒm�T�E���h�t�@�C��

	int			m_nCreateYear;
	int			m_nCreateMonth;
	int			m_nCreateDay;

	// ���̌��Ɋg���f�[�^������
}DDDATA;
#endif	// __DDDATA_H__
