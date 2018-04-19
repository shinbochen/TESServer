// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_strIPName = _T("");
	m_strNamePort1 = _T("");
	m_strPort2 = _T("");
	m_strIP = _T("");
	m_nPort1 = 0;
	m_nPort2 = 0;
	//}}AFX_DATA_INIT
	m_nTypeMode = 0;
}

//////////////////////////////////////////////////////////////////////////
//
void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDOK, m_oOn);
	DDX_Control(pDX, IDCANCEL, m_oCancel);
	DDX_Text(pDX, IDC_IP_NAME, m_strIPName);
	DDX_Text(pDX, IDC_PORT1, m_strNamePort1);
	DDX_Text(pDX, IDC_PORT2, m_strPort2);
	DDX_Text(pDX, IDC_EDIT1, m_strIP);
	DDX_Text(pDX, IDC_EDIT2, m_nPort1);
	DDX_Text(pDX, IDC_EDIT3, m_nPort2);
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oOn.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	m_oCancel.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);

	CString		strData;

	if ( m_nTypeMode == 0 ) {
		GetDlgItem(IDC_PORT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_SHOW);
		strData.Format("%s:", GetLangStr(TEXT_SERVER_IP) );
		GetDlgItem(IDC_IP_NAME)->SetWindowText(strData);
		strData.Format("TCP%s:", GetLangStr(TEXT_PORT) );
		GetDlgItem(IDC_PORT1)->SetWindowText(strData);
		strData.Format("UDP%s:", GetLangStr(TEXT_PORT) );
		GetDlgItem(IDC_PORT2)->SetWindowText(strData);
	}
	else{
		GetDlgItem(IDC_PORT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
		strData.Format("%s:", GetLangStr(TEXT_SERVER_IP) );
		GetDlgItem(IDC_IP_NAME)->SetWindowText(strData);
		
		strData.Format("TCP%s:", GetLangStr(TEXT_PORT) );
		GetDlgItem(IDC_PORT1)->SetWindowText(strData);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	}
	InitLabel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void  CConfigDlg::InitLabel( void ){
	GetDlgItem(IDOK)->SetWindowText(GetLangStr(TEXT_OK));
	GetDlgItem(IDCANCEL)->SetWindowText(GetLangStr(TEXT_CANCEL));
}
//////////////////////////////////////////////////////////////////////////
//
void CConfigDlg::OnOK(){
	
	UpdateData(TRUE);
	CDialog::OnOK();
}
