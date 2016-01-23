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

/* 体験版に使っていた部分でぇす。もったいないから、消さずにおこうとか思ったの。
#define	_MAX_GUCHIKAZU	71
static char* pszMudaguchi[ _MAX_GUCHIKAZU] =
{
	"(-_-)はぁ～、なんでかこんなの作ってるしぃ。",
	"(T-T)どうしたいンだボク。ってカンジかなぁ。",
	"(+.+)アドレナリンが不足してますぅ。",
	"(+.-)ベータエンドルフィン……下さい。",
	"(-.-)今死ぬと、無職 T.Chiba(27)って書かれちゃうの。",
	"(@_@)もう失業保険も切れるっちゃ。",
	"(-_-)この前は、要らんと言われちゃった。",
	"(^-^)ray/arkはステキです。",
	"(-_-)メイル送りたい人がいます。97年末以降すーっと考えています。",
	"(@_@)ヤバイくらい無欲になっています。",
	"(~-~)これからはどうなるのかなぁ。ボクは知りません。",
	"(-_-)恐怖の大王はどうしたの？",
	"(--; ヤバっ……、何でもない何でもない。気にしないで。",
	"(^-^)水沢 螢さんはかわいいです。",
	"(^-^)( ^-)(  ^)(   )(^  )(-^ )(^-^)くるっくるっ♪",
	"(-_-)彼女たちの時代、イタイですが、興味深いドラマです。見れ！",
	"(^-^)最近、WinCE Toolkit for VC++6.0を注文しました。",
	"(+_+)WinCE機は持ってないです。",
	"(-｡-)y-ﾟﾟﾟ時々むしょーにタバコが吸いたい気がします。",
	"(-_-)クモはコワイです。1mmくらいのもヤです。",
	"(;_;)今年の仙台の真夏日記録は連続14日で終わりました。",
	"(*_*)争いの無い世界はステキだと思うのですが……弱いですか？",
	"(･_･)濃く生きたい？薄く生きたい？",
	"(-_-)あっ、やっぱいいや。ゴメン、気にしないで。",
	"(^-^)そのページの\'Profile\'を最初に見ちゃうの。",
	"(-_-)食料が無くてもイイから、ボイジャーに乗りたかった。",
	"(^-^)Feさんの絵はとてもとても好きです。",
	"(-_-)久方ぶりに\"いたいけな瞳\"読みました。やっぱ凄いです。",
	"(^-^)源氏物語は恋愛小説だと思う。",
	"(T-T)自分の才能の無さに呆れるときがある。",
	"(///)突然昔の失敗を思い出して恥ずかしがる。思い出し反省。",
	"(^-^)TAPのTV-CF「もう一回言いなさいよぉ」は可愛いカンジです",
	"(^-^)未来少年コナンの再放送を見ました。面白かったです。",
	"(-_-)ン？なんか、狭くない？",
	"(^-^)昔\'ペケロッパ\'に感動したものだ。",
	"(-.-)Win3.0の頃BPlayモドキを作ったことがある。",
	"(-_-)冒険が終われば日常が始まる……のび太の宇宙開拓史。",
	"(--#)苦労して質問に解答してもあとは梨の礫……ヤになります。",
	"(-_-)ってゆーかさぁ、暇？遊び行かない？",
	"(-_-)y-ﾟﾟﾟふっ、気付けばアンタ、俺も２７よ。ビビっちゃ？",
	"(-.-)漂流商事……ボクも仲間に入れてくれよぉ。",
	"(^-^)９月２２日のバイオ３、またヨドバシに朝から並ぶ？",
	"(^-^)ココ余計だって？知ってる知ってる解ってる。",
	"(-_-)夏も終わるよ。",
	"(T-T)違法コピー……ソフトに対する価値と文化の否定。",
	"(?_?)そういえば、アルスラーン戦記ってどうなったっちゃ？",
	"(._.)あっ、アリ！ほらっ、下の方。……ゴメン、カーソルだった。",
	"(+_+)停電で折角書いたコードをロストしました。",
	"(^-^)だいじょうぶ、ココは本実装されないからさ。",
	"(-.-)そう言えばさ……やっぱいいや。",
	"(TﾛT)毛虫を吐く夢を見て、自分の悲鳴で起きたことがある。",
	"(~-~)暑いと、ヘロヘロな気持ちになる。",
	"(-_-)風鈴は岩手の南部鉄器にかぎるねぇ。",
	"(-_-)価値観を自分の外に置いているとしたら、残念なことね",
	"(>_<)藍い空を見ていたら、子供の頃の何かを思い出せそうだった。",
	"(x_x)知り合いの日記は見たくないかも。",
	"(--;)ren *.doc *.txtができないからといってGUI非難はナンセンスだ。\r\n沢山荷物が積め便利だって、隣の家にトラックでは行かない。",
	"(^-^)BSで\'禁じられた遊び\'を見ました。気にってしまいました。",
	"(T-T)\"珈琲好き＝ＳＥＸ好き\"って認識は何とかして欲しい。",
	"(x_x)肩凝りが酷い。一回休み",
	"(^-^)会津若松が好き！",
	"(x_x)親知らずを抜くとき、その\'破砕音\'で気が狂いそうだった。",
	"(-.-)初めてWin3.0とOS/2を見たとき、その安定性からOS/2が勝つと思っていた。",
	"(~-~)NTのOSとしてのデザインは4.0より3.Xの方がイイと思う。",
	"(--;)\'えろげ～\'って響きはサイアクだと思う。",
	"(^^;)ウィンダリアをみて不覚にも泣きそうになった。",
	"(-_-)そういえば、パトレイバーの劇場版３はどうなった？",
	"(x_x)盗聴法成立しちゃいました。",
	"(^-^)初めてbaby one more timeのプロモを見たとき、酷くドキドキした。",
	"(--;)この独り言は結構な種類がある。",
	"(^-^)マウスはLogitechのMouseMan+に限る！",
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

		/*体験版に使っていた部分でぇす。
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

			// フォーマットコード
			lstrcpy( pstSendData->szHeader, "OboeData");

			// 構造体情報
			pstSendData->nSize = nDataSize;
			pstSendData->unStructVersion = _SENDDATA_VER100;

			// バージョン差異通知
			pstSendData->m_unVersion = 0;
			pstSendData->m_nszVersion = 0;

			// 基本設定のデータ
			pstSendData->m_nszMemoTitle = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszMemoTitle], cMail.GetSubject());
			nOffset += lstrlen( cMail.GetSubject()) + 1;
			pstSendData->m_nszMemoData = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], cMail.GetBody());
			// お尻のトリミング（力技
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
			/*体験版に使っていた部分でぇす。
			if( 0 <= nMudaguchiIndex && _MAX_GUCHIKAZU > nMudaguchiIndex)
			{
				lstrcat( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], "\r\n");
				lstrcat( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData], pszMudaguchi[ nMudaguchiIndex]);
			}
			*/
			nOffset += lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszMemoData]) + 1;

			// 装飾情報
			pstSendData->m_nstLogFont = 0;
			pstSendData->m_clrForeColor = -1;
			pstSendData->m_clrBackColor = -1;
			pstSendData->m_nIconIndex = 0;

			// 関連付け
			pstSendData->m_blShellExec = FALSE;
			pstSendData->m_nszProgName = 0;
			pstSendData->m_nszFileName = 0;

			// タイマ関連
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

			// 送信のデータ
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

			// ネットワーク決議
			pstSendData->m_blCheckYesNo = FALSE;
			pstSendData->m_blOtherResult = FALSE;
			pstSendData->m_nszResult1 = 0;
			pstSendData->m_nszResult2 = 0;
			pstSendData->m_nszResult3 = 0;
			pstSendData->m_nszCHIPID = nOffset;
			lstrcpy( &( ( char*)pstSendData)[ pstSendData->m_nszCHIPID], cMail.GetMessageID());
			nOffset += lstrlen( cMail.GetMessageID()) + 1;
			pstSendData->m_nResultTime = 0;

			// 小片の作成時間
			pstSendData->m_nCreateYear = 0;
			pstSendData->m_nCreateMonth = 0;
			pstSendData->m_nCreateDay = 0;
			pstSendData->m_nZapDays = 0;

			// 描画拡張
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
