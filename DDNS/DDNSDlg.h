//

#pragma once
#include "explorer1.h"
#include "afxwin.h"
#include "hyperlink.h"
#include "RegisterForm.h"

#define WM_MYNOTIFYICON WM_USER+101

class CDDNSDlg : public CDialog
{

public:
	CDDNSDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_DDNS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMyNotifyIcon(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString m_userName;
	CString m_password;
	CString GetExternalIP(void);
	CString m_strExternalIP;
	void UpdateIP(CString strUserName, CString strPassword, CString strIpAddress);
	void GetValueFromRegister(CString* lpstrUserName, CString* lpstrPassword, DWORD* lpdwAutorun);
	void SetValueToRegister(CString strUserName, CString strPassword, DWORD dwAutorun);
	void Init(void);
	CButton m_checkBox1;
	CRegisterForm m_dlgRegister;
	CExplorer1 m_webBowser;
	DWORD m_dwAutorun;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL	m_IsShow;
	BOOL m_IsExit;
	BOOL m_IsFirtTimeShowWindow;
	NOTIFYICONDATA nc;
	void OnNotifyIconMenuExit(void);
	void OnAboutDlg(void);
	void OnMenuShowDDNSClient(void);
};
