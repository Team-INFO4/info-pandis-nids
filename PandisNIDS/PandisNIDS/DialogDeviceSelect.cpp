// CDialogDeviceSelect.cpp: 구현 파일
//

#include "pch.h"
#include "PandisNIDS.h"
#include "afxdialogex.h"
#include "DialogDeviceSelect.h"


// CDialogDeviceSelect 대화 상자

IMPLEMENT_DYNAMIC(CDialogDeviceSelect, CDialog)

CDialogDeviceSelect::CDialogDeviceSelect(CWnd* pParent /*=nullptr*/) // 초기화
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

void CDialogDeviceSelect::DoDataExchange(CDataExchange* pDX) // 화면과 데이터 교환하는 함수
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_ctrlSelectButton);
	DDX_Text(pDX, IDC_EDIT_SELECT_TRANSPARENC, m_strSelectedItem);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_ctrlDeviceList);
}


BEGIN_MESSAGE_MAP(CDialogDeviceSelect, CDialog) // 함수와 이벤트를 연동
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE, &CDialogDeviceSelect::OnLbnSelchangeListDevice)
	ON_LBN_SELCANCEL(IDC_LIST_DEVICE, &CDialogDeviceSelect::OnLbnSelcancelListDevice)
END_MESSAGE_MAP()


// CDialogDeviceSelect 메시지 처리기
BOOL CDialogDeviceSelect::OnInitDialog() // 추가 초기화 함수
{
	CDialog::OnInitDialog();

	if (pcap_findalldevs(&m_alldevs, m_strErrBuf) == -1) // 네트워크 디바이스 가져오기
	{
		CString strErr(m_strErrBuf);
		AfxMessageBox(strErr);
		return TRUE;
	}

	CString strDeviceName; // 디바이스 이름 문자열
	CString strDescription; // 디바이스 설명 문자열
	for (m_ptrdev = m_alldevs; m_ptrdev; m_ptrdev = m_ptrdev->next) // 네트워크 디바이스 하나씩 화면 목록에 추가
	{
		strDeviceName = m_ptrdev->name;
		if (m_ptrdev->description)
			strDescription = m_ptrdev->description;
		m_ctrlDeviceList.InsertString(-1, strDeviceName + ", " + strDescription); // 목록에 하나씩 추가
	}

	pcap_freealldevs(m_alldevs);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogDeviceSelect::OnLbnSelchangeListDevice() // 목록을 선택했을 때 이벤트
{
	if (m_ctrlDeviceList.GetCurSel() != LB_ERR)
	{
		m_ctrlDeviceList.GetText(m_ctrlDeviceList.GetCurSel(), m_strSelectedItem); // 선택 위치의 문자열 저장
		int nPos = 0;
		m_strDeviceID = m_strSelectedItem.Tokenize(_T(","), nPos); // ID만 저장
	}

	m_ctrlSelectButton.EnableWindow(TRUE);
}


void CDialogDeviceSelect::OnLbnSelcancelListDevice() // 목록을 선택을 취소했을 때 이벤트
{
	m_ctrlSelectButton.EnableWindow(FALSE);
}
