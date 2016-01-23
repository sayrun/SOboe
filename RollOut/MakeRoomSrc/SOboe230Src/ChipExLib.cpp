#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "ChipExLIb.h"

CSemaphore CChipExLib::sm_cSmpDoCall;

BOOL CChipExLib::LoadLibrary()
{
	if( !m_cStrPath.IsEmpty())
	{
		m_hInstance = ::LoadLibrary( m_cStrPath);

		CHIPEXSPECIFICATION	lpChipExSpecification;
		// ���@�[�W�����̊m�F
		lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( m_hInstance, "ChipExSpecification");
		if( lpChipExSpecification)
		{
			lpChipExSpecification( &m_stSpecification);
			return TRUE;
		}
		::FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	return FALSE;
}

BOOL CChipExLib::FreeLibrary()
{
	if( m_hInstance)
	{
		::FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	return TRUE;
}

BOOL CChipExLib::ChipExProc( HWND hWnd, CMemoData& cMemoData, BOOL& blReSize, BOOL& blAutoReSize)
{
	BOOL	blResult = FALSE;

	if( m_hInstance)
	{
		if( ( UINT)_IF_DATA_VER <= m_stSpecification.unUpperStructVersion && ( UINT)_IF_DATA_VER >= m_stSpecification.unLowerStructVersion)
		{
			// ���\�[�X�`�F�b�N��DLL����A�ړI�̊֐��|�C���^���擾���܂��B
			CHIPEXPROC	lpChipExProc;
			lpChipExProc = ( CHIPEXPROC)GetProcAddress( m_hInstance, "ChipExProc");
			if( NULL != lpChipExProc)
			{
				IFDATA	stIFData;
				CreateIFData( stIFData, cMemoData);
				if( !lpChipExProc( hWnd, m_nIndex, &stIFData))
				{
					if( ModifyIFData( stIFData, cMemoData, blReSize, blAutoReSize))
					{
						blResult = TRUE;
					}
				}
				ReleaseIFData( stIFData);
			}
		}
	}
	return blResult;
}

BOOL CChipExLib::CreateIFData( IFDATA& stIFData, const CMemoData& cMemoData)
{
	CString	cStr;
	CString	cStrSlave;

	// ���̍\���̂̃T�C�Y
	stIFData.m_nSize		= sizeof( IFDATA);
	// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�
	stIFData.m_nVersion		= _IF_DATA_VER;

	// DLL���ł̕ύX�t���O
	stIFData.m_unModify		= 0;

	// ���Ђ̃^�C�g��
	cMemoData.GetTitle( cStr);
	lstrcpy( stIFData.m_achszMemoTitle, cStr);

	// ���Ђ̓��e
	DWORD	dwLength = cMemoData.GetMemoLength() + 1;
	stIFData.m_hachszMemoData = GlobalAlloc( GHND, dwLength);
	LPSTR lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszMemoData);
	lstrcpy( lpsz, cMemoData.GetMemo());
	GlobalUnlock( stIFData.m_hachszMemoData);

	// �e�L�X�g�F
	cMemoData.GetForeColor( stIFData.m_clrTextColor);
	// �w�i�F
	cMemoData.GetBackColor( stIFData.m_clrBackColor);

	// �t�H���g�X�^�C��
	cMemoData.GetFont( &stIFData.m_stLogFont);

	// �y�I�[�_�[
	stIFData.m_nZOder	= cMemoData.GetZOrder();

	// �p�X���[�h
	cStr = cMemoData.GetPassWord();
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszPassWord = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszPassWord);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszPassWord);
	
	// �֘A�t���g�p�̗L��
	stIFData.m_blShellExec = cMemoData.GetLinkData( cStr, cStrSlave);
	// �֘A�t���̃^�C�}�ł̋N��
	stIFData.m_blTimerExec = cMemoData.IsTimerExec();
	// �֘A�t���̃t�@�C������
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszFileName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszFileName);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszFileName);
	// �֘A�t���̃v���O��������
	dwLength = cStrSlave.GetLength() + 1;
	stIFData.m_hachszProgName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszProgName);
	lstrcpy( lpsz, cStrSlave);
	GlobalUnlock( stIFData.m_hachszProgName);

	// �^�C�}�g�p�̗L��
	stIFData.m_blTimerEnable	= cMemoData.IsTimer();
	// �^�C�}�̎�
	stIFData.m_nHour			= cMemoData.GetTimerHour();
	// �^�C�}�̕�
	stIFData.m_nMin				= cMemoData.GetTimerMin();
	// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
	stIFData.m_nTimerType		= cMemoData.GetTimerType();
	// �^�C�}�̌�
	stIFData.m_nMonth			= cMemoData.GetTimerMonth();
	// �^�C�}�̓�
	stIFData.m_nDay				= cMemoData.GetTimerDay();
	// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
	stIFData.m_nWeekDay			= cMemoData.GetWeek();
	// �ʒm�������j���������ꍇ�̃V�t�g
	stIFData.m_nNotifyShift		= cMemoData.GetNotifyShift();
	// �����O����̒ʒm
	stIFData.m_nBeforeNotify	= cMemoData.GetBeforeNotify();
	// �o�ߌ�̃^�C�}�[
	stIFData.m_blExpireTimer	= cMemoData.IsAfterNotify();
	// �T�E���h�t�@�C��
	cMemoData.GetNotifySound( cStr);
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszSoundFile = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszSoundFile);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszSoundFile);

	// �A�C�R�����
	stIFData.m_nIconIndex		= cMemoData.GetIconIndex();

	// ���ЃT�C�Y
	stIFData.m_stRctChip		= cMemoData.GetNormalRect();
	// �A�C�R���ʒu
	stIFData.m_stPntIcon		= cMemoData.GetIconRect().TopLeft();

	return FALSE;
}

