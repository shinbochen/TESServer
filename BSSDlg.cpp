// BSSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "BSSDlg.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBSSDlg dialog
CBSSDlg::CBSSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBSSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBSSDlg)
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


void CBSSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBSSDlg)
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


BEGIN_MESSAGE_MAP(CBSSDlg, CDialog)
	//{{AFX_MSG_MAP(CBSSDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PROPERTY_BUTTON, OnPropertyButton)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SHOW_CHECK, OnShowCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBSSDlg message handlers

BOOL CBSSDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oConnectionButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	m_oPropertyButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	InitLabel();
	UpdateWindowState();
	UpdateData(FALSE);
	SetTimer(IDC_TIMER_UPDATE,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void  CBSSDlg::InitLabel( void ){
	
	CString		strData;
		
	GetDlgItem( IDC_STATIC_CONN_GROUP )->SetWindowText( GetLangStr(TEXT_CONNECT) );
	strData.Format("%s:", GetLangStr(TEXT_STATUS) );
	GetDlgItem( IDC_STATIC_STATE )->SetWindowText( strData );
	strData.Format("%s%s:", GetLangStr(TEXT_RUN), GetLangStr(TEXT_TIME));
	GetDlgItem( IDC_STATIC_RUM_TIME )->SetWindowText( strData );
	GetDlgItem( IDC_STATIC_ACTIVITY )->SetWindowText( GetLangStr(TEXT_ACTIVITY) );
	GetDlgItem( IDC_STATIC_SEND )->SetWindowText( GetLangStr(TEXT_SEND) );
	GetDlgItem( IDC_STATIC_RECV )->SetWindowText( GetLangStr(TEXT_RECEIVE) );
	strData.Format("%s:", GetLangStr(TEXT_DATA_PACKATGE) );
	GetDlgItem( IDC_STATIC_SEND_PACKATGE )->SetWindowText( strData );
	strData.Format("%s:", GetLangStr(TEXT_CONNECT_DATA) );
	GetDlgItem( IDC_STATIC_CONN_NUMBER )->SetWindowText( strData );
	GetDlgItem( IDC_SHOW_CHECK )->SetWindowText( GetLangStr(TEXT_SHOW) );
	GetDlgItem( IDC_PROPERTY_BUTTON )->SetWindowText( GetLangStr(TEXT_PROPERTY) );
	GetDlgItem( IDC_CONNECT_BUTTON )->SetWindowText( GetLangStr(TEXT_START) );
	GetDlgItem(IDC_STATIC_LOCAL_IP)->SetWindowText( GetLangStr(TEXT_LOCAL_IP) );
}
//////////////////////////////////////////////////////////////////////////
//	更新窗口状态
void  CBSSDlg::UpdateWindowState( void ){
	
	m_strRecePack.Format("%d", m_nRecvPackage);
	m_strSendPack.Format("%d", m_nSendPackage );
	m_strConnNumber.Format("%d", m_nConnectNumber );	
}
//////////////////////////////////////////////////////////////////////////
//  连接状态
void CBSSDlg::IsConnectionStatus( BOOL  bStateus ){
	
	CString				strTime;
	CString				strData;
	CTime				oTime;
	
	oTime = CTime::GetCurrentTime();
	m_strRumTime = oTime.Format("%B %d, %Y %H:%M:%S");
	if( bStateus ){
		m_bStart = TRUE;
		m_strState = "BServer Run...";
		strData.Format("&%s", GetLangStr(TEXT_STOP));
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
	}
	else{
		m_bStart = FALSE;
		m_strState = "BServer Stop!";
		strData.Format("&%s", GetLangStr(TEXT_START));
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
	}
	UpdateData(FALSE);
	return;
}
//////////////////////////////////////////////////////////////////////////
//
void CBSSDlg::OnTimer(UINT nIDEvent){

	if ( IsWindowVisible()){
		UpdateWindowState();
		UpdateData(FALSE);
	}	
	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////
//   显示数据信息
BOOL CBSSDlg::SetRecHelpInfo( int	nDataLen, BOOL	bRecv){
	
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
void CBSSDlg::OnPropertyButton(){
	
	CString		 strData;
	CConfigDlg	 oConfigDlg;
	
	oConfigDlg.m_nTypeMode = 1;		
	oConfigDlg.m_strIP = m_strTCPIP;
	oConfigDlg.m_nPort1 = m_nTCPPort;
	if ( oConfigDlg.DoModal() == IDOK ){		
		if ( m_bStart ){		
			strData.Format("BS %s", GetLangStr( TEXT_RUN_PAEASE_STOP_FIRST ) );
			MessageBox( strData , GetLangStr(TEXT_WARNING), MB_ICONWARNING );
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
void CBSSDlg::OnConnectButton(){
	
	AfxGetApp()->m_pMainWnd->SendMessage(WM_SOCKET_STARTSTOP,4,0);
}
//////////////////////////////////////////////////////////////////////////
//
void CBSSDlg::OnClose(){

	KillTimer(IDC_TIMER_UPDATE);	
	CDialog::OnClose();
}
//////////////////////////////////////////////////////////////////////////
//
void CBSSDlg::OnShowCheck(){
	
	m_bShowStatus = m_oShowCheck.GetCheck();
}
