#ifndef	__MEMOWNDLIST_H__
#define	__MEMOWNDLIST_H__

#include "MemoChip.h"

class CMemoChipList : public CList < CMemoChip*, CMemoChip*&>
{
public:
	CMemoChipList() : CList< CMemoChip*, CMemoChip*&>( 5){};
	CCriticalSection m_cCriticalSection;
};
#endif	//__MEMOWNDLIST_H__
