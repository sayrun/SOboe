#include "StdeMail.h"
#include "NetEx_IF.h"
#include "email.h"
#include "emailslv.h"
#include "EntryList.h"
#include "GroupList.h"
#include "OrgSock.h"
#include "Base64.h"
#include "RStream.hpp"
#include "Pop3Client.h"
#include "Mail.h"
#include "MailS.h"
#include "KanjiUtl.h"
#include "resource.h"
#include "Dialogs.h"

extern HANDLE			g_hModule;
extern EMAILORGDATA		g_stOrgData;

/* �̌��łɎg���Ă��������ł����B���������Ȃ�����A�������ɂ������Ƃ��v�����́B
#define	_MAX_GUCHIKAZU	71
static char* pszMudaguchi[ _MAX_GUCHIKAZU] =
{
	"(-_-)�͂��`�A�Ȃ�ł�����Ȃ̍���Ă邵���B",
	"(T-T)�ǂ������������{�N�B���ăJ���W���Ȃ��B",
	"(+.+)�A�h���i�������s�����Ă܂����B",
	"(+.-)�x�[�^�G���h���t�B���c�c�������B",
	"(-.-)�����ʂƁA���E T.Chiba(27)���ď����ꂿ�Ⴄ�́B",
	"(@_@)�������ƕی����؂�������B",
	"(-_-)���̑O�́A�v���ƌ���ꂿ������B",
	"(^-^)ray/ark�̓X�e�L�ł��B",
	"(-_-)���C�����肽���l�����܂��B97�N���ȍ~���[���ƍl���Ă��܂��B",
	"(@_@)���o�C���炢���~�ɂȂ��Ă��܂��B",
	"(~-~)���ꂩ��͂ǂ��Ȃ�̂��Ȃ��B�{�N�͒m��܂���B",
	"(-_-)���|�̑剤�͂ǂ������́H",
	"(--; ���o���c�c�A���ł��Ȃ����ł��Ȃ��B�C�ɂ��Ȃ��ŁB",
	"(^-^)���� 壂���͂��킢���ł��B",
	"(^-^)( ^-)(  ^)(   )(^  )(-^ )(^-^)������������",
	"(-_-)�ޏ������̎���A�C�^�C�ł����A�����[���h���}�ł��B����I",
	"(^-^)�ŋ߁AWinCE Toolkit for VC++6.0�𒍕����܂����B",
	"(+_+)WinCE�@�͎����ĂȂ��ł��B",
	"(-�-)y-��ߎ��X�ނ���[�Ƀ^�o�R���z�������C�����܂��B",
	"(-_-)�N���̓R���C�ł��B1mm���炢�̂����ł��B",
	"(;_;)���N�̐��̐^�ē��L�^�͘A��14���ŏI���܂����B",
	"(*_*)�����̖������E�̓X�e�L���Ǝv���̂ł����c�c�ア�ł����H",
	"(�_�)�Z�����������H�������������H",
	"(-_-)�����A����ς�����B�S�����A�C�ɂ��Ȃ��ŁB",
	"(^-^)���̃y�[�W��\'Profile\'���ŏ��Ɍ����Ⴄ�́B",
	"(-_-)�H���������Ă��C�C����A�{�C�W���[�ɏ�肽�������B",
	"(^-^)Fe����̊G�͂ƂĂ��ƂĂ��D���ł��B",
	"(-_-)�v���Ԃ��\"���������ȓ�\"�ǂ݂܂����B����ϐ����ł��B",
	"(^-^)��������͗����������Ǝv���B",
	"(T-T)�����̍˔\�̖����ɕ����Ƃ�������B",
	"(///)�ˑR�̂̎��s���v���o���Ēp����������B�v���o�����ȁB",
	"(^-^)TAP��TV-CF�u������񌾂��Ȃ����悧�v�͉����J���W�ł�",
	"(^-^)�������N�R�i���̍ĕ��������܂����B�ʔ��������ł��B",
	"(-_-)���H�Ȃ񂩁A�����Ȃ��H",
	"(^-^)��\'�y�P���b�p\'�Ɋ����������̂��B",
	"(-.-)Win3.0�̍�BPlay���h�L����������Ƃ�����B",
	"(-_-)�`�����I���Γ��킪�n�܂�c�c�̂ё��̉F���J��j�B",
	"(--#)��J���Ď���ɉ𓚂��Ă����Ƃ͗����I�c�c���ɂȂ�܂��B",
	"(-_-)���Ă�[�������A�ɁH�V�эs���Ȃ��H",
	"(-_-)y-��߂ӂ��A�C�t���΃A���^�A�����Q�V��B�r�r������H",
	"(-.-)�Y�������c�c�{�N�����Ԃɓ���Ă���悧�B",
	"(^-^)�X���Q�Q���̃o�C�I�R�A�܂����h�o�V�ɒ�������ԁH",
	"(^-^)�R�R�]�v�����āH�m���Ă�m���Ă�����Ă�B",
	"(-_-)�Ă��I����B",
	"(T-T)��@�R�s�[�c�c�\�t�g�ɑ΂��鉿�l�ƕ����̔ے�B",
	"(?_?)���������΁A�A���X���[����L���Ăǂ��Ȃ���������H",
	"(._.)�����A�A���I�ق���A���̕��B�c�c�S�����A�J�[�\���������B",
	"(+_+)��d�Ő܊p�������R�[�h�����X�g���܂����B",
	"(^-^)�������傤�ԁA�R�R�͖{��������Ȃ����炳�B",
	"(-.-)���������΂��c�c����ς�����B",
	"(T�T)�ђ���f���������āA�����̔ߖŋN�������Ƃ�����B",
	"(~-~)�����ƁA�w���w���ȋC�����ɂȂ�B",
	"(-_-)����͊��̓암�S��ɂ�����˂��B",
	"(-_-)���l�ς������̊O�ɒu���Ă���Ƃ�����A�c�O�Ȃ��Ƃ�",
	"(>_<)����������Ă�����A�q���̍��̉������v���o�������������B",
	"(x_x)�m�荇���̓��L�͌������Ȃ������B",
	"(--;)ren *.doc *.txt���ł��Ȃ�����Ƃ�����GUI���̓i���Z���X���B\r\n��R�ו����ςߕ֗������āA�ׂ̉ƂɃg���b�N�ł͍s���Ȃ��B",
	"(^-^)BS��\'�ւ���ꂽ�V��\'�����܂����B�C�ɂ��Ă��܂��܂����B",
	"(T-T)\"����D�����r�d�w�D��\"���ĔF���͉��Ƃ����ė~�����B",
	"(x_x)���Â肪�����B���x��",
	"(^-^)��Îᏼ���D���I",
	"(x_x)�e�m�炸�𔲂��Ƃ��A����\'�j�Ӊ�\'�ŋC�����������������B",
	"(-.-)���߂�Win3.0��OS/2�������Ƃ��A���̈��萫����OS/2�����Ǝv���Ă����B",
	"(~-~)NT��OS�Ƃ��Ẵf�U�C����4.0���3.X�̕����C�C�Ǝv���B",
	"(--;)\'���낰�`\'���ċ����̓T�C�A�N���Ǝv���B",
	"(^^;)�E�B���_���A���݂ĕs�o�ɂ����������ɂȂ����B",
	"(-_-)���������΁A�p�g���C�o�[�̌���łR�͂ǂ��Ȃ����H",
	"(x_x)�����@���������Ⴂ�܂����B",
	"(^-^)���߂�baby one more time�̃v�����������Ƃ��A�����h�L�h�L�����B",
	"(--;)���̓Ƃ茾�͌��\�Ȏ�ނ�����B",
	"(^-^)�}�E�X��Logitech��MouseMan+�Ɍ���I",
};
*/

