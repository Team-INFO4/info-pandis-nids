// DialogRuleSet.cpp: 구현 파일
//

#include "pch.h"
#include "PandisNIDS.h"
#include "afxdialogex.h"
#include "DialogRuleSet.h"


// CDialogRuleSet 대화 상자

IMPLEMENT_DYNAMIC(CDialogRuleSet, CDialog)

CDialogRuleSet::CDialogRuleSet(CWnd* pParent /*=nullptr*/) // 초기화
	: CDialog(IDD_DIALOG_RULE_SET, pParent)
	, m_nSourcePort(0)
	, m_nDestinationPort(0)
	, m_strAnalyzeData(_T(""))
	, m_strFilterRule(_T(""))
{
}

CDialogRuleSet::~CDialogRuleSet()
{
}

void CDialogRuleSet::DoDataExchange(CDataExchange* pDX) // 화면과 데이터 교환
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RULE_PROTOCOL_SEL, m_ctrlProtocolComboBox);
	DDX_Control(pDX, IDC_IPADDRESS_RULE_SOURCE_IP, m_ctrlSourceIP);
	DDX_Text(pDX, IDC_EDIT_RULE_SOURCE_PORT, m_nSourcePort);
	DDX_Control(pDX, IDC_IPADDRESS_RULE_DESTINATION_IP, m_ctrlDestinationIP);
	DDX_Text(pDX, IDC_EDIT_RULE_DESTINATION_PORT, m_nDestinationPort);
	DDX_Control(pDX, IDC_CHECK_RULE_SOURCE_ANY_IP, m_ctrlAnySrcIPCheckBox);
	DDX_Control(pDX, IDC_CHECK_RULE_SOURCE_ANY_PORT, m_ctrlAnySrcPortCheckBox);
	DDX_Control(pDX, IDC_CHECK_RULE_DESTINATION_ANY_IP, m_ctrlAnyDstIPCheckBox);
	DDX_Control(pDX, IDC_CHECK_RULE_DESTINATION_ANY_PORT, m_ctrlAnyDstPortCheckBox);
	DDV_MinMaxInt(pDX, IDC_EDIT_RULE_SOURCE_PORT, 0, USHORT_MAX);
	DDV_MinMaxInt(pDX, IDC_EDIT_RULE_DESTINATION_PORT, 0, USHORT_MAX);
	DDX_Text(pDX, IDC_EDIT_RULE_ANALYZE_VALUE, m_strAnalyzeData);
	DDX_Control(pDX, IDC_LIST_RULE_LIST, m_ctrlRuleList);
}


BEGIN_MESSAGE_MAP(CDialogRuleSet, CDialog) // 이벤트와 함수를 연동
	ON_BN_CLICKED(IDC_CHECK_RULE_SOURCE_ANY_IP, &CDialogRuleSet::OnBnClickedCheckRuleSourceAnyIp)
	ON_BN_CLICKED(IDC_CHECK_RULE_SOURCE_ANY_PORT, &CDialogRuleSet::OnBnClickedCheckRuleSourceAnyPort)
	ON_BN_CLICKED(IDC_CHECK_RULE_DESTINATION_ANY_IP, &CDialogRuleSet::OnBnClickedCheckRuleDestinationAnyIp)
	ON_BN_CLICKED(IDC_CHECK_RULE_DESTINATION_ANY_PORT, &CDialogRuleSet::OnBnClickedCheckRuleDestinationAnyPort)
	ON_BN_CLICKED(IDC_BUTTON_RULE_APPLY, &CDialogRuleSet::OnBnClickedButtonRuleApply)
	ON_BN_CLICKED(IDC_BUTTON_RULE_DELETE, &CDialogRuleSet::OnBnClickedButtonRuleDelete)
	ON_BN_CLICKED(IDOK, &CDialogRuleSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RULE_IMPORT, &CDialogRuleSet::OnBnClickedButtonRuleImport)
	ON_BN_CLICKED(IDC_BUTTON_RULE_EXPORT, &CDialogRuleSet::OnBnClickedButtonRuleExport)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RULE_LIST_CHANGE_NUMBER, &CDialogRuleSet::OnDeltaposSpinRuleListChangeNumber)
	ON_LBN_SELCHANGE(IDC_LIST_RULE_LIST, &CDialogRuleSet::OnLbnSelchangeListRuleList)
END_MESSAGE_MAP()


// CDialogRuleSet 메시지 처리기

