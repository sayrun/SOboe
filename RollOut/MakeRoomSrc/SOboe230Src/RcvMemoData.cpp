#include "stdafx.h"
#include "SOboe.h"
#include "RcvMemoData.h"

int CALLBACK ReceiveData( const LPVOID lpvEntry, int nEntrySize, const LPVOID lpvData, int nSize)
{
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();

	return pcSOboe->ReceiveData( lpvEntry, nEntrySize, lpvData, nSize);
}
