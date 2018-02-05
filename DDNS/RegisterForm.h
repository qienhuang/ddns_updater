#pragma once
#include "explorer1.h"



class CRegisterForm : public CDialog
{
	DECLARE_DYNAMIC(CRegisterForm)

public:
	CRegisterForm(CWnd* pParent = NULL);  
	virtual ~CRegisterForm();

	enum { IDD = IDD_DLGREGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_explorer1;
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
