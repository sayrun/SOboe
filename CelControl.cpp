#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "CelControl.h"

CSemaphore CCelControl::sm_cSmpDoCel;

// �g�p���H
BOOL CCelControl::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoCel);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

CCelControl::CCelControl( void)
{
	m_blValid = FALSE;
	m_cStrSeparator.Empty();
	for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
	{
		m_acStrMenu[ nIndex].Empty();
		m_acStrPath[ nIndex].Empty();
		m_cStrSeparator += '0';
	}
}

BOOL CCelControl::SetSelData( int nIndex, LPCSTR lpcszPath, LPCSTR lpcszMenu)
{
	if( _CEL_MAX <= nIndex && 0 > nIndex)return FALSE;
	if( 0 >= lstrlen( lpcszPath))return FALSE;

	BOOL	blResult = FALSE;

	OFSTRUCT	of;
	if( HFILE_ERROR != OpenFile( lpcszPath, &of, OF_READ | OF_EXIST))
	{
		HINSTANCE hInst = LoadLibrary( of.szPathName);
		if( NULL != hInst)
		{
			// ���@�[�W�����̊m�F
			CHIPEXSPECIFICATION	lpChipExSpecification;
			lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( hInst, "ChipExSpecification");
			if( lpChipExSpecification)
			{
				SPECIFICATION	stSpecification;
				lpChipExSpecification( &stSpecification);
				if( ( UINT)_IF_DATA_VER <= stSpecification.unUpperStructVersion && ( UINT)_IF_DATA_VER >= stSpecification.unLowerStructVersion)
				{
					m_acStrPath[ nIndex] = of.szPathName;
					m_acStrMenu[ nIndex] = stSpecification.szMenuString;
					if( NULL != lpcszMenu && 0 < lstrlen( lpcszMenu))
					{
						m_acStrMenu[ nIndex] = lpcszMenu;
					}
					else
					{
						m_acStrMenu[ nIndex] = stSpecification.szMenuString;
					}
					blResult = TRUE;
					m_blValid = TRUE;
				}
			}
			FreeLibrary( hInst);
		}
	}
	return blResult;
}

BOOL CCelControl::SetMenuSeparator( LPCSTR lpcsz)
{
	if( NULL == lpcsz)return FALSE;
	if( _CEL_MAX > lstrlen( lpcsz))return FALSE;

	m_cStrSeparator = lpcsz;

	return TRUE;
}

HMENU CCelControl::GetMenu( void) const
{
	if( FALSE == m_blValid)return NULL;

	HMENU	hMenu = CreateMenu();
	if( hMenu)
	{
		for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
		{
			if( !m_acStrMenu[ nIndex].IsEmpty())
			{
				if( '0' != m_cStrSeparator[ nIndex])
				{
					AppendMenu( hMenu, MF_SEPARATOR, 0, NULL);
				}
				AppendMenu( hMenu, MF_STRING, _CEL_MENUTOP + nIndex, m_acStrMenu[ nIndex]);
			}
		}
	}
	return hMenu;
}

DWORD CCelControl::DoCel( HWND hWnd, UINT unMenuID, CMemoData& cMemoData) const
{
	DWORD dwResult = _CELCTL_NOP;
	CSingleLock	cSlk( &sm_cSmpDoCel);
	if( cSlk.Lock( 0))
	{
		if( _CEL_MENUTOP <= unMenuID && _CEL_MENUBOTTOM >= unMenuID)
		{
			int	nIndex = unMenuID - _CEL_MENUTOP;
			if( !m_acStrPath[ nIndex].IsEmpty())
			{
				HINSTANCE		hInst = NULL;
				hInst = LoadLibrary( m_acStrPath[ nIndex]);
				if( hInst)
				{
					// ���\�[�X�`�F�b�N��DLL����A�ړI�̊֐��|�C���^���擾���܂��B
					CHIPEXPROC	lpChipExProc;
					lpChipExProc = ( CHIPEXPROC)GetProcAddress( hInst, "ChipExProc");
					if( NULL != lpChipExProc)
					{
						IFDATA	stIfData;
						MakeIFData( stIfData, cMemoData);
						if( !lpChipExProc( hWnd, nIndex, &stIfData))
						{
							dwResult = ModifyIFData( cMemoData, stIfData);
						}
						DeleteIFData( stIfData);
					}
					FreeLibrary( hInst);
				}
				else
				{
					dwResult |= _CELCTL_ERROR;
				}
			}
			else
			{
				dwResult |= _CELCTL_ERROR;
			}
		}
		else
		{
			dwResult |= _CELCTL_ERROR;
		}
	}
	return dwResult;
}

