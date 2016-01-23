#ifndef	__RSTREAMEX_HPP__
#define	__RSTREAMEX_HPP__

#include "RStream.hpp"

// CRStream��\r\n����������ċL�����Ă���N���X�������񂾂��ǁA
// ��M���Ŏg�p����ꍇ�ɂ͌����Ȃ��̂ŁA\r\n�����̂܂܈�����悤�Ɋg������
// ���Ȃ݂ɁASMTP���͏�����ύX�������Ȃ������̂ŁACRStream�͎c���Ă���B
class CRStreamEx : public CRStream
{
public:
	CRStreamEx( int nWorkSize = 100) : CRStream( nWorkSize){}

	int AddReceiveString( const char* const pszReceiveString);

	int GetHeadStringLength( void);
	int GetHeadString( char* pszData, int nDataSize);
};
#endif	//__RSTREAMEX_HPP__
