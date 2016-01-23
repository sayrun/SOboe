
inline void CMemoChip::SetMemoData( const CMemoData& cMemoData)
{
	m_cMemoData = cMemoData;
}

inline const CMemoData* CMemoChip::AccessMemoData( void) const
{
	return &m_cMemoData;
}

inline UINT CMemoChip::GetID( void) const
{
	return m_unID;
}

inline BOOL CMemoChip::IsDelOnQuit( void) const
{
	return m_blDelOnQuit;
}

inline BOOL CMemoChip::DrawResizeFrame( BOOL blResize)
{
	BOOL blResult = ( false != m_blFrameDraw);

	m_blFrameDraw = ( FALSE != blResize);

	return blResult;
}

inline void CMemoChip::SmartDelete( void)
{
	m_blSmartClose = true;

	SendMessage( WM_CLOSE, 0, 0);

	return;
}

// �����ł̊Ǘ��F
inline const COLORREF CMemoChip::GetInternalForeColor( void) const
{
	// ���������p�̕����F
	return m_clrInternalForeColor;
}

inline const COLORREF CMemoChip::GetInternalBackColor( void) const
{
	// ���������p�̔w�i�F
	return m_clrInternalBackColor;
}

inline BOOL CMemoChip::IsPack( void) const
{
	return m_blPack;
}

inline void CMemoChip::SetContextID( LPCSTR lpcszID)
{
	// ���Ƃ��ƕ\�����ŋL�����Ă����������f�[�^�ł��邪�A
	// �s�������f�[�^�Ƃ��Ĉ������ƂƕύX�B
	// ���̂��߁A�ݒ肳�ꂽ�f�[�^���o�C�p�X��������֕ύX
	m_cMemoData.SetContextID( lpcszID);
}

inline BOOL CMemoChip::IsEqualContextID( LPCSTR lpcszID)
{
	// ���Ƃ��ƕ\�����ŋL�����Ă����������f�[�^�ł��邪�A
	// �s�������f�[�^�Ƃ��Ĉ������ƂƕύX�B
	// ���̂��߁A�ݒ肳�ꂽ�f�[�^���o�C�p�X��������֕ύX
	return m_cMemoData.IsEqualContextID( lpcszID);
}

inline BOOL CMemoChip::NoCheckReceive( void) const
{
	return m_blReceive;
}

inline void CMemoChip::OneTimeTop( void)
{
	m_blOnceTop = true;
}