BOOL CCelControl::MakeIFData( IFDATA& stIfData, CMemoData& cMemoData) const
{
	// ���̍\���̂̃T�C�Y
	stIfData.m_nSize		= sizeof( IFDATA);
	// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�
	stIfData.m_nVersion		= _IF_DATA_VER;

	// DLL���ł̕ύX�t���O
	stIfData.m_unModify		= 0;

	// ���Ђ̃^�C�g��
	CString	cStr;
	cMemoData.GetTitle( cStr);
	lstrcpy( stIfData.m_achszMemoTitle, cStr);

	// ���Ђ̓��e
	DWORD	dwLength = cMemoData.GetMemoLength() + 1;
	stIfData.m_hachszMemoData = GlobalAlloc( GHND, dwLength);
	LPSTR lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszMemoData);
	lstrcpy( lpsz, cMemoData.GetMemo());
	GlobalUnlock( stIfData.m_hachszMemoData);

	// �e�L�X�g�F
	cMemoData.GetForeColor( stIfData.m_clrTextColor);
	// �w�i�F
	cMemoData.GetBackColor( stIfData.m_clrBackColor);

	// �t�H���g�X�^�C��
	cMemoData.GetFont( &stIfData.m_stLogFont);

	// �y�I�[�_�[
	stIfData.m_nZOder	= cMemoData.GetZOrder();

	// �p�X���[�h
	dwLength = cMemoData.GetPassWord().GetLength() + 1;
	stIfData.m_hachszPassWord = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszPassWord);
	lstrcpy( lpsz, cMemoData.GetPassWord());
	GlobalUnlock( stIfData.m_hachszPassWord);
	
	// �֘A�t���g�p�̗L��
	stIfData.m_blShellExec = cMemoData.IsLink();
	// �֘A�t���̃^�C�}�ł̋N��
	stIfData.m_blTimerExec = cMemoData.IsTimerExec();
	// �֘A�t���̃t�@�C������
	CString	cStrParam;
	cMemoData.GetLinkData( cStr, cStrParam);
	dwLength = cStr.GetLength() + 1;
	stIfData.m_hachszFileName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszFileName);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIfData.m_hachszFileName);
	// �֘A�t���̃v���O��������
	dwLength = cStrParam.GetLength() + 1;
	stIfData.m_hachszProgName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszProgName);
	lstrcpy( lpsz, cStrParam);
	GlobalUnlock( stIfData.m_hachszProgName);

	// �^�C�}�g�p�̗L��
	stIfData.m_blTimerEnable	= cMemoData.IsTimer();
	// �^�C�}�̎�
	stIfData.m_nHour			= cMemoData.GetTimerHour();
	// �^�C�}�̕�
	stIfData.m_nMin				= cMemoData.GetTimerMin();
	// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
	stIfData.m_nTimerType		= cMemoData.GetTimerType();
	// �^�C�}�̌�
	stIfData.m_nMonth			= cMemoData.GetTimerMonth();
	// �^�C�}�̓�
	stIfData.m_nDay				= cMemoData.GetTimerDay();
	// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
	stIfData.m_nWeekDay			= cMemoData.GetWeek();
	// �ʒm�������j���������ꍇ�̃V�t�g
	stIfData.m_nNotifyShift		= cMemoData.GetNotifyShift();
	// �����O����̒ʒm
	stIfData.m_nBeforeNotify	= cMemoData.GetBeforeNotify();
	// �o�ߌ�̃^�C�}�[
	stIfData.m_blExpireTimer	= cMemoData.IsAfterNotify();
	// �T�E���h�t�@�C��
	cMemoData.GetNotifySound( cStr);
	dwLength = cStr.GetLength() + 1;
	stIfData.m_hachszSoundFile = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszSoundFile);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIfData.m_hachszSoundFile);

	// �A�C�R�����
	stIfData.m_nIconIndex		= cMemoData.GetIconIndex();

	// ���ЃT�C�Y
	CRect	cRect = cMemoData.GetNormalRect();
	stIfData.m_stRctChip.left	= cRect.left;
	stIfData.m_stRctChip.top	= cRect.top;
	stIfData.m_stRctChip.right	= cRect.right;
	stIfData.m_stRctChip.bottom	= cRect.bottom;
	// �A�C�R���ʒu
	cRect = cMemoData.GetIconRect();
	stIfData.m_stPntIcon.x		= cRect.left;
	stIfData.m_stPntIcon.y		= cRect.top;

	return TRUE;
}

BOOL CCelControl::DeleteIFData( IFDATA& stIfData) const
{
	// ���Ђ̓��e
	GlobalFree( stIfData.m_hachszMemoData);
	// �֘A�t���̃t�@�C������
	GlobalFree( stIfData.m_hachszFileName);
	// �֘A�t���̃v���O��������
	GlobalFree( stIfData.m_hachszProgName);
	// �p�X���[�h
	GlobalFree( stIfData.m_hachszPassWord);
	// �T�E���h�t�@�C��
	GlobalFree( stIfData.m_hachszSoundFile);

	return TRUE;
}

