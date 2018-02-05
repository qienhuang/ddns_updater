// DDNS.cpp 
//

#include "stdafx.h"
#include "DDNS.h"
#include "DDNSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDDNSApp

BEGIN_MESSAGE_MAP(CDDNSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDDNSApp

CDDNSApp::CDDNSApp()
{

}




CDDNSApp theApp;




BOOL CDDNSApp::InitInstance()
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();


	SetRegistryKey(_T("DDNS Client"));

	CDDNSDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}


	return FALSE;
}
