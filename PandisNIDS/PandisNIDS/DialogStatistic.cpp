// DialogStatistic.cpp: 구현 파일
//

#include "pch.h"
#include "PandisNIDS.h"
#include "afxdialogex.h"
#include "DialogStatistic.h"


// CDialogStatistic 대화 상자

IMPLEMENT_DYNAMIC(CDialogStatistic, CDialog)

CDialogStatistic::CDialogStatistic(Pt_t* pPacketCountAddr, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_STATISTIC, pParent),
	m_pPacketCount(pPacketCountAddr),
	m_pThread(nullptr)
{
}

CDialogStatistic::~CDialogStatistic()
{
}

void CDialogStatistic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATISTIC, m_ctrlListStatistic);
}


BEGIN_MESSAGE_MAP(CDialogStatistic, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDialogStatistic::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogStatistic 메시지 처리기


BOOL CDialogStatistic::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrlListStatistic.InsertColumn(0, _T("프로토콜"), LVCFMT_LEFT, 100);
	m_ctrlListStatistic.InsertColumn(1, _T("개수"), LVCFMT_LEFT, 380);
	m_ctrlListStatistic.InsertItem(0, _T("ARP"));
	m_ctrlListStatistic.InsertItem(1, _T("IPv4"));
	m_ctrlListStatistic.InsertItem(2, _T("IPv6"));
	m_ctrlListStatistic.InsertItem(3, _T("TCP"));
	m_ctrlListStatistic.InsertItem(4, _T("UDP"));
	m_ctrlListStatistic.InsertItem(5, _T("ICMP"));
	m_ctrlListStatistic.InsertItem(6, _T("String Rule"));
	for (int i = 0; i < 7; i++)
		m_ctrlListStatistic.SetItem(i, 1, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);

	m_pThread = AfxBeginThread(RefreshThreadFunc, (LPVOID)this);
	m_pThread->SuspendThread();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDialogStatistic::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ShowWindow(SW_HIDE);
	SuspendRefreshThread();

	//CDialog::OnCancel();
}

UINT CDialogStatistic::RefreshThreadFunc(LPVOID lpParam)
{
	CDialogStatistic* PThis = (CDialogStatistic*)lpParam;
	CString strNumber;

	while (1)
	{
		strNumber.Format(_T("%u"), PThis->m_pPacketCount->arp);		// ARP
		PThis->m_ctrlListStatistic.SetItem(0, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->ip4);		// IPv4
		PThis->m_ctrlListStatistic.SetItem(1, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->ip6);		// IPv6
		PThis->m_ctrlListStatistic.SetItem(2, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->tcp);		// TCP
		PThis->m_ctrlListStatistic.SetItem(3, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->udp);		// UDP
		PThis->m_ctrlListStatistic.SetItem(4, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->icmp);	// ICMP
		PThis->m_ctrlListStatistic.SetItem(5, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		strNumber.Format(_T("%u"), PThis->m_pPacketCount->string);	// ICMP
		PThis->m_ctrlListStatistic.SetItem(6, 1, LVIF_TEXT, strNumber, NULL, NULL, NULL, NULL);

		Sleep(1000);
	}

	return 0;
}

UINT CDialogStatistic::ResumeRefreshThread()
{
	if(m_pThread)
		return m_pThread->ResumeThread();
}

void CDialogStatistic::SuspendRefreshThread()
{
	if(m_pThread)
		m_pThread->SuspendThread();
}