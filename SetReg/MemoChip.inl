
inline void CMemoChip::SetMemoData( const CMemoData& cMemoData)
{
	m_cMemoData = cMemoData;
}

inline BOOL CMemoChip::GetMemoData( CMemoData& cMemoData) const
{
	cMemoData = m_cMemoData;

	return TRUE;
}

inline const CMemoData* CMemoChip::AccessMemoData( void) const
{
	return &m_cMemoData;
}
