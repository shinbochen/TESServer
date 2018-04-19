// StopSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "StopSocketDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStopSocketDlg dialog


CStopSocketDlg::CStopSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStopSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStopSocketDlg)
	m_bTCP = FALSE;
	m_bUDP = FALSE;
	//}}AFX_DATA_INIT
}


void CStopSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStopSocketDlg)
	DDX_Control(pDX, IDC_UDP, m_oUDPCheck);
	DDX_Control(pDX, IDC_TCP, m_oTcpCheck);
	DDX_Check(pDX, IDC_TCP, m_bTCP);
	DDX_Check(pDX, IDC_UDP, m_bUDP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStopSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CStopSocketDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStopSocketDlg message handlers

BOOL CStopSocketDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	InitLabel();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void CStopSocketDlg::InitLabel( void ){

	CString		strData;

	strData.Format("%s", GetLangStr(TEXT_CONNECT) );
	GetDlgItem(IDC_STATIC_MODE)->SetWindowText(GetLangStr(TEXT_CONNECT_MODE));
	strData.Format("UDP%s", GetLangStr(TEXT_CONNECT_MODE) );
	GetDlgItem(IDC_UDP)->SetWindowText( strData );
	strData.Format("TCP%s", GetLangStr(TEXT_CONNECT_MODE) );
	GetDlgItem(IDC_TCP)->SetWindowText(strData);	
	GetDlgItem(IDOK)->SetWindowText( GetLangStr(TEXT_OK) );
	GetDlgItem(IDCANCEL)->SetWindowText( GetLangStr(TEXT_CANCEL) );	
}

//////////////////////////////////////////////////////////////////////////
//
void CStopSocketDlg::OnOK() {	

	UpdateData( TRUE );	
	
	if ( m_oTcpCheck.GetCheck() ){
		AfxGetApp()->m_pMainWnd->SendMessage(WM_SOCKET_STARTSTOP,1,0);
	}		
	if ( m_oUDPCheck.GetCheck() ){
		AfxGetApp()->m_pMainWnd->SendMessage(WM_SOCKET_STARTSTOP,2,0);
	}

	CDialog::OnOK();
}
