#include "EntryList.h"
#include "GroupList.h"
#include "MailS.h"

BOOL ReadAddress( LPCSTR lpcszFileName, CGroupList* pcGroupList, CEntryList* pcEntryList);

#define	_SMTP_TIMEOUT	( 60 * 1000)

int AsyncSend( SOCKET sClient, char* pszData, HANDLE hStopEvent, HANDLE hCancelEvent);
int AsyncResultCheck( SOCKET sClient, char* pszData, HANDLE hStopEvent, HANDLE hCancelEvent);
int AcyncSendBase64Encode( SOCKET sClient, LPCSTR lpcszHeader, LPCSTR lpcszData, HANDLE hStopEvent, HANDLE hCancelEvent);
int AsyncSendData( SOCKET sClient, CMailS& cMail, HANDLE hStopEvent, HANDLE hCancelEvent);

#define	_MAX_SUBJECT_LEN	74

int CheckExchangeBodyLen( LPCSTR lpcszSrc, BOOL blSubject = FALSE);
int ExchangeBody( LPSTR lpszDist, int nSize, LPCSTR lpcszSrc, BOOL blSubject = FALSE);

void OutputErrorString( LPCSTR lpcszErrMsg, LPCSTR lpcstrFunction);

int SendMail( CMailS& cMail, HANDLE hCancelEvent);
