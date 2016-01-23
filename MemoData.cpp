#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include <mbctype.h>

// ���h�M�Í����Ɏg�p����e�[�u��
CString CMemoData::sm_cStrData = _T( "!#$%&+=-[(<>)]*?");
UINT CMemoData::m_unOboeChip = RegisterClipboardFormat( "OboeGakiChipData100");

CMemoData::CMemoData() : CObject()
{
	// ��{�ݒ�
	{
		m_cStrTitle.Empty();			// �^�C�g��
		m_cStrMemo.Empty();				// �\�����e

		m_pstLogFont	= NULL;			// �\���t�H���g
		m_clrForeColor	= CLR_INVALID;	// �����F
		m_clrBackColor	= CLR_INVALID;	// �w�i�F

		m_nIconIndex	= -1;			// �A�C�R�����
	}

	// �g���ݒ�
	{
		m_blLink			= FALSE;	// �֘A�t���L������
		m_cStrFile.Empty();				// �t�@�C��
		m_cStrParameters.Empty();		// �p�����[�^
		m_blTimerExec		= FALSE;	// �^�C�}�ŋN��

		m_blPassWordLock	= FALSE;	// �p�X���[�h�̗L������
		m_cStrPassWord.Empty();			// �p�X���[�h

		// �`��g��
		m_unDxlID			= 0;		// Dxl-ID
		m_unDxlExtDataSize	= 0;		// Dxl �g���f�[�^�T�C�Y
		m_pabyDxlExtData	= NULL;		// Dxl �g���f�[�^
	}

	// �^�C�}
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

		m_blTimer		= FALSE;				// �^�C�}�ʒm�L��
		m_nTimerType	= TIMERTYPE_EVERYDAY;	// �^�C�}���
		m_nNotifyShift	= NOTIFYSHIFT_NOMOVE;	// �ʒm���̈ړ�����
		m_nMonth		= cTime.GetMonth();		// �ʒm��
		m_nDay			= cTime.GetDay();		// �ʒm��
		m_nHour			= 0;					// �ʒm��
		m_nMin			= 0;					// �ʒm��
		m_nWeek			= WEEK_DISABLE;			// �ʒm�j��

		m_nBeforeNotify	= 0;					// ���O�ʒm
		m_blAfterNotify	= FALSE;				// �o�ߒʒm

		m_cStrNotifySound.Empty();				// �ʒm�T�E���h�t�@�C��
	}

	// Window�֌W
	{
		m_cRctNormal.SetRectEmpty();
		m_cRctTitle.SetRectEmpty();
		m_cRctIcon.SetRectEmpty();

		m_nWindowType = WINDOW_TYPE_NORMAL;	// �`��
		m_blPosLock = FALSE;

		m_blShow = TRUE;
		m_nZOrder = ZORDER_NORMAL;

		m_blSeeThrough = FALSE;

		m_byAlphaBlend = ALPHABLEND_000;
	}

	// �쐬���t�L��
	SetCreateDate();

	// ���ʎ�M�p�h�c
	m_pcStrContextID = NULL;
}

CMemoData::~CMemoData()
{
	if( m_pstLogFont)
	{
		delete m_pstLogFont;
		m_pstLogFont = NULL;
	}
	if( m_pabyDxlExtData)
	{
		delete [] m_pabyDxlExtData;
		m_pabyDxlExtData = NULL;
	}
	if( m_pcStrContextID)
	{
		delete m_pcStrContextID;
		m_pcStrContextID = NULL;
	}
}

// �X�^�C���̏�����
BOOL CMemoData::SetDefaultStyle( void)
{
	// ���̂悤�Ȋ֐��͂Ȃ������X�e�L���Ƃ͎v�����A
	// �R���X�g���N�^�ŏ���������΁A�g�p���Ȃ��l�ɋL�q�ł���񂾂���ǂˁB
	// �ł��A���x���x�R���X�g���N�^�ł��̏���������邩�Ǝv���Ƃ˂��c�c�B

	CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe)
	{
		// �\���t�H���g
		if( NULL == m_pstLogFont)
		{
			m_pstLogFont = new LOGFONT;
			if( NULL == m_pstLogFont)return TRUE;
		}
		pcSOboe->GetDefaultFont( m_pstLogFont);

		m_clrForeColor = pcSOboe->GetDefaultForeColor();	// �����F
		m_clrBackColor = pcSOboe->GetDefaultBackColor();	// �w�i�F

		m_nIconIndex = pcSOboe->GetDefaultIconIndex();		// �A�C�R�����

		m_unDxlID = pcSOboe->GetDefaultDxlID(); //GetDefaultDxlID

		return FALSE;
	}
	return TRUE;
}