DWORD CCelControl::ModifyIFData( CMemoData& cMemoData, IFDATA& stIfData) const
{
	DWORD dwResult = _CELCTL_REDRAW;
	LPCSTR	lpcsz;

	if( _MODIFY_MEMO & stIfData.m_unModify)
	{
		// ���Ђ̃^�C�g��
		CString	cStrTitle;
		cMemoData.GetTitle( cStrTitle);
		if( lstrcmp( stIfData.m_achszMemoTitle, cStrTitle))
		{
			cMemoData.SetTitle( cStrTitle);
		}

		// ���Ђ̓��e
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszMemoData);
		if( lstrlen( lpcsz))
		{
			cMemoData.SetMemo( lpcsz);
		}
		GlobalUnlock( stIfData.m_hachszMemoData);
	}
	if( _MODIFY_COLOR & stIfData.m_unModify)
	{
		// �����F
		cMemoData.SetForeColor( stIfData.m_clrTextColor);
		// �o�b�N�J���[
		cMemoData.SetBackColor( stIfData.m_clrBackColor);
	}
	if( _MODIFY_FONT & stIfData.m_unModify)
	{
		// �t�H���g�X�^�C��
		cMemoData.SetFont( &stIfData.m_stLogFont);
	}
	if( _MODIFY_PASSWORD & stIfData.m_unModify)
	{
		// ���Ђ̓��e
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszMemoData);
		if( 4 <= lstrlen( lpcsz))
		{
			cMemoData.SetPassWord( lpcsz);
		}
		GlobalUnlock( stIfData.m_hachszMemoData);
	}
	if( _MODIFY_ZODER & stIfData.m_unModify)
	{
		// �y�I�[�_�[
		cMemoData.SetZOrder( stIfData.m_nZOder);
	}

	if( _MODIFY_SHELL & stIfData.m_unModify)
	{
		// �֘A�t���g�p�̗L��
		cMemoData.Link( stIfData.m_blShellExec);
		// �֘A�t���̃^�C�}�ł̋N��
		cMemoData.TimerExec( stIfData.m_blTimerExec);
		// �֘A�t���̃t�@�C������
		LPCSTR lpcszFileName = ( LPCSTR)GlobalLock( stIfData.m_hachszFileName);
		// �֘A�t���̃v���O��������
		LPCSTR lpcszProgName = ( LPCSTR)GlobalLock( stIfData.m_hachszProgName);
		cMemoData.SetLinkData( lpcszProgName, lpcszFileName);
		GlobalUnlock( stIfData.m_hachszProgName);
		GlobalUnlock( stIfData.m_hachszFileName);
	}

	if( _MODIFY_TIMER & stIfData.m_unModify)
	{
		// �^�C�}�g�p�̗L��
		cMemoData.Timer( stIfData.m_blTimerEnable);
		// �^�C�}�̎�
		cMemoData.SetTimerHour( stIfData.m_nHour);
		// �^�C�}�̕�
		cMemoData.SetTimerMin( stIfData.m_nMin);
		// �^�C�}�^�C�v 0/1/2 = ����/�j��/����
		cMemoData.SetTimerType( stIfData.m_nTimerType);
		// �^�C�}�̌�
		cMemoData.SetTimerMonth( stIfData.m_nMonth);
		// �^�C�}�̓�
		cMemoData.SetTimerDay( stIfData.m_nDay);
		// �j����ʃ^�C�}�̗j���w�� 0bit=���j�� 1bit���j��...
		cMemoData.SetWeek( stIfData.m_nWeekDay);
		// �ʒm�������j���������ꍇ�̃V�t�g
		cMemoData.SetNotifyShift( stIfData.m_nNotifyShift);
		// �����O����̒ʒm
		cMemoData.SetBeforeNotify( stIfData.m_nBeforeNotify);
		// �o�ߌ�̃^�C�}�[
		cMemoData.AfterNotify( stIfData.m_blExpireTimer);
		// �T�E���h�t�@�C��
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszSoundFile);
		cMemoData.SetNotifySound( lpcsz);
		GlobalUnlock( stIfData.m_hachszSoundFile);
	}

	if( _MODIFY_ICON & stIfData.m_unModify)
	{
		// �A�C�R�����
		cMemoData.SetIconIndex( stIfData.m_nIconIndex);
	}

	if( _MODIFY_POINTICON & stIfData.m_unModify)
	{
		// �A�C�R���ʒu
		CRect	cRctIcon;
		cRctIcon.left	= stIfData.m_stPntIcon.x;
		cRctIcon.top	= stIfData.m_stPntIcon.y;
		cRctIcon.right	= cRctIcon.left + _ICON_WIDTH;
		cRctIcon.bottom	= cRctIcon.top + _ICON_HEIGHT;

		cMemoData.SetIconRect( cRctIcon);
	}

	if( _MODIFY_RECTCHIP & stIfData.m_unModify)
	{
		// ���ЃT�C�Y
		CRect	cRctNormal	= stIfData.m_stRctChip;
		cMemoData.SetNormalRect( cRctNormal);
		dwResult = _CELCTL_MOVE;
	}
	else
	{
		if( _MODIFY_AUTORESIZE & stIfData.m_unModify)
		{
			dwResult = _CELCTL_RESIZE;
		}
	}
	return dwResult;
}