BOOL CChipExLib::ReleaseIFData( IFDATA& stIFData)
{
	// ���Ђ̓��e
	GlobalFree( stIFData.m_hachszMemoData);
	// �֘A�t���̃t�@�C������
	GlobalFree( stIFData.m_hachszFileName);
	// �֘A�t���̃v���O��������
	GlobalFree( stIFData.m_hachszProgName);
	// �p�X���[�h
	GlobalFree( stIFData.m_hachszPassWord);
	// �T�E���h�t�@�C��
	GlobalFree( stIFData.m_hachszSoundFile);

	return FALSE;
}

BOOL CChipExLib::ModifyIFData( IFDATA& stIFData, CMemoData& cMemoData, BOOL& blResize, BOOL& blAutoResize)
{
	CString	cStr;
	CString	cStrSlave;
	BOOL	blResult = FALSE;
	LPCSTR	lpcsz;

	if( _MODIFY_MEMO & stIFData.m_unModify)
	{
		// ���Ђ̃^�C�g��
		cMemoData.GetTitle( cStr);
		if( cStr != stIFData.m_achszMemoTitle)
		{
			cMemoData.SetTitle( stIFData.m_achszMemoTitle);
			blResult = TRUE;
		}

		// ���Ђ̓��e
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszMemoData);
		if( lstrlen( lpcsz))
		{
			cMemoData.SetMemo( lpcsz);
			blResult = TRUE;
		}
		GlobalUnlock( stIFData.m_hachszMemoData);
	}
	if( _MODIFY_COLOR & stIFData.m_unModify)
	{
		// �����F
		cMemoData.SetForeColor( stIFData.m_clrTextColor);
		// �o�b�N�J���[
		cMemoData.SetBackColor( stIFData.m_clrBackColor);
		blResult = TRUE;
	}
	if( _MODIFY_FONT & stIFData.m_unModify)
	{
		// �t�H���g�X�^�C��
		cMemoData.SetFont( &stIFData.m_stLogFont);
		blResult = TRUE;
	}
	if( _MODIFY_PASSWORD & stIFData.m_unModify)
	{
		// ���Ђ̓��e
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszMemoData);
		if( 4 <= lstrlen( lpcsz))
		{
			cMemoData.SetPassWord( lpcsz);
			blResult = TRUE;
		}
		GlobalUnlock( stIFData.m_hachszMemoData);
	}
	if( _MODIFY_ZODER & stIFData.m_unModify)
	{
		// �y�I�[�_�[
		cMemoData.SetZOrder( stIFData.m_nZOder);
		blResult = TRUE;
	}

	if( _MODIFY_SHELL & stIFData.m_unModify)
	{
		// �֘A�t���g�p�̗L��
		cMemoData.Link( stIFData.m_blShellExec);
		// �֘A�t���̃^�C�}�ł̋N��
		cMemoData.TimerExec( stIFData.m_blTimerExec);
		// �֘A�t���̃t�@�C������
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszFileName);
		cStr = lpcsz;
		GlobalUnlock( stIFData.m_hachszFileName);
		// �֘A�t���̃v���O��������
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszProgName);
		cStrSlave = lpcsz;
		GlobalUnlock( stIFData.m_hachszProgName);
		cMemoData.SetLinkData( cStr, cStrSlave);
		blResult = TRUE;
	}

	if( _MODIFY_TIMER & stIFData.m_unModify)
	{
		// �^�C�}�g�p�̗L��
		cMemoData.Timer( stIFData.m_blTimerEnable);
		// �^�C�}�̎�
		cMemoData.SetTimerHour( stIFData.m_nHour);
		// �^�C�}�̕�
		cMemoData.SetTimerMin( stIFData.m_nMin);
		// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
		cMemoData.SetTimerType( stIFData.m_nTimerType);
		// �^�C�}�̌�
		cMemoData.SetTimerMonth( stIFData.m_nMonth);
		// �^�C�}�̓�
		cMemoData.SetTimerDay( stIFData.m_nDay);
		// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
		cMemoData.SetWeek( stIFData.m_nWeekDay);
		// �ʒm�������j���������ꍇ�̃V�t�g
		cMemoData.SetNotifyShift( stIFData.m_nNotifyShift);
		// �����O����̒ʒm
		cMemoData.SetBeforeNotify( stIFData.m_nBeforeNotify);
		// �o�ߌ�̃^�C�}�[
		cMemoData.AfterNotify( stIFData.m_blExpireTimer);
		// �T�E���h�t�@�C��
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszSoundFile);
		cStr = lpcsz;
		GlobalUnlock( stIFData.m_hachszSoundFile);
		cMemoData.SetNotifySound( cStr);

		blResult = TRUE;
	}

	if( _MODIFY_ICON & stIFData.m_unModify)
	{
		// �A�C�R�����
		cMemoData.SetIconIndex( stIFData.m_nIconIndex);
		blResult = TRUE;
	}

	if( _MODIFY_POINTICON & stIFData.m_unModify)
	{
		// �A�C�R���ʒu
		CSize cSize;
		cSize = cMemoData.GetIconRect().Size();

		CRect	cRect;
		cRect.left		= stIFData.m_stPntIcon.x;
		cRect.top		= stIFData.m_stPntIcon.y;
		cRect.right		= cRect.left + cSize.cx;
		cRect.bottom	= cRect.top + cSize.cy;
		cMemoData.SetIconRect( cRect);

		blResult = TRUE;
	}

	if( _MODIFY_RECTCHIP & stIFData.m_unModify)
	{
		// ���ЃT�C�Y
		cMemoData.SetNormalRect( stIFData.m_stRctChip);
		blResize = TRUE;
		blResult = TRUE;
	}
	else
	{
		if( _MODIFY_AUTORESIZE & stIFData.m_unModify)
		{
			blAutoResize = TRUE;
			blResult = TRUE;
		}
	}

	return blResult;
}