void CMemoData::RestoreFromOldFormat( CArchive& ar, UINT unVersion)
{
	int		nDummy;
	BOOL	blDummy;
	CString	cStrDummy;
	CRect	cRctDummy;

	BOOL blWork;
	ar >> blWork;
	m_nWindowType = ( FALSE != blWork) ? CMemoData::WINDOW_TYPE_ICON : CMemoData::WINDOW_TYPE_NORMAL;

	ar >> blWork;	// Dummy
	ar >> m_blShow;
	ar >> m_blPosLock;

	ar >> m_cStrTitle;			// �����̃^�C�g��
	ar >> m_cStrMemo;			// �����������e

	if( m_pstLogFont)
	{
		delete m_pstLogFont;
		m_pstLogFont = NULL;
	}
	m_pstLogFont = new LOGFONT;
	ar.Read( m_pstLogFont, sizeof( LOGFONT));	// Font���
	
	ar >> m_clrForeColor;				// �����F
	ar >> m_clrBackColor;				// �o�b�N�J���[
	ar >> nDummy;						// ���ŗL���ʂh�c

	// �^�C�}�[�ʒm�֌W
	ar >> m_blTimer;				// �^�C�}�[�g�p�ݒ�
	ar >> m_nTimerType;				// �^�C�}�[�̃^�C�v 0=���� 1-7=�j�� 8=����
	ar >> m_nHour;						// �ʒm����
	ar >> m_nMin;						// ����
	ar >> m_nMonth;					// �����^�C�v�̌�
	ar >> m_nDay;					// �����^�C�v�̓�

	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		BYTE	byWork;
		ar >> byWork;					// �j��:1.40
		m_nWeek = byWork;
		ar >> m_cStrNotifySound;		// �ʒm����:1.40
		ar >> m_blAfterNotify;			// �o�ߌ�̒ʒm:1.40
		ar >> m_nBeforeNotify;			// �O���ʒm:1.40
		ar >> m_nNotifyShift;			// �x���ړ�:1.40
	}
	else
	{
		if( 0 < m_nTimerType && 8 > m_nTimerType)
		{
			m_nWeek = ( BYTE)0x01 << ( m_nTimerType - 1);
			m_nTimerType = CMemoData::TIMERTYPE_WEEK;
		}
		else
		{
			if( 8 == m_nTimerType)m_nTimerType = CMemoData::TIMERTYPE_DAY;
		}
	}
	// Window��ԏ��
	ar >> m_nIconIndex;					// �\���A�C�R��
	ar >> blDummy;						// �\��Window ���
	ar >> m_cRctNormal;					// �ʏ펞��Window Size
	ar >> m_cRctIcon;					// �ŏ������̃T�C�Y
	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		ar >> blDummy;					// ���Ԃ�:1.40
		ar >> blDummy;					// �I�[�v�����p�X���[�h:1.40
	}
	// �p�X���[�h
	ar >> m_blPassWordLock;				// �p�X���[�h�ݒ�
	ar >> m_cStrPassWord;				// �p�X���[�h
	ar >> blDummy;						// �`�b�v�w���v
	// �y�I�[�_�[
	ar >> m_nZOrder;						// ��ɐ擪
	// �֘A�t���@�\�̃f�[�^
	ar >> m_cStrParameters;
	ar >> m_cStrFile;
	ar >> m_blTimerExec;
	ar >> m_blLink;

	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		int	nCreateDate;
		ar >> nCreateDate;					// �L�������̔N
		m_uniCreateDate.m_btCreateYear = nCreateDate;
		ar >> nCreateDate;					// �L�������̌�
		m_uniCreateDate.m_btCreateMonth = nCreateDate;
		ar >> nCreateDate;					// �L�������̓�
		m_uniCreateDate.m_btCreateDay = nCreateDate;
		ar >> nDummy;						// �폜����

		ar >> m_blSeeThrough;
		ar >> m_unDxlID;						// �`��g��Dxl ID 1.60�ȍ~
		ar >> cStrDummy;
		ar >> cRctDummy;
	}

	// 1.60 later
	if( _SCHEMA_ID_TYPE2 < unVersion)
	{
		ar >> m_unDxlExtDataSize;
		if( 0 < m_unDxlExtDataSize)
		{
			if( m_pabyDxlExtData)
			{
				delete [] m_pabyDxlExtData;
				m_pabyDxlExtData = NULL;
			}
			m_pabyDxlExtData = new BYTE [ m_unDxlExtDataSize];
			ar.Read( m_pabyDxlExtData, m_unDxlExtDataSize);
		}
		ar >> nDummy;
		ar >> nDummy;
		ar >> nDummy;
		ar >> nDummy;
	}
}