BOOL CDialogRuleSet::OnInitDialog() // 추가 초기화
{
	CDialog::OnInitDialog();

	/* 콤보박스에 내용 추가 */
	m_ctrlProtocolComboBox.AddString(_T("TCP"));
	m_ctrlProtocolComboBox.AddString(_T("UDP"));
	m_ctrlProtocolComboBox.AddString(_T("ICMP"));
	m_ctrlProtocolComboBox.AddString(_T("ICMP6"));
	m_ctrlProtocolComboBox.AddString(_T("ARP"));
	m_ctrlProtocolComboBox.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDialogRuleSet::OnBnClickedCheckRuleSourceAnyIp() // 모든 출발지 ip 체크 박스 여부
{
	if (m_ctrlAnySrcIPCheckBox.GetCheck()) // 체크에 따라 ip 입력칸 활성화 비활성화
		m_ctrlSourceIP.EnableWindow(FALSE);
	else
		m_ctrlSourceIP.EnableWindow(TRUE);
}


void CDialogRuleSet::OnBnClickedCheckRuleSourceAnyPort() // 모든 도착지 ip 체크 박스 여부
{
	if (m_ctrlAnySrcPortCheckBox.GetCheck()) // 체크에 따라 ip 입력칸 활성화 비활성화
		GetDlgItem(IDC_EDIT_RULE_SOURCE_PORT)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_EDIT_RULE_SOURCE_PORT)->EnableWindow(TRUE);
}


void CDialogRuleSet::OnBnClickedCheckRuleDestinationAnyIp() // 모든 출발지 port 체크 박스 여부
{
	if (m_ctrlAnyDstIPCheckBox.GetCheck()) // 체크에 따라 port 입력칸 활성화 비활성화
		m_ctrlDestinationIP.EnableWindow(FALSE);
	else
		m_ctrlDestinationIP.EnableWindow(TRUE);
}


void CDialogRuleSet::OnBnClickedCheckRuleDestinationAnyPort() // 모든 도착지 port 체크 박스 여부
{
	if (m_ctrlAnyDstPortCheckBox.GetCheck()) // 체크에 따라 port 입력칸 활성화 비활성화
		GetDlgItem(IDC_EDIT_RULE_DESTINATION_PORT)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_EDIT_RULE_DESTINATION_PORT)->EnableWindow(TRUE);
}

void CDialogRuleSet::OnBnClickedButtonRuleApply() // 규칙 적용 버튼
{
	UpdateData(TRUE); // 화면에서 변수로 데이터를 동기화

	if (m_ctrlProtocolComboBox.GetCurSel() == LB_ERR)
		return;

	CString strRule; // 규칙을 만들 문자열
	CString strProtocol, strSrcIp, strSrcPort, strDstIp, strDstPort; // ip와 포트 문자열

	m_ctrlProtocolComboBox.GetLBText(m_ctrlProtocolComboBox.GetCurSel(), strProtocol); // 프로토콜 문자열을 가져옴

	/* ip와 포트를 가져옴 */
	strSrcPort.Format(_T("%u"), m_nSourcePort);
	strDstPort.Format(_T("%u"), m_nDestinationPort);
	GetDlgItemText(IDC_IPADDRESS_RULE_SOURCE_IP, strSrcIp);
	GetDlgItemText(IDC_IPADDRESS_RULE_DESTINATION_IP, strDstIp);

	/* 패킷 규칙 필터을 만듬 (ip 또는 port가 0이면 스킵) */
	strRule += strProtocol.MakeLower();

	if (strSrcIp != "0.0.0.0" && !m_ctrlAnySrcIPCheckBox.GetCheck())
		strRule += _T(" and src host ") + strSrcIp;

	if (strSrcPort != '0' && !m_ctrlAnySrcPortCheckBox.GetCheck())
		strRule += _T(" and src port ") + strSrcPort;

	if (strDstIp != "0.0.0.0" && !m_ctrlAnyDstIPCheckBox.GetCheck())
		strRule += _T(" and dst host ") + strDstIp;

	if (strDstPort != '0' && !m_ctrlAnyDstPortCheckBox.GetCheck())
		strRule += _T(" and dst port ") + strDstPort;

	if (!m_strAnalyzeData.IsEmpty())
		strRule += _T("->") + m_strAnalyzeData; // 패킷에서 감지할 문자열 (화면과 연결되어 있어 변경 시 자동으로 내용이 바뀜.)

	if (m_ctrlRuleList.FindStringExact(-1, strRule) == LB_ERR) // 중복 방지 후 추가
	{
		m_ctrlRuleList.AddString(strRule);
		m_vtAnalyzeDatas.push_back(m_strAnalyzeData); // 패킷에서 감지할 문자열을 백터에 추가
		SetLbVScrollLength(); // 세로 스크롤 막대 관련
	}
}


