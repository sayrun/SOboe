#ifndef	__KANJIUTL_H__
#define	__KANJIUTL_H__

int MimeEncode( LPSTR szEncoded, LPCSTR lpcsz, int nIndex, int nMaxLen);
int MimeDecode( LPSTR lpsz, int nSize, LPCSTR lpcszSource);
int CheckSjis2JisLen( LPCSTR lpcszShiftJis);
int Sjis2Jis( LPSTR lpszJis, int nSize, LPCSTR lpcszShiftJis);
int CheckJis2SjisLen( LPCSTR lpcszJis);
int Jis2Sjis( LPSTR lpszShiftJis, int nSize, LPCSTR lpcszJis);

#endif	//__KANJIUTL_H__