void CMemoData::Restore( CArchive& ar, UINT unVersion)
{
	CString	cStrWork;
	CString	cStrDummy;
	int		nDummy;

	try
	{
		if( ar.IsLoading())
		{
			if( _SCHEMA_ID_TYPE3 >= unVersion)
			{
				RestoreFromOldFormat( ar, unVersion);
				return;
			}
			// �ǂݏo����
			// Window�֘A
			{
				ar >> m_cRctNormal;
				ar >> m_cRctTitle;
				ar >> m_cRctIcon;
				ar >> m_nWindowType;
				ar >> m_blPosLock;
				ar >> m_blShow;
				ar >> m_nZOrder;

				ar >> m_blSeeThrough;
			}

			ar >> m_cStrTitle;			// �^�C�g��
			// �\�����e
			ar >> cStrWork;
			Decode( m_cStrMemo, cStrWork);

			// �\���X�^�C��
			// �\���t�H���g
			BOOL blFont;
			ar >> blFont;
			if( blFont)
			{
				if( NULL == m_pstLogFont)
				{
					m_pstLogFont = new LOGFONT;
				}
				ar.Read( m_pstLogFont, sizeof( LOGFONT));
			}
			ar >> m_clrForeColor;		// �����F
			ar >> m_clrBackColor;		// �w�i�F
			ar >> m_nIconIndex;			// �A�C�R�����

			// �֘A�t��
			ar >> m_blLink;				// �֘A�t���L������
			// �t�@�C��
			ar >> cStrWork;
			Decode( m_cStrFile, cStrWork);
			// �p�����[�^
			ar >> cStrWork;
			Decode( m_cStrParameters, cStrWork);
			ar >> m_blTimerExec;		// �^�C�}�ŋN��

			// �p�X���[�h�֘A
			ar >> m_blPassWordLock;		// �p�X���[�h�̗L������
			// �p�X���[�h
			ar >> cStrWork;
			Decode( m_cStrPassWord, cStrWork);

			// �`��g��
			ar >> m_unDxlID;			// �`��g��ID
			ar >> m_unDxlExtDataSize;	// DXL�Ɋւ������ۑ��f�[�^�T�C�Y
			// DXL�Ɋւ������ۑ��f�[�^
			if( 0 < m_unDxlExtDataSize)
			{
				if( m_pabyDxlExtData)
				{
					delete [] m_pabyDxlExtData;
					m_pabyDxlExtData = NULL;
				}
				m_pabyDxlExtData = new BYTE [ m_unDxlExtDataSize];
				ar.Read( m_pabyDxlExtData, m_unDxlExtDataSize);
			}

			// �^�C�}
			ar >> m_blTimer;			// �^�C�}�ʒm�L��
			ar >> m_nTimerType;			// �^�C�}���
			ar >> m_nNotifyShift;		// �ʒm���̈ړ�����
			ar >> m_nMonth;				// �ʒm��
			ar >> m_nDay;				// �ʒm��
			ar >> m_nHour;				// �ʒm��
			ar >> m_nMin;				// �ʒm��
			ar >> m_nWeek;				// �ʒm�j��
			ar >> m_nBeforeNotify;		// ���O�ʒm
			ar >> m_blAfterNotify;		// �o�ߒʒm
			ar >> m_cStrNotifySound;	// �ʒm�T�E���h�t�@�C��

			ar >> m_uniCreateDate.m_unCreateDate;		// �쐬���t

			// ���c�ȂǂŎg�p����h�c
			ar >> cStrDummy;
			if( !cStrDummy.IsEmpty())
			{
				SetContextID( cStrDummy);
			}
			// �ȒP�g���p�E�t�@�C���o�[�W������ύX���Ȃ��ł̊g��
			ar >> cStrDummy;
			ar >> cStrDummy;
			ar >> nDummy;
			// ���ߗ�
			m_byAlphaBlend = LOBYTE( LOWORD( nDummy));
			ar >> nDummy;
			ar >> nDummy;
			ar >> nDummy;
		}
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}

void CMemoData::Store( CArchive& ar )
{
	CString	cStrWork;
	CString	cStrDummy;
	int		nDummy;

	try
	{
		if( ar.IsStoring())
		{
			// �ۑ���
			// Window�֘A
			{
				ar << m_cRctNormal;
				ar << m_cRctTitle;
				ar << m_cRctIcon;
				ar << m_nWindowType;
				ar << m_blPosLock;
				ar << m_blShow;
				ar << m_nZOrder;

				ar << m_blSeeThrough;
			}

			ar << m_cStrTitle;			// �^�C�g��
			// �\�����e
			Encode( cStrWork, m_cStrMemo);
			ar << cStrWork;

			// �\���X�^�C��
			// �\���t�H���g
			if( m_pstLogFont)
			{
				ar << BOOL( TRUE);
				ar.Write( m_pstLogFont, sizeof( LOGFONT));
			}
			else
			{
				ar << BOOL( FALSE);
			}
			ar << m_clrForeColor;		// �����F
			ar << m_clrBackColor;		// �w�i�F
			ar << m_nIconIndex;			// �A�C�R�����

			// �֘A�t��
			ar << m_blLink;				// �֘A�t���L������
			// �t�@�C��
			Encode( cStrWork, m_cStrFile);
			ar << cStrWork;
			// �p�����[�^
			Encode( cStrWork, m_cStrParameters);
			ar << cStrWork;
			ar << m_blTimerExec;		// �^�C�}�ŋN��

			// �p�X���[�h�֘A
			ar << m_blPassWordLock;		// �p�X���[�h�̗L������
			// �p�X���[�h
			Encode( cStrWork, m_cStrPassWord);
			ar << cStrWork;

			// �`��g��
			ar << m_unDxlID;			// �`��g��ID
			ar << m_unDxlExtDataSize;	// DXL�Ɋւ������ۑ��f�[�^�T�C�Y
			// DXL�Ɋւ������ۑ��f�[�^
			if( 0 < m_unDxlExtDataSize)
			{
				if( m_pabyDxlExtData)
				{
					ar.Write( m_pabyDxlExtData, m_unDxlExtDataSize);
				}
			}

			// �^�C�}
			ar << m_blTimer;			// �^�C�}�ʒm�L��
			ar << m_nTimerType;			// �^�C�}���
			ar << m_nNotifyShift;		// �ʒm���̈ړ�����
			ar << m_nMonth;				// �ʒm��
			ar << m_nDay;				// �ʒm��
			ar << m_nHour;				// �ʒm��
			ar << m_nMin;				// �ʒm��
			ar << m_nWeek;				// �ʒm�j��
			ar << m_nBeforeNotify;		// ���O�ʒm
			ar << m_blAfterNotify;		// �o�ߒʒm
			ar << m_cStrNotifySound;	// �ʒm�T�E���h�t�@�C��

			ar << m_uniCreateDate.m_unCreateDate;	// �쐬���t

			cStrDummy.Empty();
			// ���c�ȂǂŎg�p����h�c�̋L�^
			if( NULL != m_pcStrContextID)
			{
				ar << *m_pcStrContextID;
			}
			else
			{
				ar << cStrDummy;
			}
			// �ȒP�g���p�E�t�@�C���o�[�W������ύX���Ȃ��ł̊g��
			ar << cStrDummy;
			ar << cStrDummy;
			nDummy = MAKELONG( MAKEWORD( m_byAlphaBlend, 0), 0);;
			ar << nDummy;
			nDummy = -1;
			ar << nDummy;
			nDummy = 0;
			ar << nDummy;
			ar << nDummy;
		}
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}

// �ۑ����ɁA����Ƃ����u�Í����v���h�L���{�����߂Ɏg�p
// �Í�����BASE16�Ƃł��������Ƃ��납�Ȃ��c�c
void CMemoData::Encode( CString& cStrDst, const CString& cStrSrc)
{
	// 1999/03/05 mas����ɂ�鍂�����̂��ߏC����Ăɂ��
	int	nLength = cStrSrc.GetLength();

	cStrDst.Empty();
	int		nData;
	char	*pszData = new char[ nLength * 2 + 1];
	ZeroMemory( pszData, nLength * 2 + 1);

	for( int nIndex = 0; nIndex < nLength; nIndex++)
	{
		nData = cStrSrc[ nIndex];
		pszData[ nIndex * 2]		= sm_cStrData[ nData & 0x0F];
		pszData[ nIndex * 2 + 1]	= sm_cStrData[ ( nData >> 4) & 0x0F];
	}

	cStrDst = pszData;
	delete [] pszData;
	/*
	���̕����ł̓A���P�[�V������Ƃ��ׂ�������\��������
	CString	cStr;
	int nLength = cStrSrc.GetLength();

	cStrDst.Empty();
	int nData;
	for( int nIndex = 0; nIndex < nLength; nIndex++)
	{
		nData = cStrSrc[ nIndex];
		cStrDst += sm_cStrData[ nData & 0x0F];
		cStrDst += sm_cStrData[ ( nData >> 4) & 0x0F];
	}
	*/
}

void CMemoData::Decode( CString& cStrDst, const CString& cStrSrc)
{
	cStrDst.Empty();
	int nLength = cStrSrc.GetLength();
	if( 0 < nLength)
	{
		char*	pszData;
		pszData = new char[ nLength];
		ZeroMemory( pszData, nLength);

		int		nData;
		char	chData;
		for( int nIndex = 0; nIndex < nLength; nIndex += 2)
		{
			nData = sm_cStrData.Find( cStrSrc[ nIndex]);
			chData = nData;
			nData = sm_cStrData.Find( cStrSrc[ nIndex + 1]);
			chData |= ( nData << 4);
			pszData[ nIndex / 2] = chData;
		}

		cStrDst = pszData;
		delete [] pszData;
	}
}

BOOL CMemoData::CopyMember( const CMemoData& cMemoData)
{
	// ��{�ݒ�
	{
		m_cStrTitle		= cMemoData.m_cStrTitle;	// �^�C�g��
		m_cStrMemo		= cMemoData.m_cStrMemo;		// �\�����e

		// �\���t�H���g
		if( NULL != cMemoData.m_pstLogFont)
		{
			if( NULL == m_pstLogFont)
			{
				m_pstLogFont = new LOGFONT;
			}
			*m_pstLogFont = *cMemoData.m_pstLogFont;
		}
		else
		{
			if( m_pstLogFont)
			{
				delete m_pstLogFont;
				m_pstLogFont = NULL;
			}
		}
		m_clrForeColor	= cMemoData.m_clrForeColor;	// �����F
		m_clrBackColor	= cMemoData.m_clrBackColor;	// �w�i�F
		m_nIconIndex	= cMemoData.m_nIconIndex;	// �A�C�R�����
	}

	// �g���ݒ�
	{
		m_blLink			= cMemoData.m_blLink;			// �֘A�t���L������
		m_cStrFile			= cMemoData.m_cStrFile;			// �t�@�C��
		m_cStrParameters	= cMemoData.m_cStrParameters;	// �p�����[�^
		m_blTimerExec		= cMemoData.m_blTimerExec;		// �^�C�}�ŋN��

		m_blPassWordLock	= cMemoData.m_blPassWordLock;	// �p�X���[�h�̗L������
		m_cStrPassWord		= cMemoData.m_cStrPassWord;		// �p�X���[�h

		// �`��g��
		m_unDxlID			= cMemoData.m_unDxlID;			// Dxl-ID
		// Dxl �g���f�[�^
		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		if( cMemoData.m_unDxlID)
		{
			m_unDxlExtDataSize	= cMemoData.m_unDxlExtDataSize;	// Dxl �g���f�[�^�T�C�Y
			if( cMemoData.m_unDxlExtDataSize)
			{
				m_pabyDxlExtData = new BYTE[ cMemoData.m_unDxlExtDataSize];
				CopyMemory( m_pabyDxlExtData, cMemoData.m_pabyDxlExtData, cMemoData.m_unDxlExtDataSize);
			}
		}
		else
		{
			m_unDxlExtDataSize = 0;
		}
	}

	// �^�C�}
	{
		m_blTimer		= cMemoData.m_blTimer;			// �^�C�}�ʒm�L��
		m_nTimerType	= cMemoData.m_nTimerType;		// �^�C�}���
		m_nNotifyShift	= cMemoData.m_nNotifyShift;		// �ʒm���̈ړ�����
		m_nMonth		= cMemoData.m_nMonth;			// �ʒm��
		m_nDay			= cMemoData.m_nDay;				// �ʒm��
		m_nHour			= cMemoData.m_nHour;			// �ʒm��
		m_nMin			= cMemoData.m_nMin;				// �ʒm��
		m_nWeek			= cMemoData.m_nWeek;			// �ʒm�j��

		m_nBeforeNotify	= cMemoData.m_nBeforeNotify;	// ���O�ʒm
		m_blAfterNotify	= cMemoData.m_blAfterNotify;	// �o�ߒʒm

		m_cStrNotifySound	= cMemoData.m_cStrNotifySound;	// �ʒm�T�E���h�t�@�C��
	}

	// Window�֘A
	{
		m_cRctNormal	= cMemoData.m_cRctNormal;
		m_cRctTitle		= cMemoData.m_cRctTitle;
		m_cRctIcon		= cMemoData.m_cRctIcon;
		m_nWindowType	= cMemoData.m_nWindowType;
		m_blPosLock		= cMemoData.m_blPosLock;
		m_blShow		= cMemoData.m_blShow;
		m_nZOrder		= cMemoData.m_nZOrder;

		m_blSeeThrough	= cMemoData.m_blSeeThrough;

		m_byAlphaBlend	= cMemoData.m_byAlphaBlend;
	}

	// �쐬���t�L��
	m_uniCreateDate.m_unCreateDate = cMemoData.m_uniCreateDate.m_unCreateDate;

	return TRUE;
}

void CMemoData::SetTitle( const CString& cStrTitle)
{
	// �^�C�g���͍Œ��U�R�Œ�ł�
	if( _MAX_TITLE_LEN >= cStrTitle.GetLength())
	{
		m_cStrTitle = cStrTitle;
	}
	else
	{
		char	szDummy[ _MAX_TITLE_LEN + 1]; 
		ZeroMemory( szDummy, _MAX_TITLE_LEN + 1);
		for( int nIndex = 0; nIndex < ( _MAX_TITLE_LEN + 1); nIndex++)
		{
			szDummy[ nIndex] = cStrTitle[ nIndex];
			if( _ismbblead( szDummy[ nIndex]))
			{
				if( _ismbbtrail( cStrTitle[ nIndex + 1]))
				{
					nIndex++;
					if( nIndex >= _MAX_TITLE_LEN)
					{
						szDummy[ _MAX_TITLE_LEN] = 0;
						break;
					}
					szDummy[ nIndex] = cStrTitle[ nIndex];
				}
			}
			else
			{
				if( 0 == szDummy[ nIndex])break;
			}
		}
		m_cStrTitle = szDummy;
	}
	m_cStrTitle.Replace( '\t', ' ');
	m_cStrTitle.TrimLeft();
	m_cStrTitle.TrimRight();
}

BOOL CMemoData::GetTitle( CString& cStrTitle) const
{
	// �^�C�g����������΁A�擪�s��Ԃ�
	if( m_cStrTitle.IsEmpty())
	{
		cStrTitle = m_cStrMemo;
		cStrTitle.TrimLeft();
		int nIndex = cStrTitle.Find( "\r\n");
		if( 0 < nIndex)cStrTitle = cStrTitle.Left( nIndex);
		if( _MAX_TITLE_LEN < cStrTitle.GetLength())
		{
			char	szDummy[ _MAX_TITLE_LEN + 1]; 
			ZeroMemory( szDummy, _MAX_TITLE_LEN + 1);
			for( int nIndex = 0; nIndex < ( _MAX_TITLE_LEN + 1); nIndex++)
			{
				szDummy[ nIndex] = cStrTitle[ nIndex];
				if( _ismbblead( szDummy[ nIndex]))
				{
					if( _ismbbtrail( cStrTitle[ nIndex + 1]))
					{
						nIndex++;
						if( nIndex >= _MAX_TITLE_LEN)
						{
							szDummy[ _MAX_TITLE_LEN] = 0;
							break;
						}
						szDummy[ nIndex] = cStrTitle[ nIndex];
					}
				}
				else
				{
					if( 0 == szDummy[ nIndex])break;
				}
			}
			cStrTitle = szDummy;
		}
		cStrTitle.Replace( '\t', ' ');

		/*
		if( nIndex >= 1 && cStrTitle.GetLength() >= nIndex)
		{
			char chChar = cStrTitle[ nIndex - 1];
			if( _ismbblead( chChar))
			{
				if( _ismbbtrail( cStrTitle[ nIndex]))
				{
					nIndex--;
				}
			}
		}
		CString	cStr;
		cStr = cStrTitle.Left( nIndex);
		cStrTitle = cStrTitle.Left( nIndex);
#if	_DEBUG
		int nLen = cStrTitle.GetLength();
		nLen = cStr.GetLength();
		char*	pcszData;
		pcszData = new char [ nLen + 10];
		lstrcpyn( pcszData, cStrTitle, nLen);
		delete [] pcszData;
#endif
		*/
		return FALSE;
	}
	// �^�C�g���͐ݒ肳��Ă���̂ŁA�����Ԃ��܂�
	cStrTitle = m_cStrTitle;
	return TRUE;
}

void CMemoData::SetFont( const LOGFONT* pstLogFont)
{
	if( NULL == pstLogFont)
	{
		m_pstLogFont = NULL;
	}
	else
	{
		if( NULL == m_pstLogFont)
		{
			// �t�H���g�̈悪������΁A�V�K�m��
			m_pstLogFont = new LOGFONT;
		}
		*m_pstLogFont = *pstLogFont;
	}
}

BOOL CMemoData::GetFont( LOGFONT* const pstLogFont) const
{
	if( NULL == m_pstLogFont)
	{
		// �t�H���g�������Ă��Ȃ��ꍇ�́A�f�t�H���g��Ԃ�
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			pcSOboe->GetDefaultFont( pstLogFont);
		}
		return FALSE;
	}
	*pstLogFont = *m_pstLogFont;
	return TRUE;
}

BOOL CMemoData::GetForeColor( COLORREF& clrFore) const
{
	if( CLR_INVALID == m_clrForeColor)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			clrFore = pcSOboe->GetDefaultForeColor();
		}
		return FALSE;
	}
	clrFore = m_clrForeColor;
	return TRUE;
}

