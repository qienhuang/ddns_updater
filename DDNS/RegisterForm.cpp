// RegisterForm.cpp :
//

#include "stdafx.h"
#include "DDNS.h"
#include "RegisterForm.h"



IMPLEMENT_DYNAMIC(CRegisterForm, CDialog)

CRegisterForm::CRegisterForm(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterForm::IDD, pParent)
{

}

CRegisterForm::~CRegisterForm()
{
}

void CRegisterForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER2, m_explorer1);
}


BEGIN_MESSAGE_MAP(CRegisterForm, CDialog)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CRegisterForm 



BEGIN_EVENTSINK_MAP(CRegisterForm, CDialog)
END_EVENTSINK_MAP()

void CRegisterForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_explorer1.Navigate(_T("http://ns.dipmap.com/register.aspx"), NULL,NULL,NULL,NULL);

}
