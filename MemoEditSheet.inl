
// ���T�C�Y���̎擾
inline BOOL CMemoEditSheet::IsResize( void) const
{
	return m_cBasePage.m_blResize;
}

// �I�����̔j���t���O
inline BOOL CMemoEditSheet::IsDelOnQuit( void) const
{
	return m_cBasePage.m_blDelOnQuit;
}

inline void CMemoEditSheet::DelOnQuit( BOOL blDelOnQuit)
{
	m_cBasePage.m_blDelOnQuit = blDelOnQuit;
}

// �f�[�^�������e�X�g�F���e
inline BOOL CMemoEditSheet::IsMemoEmpty( void) const
{
	return m_cBasePage.m_cStrMemo4IF.IsEmpty();
}

// Dxl-ID�]���p�̃��\�b�h
inline BOOL CMemoEditSheet::ForwardDxlID( UINT unDxlID)
{
	return m_cBasePage.m_unDxlID = unDxlID;
}

// ���T�C�Y�̋֎~
inline void CMemoEditSheet::ResizeLock( BOOL blResizeLock)
{
	m_cBasePage.m_blResizeLock = blResizeLock;
}
