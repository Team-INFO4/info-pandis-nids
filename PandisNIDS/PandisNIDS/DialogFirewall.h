#pragma once
#include "afxdialogex.h"


// CDialogFirewall 대화 상자

class CDialogFirewall : public CDialog
{
	DECLARE_DYNAMIC(CDialogFirewall)

public:
	CDialogFirewall(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogFirewall();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