BOOL CMemoData::GetBackColor( COLORREF& clrBack) const
{
	if( CLR_INVALID == m_clrBackColor)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			clrBack = pcSOboe->GetDefaultBackColor();
		}
		return FALSE;
	}
	clrBack = m_clrBackColor;
	return TRUE;
}

int CMemoData::GetIconIndex( void) const
{
	if( 0 > m_nIconIndex)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			return pcSOboe->GetDefaultIconIndex();
		}
	}/*
	if( _MAX_ICONINDEX < m_nIconIndex)
	{
		return _MAX_ICONINDEX + 1;
	}*/
	return m_nIconIndex;
}

BOOL CMemoData::GetLinkData( CString& cStrFile, CString& cStrParameters) const
{
	cStrFile = m_cStrFile;
	cStrParameters = m_cStrParameters;

	BOOL blResult = ( cStrFile.IsEmpty() && cStrParameters.IsEmpty()) ? FALSE : m_blLink;

	return blResult;
}

int CMemoData::SetTimerType( int nType)
{
	int	nResult;

	nResult = m_nTimerType;

	if( TIMERTYPE_EVERYDAY != nType && TIMERTYPE_WEEK != nType && TIMERTYPE_DAY != nType)
	{
		m_nTimerType = TIMERTYPE_EVERYDAY;
	}
	else
	{
		m_nTimerType = nType;
	}

	return nResult;
}

