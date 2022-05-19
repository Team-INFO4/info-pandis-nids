#pragma once
#include "afxdialogex.h"

// CDialogRuleSet 대화 상자

class CDialogRuleSet : public CDialog
{
	DECLARE_DYNAMIC(CDialogRuleSet)

public:
	CDialogRuleSet(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogRuleSet();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RULE_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 프로토콜 콤보 박스
	CComboBox m_ctrlProtocolComboBox;
	// 출발지 ip 컨트롤
	CIPAddressCtrl m_ctrlSourceIP;
	// 출발지 port 숫자값
	UINT m_nSourcePort;
	// 도착지 ip 컨트롤
	CIPAddressCtrl m_ctrlDestinationIP;
	// 도착지 port 숫자값
	UINT m_nDestinationPort;
	// 패킷에서 감지할 데이터
	CString m_strAnalyzeData;
public:
	// 모든 출발지 ip 체크박스 컨트롤
	CButton m_ctrlAnySrcIPCheckBox;
	// 모든 출발지 포트 체크박스 컨트롤
	CButton m_ctrlAnySrcPortCheckBox;
	// 모든 도착지 ip 체크박스 컨트롤
	CButton m_ctrlAnyDstIPCheckBox;
	// 모든 도착지 포트 체크박스 컨트롤
	CButton m_ctrlAnyDstPortCheckBox;
public:
	// 규칙 전체 문자열
	CString m_strFilterRule;
	// 감지할 데이터 벡터
	std::vector<CString> m_vtAnalyzeDatas;
	// 필터 규칙 리스트박스
	CListBox m_ctrlRuleList;
	// 가로 스크롤 조정
	void SetLbVScrollLength();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckRuleSourceAnyIp();
	afx_msg void OnBnClickedCheckRuleSourceAnyPort();
	afx_msg void OnBnClickedCheckRuleDestinationAnyIp();
	afx_msg void OnBnClickedCheckRuleDestinationAnyPort();
	afx_msg void OnBnClickedButtonRuleApply();
	afx_msg void OnBnClickedButtonRuleDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonRuleImport();
	afx_msg void OnBnClickedButtonRuleExport();
	afx_msg void OnDeltaposSpinRuleListChangeNumber(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLbnSelchangeListRuleList();
};
