// DialogFirewall.cpp: 구현 파일
//

#include "pch.h"
#include "PandisNIDS.h"
#include "afxdialogex.h"
#include "DialogFirewall.h"


// CDialogFirewall 대화 상자

IMPLEMENT_DYNAMIC(CDialogFirewall, CDialog)

CDialogFirewall::CDialogFirewall(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CDialogFirewall::~CDialogFirewall()
{
}

void CDialogFirewall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogFirewall, CDialog)
END_MESSAGE_MAP()


// CDialogFirewall 메시지 처리기