int CMemoData::SetNotifyShift( int nShift)
{
	int	nResult;

	nResult = m_nNotifyShift;

	if( NOTIFYSHIFT_NOMOVE != nShift &&	// �ړ�����
		NOTIFYSHIFT_TOSAT != nShift &&	// �y�j����
		NOTIFYSHIFT_TOFRI != nShift &&	// ���j����
		NOTIFYSHIFT_TOMON_FROMSATSUN != nShift &&	// ���j���� �y�j���^���j��
		NOTIFYSHIFT_TOMON_FROMSUN != nShift)	// ���j���� ���j��
	{
		m_nNotifyShift = NOTIFYSHIFT_NOMOVE;
	}
	else
	{
		m_nNotifyShift = nShift;
	}

	return nResult;
}

// �ʒm��
int CMemoData::SetTimerMonth( int nMonth)
{
	int nResult;

	nResult = m_nMonth;

	if( 0 > nMonth || 12 < nMonth)
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		m_nMonth = cTime.GetMonth();
	}
	else
	{
		m_nMonth = nMonth;
	}

	return nResult;
}

// �ʒm��
int CMemoData::SetTimerDay( int nDay)
{
	int nResult;

	nResult = m_nDay;

	if( 0 >= nDay || 31 < nDay)
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		m_nDay = cTime.GetDay();
	}
	else
	{
		m_nDay = nDay;
	}

	return nResult;
}

