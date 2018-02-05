// DDNSDlg.cpp : 
//

#include "stdafx.h"
#include "DDNS.h"
#include "DDNSDlg.h"
#include <afxinet.h>
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDDNSDlg::CDDNSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDDNSDlg::IDD, pParent)
	, m_userName(_T(""))
	, m_password(_T(""))
	, m_dwAutorun(0)
	, m_strExternalIP(_T("127.0.0.1"))
	, m_IsExit(FALSE)
	, m_IsFirtTimeShowWindow(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDDNSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_userName);
	DDX_Text(pDX, IDC_EDIT2, m_password);
	DDX_Control(pDX, IDC_EXPLORER1, m_webBowser);
	DDX_Control(pDX, IDC_CHECK1, m_checkBox1);
}

BEGIN_MESSAGE_MAP(CDDNSDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDDNSDlg::OnBnClickedBtnSave)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CDDNSDlg::OnNMClickSyslink1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CDDNSDlg::OnBnClickedCheck1)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYNOTIFYICON, &CDDNSDlg::OnMyNotifyIcon)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU1_EXIT, OnNotifyIconMenuExit)
	ON_COMMAND(ID_MENU_ABOUT, OnAboutDlg)
	ON_COMMAND(ID_MENU_SHOWDDNSCLIENT, OnMenuShowDDNSClient)
END_MESSAGE_MAP()

BOOL CDDNSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	


	return TRUE;  
}

void CDDNSDlg::Init(void)
{

	GetValueFromRegister(&m_userName, &m_password, &m_dwAutorun);
	UpdateData(FALSE);
	m_strExternalIP = GetExternalIP();

	if(m_dwAutorun == 1)
	{
		m_checkBox1.SetCheck(TRUE);
		UpdateIP(m_userName,m_password,m_strExternalIP);
	}
	else
		m_checkBox1.SetCheck(FALSE);

	SetTimer(1,1800000,NULL);
	SetTimer(2,3600000,NULL);


	nc.cbSize=(DWORD)sizeof(NOTIFYICONDATA);	//
	nc.hWnd=m_hWnd;					//
	nc.uID=(UINT)m_hIcon;      //
	nc.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; //
	nc.uCallbackMessage=WM_MYNOTIFYICON;//
	nc.hIcon=m_hIcon;         //
	wcscpy(nc.szTip,_T("DDNS Client"));//
	Shell_NotifyIcon(NIM_ADD,&nc);
}


void CDDNSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		//
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


HCURSOR CDDNSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDDNSDlg::OnBnClickedBtnSave()
{
	UpdateData(TRUE);
	if(m_checkBox1.GetCheck())
		m_dwAutorun =1;
	else
		m_dwAutorun =0;

	SetValueToRegister(m_userName, m_password, m_dwAutorun);

	if(m_userName=="" || m_password=="")
	{
		return;
	}
	UpdateIP(m_userName, m_password, m_strExternalIP);
}

void CDDNSDlg::UpdateIP(CString strUserName, CString strPassword, CString strIpAddress)
{
	if(strUserName=="" || strPassword=="" || strIpAddress=="")
		return;
	CString strUrl;
	strUrl.Format(_T("http://ns.dipmap.com/updateip.aspx?username=%s&password=%s&ipaddress=%s"),
						strUserName, strPassword, strIpAddress);
	m_webBowser.Navigate(strUrl,NULL,NULL,NULL,NULL);
}

void CDDNSDlg::GetValueFromRegister(CString* lpstrUserName, CString* lpstrPassword, DWORD* lpdwAutorun)
{
	HKEY	hKey;
	LONG	ret1 =::RegOpenKeyEx(HKEY_CURRENT_USER,_T("Software\\DNSClient\\DNSClient\\Profile"),
		0, KEY_READ, &hKey );
	if(ret1!=ERROR_SUCCESS)
	{
		return;
	}

	DWORD keyTypeSZ  = REG_SZ;
	DWORD keyTypeDW  = REG_DWORD;
	DWORD cbDataID = 255;
	DWORD cbDataPassword = 255;
	DWORD cbDataAutorun = 4;
	LONG  ret2 = ::RegQueryValueEx(hKey, _T("ID"), NULL, &keyTypeSZ, (LPBYTE)lpstrUserName->GetBuffer(255), &cbDataID);
	LONG  ret3 = ::RegQueryValueEx(hKey, _T("Password"), NULL, &keyTypeSZ, (LPBYTE)lpstrPassword->GetBuffer(255), &cbDataPassword); 
	LONG  ret4 = ::RegQueryValueEx(hKey, _T("Autorun"), NULL, &keyTypeDW, (LPBYTE)lpdwAutorun, &cbDataAutorun);

	::RegCloseKey(hKey);
}

