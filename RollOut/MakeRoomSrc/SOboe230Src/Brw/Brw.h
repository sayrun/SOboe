// Brw.h : BRW �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_)
#define AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CBrwApp
// ���̃N���X�̓���̒�`�Ɋւ��Ă� Brw.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CBrwApp : public CWinApp
{
public:
	CBrwApp();

	BOOL GetDefaultFont( LOGFONT* pstLogFont);
	COLORREF GetDefaultForeColor( void);
	COLORREF GetDefaultBackColor( void);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CBrwApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBrwApp)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_BRW_H__4CEAD6CE_8BCF_11D1_9011_00804C15184E__INCLUDED_)
