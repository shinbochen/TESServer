// DBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "DBSDlg.h"
#include "ConfigDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBSDlg dialog


CDBSDlg::CDBSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDBSDlg)
	m_strConnNumber = _T("");
	m_strLocalIP = _T("");
	m_strRecePack = _T("");
	m_strRumTime = _T("");
	m_strSendPack = _T("");
	m_strState = _T("");
	//}}AFX_DATA_INIT
	m_nRecvPackage = 0;
	m_nSendPackage = 0;
	m_nConnectNumber = 0;
	m_bStart = FALSE;
	m_bShowStatus = FALSE;
}


void CDBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDBSDlg)
	DDX_Control(pDX, IDC_SHOW_CHECK, m_oShowCheck);
	DDX_Control(pDX, IDC_PROPERTY_BUTTON, m_oPropertyButton);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_oConnectionButton);
	DDX_Text(pDX, IDC_CONN_NUMBER, m_strConnNumber);
	DDX_Text(pDX, IDC_LOCAL_IP, m_strLocalIP);
	DDX_Text(pDX, IDC_RECE_PACKATGE, m_strRecePack);
	DDX_Text(pDX, IDC_RUM_TIME, m_strRumTime);
	DDX_Text(pDX, IDC_SEND_PACKATGE, m_strSendPack);
	DDX_Text(pDX, IDC_STATE, m_strState);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDBSDlg, CDialog)
	//{{AFX_MSG_MAP(CDBSDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PROPERTY_BUTTON, OnPropertyButton)
	ON_BN_CLICKED(IDC_SHOW_CHECK, OnShowCheck)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBSDlg message handlers

BOOL CDBSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oConnectionButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	m_oPropertyButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	UpdateWindowState();
	UpdateData(FALSE);
	SetTimer(IDC_TIMER_UPDATE,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
//	更新窗口状态
void  CDBSDlg::UpdateWindowState( void ){
	
	
	m_strRecePack.Format("%d", m_nRecvPackage);
	m_strSendPack.Format("%d", m_nSendPackage );
	m_strConnNumber.Format("%d", m_nConnectNumber );
	
}

//////////////////////////////////////////////////////////////////////////
//  连接状态
void CDBSDlg::IsConnectionStatus( BOOL  bStateus ){
	
	CString				strTime;
	CTime				oTime;
	
	oTime = CTime::GetCurrentTime();
	m_strRumTime = oTime.Format("%B %d, %Y %H:%M:%S");
	if( bStateus ){
		m_strState = "DBServer Run...";
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText("&停 止");		
	}
	else{
		m_strState = "DBServer Stop!";
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText("&开 始");
	}
	UpdateData(FALSE);
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CDBSDlg::OnTimer(UINT nIDEvent) 
{
	if ( IsWindowVisible()){
		UpdateWindowState();
		UpdateData(FALSE);
	}		
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//
void CDBSDlg::OnClose(){

	KillTimer(IDC_TIMER_UPDATE);
	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
//
void CDBSDlg::OnPropertyButton(){

	CConfigDlg	 oConfigDlg;
	
	oConfigDlg.m_nTypeMode = 1;		
	oConfigDlg.m_strIP = m_strTCPIP;
	oConfigDlg.m_nPort1 = m_nTCPPort;
	if ( oConfigDlg.DoModal() == IDOK ){
		if ( m_bStart ){			
			MessageBox( "DBS在运行中,请停止后再做更改!", "提示", MB_ICONWARNING );
		}
		else{
			m_strTCPIP = oConfigDlg.m_strIP;
			m_nTCPPort = oConfigDlg.m_nPort1;
			AfxGetApp()->m_pMainWnd->SendMessage(WM_SAVE_CONFIG, NULL, NULL );
		}		
	}		
}

//////////////////////////////////////////////////////////////////////////
//
void CDBSDlg::OnShowCheck(){

	m_bShowStatus = m_oShowCheck.GetCheck();
}

//////////////////////////////////////////////////////////////////////////
//   显示数据信息
BOOL CDBSDlg::SetRecHelpInfo( int	nDataLen, BOOL	bRecv){
	
	if( nDataLen == 0 ){
		return FALSE;
	}
	if( bRecv ){		
		m_nRecvPackage += 1;
	}
	else{
		m_nSendPackage += 1;
	}
	UpdateWindowState();
	return m_bShowStatus;
}

//////////////////////////////////////////////////////////////////////////
//
void CDBSDlg::OnConnectButton(){
	
	AfxGetApp()->m_pMainWnd->SendMessage(WM_SOCKET_STARTSTOP,0,0);
}
