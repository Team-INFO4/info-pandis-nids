#pragma once
#include "afxdialogex.h"


// CDialogStatistic 대화 상자

class CDialogStatistic : public CDialog
{
	DECLARE_DYNAMIC(CDialogStatistic)

public:
	CDialogStatistic(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogStatistic();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STATISTIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