static char* pszDispatchMail = "DispatchMail";

BOOL DispatchMail( CMail& cMail)
{
	if( g_stOrgData.blLogging)OutputErrorString( "Entry", pszDispatchMail);

	BOOL	blResult = FALSE;

	SENDDATA*	pstSendData;
	int			nDataSize = cMail.GetRawDataSize();

	if( 0 < nDataSize)
	{
		pstSendData = ( SENDDATA*)malloc( nDataSize);
		if( pstSendData)
		{
			cMail.GetRawData( ( LPBYTE)pstSendData, nDataSize);
			int			nEntryDataSize = sizeof( ENTRYDATA) + ( lstrlen( cMail.GetFrom()) + 1);
			ENTRYDATA*	pstEntryData = ( ENTRYDATA*)malloc( nEntryDataSize);
			if( pstEntryData)
			{
				pstEntryData->nSize = nEntryDataSize;
				pstEntryData->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntryData->unNxlID = _EMAILPLUGIN_ID;
				pstEntryData->nEntryStat = 2;
				pstEntryData->nParentGroupCount = 0;
				pstEntryData->nOffsetParentGroup = 0;
				lstrcpy( ( LPSTR)&( ( LPBYTE)pstEntryData)[ sizeof( ENTRYDATA)], cMail.GetFrom());
				pstEntryData->nOffsetEntryName = sizeof( ENTRYDATA);

				RECEIVEDATA lpfnReceiveData = g_stOrgData.lpfnReceiveData;
				blResult = lpfnReceiveData( pstEntryData, nEntryDataSize, pstSendData, nDataSize);

				if( sizeof( SENDDATA) < pstSendData->m_nszCHIPID &&
					sizeof( SENDDATA) < pstSendData->m_nszMemoTitle &&
					!lstrcmp( pstSendData->szHeader, "OboeData"))
				{
					g_stOrgData.pcIDMapSubject->AddSubject( ( LPSTR)&( ( LPBYTE)pstSendData)[ pstSendData->m_nszCHIPID], ( LPSTR)&( ( LPBYTE)pstSendData)[ pstSendData->m_nszMemoTitle]);
				}

				free( pstEntryData);
			}
			free( pstSendData);
		}
	}
	else
	{
		nDataSize = sizeof( SENDDATA) + ( lstrlen( cMail.GetBody()) + 1) + ( lstrlen( cMail.GetSubject()) + 1) + ( lstrlen( cMail.GetFrom()) + 1) + ( lstrlen( cMail.GetMessageID()) + 1);

		/*�̌��łɎg���Ă��������ł����B
		int nMudaguchiIndex = -1;
		{
			nMudaguchiIndex = rand() % _MAX_GUCHIKAZU;
			nDataSize += lstrlen( pszMudaguchi[ nMudaguchiIndex]) + 1 + 2;
		}
		*/

		pstSendData = ( SENDDATA*)malloc( nDataSize);
		if( pstSendData)
		{
			int nOffset = sizeof( SENDDATA);

			ZeroMemory( pstSendData, nDataSize);

			// �t�H�[�}�b�g�R�[�h
			lstrcpy( pstSendData->szHeader, "OboeData");

			// �\���̏��
			pstSendData->nSize = nDataSize;
			pstSendData->unStructVersion = _SENDDATA_VER100;

			// �o�[�W�������ْʒm
			pstSendData->m_unVersion = 0;
			pstSendData->m_nszVersion = 0;

			// ��{�ݒ�̃f�[�^
			pstSendData->m_nszMemoTitle = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszMemoTitle], cMail.GetSubject());
			nOffset += lstrlen( cMail.GetSubject()) + 1;
			pstSendData->m_nszMemoData = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], cMail.GetBody());
			// ���K�̃g���~���O�i�͋Z
			int nTailPos = lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData]) - 1;
			while( 0 < nTailPos)
			{
				if( '\n' == ( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos])
				{
					( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos] = 0;
					nTailPos--;
					if( 0 >= nTailPos)break;
				}
				if( '\r' == ( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos])
				{
					( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos] = 0;
					nTailPos--;
					if( 0 >= nTailPos)break;
				}
				if( '\r' != ( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos] &&
					'\n' != ( ( char*)pstSendData)[ pstSendData->m_nszMemoData + nTailPos])
				{
					break;
				}
			}
			/*�̌��łɎg���Ă��������ł����B
			if( 0 <= nMudaguchiIndex && _MAX_GUCHIKAZU > nMudaguchiIndex)
			{
				lstrcat( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], "\r\n");
				lstrcat( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], pszMudaguchi[ nMudaguchiIndex]);
			}
			*/
			nOffset += lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData]) + 1;

			// �������
			pstSendData->m_nstLogFont = 0;
			pstSendData->m_clrForeColor = -1;
			pstSendData->m_clrBackColor = -1;
			pstSendData->m_nIconIndex = 0;

			// �֘A�t��
			pstSendData->m_blShellExec = FALSE;
			pstSendData->m_nszProgName = 0;
			pstSendData->m_nszFileName = 0;

			// �^�C�}�֘A
			pstSendData->m_blTimerEnable = FALSE;
			pstSendData->m_nHour = 0;
			pstSendData->m_nMin = 0;
			pstSendData->m_nTimerType = 0;
			pstSendData->m_nMonth = 0;
			pstSendData->m_nDay = 0;
			pstSendData->m_nNotifyShift = 0;
			pstSendData->m_nBeforeNotify = 0;
			pstSendData->m_blExpireTimer = FALSE;
			pstSendData->m_nszWaveFile = 0;
			pstSendData->m_nWeekDay = 0;

			// ���M�̃f�[�^
			pstSendData->m_blSendMaster = FALSE;
			pstSendData->m_nAppendPos = 1;
			pstSendData->m_nszSignature = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszSignature], cMail.GetFrom());
			nOffset += lstrlen( cMail.GetFrom()) + 1;
			pstSendData->m_blSendTime = FALSE;
			pstSendData->m_blSendResize = TRUE;
			ZeroMemory( &pstSendData->m_stRctNormal, sizeof( RECT));
			pstSendData->m_blSendCenter = TRUE;
			pstSendData->m_blPack = FALSE;
			pstSendData->m_blPassWord = FALSE;
			pstSendData->m_blCheckOpen = FALSE;
			pstSendData->m_blDelOnQuit = FALSE;
			pstSendData->m_nszFrom = 0;
			pstSendData->m_nszTo = 0;

			// �l�b�g���[�N���c
			pstSendData->m_blCheckYesNo = FALSE;
			pstSendData->m_blOtherResult = FALSE;
			pstSendData->m_nszResult1 = 0;
			pstSendData->m_nszResult2 = 0;
			pstSendData->m_nszResult3 = 0;
			pstSendData->m_nszCHIPID = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszCHIPID], cMail.GetMessageID());
			nOffset += lstrlen( cMail.GetMessageID()) + 1;
			pstSendData->m_nResultTime = 0;

			// ���Ђ̍쐬����
			pstSendData->m_nCreateYear = 0;
			pstSendData->m_nCreateMonth = 0;
			pstSendData->m_nCreateDay = 0;
			pstSendData->m_nZapDays = 0;

			// �`��g��
			pstSendData->m_unDxlID = 0;
			pstSendData->m_nszDxlDummy = 0;
			pstSendData->m_nDxlDataSize = 0;
			pstSendData->m_nabyDxlData = 0;

			ENTRYDATA*	pstEntryData = NULL;
			int			nEntryDataSize = sizeof( ENTRYDATA) + ( lstrlen( cMail.GetFrom()) + 1);
			pstEntryData = ( ENTRYDATA*)malloc( nEntryDataSize);
			if( pstEntryData)
			{
				pstEntryData->nSize = nEntryDataSize;
				pstEntryData->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntryData->unNxlID = _EMAILPLUGIN_ID;
				pstEntryData->nEntryStat = 2;
				pstEntryData->nParentGroupCount = 0;
				pstEntryData->nOffsetParentGroup = 0;
				lstrcpy( ( LPSTR)&( ( LPBYTE)pstEntryData)[ sizeof( ENTRYDATA)], cMail.GetFrom());
				pstEntryData->nOffsetEntryName = sizeof( ENTRYDATA);

				RECEIVEDATA lpfnReceiveData = g_stOrgData.lpfnReceiveData;
				blResult = lpfnReceiveData( pstEntryData, nEntryDataSize, pstSendData, nDataSize);

				free( pstEntryData);
			}
			free( pstSendData);
		}
	}

	if( g_stOrgData.blLogging)OutputErrorString( "Exit", pszDispatchMail);
	return blResult;
}

