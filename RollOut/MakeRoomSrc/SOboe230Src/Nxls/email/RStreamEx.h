#ifndef	__RSTREAMEX_HPP__
#define	__RSTREAMEX_HPP__

#include "RStream.hpp"

// CRStreamは\r\nを取っ払って記憶しているクラスだったんだけど、
// 受信側で使用する場合には向かないので、\r\nをそのまま扱えるように拡張した
// ちなみに、SMTP側は処理を変更したくなかったので、CRStreamは残してある。
class CRStreamEx : public CRStream
{
public:
	CRStreamEx( int nWorkSize = 100) : CRStream( nWorkSize){}

	int AddReceiveString( const char* const pszReceiveString);

	int GetHeadStringLength( void);
	int GetHeadString( char* pszData, int nDataSize);
};
#endif	//__RSTREAMEX_HPP__
