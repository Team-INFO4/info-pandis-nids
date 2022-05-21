
// PandisNIDS.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "PandisNIDS.h"
#include "DialogPandisNIDS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPandisNIDSApp

BEGIN_MESSAGE_MAP(CPandisNIDSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPandisNIDSApp 생성

CPandisNIDSApp::CPandisNIDSApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CPandisNIDSApp 개체입니다.

CPandisNIDSApp theApp;


// CPandisNIDSApp 초기화

BOOL CPandisNIDSApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	SetRegistryKey(_T("FireBat NIDS Copyright 2022. kmj36 All rights reserved."));

	CDialogPandisNIDS dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