static char* pszGetPop3Message = "GetPop3Message";

BOOL GetPop3Message( HANDLE hAbortEvent)
{
	if( g_stOrgData.blLogging)OutputErrorString( "Entry", pszGetPop3Message);

	BOOL	blResult = FALSE;

	CPop3Client	cPop3Client( g_stOrgData.blPop3APOP ? true : false);

	try
	{
		if( cPop3Client.Create())
		{
			if( g_stOrgData.blLogging)OutputErrorString( "cPop3Client.Create():success", pszGetPop3Message);
			if( WAIT_OBJECT_0 == WaitForSingleObject( hAbortEvent, 10))return FALSE;
			if( cPop3Client.Connect( g_stOrgData.szPop3ServerName, g_stOrgData.unPop3Port))
			{
				if( g_stOrgData.blLogging)OutputErrorString( "cPop3Client.Connect():success", pszGetPop3Message);
				if( WAIT_OBJECT_0 == WaitForSingleObject( hAbortEvent, 10))return FALSE;
				if( cPop3Client.LogIn( g_stOrgData.szPop3ID, g_stOrgData.szPop3PW))
				{
					if( g_stOrgData.blLogging)OutputErrorString( "cPop3Client.LogIn():success", pszGetPop3Message);
					int nCount = cPop3Client.GetMessageCount();
					if( g_stOrgData.blLogging)
					{
						char	szWork[ 64];
						wsprintf( szWork, "%d = cPop3Client.GetMessageCount():success", nCount);
						OutputErrorString( szWork, pszGetPop3Message);
					}

#ifdef	_DEBUG
					HANDLE hFile;
					DWORD dwLen;
					char	szFile[ _MAX_PATH];
#endif
					char*	pszOboeEMail = "OboeGaki-email.nxl";
					for( int nMailIndex = 0 ; nMailIndex < nCount; nMailIndex++)
					{
						if( WAIT_OBJECT_0 == WaitForSingleObject( hAbortEvent, 10))return FALSE;

						if( cPop3Client.TestMailer( nMailIndex, pszOboeEMail))
						{
							int nSize = cPop3Client.GetMessageSize( nMailIndex);
							if( g_stOrgData.blLogging)
							{
								char	szWork[ 64];
								wsprintf( szWork, "%d = cPop3Client.GetMessageSize( %d):success", nSize, nMailIndex);
								OutputErrorString( szWork, pszGetPop3Message);
							}

							if( 0 < nSize)
							{

								char*	pszMailBody = NULL;
								// pszMailBody = new char [ nSize + 1];
								// cPop3Client.GetMessageBody( nMailIndex, pszMailBody, nSize + 1);
								nSize = cPop3Client.GetMessageBody( nMailIndex, &pszMailBody);
#ifdef	_DEBUG
								wsprintf( szFile, "MAIL%d.ALL", nMailIndex);
								hFile = CreateFile( szFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
								WriteFile( hFile, pszMailBody, lstrlen( pszMailBody), &dwLen, NULL);
								CloseHandle( hFile);
#endif

								CMail	cMail( pszMailBody);
								delete [] pszMailBody;

								if( g_stOrgData.blLogging)
								{
									OutputErrorString( "Start cMail.DoDecode", pszGetPop3Message);
								}
								if( cMail.DoDecode())
								{
									if( g_stOrgData.blLogging)
									{
										OutputErrorString( "Success cMail.DoDecode", pszGetPop3Message);
									}
#ifdef	_DEBUG
									wsprintf( szFile, "MAIL%d.DAT", nMailIndex);
									hFile = CreateFile( szFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
									WriteFile( hFile, "From-", lstrlen( "From-"), &dwLen, NULL);
									WriteFile( hFile, cMail.GetFrom(), lstrlen( cMail.GetFrom()), &dwLen, NULL);
									WriteFile( hFile, "\r\nSubject-", lstrlen( "\r\nSubject-"), &dwLen, NULL);
									WriteFile( hFile, cMail.GetSubject(), lstrlen( cMail.GetSubject()), &dwLen, NULL);
									WriteFile( hFile, "\r\n-----\r\n", lstrlen( "\r\n-----\r\n"), &dwLen, NULL);
									WriteFile( hFile, cMail.GetBody(), lstrlen( cMail.GetBody()), &dwLen, NULL);
									CloseHandle( hFile);
#endif
									if( DispatchMail( cMail))
									{
										cPop3Client.DeleteMessage( nMailIndex);
									}
									blResult = TRUE;
								}
							}
						}
					}
					cPop3Client.Quit();
					if( g_stOrgData.blLogging)OutputErrorString( "cPop3Client.Quit()", pszGetPop3Message);
					cPop3Client.Disconnect();
					if( g_stOrgData.blLogging)OutputErrorString( "cPop3Client.Disconnect()", pszGetPop3Message);
				}
			}
		}
		if( g_stOrgData.blLogging)OutputErrorString( "Exit", pszGetPop3Message);
	}
	catch( char* pszErrorMsg)
	{
		if( g_stOrgData.blLogging)OutputErrorString( pszErrorMsg, pszGetPop3Message);
	}
	return blResult;
}
int GetPassWord( LPSTR lpszPW)
{
	return DialogBoxParam( ( HINSTANCE)g_hModule, MAKEINTRESOURCE( IDD_PINPUTDLG), NULL, ( DLGPROC)PWInputDialogProc, ( LPARAM)lpszPW);
}

static char*	pszCycleCheckThread = "CycleCheckThread";

void CycleCheckThread( void* lpvParam)
{
	if( g_stOrgData.blLogging)OutputErrorString( "Entry", pszCycleCheckThread);

	HANDLE	hForceRead = CreateEvent( NULL, FALSE, FALSE, "email|nxl#ForceRead");
	if( hForceRead)
	{
		HANDLE	hStopEvent = g_stOrgData.hStopEvent;
		ULONG	ulTimeOut = g_stOrgData.ulPop3Interval;
		HANDLE	hEvents[ 2] = { hForceRead, hStopEvent};
		BOOL	blInvalidatePW = g_stOrgData.blInvalidatePW;

		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		srand( ( stSysTime.wDay * 24 + stSysTime.wHour) * 60 + stSysTime.wMinute);

		DWORD dwResult;
		while( true)
		{
			dwResult = WaitForMultipleObjects( 2, hEvents, FALSE, ulTimeOut);
			if( WAIT_TIMEOUT == dwResult || WAIT_OBJECT_0 == dwResult)
			{
				if( blInvalidatePW)
				{
					if( IDOK != GetPassWord( g_stOrgData.szPop3PW))break;
					blInvalidatePW = FALSE;
				}

				GetPop3Message( hStopEvent);
				continue;
			}
			break;
		}
	}

	if( g_stOrgData.blLogging)OutputErrorString( "Exit", pszCycleCheckThread);
}
