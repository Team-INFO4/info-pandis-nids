
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
CDialogPandisNIDS::CDialogPandisNIDS(CWnd* pParent /*=nullptr*/) // 초기화
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

void CDialogPandisNIDS::DoDataExchange(CDataExchange* pDX) // 화면과 데이터 교환
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGTEXT, m_ctrlListLogText);
	DDX_Control(pDX, IDC_STATE, m_ctrlStaticStateText);
}

BEGIN_MESSAGE_MAP(CDialogPandisNIDS, CDialog) // 함수와 이벤트를 연동
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDialogPandisNIDS::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDialogPandisNIDS::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDialogPandisNIDS::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_STATISTIC, &CDialogPandisNIDS::OnBnClickedButtonOpenStatistic)
END_MESSAGE_MAP()


// CDialogPandisNIDS 메시지 처리기
BOOL CDialogPandisNIDS::OnInitDialog() // 추가 초기화
{
	CDialog::OnInitDialog();

	/* 출력용 리스트 컨트롤에 열 추가 */
	m_ctrlListLogText.InsertColumn(0, _T("순서"), LVCFMT_LEFT, 40);
	m_ctrlListLogText.InsertColumn(1, _T("프로토콜"), LVCFMT_LEFT, 80);
	m_ctrlListLogText.InsertColumn(2, _T("출발지"), LVCFMT_LEFT, 120);
	m_ctrlListLogText.InsertColumn(3, _T("도착지"), LVCFMT_LEFT, 120);
	m_ctrlListLogText.InsertColumn(4, _T("정보"), LVCFMT_LEFT, 550);

	// 통계 컨트롤를 미리 생성
	m_DlgStat = new CDialogStatistic(&m_PktCount);
	m_DlgStat->Create(IDD_DIALOG_STATISTIC);

	m_ctrlStaticStateText.SetWindowText(_T("상태: 중지"));

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX); // (자동 생성) 
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