void CDDNSDlg::SetValueToRegister(CString strUserName, CString strPassword, DWORD dwAutorun)
{
	HKEY	hKey;
	LONG	ret1 =::RegCreateKey(HKEY_CURRENT_USER, _T("Software\\DNSClient\\DNSClient\\Profile"), &hKey );
	LONG	ret2 = 0;

	HKEY	hKeyAutoRun;
	LONG	ret3 =::RegCreateKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &hKeyAutoRun);

	TCHAR* szUserName ;
	TCHAR* szPassword ;

	szUserName = strUserName.GetBuffer(strUserName.GetLength());
	szPassword = strPassword.GetBuffer(strPassword.GetLength());

	TCHAR pFileName[MAX_PATH] = {0}; 
	DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
	CString strPath = pFileName;

	if(ret1 == ERROR_SUCCESS)
	{

		::RegSetValueEx(hKey,_T("ID"),NULL, REG_SZ, (LPBYTE)szUserName, strUserName.GetLength()*2);
		::RegSetValueEx(hKey,_T("Password"),NULL, REG_SZ, (LPBYTE)szPassword, strPassword.GetLength()*2);
		::RegSetValueEx(hKey,_T("Autorun"),NULL, REG_DWORD, (LPBYTE)&dwAutorun, 4);

	}
	
	if(ret3 == ERROR_SUCCESS)
	{
		if(m_dwAutorun==1)
		{
			::RegSetValueEx(hKeyAutoRun,_T("DDNS Client"),NULL, REG_EXPAND_SZ, (LPBYTE)pFileName, strPath.GetLength()*2);
		}
		else
		{
			::RegDeleteValue(hKeyAutoRun, _T("DDNS Client"));
		}
	}
	else
	{
		MessageBox(_T("Set run at startup fail!"));
	}

	if(ret2!=ERROR_SUCCESS)
	{
		MessageBox(_T("Save username and password fail!"));
	}
}

CString CDDNSDlg::GetExternalIP(void)
{
	CString strIP;
	CInternetSession sess;
	CHttpFile* pHttpFile = NULL;
	pHttpFile = (CHttpFile*)sess.OpenURL(_T("http://checkip.dyndns.com/"));

	try
	{
		CString strRecived;
		char arRecived[1024];
		if(pHttpFile)
		{
			while(pHttpFile->ReadString((LPTSTR)arRecived,1024))
			{
				strIP += arRecived;   
			}
		}
		else
		{
			return _T("127.0.0.1");
		}

		int iLocate =strIP.Find(_T("Address:"));
		strIP.Delete(0, iLocate+8);
		iLocate = strIP.Find(_T("<"));
		strIP.Delete(iLocate, strIP.GetLength()-1);
		while(strIP.Remove(' '));

		pHttpFile->Close();
		sess.Close();	
	}
	catch (CException* e)
	{
	}


	return strIP;
}



void CDDNSDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_dlgRegister.DoModal();

	*pResult = 0;
}

void CDDNSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		m_strExternalIP = GetExternalIP();
		UpdateData(TRUE);
		if(m_userName=="" || m_password=="")
		{
			return;
		}
		UpdateIP(m_userName, m_password, m_strExternalIP);
	}

	if(nIDEvent==2 || m_userName != "")
	{
		try
		{
			CInternetSession sess;
			CHttpFile* pHttpFile;
			CString strUrl;
			strUrl.Format(_T("http://%s.dipmap.com/"), m_userName);
			pHttpFile =(CHttpFile*) sess.OpenURL(strUrl);
			pHttpFile->Close();
			sess.Close();

		}
		catch(CException* e)
		{
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDDNSDlg::OnBnClickedCheck1()
{
	if(m_checkBox1.GetCheck()==TRUE)
		m_dwAutorun = 1;
	else
		m_dwAutorun = 0;
}

void CDDNSDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(m_IsFirtTimeShowWindow)
	{
		m_IsFirtTimeShowWindow = 0;
		Init();
	}
}

void CDDNSDlg::OnDestroy()
{
	CDialog::OnDestroy();

	Shell_NotifyIcon(NIM_DELETE,&nc);
}

LRESULT  CDDNSDlg::OnMyNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case  WM_LBUTTONDBLCLK:
		{
			if(::IsWindowVisible(m_hWnd))
			{
				ShowWindow(SW_MINIMIZE);
				ShowWindow(SW_HIDE);

			}
			else
			{
				ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOWNORMAL);
			}
			break;
		}
	case WM_RBUTTONDOWN:
		{
			CMenu menu ;
			CPoint curpos;
			GetCursorPos(&curpos);
			if(menu.LoadMenu(IDR_MENU1))
			{
				CMenu* pPopup;
				pPopup = menu.GetSubMenu(0);
				ASSERT(pPopup != NULL);
				SetForegroundWindow();
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
					curpos.x,curpos.y,AfxGetMainWnd());
			}
			break;
		}
	default :
		{
			break;
		}
	}
	return 0;
}
void CDDNSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_CLOSE)
	{
		if(m_IsExit)
		{
			CDialog::OnSysCommand(nID, lParam);
		}
		else
		{
			ShowWindow(SW_HIDE);
		}
	}
	else
		CDialog::OnSysCommand(nID, lParam);
}

void CDDNSDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
}

void CDDNSDlg::OnNotifyIconMenuExit(void)
{
	m_IsExit = TRUE;
	PostMessage(WM_QUIT);
}

void CDDNSDlg::OnAboutDlg(void)
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CDDNSDlg::OnMenuShowDDNSClient(void)
{
	if(!IsWindowVisible())
	{
		ShowWindow(SW_SHOW);
		ShowWindow(SW_SHOWNORMAL);
	}
}
