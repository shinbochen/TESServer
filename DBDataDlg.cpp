// CSSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "DBDataDlg.h"
#include "ConfigDlg.h"
#include "TEServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBDataDlg dialog

CDBDataDlg::CDBDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBDataDlg)
	m_strDatabaseName = _T("");
	m_strServerName = _T("");
	m_strStausInfo = _T("");
	m_strUserName = _T("");
	m_strUserPsd = _T("");
	//}}AFX_DATA_INIT
	m_bConnection = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
void CDBDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBDataDlg)
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_oConnectButton);
	DDX_Text(pDX, IDC_DATABASE_NAME, m_strDatabaseName);
	DDX_Text(pDX, IDC_SERVER_NAME, m_strServerName);
	DDX_Text(pDX, IDC_STATIC_INFO, m_strStausInfo);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_PSD, m_strUserPsd);
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDBDataDlg, CDialog)
	//{{AFX_MSG_MAP(CDBDataDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	ON_EN_CHANGE(IDC_SERVER_NAME, OnChangeServerName)
	ON_EN_CHANGE(IDC_DATABASE_NAME, OnChangeServerName)
	ON_EN_CHANGE(IDC_USER_NAME, OnChangeServerName)
	ON_EN_CHANGE(IDC_USER_PSD, OnChangeServerName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBDataDlg message handlers

BOOL CDBDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oConnectButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	UpdateData(FALSE);
	InitLabel( );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void  CDBDataDlg::InitLabel( void ){

	GetDlgItem( IDC_STATIC_DB_CONFIG )->SetWindowText( GetLangStr(TEXT_DATABASE_CONFIG) );
	GetDlgItem( IDC_STATIC_SERVER_NAME )->SetWindowText( GetLangStr(TEXT_SERVER) );
	GetDlgItem( IDC_STATIC_DATABASE_NAME )->SetWindowText( GetLangStr(TEXT_DATABSE) );
	GetDlgItem( IDC_STATIC_USER_NAME )->SetWindowText( GetLangStr(TEXT_USER) );	
	GetDlgItem( IDC_STATIC_USER_PSD )->SetWindowText( GetLangStr(TEXT_PASSWORD) );
	GetDlgItem( IDC_CONNECT_BUTTON )->SetWindowText( GetLangStr(TEXT_START));
}
//////////////////////////////////////////////////////////////////////////
//
void CDBDataDlg::OnTimer(UINT nIDEvent){
	
	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////
//
void CDBDataDlg::OnClose(){		
	
	CDialog::OnClose();
}
//////////////////////////////////////////////////////////////////////////
//
void CDBDataDlg::OnConnectButton(){
	
	UpdateData(TRUE);
	AfxGetMainWnd()->SendMessage(WM_SAVE_CONFIG,NULL,NULL );
	AfxGetMainWnd()->SendMessage( WM_SOCKET_STARTSTOP, 3, 0 );
}
//////////////////////////////////////////////////////////////////////////
//
void CDBDataDlg::IsConnectionStatus(BOOL bFlag ){

	CString			strData;	
	CString			strTime;
	CTime			oTime;
	CTEServerDlg	*pDlg = (CTEServerDlg*)AfxGetMainWnd();
	
	UpdateData(TRUE);
	if (bFlag){				
		m_strStausInfo = "Connected Succeed!";
		strData.Format("%s", GetLangStr(TEXT_STOP) );
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
		GetDlgItem(IDC_DATABASE_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVER_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_USER_PSD)->EnableWindow(FALSE);
		GetDlgItem(IDC_USER_NAME)->EnableWindow(FALSE);
	}
	else {
		m_strStausInfo = "DisConnect Error!";
		strData.Format("%s", GetLangStr(TEXT_START) );
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
		GetDlgItem(IDC_DATABASE_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVER_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_PSD)->EnableWindow(TRUE);
		GetDlgItem(IDC_USER_NAME)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDBDataDlg::OnChangeServerName() 
{
	UpdateData(TRUE);
	AfxGetMainWnd()->SendMessage(WM_SAVE_CONFIG,NULL,NULL );
}
