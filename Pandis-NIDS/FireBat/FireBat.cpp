#include "stdafx.h"
#include "FireBat.h"
#include "DialogFireBat.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFireBatApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CFireBatApp::CFireBatApp()
{
}

CFireBatApp theApp;

BOOL CFireBatApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	SetRegistryKey(_T("FireBat NIDS Copyright 2022. kmj36 All rights reserved."));

	CDialogFireBat dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
