#pragma once
#include "afxdialogex.h"


// CDialogStatistic 대화 상자

class CDialogStatistic : public CDialog
{
	DECLARE_DYNAMIC(CDialogStatistic)

public:
	CDialogStatistic(Pt_t* PacketCountAddr, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogStatistic();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATISTIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 통계 출력 리스트 컨트롤
	CListCtrl m_ctrlListStatistic;
	// 갱신되는 통계값
	Pt_t* m_pPacketCount;
public:
	// 스레드 클래스
	CWinThread* m_pThread;
	// 갱신 스레드 함수
	static UINT RefreshThreadFunc(LPVOID lpParam);
	// 스레드 시작
	UINT ResumeRefreshThread();
	// 스레드 중지
	void SuspendRefreshThread();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
