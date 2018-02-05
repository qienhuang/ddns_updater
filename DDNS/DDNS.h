

#pragma once

#ifndef __AFXWIN_H__

#endif

#include "resource.h"		


// CDDNSApp:

class CDDNSApp : public CWinApp
{
public:
	CDDNSApp();

	public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CDDNSApp theApp;