// �ʒm���̓����ϊ�
int CMemoData::ExchangeNotifyDay( const COleDateTime& cTimeNow, const int nGetType) const
{
	COleDateTime	cTimeCheck( cTimeNow.GetYear(), ( ( 0 != m_nMonth) ? m_nMonth : cTimeNow.GetMonth()), m_nDay, cTimeNow.GetHour(), cTimeNow.GetMinute(), cTimeNow.GetSecond());
	int nDayOfWeek = cTimeCheck.GetDayOfWeek();
	switch( m_nNotifyShift)
	{
	case NOTIFYSHIFT_NOMOVE:	// �ړ�����
		break;
	case NOTIFYSHIFT_TOSAT:	// �y�j���ֈړ�
		if( 1 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 1, 0, 0, 0);
		break;
	case NOTIFYSHIFT_TOFRI:	// ���j���ֈړ�
		if( 1 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 2, 0, 0, 0);
		if( 7 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 1, 0, 0, 0);
		break;
	case NOTIFYSHIFT_TOMON_FROMSATSUN: // ���j���ֈړ��F�y�^���j��
		if( 7 == nDayOfWeek)cTimeCheck += COleDateTimeSpan( 2, 0, 0, 0);
	case NOTIFYSHIFT_TOMON_FROMSUN:	// ���j���ֈړ��F���j��
		if( 1 == nDayOfWeek)cTimeCheck += COleDateTimeSpan( 1, 0, 0, 0);
		break;
	}

	int nResult;
	if( GET_MONTH == nGetType)
	{
		nResult = ( 0 == m_nMonth) ? 0 : cTimeCheck.GetMonth();
	}
	else
	{
		nResult = cTimeCheck.GetDay();
	}

	return nResult;
}

// �j���̒ʒm
int CMemoData::SetWeek( int nWeek)
{
	int	nResult;

	nResult = m_nWeek;

	if( WEEK_DISABLE == ( WEEK_ALL & nWeek))
	{
		m_nWeek = WEEK_DISABLE;
	}
	else
	{
		m_nWeek = nWeek;
	}

	return nWeek;
}

BOOL CMemoData::GetNotifySound( CString& cStrNotifySound) const
{
	BOOL	blResult = TRUE;

	cStrNotifySound = m_cStrNotifySound;

	WIN32_FIND_DATA	stFindData;
	HANDLE hHandle = FindFirstFile( cStrNotifySound, &stFindData);
	if( INVALID_HANDLE_VALUE != hHandle)
	{
		blResult = FALSE;
		FindClose( hHandle);
	}
	return blResult;
}

BOOL CMemoData::SetDxlExtData( const BYTE* pabyDxlExtData, const UINT unSize)
{
	try
	{
		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		m_unDxlExtDataSize = unSize;
		if( unSize)
		{
			m_pabyDxlExtData = new BYTE [ unSize];
			CopyMemory( m_pabyDxlExtData, pabyDxlExtData, unSize);
		}
		return TRUE;
	}
	catch( CMemoryException* e)
	{
		e->Delete();
		return FALSE;
	}
}

UINT CMemoData::GetDxlExtData( BYTE* pabyDxlExtData, UINT unSize) const
{
	if( NULL == pabyDxlExtData || 0 == unSize)return m_unDxlExtDataSize;

	UINT unCopySize = min( unSize, m_unDxlExtDataSize);
	if( 0 < unCopySize)
	{
		CopyMemory( pabyDxlExtData, m_pabyDxlExtData, unCopySize);
	}
	return unCopySize;
}

// �쐬���t
void CMemoData::SetCreateDate( void)
{
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	m_uniCreateDate.m_btCreateYear = cTime.GetYear();
	m_uniCreateDate.m_btCreateMonth = cTime.GetMonth();
	m_uniCreateDate.m_btCreateDay = cTime.GetDay();
}

void CMemoData::GetCreateDate( int& nYear, int& nMonth, int& nDay) const
{
	nYear = m_uniCreateDate.m_btCreateYear;
	nMonth = m_uniCreateDate.m_btCreateMonth;
	nDay = m_uniCreateDate.m_btCreateDay;
}

int CMemoData::SetWindowType( int nWindowType)
{
	int nResult;

	nResult = m_nWindowType;
	m_nWindowType = nWindowType;

	return nResult;
}