void CDialogPandisNIDS::OnSysCommand(UINT nID, LPARAM lParam) // (자동 생성) 
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
void CDialogPandisNIDS::OnPaint() // (자동 생성)
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
HCURSOR CDialogPandisNIDS::OnQueryDragIcon() // (자동 생성)
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDialogPandisNIDS::OnBnClickedButtonStart() // 시작 버튼 함수
{
	if (m_pThread) // 스레드 중복 방지
	{
		AfxMessageBox(_T("FireBatNIDS 이(가) 실행 중입니다."));
		return;
	}

	m_DlgDevSel = new CDialogDeviceSelect; // 디바이스 선택창 생성
	m_DlgRuleSet = new CDialogRuleSet; // 규칙 설정창 생성
	m_DlgPcapFile = new CFileDialog(false, _T("pcap"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("Pcap Files(*.pcap)|*.pcap|All Files(*.*)|*.*||"), NULL); // 패킷 저장창 생성

	if (m_DlgDevSel->DoModal() == IDCANCEL) // 다비아스 선택창 표시
		return;

	if (m_DlgRuleSet->DoModal() == IDCANCEL) // 규칙 설정창 표시
		return;

	if (m_DlgPcapFile->DoModal() == IDCANCEL) // 패킷 저장창 표시
		return;
	m_strSavePath = m_DlgPcapFile->GetPathName(); // 파일 경로 저장

	m_ctrlListLogText.DeleteAllItems(); // 출력창 초기화
	m_index = 0;

	memset(&m_PktCount, 0, sizeof(m_PktCount));

	m_pThread = AfxBeginThread(CaptureThreadFunc, (LPVOID)this); // 스레드 시작
	if (m_pThread == NULL) // 시작 못 했을 때
	{
		AfxMessageBox(_T("캡처 스레드를 초기화하지 못했습니다."));
		m_ThreadStatus = THREAD_STOP;
		m_ctrlStaticStateText.SetWindowText(_T("상태: 중지(시작하지 못함)"));
		return;
	}

	m_ThreadStatus = THREAD_RUNNING;
	m_ctrlStaticStateText.SetWindowText(_T("상태: 동작중."));
}

UINT CDialogPandisNIDS::CaptureThreadFunc(LPVOID lpParam) // 스레드 함수
{
	CDialogPandisNIDS* PThis = (CDialogPandisNIDS*)lpParam; // this 클래스
	bool bIsError = false; // 에러 감지 변수

	PThis->m_CS.Lock(); // 임계영역 락

	if(!bIsError && ((PThis->m_hPcap = pcap_open_live((CStringA)PThis->m_DlgDevSel->m_strDeviceID, 65536, 1, 1000, PThis->m_lpszErrbuf)) == NULL)) // pcap 캡처 오픈
	{
		AfxMessageBox(_T("디바이스를 열 수 없습니다."));
		bIsError = true;
	}

	if (!bIsError && (pcap_compile(PThis->m_hPcap, &PThis->m_fcode, (CStringA)PThis->m_DlgRuleSet->m_strFilterRule, 1, PCAP_NETMASK_UNKNOWN) < 0)) // 규칙 문자열으로 필터 컴파일
	{
		AfxMessageBox(_T("규칙을 설정하는 과정에서 오류가 발생했습니다."));
		bIsError = true;
	}

	if (!bIsError && (pcap_setfilter(PThis->m_hPcap, &PThis->m_fcode) < 0)) // 필터 세팅
	{
		AfxMessageBox(_T("규칙을 적용하는 과정에서 오류가 발생했습니다."));
		bIsError = true;
	}

	if (!bIsError && ((PThis->m_dumpfile = pcap_dump_open(PThis->m_hPcap, (CStringA)PThis->m_strSavePath)) == NULL)) // 패킷 덤프(저장) 오픈
	{
		AfxMessageBox(_T("파일 저장 경로를 열 수 없습니다."));
		bIsError = true;
	}

	if (bIsError) // 에러 시 Close 부분
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

	/* Close 부분 */
	pcap_close(PThis->m_hPcap);
	pcap_freecode(&PThis->m_fcode);
	PThis->m_pThread = NULL;

	PThis->m_CS.Unlock();
	delete PThis->m_DlgDevSel;
	delete PThis->m_DlgRuleSet;
	delete PThis->m_DlgPcapFile;
	return 0;
}

void CDialogPandisNIDS::packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data) // 패킷 감지될 시 함수
{
	CDialogPandisNIDS* TThis = (CDialogPandisNIDS*)param; // this 클래스

	pcap_dump((u_char*)TThis->m_dumpfile, header, pkt_data); // 받은 패킷을 파일에 저장
	CString strIndex, strProtocol, strSrc, strDst, strInfo; // 각 출력란 문자열

	hdr_t pkth = PacketAnalyzing(pkt_data, TThis->m_PktCount); // 패킷에 대해서 유효한 프로토콜 감지 후 정보 반환

	/* 정보 문자열 만들기 */
	strIndex.Format(_T("%u"), TThis->m_index + 1);
	ProtocolAnalyzing(pkth.type, strProtocol); // 정보에서 프로토콜 파싱
	AddressAnalyzing(pkth, strSrc, strDst); // 정보에서 주소 파싱
	InfoAnalzing(pkth, header->len, strInfo); // 정보에서 패킷 정보 파싱

	TThis->m_PktCount.string += FindStringPacketData(header, pkt_data, TThis->m_DlgRuleSet->m_vtAnalyzeDatas); // 문자열 탐지
	
	/* 정보 문자열 출력 */
	TThis->m_ctrlListLogText.InsertItem(TThis->m_index, strIndex);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 1, LVIF_TEXT, strProtocol, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 2, LVIF_TEXT, strSrc, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 3, LVIF_TEXT, strDst, NULL, NULL, NULL, NULL);
	TThis->m_ctrlListLogText.SetItem(TThis->m_index, 4, LVIF_TEXT, strInfo, NULL, NULL, NULL, NULL);

	TThis->m_index++; // 순서 추가
	TThis->m_ctrlListLogText.EnsureVisible(TThis->m_ctrlListLogText.GetItemCount() - 1, FALSE); // 스크롤를 아래로 내림

	while (TThis->m_ThreadStatus == THREAD_PAUSE); // 일시정지
}

void CDialogPandisNIDS::OnBnClickedButtonStop() // 정지 버튼
{
	if (m_ThreadStatus == THREAD_STOP)
		return;

	m_ThreadStatus = THREAD_STOP;
	m_ctrlStaticStateText.SetWindowText(_T("상태: 중지"));
	pcap_breakloop(m_hPcap); // pcap 강제 정지 명령
}

void CDialogPandisNIDS::OnBnClickedButtonPause() // 일시 정지 버튼
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

void CDialogPandisNIDS::OnBnClickedButtonOpenStatistic() // 통계 창 표시(원래 생성되어 있던것을 표시)
{
	m_DlgStat->ShowWindow(SW_SHOW);
	m_DlgStat->ResumeRefreshThread(); // 스레드를 재개
} 