void CDialogRuleSet::OnBnClickedButtonRuleDelete() // 규칙 삭제 버튼
{
	int nSelectIndex;
	if ((nSelectIndex = m_ctrlRuleList.GetCurSel()) == LB_ERR)
		return;

	m_ctrlRuleList.DeleteString(nSelectIndex); // 삭제
	if (!m_vtAnalyzeDatas.empty())
		m_vtAnalyzeDatas.erase(m_vtAnalyzeDatas.begin() + nSelectIndex); // 감지할 문자열 삭제

	m_ctrlRuleList.SetCurSel(nSelectIndex);
	SetLbVScrollLength(); // 세로 스크롤 막대 관련
}


void CDialogRuleSet::OnBnClickedOk() // 확인 버튼
{
	CString strTemp; // 임시로 규칙을 저장할 문자열
	CString strRule; // 결과적으로 만들어 지는 규칙

	for (int i = 0, len = m_ctrlRuleList.GetCount(); i < len; i++) // 모든 규칙을 or로 묶음
	{
		m_ctrlRuleList.GetText(i, strTemp);

		int nSeparatoridx = strTemp.Find(_T("->"));
		if (nSeparatoridx > -1)
		{
			strRule = strTemp.Mid(0, nSeparatoridx);
			m_strFilterRule += strRule;
		}
		else
			m_strFilterRule += strTemp;

		if (i + 1 == len)
			break;

		m_strFilterRule += " or ";
	}

	CDialog::OnOK();
}


void CDialogRuleSet::OnBnClickedButtonRuleImport() // 규칙을 ini 파일에서 가져옴
{
	CFileDialog dlg(true, _T("*.ini"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("INI Files(*.ini)|*.ini|"), NULL); // ini 파일만 표시

	if (dlg.DoModal() != IDOK) // "가져오기" 창 생성
		return;

	CString strFilePath; // 파일 경로
	CStdioFile fileRule; // ini 규칙 파일
	CFileException fileEx; // 파일 예외처리

	strFilePath = dlg.GetPathName();

	if (fileRule.Open(strFilePath, CFile::modeReadWrite, &fileEx)) // ini 규칙 파일 열기
	{
		CString strReadData; // 규칙 읽기
		CString strAnalyzeData; // 감지할 문자열 데이터 가져오기
		int i = 0;

		m_ctrlRuleList.ResetContent(); // 초기화
		m_vtAnalyzeDatas.clear(); // 초기화

		while (fileRule.ReadString(strReadData)) // 하나씩 규칙 읽기
		{
			if (m_ctrlRuleList.FindStringExact(-1, strReadData) != LB_ERR)
				continue;

			int tokenindex = strReadData.Find(_T("->"));
			m_ctrlRuleList.InsertString(i, strReadData);

			if (tokenindex == -1)
				m_vtAnalyzeDatas.push_back(_T(""));
			else
			{
				strAnalyzeData = strReadData.Mid(tokenindex + 2);
				m_vtAnalyzeDatas.push_back(strAnalyzeData);
			}

			i++;
		}

		fileRule.Close();
		UpdateData(FALSE);
		SetLbVScrollLength();
	}
	else
		AfxMessageBox(_T("파일을 열 수 없습니다. 에러 = %d\n"), fileEx.m_cause);
}


void CDialogRuleSet::OnBnClickedButtonRuleExport() // ini 규칙 파일 내보내기
{
	CFileDialog dlg(false, _T("*.ini"), NULL, OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT, _T("INI Files(*.ini)|*.ini|"), NULL); // ini 규칙 파일만 표시

	if (dlg.DoModal() != IDOK) // "다른 이름으로 저장" 창 생성
		return;

	CString strFilePath;
	CStdioFile fileRule;
	CFileException fileEx;

	strFilePath = dlg.GetPathName(); // 파일 경로 가져오기

	if (fileRule.Open(strFilePath, CFile::modeCreate | CFile::modeReadWrite, &fileEx)) // ini 규칙 파일 생성
	{
		UpdateData(TRUE); // 화면에서 데이터를 가져옴
		CString strRuleValue;

		for (int i = 0, len = m_ctrlRuleList.GetCount(); i < len; i++) // 규칙을 파일에 한줄씩 저장
		{
			m_ctrlRuleList.GetText(i, strRuleValue);
			fileRule.WriteString(strRuleValue + "\n");
		}

		fileRule.Close();
		AfxMessageBox(strFilePath + _T(" 저장되었습니다."));
	}
	else
		AfxMessageBox(_T("파일을 열 수 없습니다. 에러 = %d\n"), fileEx.m_cause);
}


void CDialogRuleSet::OnDeltaposSpinRuleListChangeNumber(NMHDR* pNMHDR, LRESULT* pResult) // 규칙 우선순위 변경
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(TRUE);
	int nSel = m_ctrlRuleList.GetCurSel();
	int nCount = m_ctrlRuleList.GetCount();
	int move;

	if (nSel != LB_ERR)
	{
		CString strRuleTemp;
		CString strvtTemp;

		if (nCount == 1)
			return;

		if (pNMUpDown->iDelta < 0)
		{
			if (nSel == 0)
				return;
			move = -1;
		}
		else
		{
			if (nSel == nCount - 1)
				return;
			move = 1;
		}

		m_ctrlRuleList.GetText(nSel, strRuleTemp);
		m_ctrlRuleList.DeleteString(nSel);
		m_ctrlRuleList.InsertString(nSel + move, strRuleTemp);

		strvtTemp = m_vtAnalyzeDatas[nSel];
		m_vtAnalyzeDatas[nSel] = m_vtAnalyzeDatas[nSel + move];
		m_vtAnalyzeDatas[nSel + move] = strvtTemp;

		m_ctrlRuleList.SetCurSel(nSel + move);
	}
	*pResult = 0;
}


