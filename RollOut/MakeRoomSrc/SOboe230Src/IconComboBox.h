// IconComboBox.h : �w�b�_�[ �t�@�C��
//
#ifndef	__ICONCOMBOBOX_H__
#define	__ICONCOMBOBOX_H__

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox �E�B���h�E

class CIconComboBox : public CComboBox
{
// �R���X�g���N�V����
public:
	CIconComboBox();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CIconComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CIconComboBox();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CIconComboBox)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	//__ICONCOMBOBOX_H__
