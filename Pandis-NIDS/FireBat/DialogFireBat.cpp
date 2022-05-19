// DialogFireBat.cpp: 구현 파일
//

#include "stdafx.h"
#include "FireBat.h"
#include "afxdialogex.h"
#include "DialogFireBat.h"


// CDialogFireBat 대화 상자

IMPLEMENT_DYNAMIC(CDialogFireBat, CDialog)

CDialogFireBat::CDialogFireBat(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_FIREBAT, pParent)
	, m_hPcap(NULL)
	, m_pThread(NULL)
	, m_lpszErrbuf("")
	, m_CS()
	, m_ThreadStatus(THREAD_STOP)
{
}

CDialogFireBat::~CDialogFireBat()
{
}

void CDialogFireBat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DISPLAY, m_ctrlLoggingOut);
}


BEGIN_MESSAGE_MAP(CDialogFireBat, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDialogFireBat::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDialogFireBat::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDialogFireBat::OnBnClickedButtonPause)
END_MESSAGE_MAP()


// CDialogFireBat 메시지 처리기


void CDialogFireBat::OnBnClickedButtonStart()
{
	if (m_pThread)
	{
		AfxMessageBox(_T("FireBatNIDS 이(가) 실행 중입니다."));
		return;
	}

	if (m_DlgDevSel.DoModal() == IDCANCEL)
		return;

	if (m_DlgRuleSet.DoModal() == IDCANCEL)
		return;

	m_ctrlLoggingOut.SetWindowText(_T(""));

	m_ThreadStatus = THREAD_RUNNING;
	m_pThread = AfxBeginThread(CaptureThreadFunc, (LPVOID)this);
	if (m_pThread == NULL)
	{
		AfxMessageBox(_T("시작하지 못했습니다."));
		m_ThreadStatus = THREAD_STOP;
	}
	
}

UINT CDialogFireBat::CaptureThreadFunc(LPVOID lpParam)
{
	CDialogFireBat* PThis = (CDialogFireBat*)lpParam;

	PThis->m_CS.Lock();
	
	if ((PThis->m_hPcap = pcap_open_live(
		(CStringA)PThis->m_DlgDevSel.m_strDeviceID,
		65536,
		1,
		1000,
		PThis->m_lpszErrbuf)) == NULL)
	{
		AfxMessageBox(_T("디바이스를 열 수 없습니다."));
		return -1;
	}

	if ((pcap_compile(PThis->m_hPcap,
		&PThis->m_fcode,
		(CStringA)PThis->m_DlgRuleSet.m_strFilterRule,
		1,
		PCAP_NETMASK_UNKNOWN)) < 0)
	{
		AfxMessageBox(_T("규칙을 설정하는 과정에서 오류가 발생했습니다."));
		return -1;
	}

	if ((pcap_setfilter(PThis->m_hPcap, &PThis->m_fcode)) < 0)
	{
		AfxMessageBox(_T("규칙을 적용하는 과정에서 오류가 발생했습니다."));
		return -1;
	}

	pcap_loop(PThis->m_hPcap, 0, packet_handler, (u_char*)PThis);

	pcap_close(PThis->m_hPcap);
	pcap_freecode(&PThis->m_fcode);
	PThis->m_pThread = NULL;

	PThis->m_CS.Unlock();
	return 0;
}

void CDialogFireBat::packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	CDialogFireBat* TThis = (CDialogFireBat*)param;
	int len;
	CString strResult;

	hdr_t pkth = PacketAnalyzing(pkt_data);
	PrintPacketData(pkth, strResult); // TODO

	len = TThis->m_ctrlLoggingOut.GetWindowTextLength();
	TThis->m_ctrlLoggingOut.SetSel(len, len);

	if (TThis->m_ThreadStatus == THREAD_STOP)
	{
		TThis->m_ctrlLoggingOut.ReplaceSel(_T("[TERMINATED]\r\n"));
		pcap_breakloop(TThis->m_hPcap);
	}

	if (TThis->m_ThreadStatus == THREAD_PAUSE)
	{
		TThis->m_ctrlLoggingOut.ReplaceSel(_T("[PAUSE]\r\n"));
		while (TThis->m_ThreadStatus == THREAD_PAUSE);
	}

	TThis->m_ctrlLoggingOut.ReplaceSel(strResult);
}

void CDialogFireBat::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ThreadStatus = THREAD_STOP;
}

void CDialogFireBat::OnBnClickedButtonPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_ThreadStatus == THREAD_PAUSE)
	{
		m_ThreadStatus = THREAD_RUNNING;
		return;
	}

	if (m_ThreadStatus == THREAD_RUNNING)
	{
		m_ThreadStatus = THREAD_PAUSE;
		return;
	}
}