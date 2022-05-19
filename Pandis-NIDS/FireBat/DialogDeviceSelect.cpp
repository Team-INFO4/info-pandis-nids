// DialogDeviceSelect.cpp: 구현 파일
//

#include "stdafx.h"
#include "FireBat.h"
#include "afxdialogex.h"
#include "DialogDeviceSelect.h"


// CDialogDeviceSelect 대화 상자

IMPLEMENT_DYNAMIC(CDialogDeviceSelect, CDialog)

CDialogDeviceSelect::CDialogDeviceSelect(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_DEVICE_SELECT, pParent)
	, m_strSelectedItem(_T(""))
	, m_strDeviceID(_T(""))
	, m_alldevs(NULL)
	, m_ptrdev(NULL)
{
	m_strErrBuf[0] = '\0';
}

CDialogDeviceSelect::~CDialogDeviceSelect()
{
}

void CDialogDeviceSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_ctrlSelectButton);
	DDX_Text(pDX, IDC_EDIT_SELECT_TRANSPARENC, m_strSelectedItem);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_ctrlDeviceList);
}


BEGIN_MESSAGE_MAP(CDialogDeviceSelect, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE, &CDialogDeviceSelect::OnLbnSelchangeListDevice)
	ON_LBN_SELCANCEL(IDC_LIST_DEVICE, &CDialogDeviceSelect::OnLbnSelcancelListDevice)
END_MESSAGE_MAP()


// CDialogDeviceSelect 메시지 처리기


BOOL CDialogDeviceSelect::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if (pcap_findalldevs(&m_alldevs, m_strErrBuf) == -1)
	{
		CString strErr(m_strErrBuf);
		AfxMessageBox(strErr);
		return TRUE;
	}

	CString strDeviceName;
	CString strDescription;
	for (m_ptrdev = m_alldevs; m_ptrdev; m_ptrdev = m_ptrdev->next)
	{
		strDeviceName = m_ptrdev->name;
		if (m_ptrdev->description)
			strDescription = m_ptrdev->description;
		m_ctrlDeviceList.InsertString(-1, strDeviceName + ", " + strDescription);
	}

	pcap_freealldevs(m_alldevs);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogDeviceSelect::OnLbnSelchangeListDevice()
{
	if (m_ctrlDeviceList.GetCurSel() != LB_ERR)
	{
		m_ctrlDeviceList.GetText(m_ctrlDeviceList.GetCurSel(), m_strSelectedItem);
		int nPos = 0;
		m_strDeviceID = m_strSelectedItem.Tokenize(_T(","), nPos);
	}

	m_ctrlSelectButton.EnableWindow(TRUE);
}


void CDialogDeviceSelect::OnLbnSelcancelListDevice()
{
	m_ctrlSelectButton.EnableWindow(FALSE);
}
