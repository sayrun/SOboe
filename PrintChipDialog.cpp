// PrintChipDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "PrintChipDialog.h"
#include "MemoData.h"
#include "PrinterDC.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintChipDialog �_�C�A���O

typedef struct tagPRINTCHIPDATA
{
	size_t		m_cbSize;

	int			nMemoCount;
	CMemoData*	pcMemoData;

	HDC			hPrinterDC;
	RECT		stRctMargin;

	HANDLE		hCancelEvent;

	HWND		hParentWnd;
	UINT		unMessage;
}PRINTCHIPDATA;

void PrintChipThread( void* lpvData)
{
	// �Ƃ肠�����̓G���[���Ď��ɂ��Ă������Ƃ�
	int	nResultCode = CPrintChipDialog::RESULT_ERROR;

	PRINTCHIPDATA*	pstPrintChipData = ( PRINTCHIPDATA*)lpvData;
	if( NULL != pstPrintChipData)
	{
		if( NULL != pstPrintChipData->hPrinterDC)
		{
			CPrinterDC	cPrinterDC;
			if( cPrinterDC.Attach( pstPrintChipData->hPrinterDC, pstPrintChipData->stRctMargin))
			{
				CString	cStrDocName;
				cStrDocName.LoadString( AFX_IDS_APP_TITLE);
				if( 0 <= cPrinterDC.StartDoc( cStrDocName))
				{
					if( 0 <= cPrinterDC.StartPage())
					{
						cPrinterDC.PrintForm();

						CString	cStrTitle;
						for( int nIndex = 0; nIndex < pstPrintChipData->nMemoCount; nIndex++)
						{
							if( WAIT_OBJECT_0 == WaitForSingleObject( pstPrintChipData->hCancelEvent, 0))
							{
								cPrinterDC.AbortDoc();
								cPrinterDC.Detach();
								// ���[�U�Œ��f�I�I
								nResultCode = CPrintChipDialog::RESULT_ABORT;
								goto cleanup;
							}
							cPrinterDC.PrintChip( &pstPrintChipData->pcMemoData[ nIndex]);

							pstPrintChipData->pcMemoData[ nIndex].GetTitle( cStrTitle);
							SendMessage( pstPrintChipData->hParentWnd, pstPrintChipData->unMessage, 1, ( LPARAM)( LPCSTR)cStrTitle);
						}
						// ����͐����I�I
						nResultCode = CPrintChipDialog::RESULT_SUCCESS;
						cPrinterDC.EndPage();
					}
					cPrinterDC.EndDoc();
				}
			}
			cPrinterDC.Detach();
		}
	}

cleanup:
	// �n���ꂽ���[�N�G���A�͍폜���Ă��āI
	if( pstPrintChipData)
	{
		// �폜�̑O�ɒʒm�ˁI�I����ʒm���Ă����܂��傤�I
		PostMessage( pstPrintChipData->hParentWnd, pstPrintChipData->unMessage, 2, nResultCode);

		if( pstPrintChipData->hPrinterDC)
		{
			DeleteDC( pstPrintChipData->hPrinterDC);
		}
		if( pstPrintChipData->pcMemoData)
		{
			delete [] pstPrintChipData->pcMemoData;
		}
		delete pstPrintChipData;
	}

	_endthread();
}

// �Ђǂ�������Ȃ����ǁA���M�_�C�A���O�Ɠ������b�Z�[�W�𗘗p���邱�Ƃɂ����́I
// ���\�[�X�̌y���̂��߂�
UINT CPrintChipDialog::sm_unNotifyEnd = RegisterWindowMessage( "SOBOE#MSG!FROM-SENDTHREAD");;

CPrintChipDialog::CPrintChipDialog( HDC& hPrinterDC, RECT& stRctMargin, CWnd* pParent /*=NULL*/)
	: CDialog(CPrintChipDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintChipDialog)
	//}}AFX_DATA_INIT

	m_hPrinterDC = hPrinterDC;
	m_stRctMargin = stRctMargin;
	m_hCancelEvent = NULL;
}