int CMemoData::SetZOrder( int nZOrder)
{
	int nResult;

	nResult = m_nZOrder;
	if( ZORDER_NORMAL != nZOrder && ZORDER_TOPMOST != nZOrder && ZORDER_BOTTOMMOST != nZOrder)
	{
		m_nZOrder = ZORDER_NORMAL;
	}
	else
	{
		m_nZOrder = nZOrder;
	}

	return nResult;
}

CRect CMemoData::SetNormalRect( const CRect& cRctNormal)
{
	CRect cRct;

	cRct = m_cRctNormal;
	m_cRctNormal = cRctNormal;

	return cRct;
}

CRect CMemoData::SetTitleRect( const CRect& cRctTitle)
{
	CRect cRct;

	cRct = m_cRctTitle;
	m_cRctTitle = cRctTitle;

	return cRct;
}

CRect CMemoData::SetIconRect( const CRect& cRctIcon)
{
	CRect cRct;

	cRct = m_cRctIcon;
	m_cRctIcon = cRctIcon;

	return cRct;
}

int	CMemoData::CalcDDDataSize( void)
{
	int	nDDDataSize;

	nDDDataSize = sizeof( DDDATA);

	nDDDataSize += m_cStrTitle.GetLength() + 1;

	nDDDataSize += m_cStrMemo.GetLength() + 1;

	if( m_pstLogFont)
	{
		nDDDataSize += sizeof( LOGFONT);
	}


	nDDDataSize += m_cStrFile.GetLength() + 1;
	nDDDataSize += m_cStrParameters.GetLength() + 1;

	nDDDataSize += m_cStrPassWord.GetLength() + 1;

	if( m_unDxlID)
	{
		nDDDataSize += m_unDxlExtDataSize;
	}

	nDDDataSize += m_cStrNotifySound.GetLength() + 1;

	return nDDDataSize;
}

int CMemoData::GetDDData( DDDATA* pstDDData, int nSize)
{
	int	nResult = CalcDDDataSize();
	if( nSize < nResult)return nResult;

	CString	cStr;
	int		nExtendDataPos = sizeof( DDDATA);
	char*	pWorkPointer = ( char*)pstDDData;

	pstDDData->m_nSize	= nSize;
	lstrcpy( pstDDData->m_szDataFmt, "OboeGakiChipData100");

	pstDDData->m_stRctNormal	= m_cRctNormal;
	pstDDData->m_stRctTitle		= m_cRctTitle;
	pstDDData->m_stRctIcon		= m_cRctIcon;
	pstDDData->m_nWindowType	= m_nWindowType;

	pstDDData->m_blShow			= m_blShow;
	pstDDData->m_nZOrder		= m_nZOrder;
	pstDDData->m_blPosLock		= m_blPosLock;
	pstDDData->m_blSeeThrough	= m_blSeeThrough;

	pstDDData->m_nszTitle = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrTitle);
	nExtendDataPos += m_cStrTitle.GetLength() + 1;

	pstDDData->m_nszMemo = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrMemo);
	nExtendDataPos += m_cStrMemo.GetLength() + 1;

	if( m_pstLogFont)
	{
		pstDDData->m_nstLogFont = nExtendDataPos;
		CopyMemory( &pWorkPointer[ nExtendDataPos], m_pstLogFont, sizeof( LOGFONT));
		nExtendDataPos += sizeof( LOGFONT);
	}
	else
	{
		pstDDData->m_nstLogFont = -1;
	}
	pstDDData->m_clrForeColor = m_clrForeColor;
	pstDDData->m_clrBackColor = m_clrBackColor;
	pstDDData->m_nIconIndex = m_nIconIndex;


	pstDDData->m_blLink = m_blLink;
	pstDDData->m_nszFile = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrFile);
	nExtendDataPos += m_cStrFile.GetLength() + 1;

	pstDDData->m_nszParam = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrParameters);
	nExtendDataPos += m_cStrParameters.GetLength() + 1;

	pstDDData->m_blTimerExec = m_blTimerExec;

	pstDDData->m_blPassWordLock = m_blPassWordLock;

	pstDDData->m_nszPassWord = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrPassWord);
	nExtendDataPos += m_cStrPassWord.GetLength() + 1;

	pstDDData->m_unDxlID = m_unDxlID;
	pstDDData->m_unDxlExtDataSize = m_unDxlExtDataSize;
	if( 0 < m_unDxlExtDataSize)
	{
		pstDDData->m_nabyDxlExtData = nExtendDataPos;
		CopyMemory( &pWorkPointer[ nExtendDataPos], m_pabyDxlExtData, m_unDxlExtDataSize);
		nExtendDataPos += pstDDData->m_unDxlExtDataSize;
	}
	else
	{
		pstDDData->m_nabyDxlExtData = -1;
	}


	pstDDData->m_blTimer		= m_blTimer;			// �^�C�}�ʒm�L��
	pstDDData->m_nTimerType		= m_nTimerType;			// �^�C�}���
	pstDDData->m_nNotifyShift	= m_nNotifyShift;		// �ʒm���̈ړ�����
	pstDDData->m_nMonth			= m_nMonth;				// �ʒm��
	pstDDData->m_nDay			= m_nDay;				// �ʒm��
	pstDDData->m_nHour			= m_nHour;				// �ʒm��
	pstDDData->m_nMin			= m_nMin;				// �ʒm��
	pstDDData->m_nWeek			= m_nWeek;				// �ʒm�j��
	pstDDData->m_nBeforeNotify	= m_nBeforeNotify;		// ���O�ʒm
	pstDDData->m_blAfterNotify	= m_blAfterNotify;		// �o�ߒʒm

	pstDDData->m_nszNotifySound = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrNotifySound);
	nExtendDataPos += m_cStrNotifySound.GetLength() + 1;

	pstDDData->m_nCreateYear	= m_uniCreateDate.m_btCreateYear;
	pstDDData->m_nCreateMonth	= m_uniCreateDate.m_btCreateMonth;
	pstDDData->m_nCreateDay		= m_uniCreateDate.m_btCreateDay;

	return nResult;
}

