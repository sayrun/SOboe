// Brw.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "Brw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	����!
//
//		���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//		MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//		�ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE 
//		�}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//		}
//
//		���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//		�ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//		����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//		��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC 
//		DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//		�W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//		�ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//		58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CBrwApp

BEGIN_MESSAGE_MAP(CBrwApp, CWinApp)
	//{{AFX_MSG_MAP(CBrwApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrwApp �̍\�z

CBrwApp::CBrwApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CBrwApp �I�u�W�F�N�g

CBrwApp theApp;

BOOL CBrwApp::GetDefaultFont( LOGFONT* pstLogFont)
{
	LOGFONT		stLogFont = { -19, 0, 0, 0, 400, FALSE, FALSE, FALSE, 128, 3, 2, 1, 50, "�l�r �o�S�V�b�N"};
	CopyMemory( pstLogFont, &stLogFont, sizeof( LOGFONT));

	return 0;
}

COLORREF CBrwApp::GetDefaultForeColor( void)
{
	return GetSysColor( COLOR_WINDOWTEXT);
}

COLORREF CBrwApp::GetDefaultBackColor( void)
{
	return GetSysColor( COLOR_WINDOW);
}