void CDialogRuleSet::OnLbnSelchangeListRuleList() // 규칙을 선택했을 때 자동으로 컨트롤에 내용을 표시
{
	
	int nSel = m_ctrlRuleList.GetCurSel();
	if (nSel != LB_ERR)
	{
		int nPos = 0;
		CString strSelData;
		CString strRule;
		CString strToken;

		m_ctrlRuleList.GetText(nSel, strSelData);
		strRule = strSelData.Tokenize(_T("->"), nPos);
		nPos = 0;

		m_ctrlSourceIP.ClearAddress(); // 초기화
		m_nSourcePort = 0;
		m_ctrlDestinationIP.ClearAddress();
		m_nDestinationPort = 0;
		m_strAnalyzeData = _T("");

		m_ctrlProtocolComboBox.SetCurSel(m_ctrlProtocolComboBox.FindString(0, strRule.Tokenize(_T(" "), nPos))); // 프로토콜

		while ((strToken = strRule.Tokenize(_T(" "), nPos)) != _T("")) // ip, port
		{
			if (strToken == _T("src"))
			{
				strToken = strRule.Tokenize(_T(" "), nPos);
				if (strToken == _T("host"))
				{
					strToken = strRule.Tokenize(_T(" "), nPos);
					m_ctrlSourceIP.SetWindowText(strToken);
				}
				else // port
				{
					strToken = strRule.Tokenize(_T(" "), nPos);
					m_nSourcePort = _ttoi(strToken);
				}
			}

			if (strToken == _T("dst"))
			{
				strToken = strRule.Tokenize(_T(" "), nPos);
				if (strToken == _T("host"))
				{
					strToken = strRule.Tokenize(_T(" "), nPos);
					m_ctrlDestinationIP.SetWindowText(strToken);
				}
				else // port
				{
					strToken = strRule.Tokenize(_T(" "), nPos);
					m_nDestinationPort = _ttoi(strToken);
				}
			}
		}

		int tokenindex = strSelData.Find(_T("->")); // 데이터
		if (tokenindex != -1)
			m_strAnalyzeData = strSelData.Mid(tokenindex + 2);

		UpdateData(FALSE);
	}
}

void CDialogRuleSet::SetLbVScrollLength() // 세로 스크롤 막대 관련
{
	CString str;
	CSize sz;
	CDC* pDC = m_ctrlRuleList.GetDC();
	int dx = 0;

	for (int i = 0, len = m_ctrlRuleList.GetCount(); i < len; i++)
	{
		m_ctrlRuleList.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_ctrlRuleList.ReleaseDC(pDC);

	m_ctrlRuleList.SetHorizontalExtent(dx);
}