
// DialogPandisNIDS.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "PandisNIDS.h"
#include "DialogPandisNIDS.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- //


// CDialogPandisNIDS 대화 상자
CDialogPandisNIDS::CDialogPandisNIDS(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PANDISNIDS_DIALOG, pParent)
	, m_hPcap(NULL)
	, m_pThread(NULL)
	, m_lpszErrbuf("")
	, m_CS()
	, m_ThreadStatus(THREAD_STOP)
	, m_index(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PANDIS);
}

void CDialogPandisNIDS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGTEXT, m_ctrlListLogText);
	DDX_Control(pDX, IDC_STATE, m_ctrlStaticStateText);
}

BEGIN_MESSAGE_MAP(CDialogPandisNIDS, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDialogPandisNIDS::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDialogPandisNIDS::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDialogPandisNIDS::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_STATISTIC, &CDialogPandisNIDS::OnBnClickedButtonOpenStatistic)
END_MESSAGE_MAP()


// CDialogPandisNIDS 메시지 처리기
BOOL CDialogPandisNIDS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 추가 동작을 초기화합니다.
	m_ctrlListLogText.InsertColumn(0, _T("순서"), LVCFMT_LEFT, 40);
	m_ctrlListLogText.InsertColumn(1, _T("프로토콜"), LVCFMT_LEFT, 80);
	m_ctrlListLogText.InsertColumn(2, _T("출발지"), LVCFMT_LEFT, 120);
	m_ctrlListLogText.InsertColumn(3, _T("도착지"), LVCFMT_LEFT, 120);
	m_ctrlListLogText.InsertColumn(4, _T("정보"), LVCFMT_LEFT, 550);

	m_DlgStat = new CDialogStatistic(&m_PktCount);
	m_DlgStat->Create(IDD_DIALOG_STATISTIC);

	m_ctrlStaticStateText.SetWindowText(_T("상태: 중지"));

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDialogPandisNIDS::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CDialogPandisNIDS::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDialogPandisNIDS::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDialogPandisNIDS::OnBnClickedButtonStart()
{
	if (m_pThread)
	{
		AfxMessageBox(_T("FireBatNIDS 이(가) 실행 중입니다."));
		return;
	}

	m_DlgDevSel = new CDialogDeviceSelect;
	m_DlgRuleSet = new CDialogRuleSet;
	m_DlgPcapFile = new CFileDialog(false, _T("pcap"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("Pcap Files(*.pcap)|*.pcap|All Files(*.*)|*.*||"), NULL);

	if (m_DlgDevSel->DoModal() == IDCANCEL)
		return;

	if (m_DlgRuleSet->DoModal() == IDCANCEL)
		return;

	if (m_DlgPcapFile->DoModal() == IDCANCEL)
		return;
	m_strSavePath = m_DlgPcapFile->GetPathName();

	m_ctrlListLogText.DeleteAllItems();
	m_index = 0;

	memset(&m_PktCount, 0, sizeof(m_PktCount));

	m_ThreadStatus = THREAD_RUNNING;
	m_pThread = AfxBeginThread(CaptureThreadFunc, (LPVOID)this);
	if (m_pThread == NULL)
	{
		AfxMessageBox(_T("시작하지 못했습니다."));
		m_ThreadStatus = THREAD_STOP;
		m_ctrlStaticStateText.SetWindowText(_T("상태: 중지"));
		return;
	}

	m_ctrlStaticStateText.SetWindowText(_T("상태: 동작중."));
}

UINT CDialogPandisNIDS::CaptureThreadFunc(LPVOID lpParam)
{
	CDialogPandisNIDS* PThis = (CDialogPandisNIDS*)lpParam;
	bool bIsError = false;

	PThis->m_CS.Lock();

	if(!bIsError && ((PThis->m_hPcap = pcap_open_live((CStringA)PThis->m_DlgDevSel->m_strDeviceID, 65536, 1, 1000, PThis->m_lpszErrbuf)) == NULL))
	{
		AfxMessageBox(_T("디바이스를 열 수 없습니다."));
		bIsError = true;
	}

	if (!bIsError && (pcap_compile(PThis->m_hPcap, &PThis->m_fcode, (CStringA)PThis->m_DlgRuleSet->m_strFilterRule, 1, PCAP_NETMASK_UNKNOWN) < 0))
	{
		AfxMessageBox(_T("규칙을 설정하는 과정에서 오류가 발생했습니다."));
		bIsError = true;
	}

	if (!bIsError && (pcap_setfilter(PThis->m_hPcap, &PThis->m_fcode) < 0))
	{
		AfxMessageBox(_T("규칙을 적용하는 과정에서 오류가 발생했습니다."));
		bIsError = true;
	}

	if (!bIsError && ((PThis->m_dumpfile = pcap_dump_open(PThis->m_hPcap, (CStringA)PThis->m_strSavePath)) == NULL))
	{
		AfxMessageBox(_T("저장 경로를 열 수 없습니다."));
		bIsError = true;
	}

	if (bIsError)
	{
		PThis->m_ThreadStatus = THREAD_STOP;
		PThis->m_pThread = NULL;
		PThis->m_CS.Unlock();
		delete PThis->m_DlgDevSel;
		delete PThis->m_DlgRuleSet;
		delete PThis->m_DlgPcapFile;
		return -1;
	}

	pcap_loop(PThis->m_hPcap, 0, packet_handler, (u_char*)lpParam);

	pcap_close(PThis->m_hPcap);
	pcap_freecode(&PThis->m_fcode);
	PThis->m_pThread = NULL;

	PThis->m_CS.Unlock();
	delete PThis->m_DlgDevSel;
	delete PThis->m_DlgRuleSet;
	delete PThis->m_DlgPcapFile;
	return 0;
}

void CDialogPandisNIDS::packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)
{
	CDialogPandisNIDS* TThis = (CDialogPandisNIDS*)param;
	pcap_dump((u_char*)TThis->m_dumpfile, header, pkt_data);
	CString strIndex, strProtocol, strSrc, strDst, strInfo;

	hdr_t pkth = PacketAnalyzing(pkt_data, TThis->m_PktCount);

	strIndex.Format(_T("%u"), TThis->m_index + 1);
	ProtocolAnalyzing(pkth.type, strProtocol);
	AddressAnalyzing(pkth, strSrc, strDst);
	InfoAnalzing(pkth, header->len, strInfo);

	TThis->m_PktCount.string += FindStringPacketData(header, pkt_data, TThis->m_DlgRuleSet->m_vtAnalyzeDatas);
	
	TThis->m_ctrlListLogText.InsertItem(TThis->m_index, strIndex);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 1, LVIF_TEXT, strProtocol, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 2, LVIF_TEXT, strSrc, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 3, LVIF_TEXT, strDst, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 4, LVIF_TEXT, strInfo, NULL, NULL, NULL, NULL);

	TThis->m_index++;
	while (TThis->m_ThreadStatus == THREAD_PAUSE);
}

void CDialogPandisNIDS::OnBnClickedButtonStop()
{
	if (m_ThreadStatus == THREAD_STOP)
		return;

	m_ThreadStatus = THREAD_STOP;
	m_ctrlStaticStateText.SetWindowText(_T("상태: 중지"));
	pcap_breakloop(m_hPcap);
}

void CDialogPandisNIDS::OnBnClickedButtonPause()
{
	if (m_ThreadStatus == THREAD_PAUSE)
	{
		m_ThreadStatus = THREAD_RUNNING;
		m_ctrlStaticStateText.SetWindowText(_T("상태: 동작중."));
		return;
	}

	if (m_ThreadStatus == THREAD_RUNNING)
	{
		m_ThreadStatus = THREAD_PAUSE;
		m_ctrlStaticStateText.SetWindowText(_T("상태: 일시정지"));
		return;
	}
}

void CDialogPandisNIDS::OnBnClickedButtonOpenStatistic()
{
	m_DlgStat->ShowWindow(SW_SHOW);
	m_DlgStat->ResumeRefreshThread();
}