CPrintChipDialog::~CPrintChipDialog( void)
{
	CMemoData* pcMemoDataNode;
	while( m_cLstMemoData.GetCount())
	{
		pcMemoDataNode = m_cLstMemoData.RemoveHead();
		if( pcMemoDataNode)delete pcMemoDataNode;
	}
	if( m_hCancelEvent)CloseHandle( m_hCancelEvent);
}

void CPrintChipDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintChipDialog)
	DDX_Control(pDX, IDC_TITLE, m_cStcTitle);
	DDX_Control(pDX, IDCANCEL, m_cBtnCancel);
	DDX_Control(pDX, IDC_PRINTPRGRS, m_cPrgrsPrintChip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintChipDialog, CDialog)
	//{{AFX_MSG_MAP(CPrintChipDialog)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( sm_unNotifyEnd, OnNotifyEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintChipDialog ���b�Z�[�W �n���h��

BOOL CPrintChipDialog::AddPrintChip( const CMemoData* pcMemoData)
{
	if( NULL == pcMemoData)return FALSE;

	CMemoData* pcMemoDataNode = new CMemoData;
	if( pcMemoDataNode)
	{
		*pcMemoDataNode = *pcMemoData;
		m_cLstMemoData.AddTail( pcMemoDataNode);

		return TRUE;
	}
	return FALSE;
}


BOOL CPrintChipDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_hCancelEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	ResetEvent( m_hCancelEvent);

	if( 0 < m_cLstMemoData.GetCount())
	{
		PRINTCHIPDATA*	pstPrintChipData;
		pstPrintChipData = new PRINTCHIPDATA;
		if( pstPrintChipData)
		{
			pstPrintChipData->m_cbSize = sizeof( PRINTCHIPDATA);
			pstPrintChipData->hPrinterDC = m_hPrinterDC;
			pstPrintChipData->stRctMargin = m_stRctMargin;

			pstPrintChipData->hCancelEvent = m_hCancelEvent;
			pstPrintChipData->hParentWnd = GetSafeHwnd();
			pstPrintChipData->unMessage = sm_unNotifyEnd;

			pstPrintChipData->nMemoCount = m_cLstMemoData.GetCount();
			pstPrintChipData->pcMemoData = new CMemoData[ pstPrintChipData->nMemoCount];
			CMemoData* pcMemoDataNode;
			for( int nIndex = 0; nIndex < pstPrintChipData->nMemoCount; nIndex++)
			{
				pcMemoDataNode = m_cLstMemoData.GetAt( m_cLstMemoData.FindIndex( nIndex));
				pstPrintChipData->pcMemoData[ nIndex] = *pcMemoDataNode;
			}

			m_cPrgrsPrintChip.SetRange( 0, m_cLstMemoData.GetCount());
			m_cPrgrsPrintChip.SetPos( 0);

			if( -1 != _beginthread( PrintChipThread, 0, pstPrintChipData))
			{
				return TRUE;
			}
			DeleteDC( m_hPrinterDC);
			delete [] pstPrintChipData->pcMemoData;
			delete pstPrintChipData;

		}
		// �������g�ɃG���[�𑗂��Ă����I
		m_cBtnCancel.EnableWindow( FALSE);
		PostMessage( sm_unNotifyEnd, 2, RESULT_ERROR);
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CPrintChipDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	SetEvent( m_hCancelEvent);
	m_cBtnCancel.EnableWindow( FALSE);
}

void CPrintChipDialog::PostNcDestroy() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	CDialog::PostNcDestroy();

	delete this;
}

LONG CPrintChipDialog::OnNotifyEnd( UINT wParam, LPARAM lParam)
{
	if( 1 == wParam)
	{
		m_cStcTitle.SetWindowText( ( LPCSTR)lParam);
		m_cPrgrsPrintChip.SetPos( m_cPrgrsPrintChip.GetPos() + 1);
	}
	else
	{
		m_cBtnCancel.EnableWindow( FALSE);
		if( 2 == wParam)
		{
			if( RESULT_ABORT == lParam)
			{
				MessageBox( "����̓��[�U�ɂ�蒆�f����܂���");
			}
			else
			{
				if( RESULT_ERROR == lParam)
				{
					MessageBox( "����̓G���[�ɂ�蒆�f����܂���");
				}
			}
			DestroyWindow();
		}
	}

	return 0;
}
