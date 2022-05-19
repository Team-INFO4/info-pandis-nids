#pragma once
#include "afxdialogex.h"

// CDialogDeviceSelect 대화 상자

class CDialogDeviceSelect : public CDialog
{
	DECLARE_DYNAMIC(CDialogDeviceSelect)

public:
	CDialogDeviceSelect(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogDeviceSelect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEVICE_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 디바이스 선택 컨트롤
	CButton m_ctrlSelectButton;
	// 선택한 리스트의 아이템을 저장한다.
	CString m_strSelectedItem;
	// 마지막에 선택한 디바이스 ID를 저장한다.
	CString m_strDeviceID;
	// 현재 인터넷 인터페이스 디바이스를 저장한다.
	pcap_if_t* m_alldevs;
	// 인터페이스 디바이스 구조체 포인터
	pcap_if_t* m_ptrdev;
	// 디바이스 저장 리스트 박스 컨트롤
	CListBox m_ctrlDeviceList;
	// PCAP 에러 버퍼
	char m_strErrBuf[PCAP_ERRBUF_SIZE];
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnLbnSelchangeListDevice();
	afx_msg void OnLbnSelcancelListDevice();
};
