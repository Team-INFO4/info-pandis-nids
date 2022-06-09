// DialogStatistic.cpp: 구현 파일
//

#include "pch.h"
#include "PandisNIDS.h"
#include "afxdialogex.h"
#include "DialogStatistic.h"


// CDialogStatistic 대화 상자

IMPLEMENT_DYNAMIC(CDialogStatistic, CDialog)

CDialogStatistic::CDialogStatistic(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_STATISTIC, pParent)
{
}

CDialogStatistic::~CDialogStatistic()
{
}

void CDialogStatistic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogStatistic, CDialog)
END_MESSAGE_MAP()


// CDialogStatistic 메시지 처리기