CMemoData* CMemoData::CreateMemoFromBinData( BYTE* pabyData, int nSize)
{
	DDDATA*	pstDDData = ( DDDATA*)pabyData;

	if( nSize < pstDDData->m_nSize)return NULL;
	if( lstrcmp( pstDDData->m_szDataFmt, "OboeGakiChipData100"))return NULL;
	if( 0 > pstDDData->m_nszMemo)return NULL;

	CMemoData*	pcMemoData;
	pcMemoData = new CMemoData;

	pcMemoData->m_cRctNormal	= pstDDData->m_stRctNormal;
	pcMemoData->m_cRctTitle		= pstDDData->m_stRctTitle;
	pcMemoData->m_cRctIcon		= pstDDData->m_stRctIcon;
	pcMemoData->m_nWindowType	= pstDDData->m_nWindowType;

	pcMemoData->m_blShow		= pstDDData->m_blShow;
	pcMemoData->m_nZOrder		= pstDDData->m_nZOrder;
	pcMemoData->m_blPosLock		= pstDDData->m_blPosLock;
	pcMemoData->m_blSeeThrough	= pstDDData->m_blSeeThrough;

	CString	cStr;
	if( 0 <= pstDDData->m_nszTitle)
	{
		pcMemoData->m_cStrTitle = ( LPCSTR)&pabyData[ pstDDData->m_nszTitle];
	}
	if( 0 > pstDDData->m_nszMemo)return NULL;
	pcMemoData->m_cStrMemo = ( LPCSTR)&pabyData[ pstDDData->m_nszMemo];


	if( 0 <= pstDDData->m_nstLogFont)
	{
		if( pcMemoData->m_pstLogFont)
		{
			delete pcMemoData->m_pstLogFont;
			pcMemoData->m_pstLogFont = NULL;
		}
		pcMemoData->m_pstLogFont = new LOGFONT;
		CopyMemory( pcMemoData->m_pstLogFont, &pabyData[ pstDDData->m_nstLogFont], sizeof( LOGFONT));
	}
	pcMemoData->m_clrForeColor	= pstDDData->m_clrForeColor;
	pcMemoData->m_clrBackColor	= pstDDData->m_clrBackColor;
	pcMemoData->m_nIconIndex	= pstDDData->m_nIconIndex;


	if( 0 <= pstDDData->m_nszFile)
	{
		pcMemoData->m_cStrFile = ( LPCSTR)&pabyData[ pstDDData->m_nszFile];
	}
	if( 0 <= pstDDData->m_nszParam)
	{
		pcMemoData->m_cStrParameters = ( LPCSTR)&pabyData[ pstDDData->m_nszParam];
	}
	pcMemoData->m_blLink		= ( pcMemoData->m_cStrFile.IsEmpty() && pcMemoData->m_cStrParameters.IsEmpty() && pstDDData->m_blLink) ? FALSE : TRUE;
	pcMemoData->m_blTimerExec	= pstDDData->m_blTimerExec;

	if( 0 <= pstDDData->m_nszPassWord)
	{
		pcMemoData->m_cStrPassWord = ( LPCSTR)&pabyData[ pstDDData->m_nszPassWord];
	}
	pcMemoData->m_blPassWordLock = pcMemoData->m_cStrPassWord.IsEmpty() ? FALSE : pstDDData->m_blPassWordLock;

	pcMemoData->m_unDxlID	= pstDDData->m_unDxlID;
	if( pstDDData->m_unDxlID)
	{
		LPBYTE lpbyData = ( 0 <= pstDDData->m_nabyDxlExtData && 0 < pstDDData->m_unDxlExtDataSize) ? &pabyData[ pstDDData->m_nabyDxlExtData] : NULL;
		pcMemoData->SetDxlExtData( lpbyData, pstDDData->m_unDxlExtDataSize);
	}

	pcMemoData->m_blTimer		= pstDDData->m_blTimer;			// �^�C�}�ʒm�L��
	pcMemoData->m_nTimerType	= pstDDData->m_nTimerType;			// �^�C�}���
	pcMemoData->m_nNotifyShift	= pstDDData->m_nNotifyShift;		// �ʒm���̈ړ�����
	pcMemoData->m_nMonth		= pstDDData->m_nMonth;				// �ʒm��
	pcMemoData->m_nDay			= pstDDData->m_nDay;				// �ʒm��
	pcMemoData->m_nHour			= pstDDData->m_nHour;				// �ʒm��
	pcMemoData->m_nMin			= pstDDData->m_nMin;				// �ʒm��
	pcMemoData->m_nWeek			= pstDDData->m_nWeek;				// �ʒm�j��
	pcMemoData->m_nBeforeNotify	= pstDDData->m_nBeforeNotify;		// ���O�ʒm
	pcMemoData->m_blAfterNotify	= pstDDData->m_blAfterNotify;		// �o�ߒʒm
	// �ʒm�T�E���h�t�@�C��
	if( 0 <= pstDDData->m_nszNotifySound)
	{
		pcMemoData->m_cStrNotifySound = ( LPCSTR)&pabyData[ pstDDData->m_nszNotifySound];
	}

//	cMemoData.SetCreateDate( pstDDData->m_nCreateYear, pstDDData->m_nCreateMonth, pstDDData->m_nCreateDay);
	pcMemoData->SetCreateDate();

	return pcMemoData;
}

void CMemoData::SetContextID( LPCSTR lpcszID)
{
	if( NULL == lpcszID)
	{
		if( m_pcStrContextID)
		{
			delete m_pcStrContextID;
			m_pcStrContextID = NULL;
		}
	}
	else
	{
		if( NULL == m_pcStrContextID)
		{
			m_pcStrContextID = new CString;
		}
		*m_pcStrContextID = lpcszID;
	}
}
