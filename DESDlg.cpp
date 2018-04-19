// DESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "DESDlg.h"
#include "ConfigDlg.h"
#include "TEServerDlg.h"
#include "ComConfigDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDESDlg dialog


CDESDlg::CDESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDESDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDESDlg)
	m_strRecePack = _T("0");
	m_strSendPack = _T("0");
	m_strState = _T("");
	m_strRumTime = _T("");
	m_strLocalIP = _T("");
	m_strConnNumber = _T("0");
	//}}AFX_DATA_INIT
	m_nRecvPackage = 0;
	m_nSendPackage = 0;
	m_nConnectNumber = 0;
	m_bStart = FALSE;
	m_bShowStatus = FALSE;
}


void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDESDlg)
	DDX_Control(pDX, IDC_SHOW_CHECK, m_oShowCheck);
	DDX_Control(pDX, IDC_PROPERTY_BUTTON, m_oPropertyButton);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_oConnectButton);
	DDX_Text(pDX, IDC_RECE_PACKATGE, m_strRecePack);
	DDX_Text(pDX, IDC_SEND_PACKATGE, m_strSendPack);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDX_Text(pDX, IDC_RUM_TIME, m_strRumTime);
	DDX_Text(pDX, IDC_LOCAL_IP, m_strLocalIP);
	DDX_Text(pDX, IDC_CONN_NUMBER, m_strConnNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDESDlg, CDialog)
	//{{AFX_MSG_MAP(CDESDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PROPERTY_BUTTON, OnPropertyButton)
	ON_BN_CLICKED(IDC_SHOW_CHECK, OnShowCheck)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDESDlg message handlers

BOOL CDESDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oConnectButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	m_oPropertyButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	InitLabel( );
	UpdateWindowState();
	UpdateData(FALSE);
	SetTimer(IDC_TIMER_UPDATE,1000,NULL);
	m_oStopSocketDlg.Create(IDD_STOPSOCKET_DLG, this );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void  CDESDlg::InitLabel( void ){
	
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
void  CDESDlg::UpdateWindowState( void ){

	CTEServerDlg	*pDlg = (CTEServerDlg*)AfxGetApp()->m_pMainWnd;

	m_strRecePack.Format("%d", m_nRecvPackage);
	m_strSendPack.Format("%d", m_nSendPackage );
	m_strConnNumber.Format("%d", m_nConnectNumber );	
	m_strConnNumber.Format("%d", pDlg->GetDESTCPActiveConnect());
}

//////////////////////////////////////////////////////////////////////////
//  连接状态
void CDESDlg::IsConnectionStatus( BOOL  bStateus ){

	CString				strData;
	CString				strTime;
	CTime				oTime;
	
	oTime = CTime::GetCurrentTime();
	m_strRumTime = oTime.Format("%B %d, %Y %H:%M:%S");
	if( bStateus ){
		m_bStart = TRUE;
		m_strState = "DEServer Run...";
		strData.Format("&%s", GetLangStr(TEXT_STOP));
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);		
	}
	else{
		m_bStart = FALSE;
		m_strState = "DEServer Stop!";
		strData.Format("&%s", GetLangStr(TEXT_START));
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
	}
	UpdateData(FALSE);
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CDESDlg::OnTimer(UINT nIDEvent) 
{
	if ( IsWindowVisible()){
		UpdateWindowState();
		UpdateData(FALSE);
	}		
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//
void CDESDlg::OnClose(){

	KillTimer(IDC_TIMER_UPDATE);
	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
//  属性
void CDESDlg::OnPropertyButton(){

	CString		 strData;	
	CConfigDlg	 oConfigDlg;
	
	oConfigDlg.m_nTypeMode = 0;	
	oConfigDlg.m_nPort1 = m_nTCPPort;
	oConfigDlg.m_nPort2 = m_nUDPPort;
	oConfigDlg.m_strIP  = m_strLocalIP;

	if ( oConfigDlg.DoModal() == IDOK ){
		if ( m_bStart ){
			strData.Format("DES %s", GetLangStr( TEXT_RUN_PAEASE_STOP_FIRST ) );
			MessageBox( strData, GetLangStr(TEXT_WARNING), MB_ICONWARNING );
		}
		else{
			m_nTCPPort = oConfigDlg.m_nPort1 ;
			m_nUDPPort = oConfigDlg.m_nPort2 ;
			m_strLocalIP = oConfigDlg.m_strIP;
			AfxGetApp()->m_pMainWnd->SendMessage(WM_SAVE_CONFIG, NULL, NULL );
		}		
	}	
}

//////////////////////////////////////////////////////////////////////////
//
void CDESDlg::OnShowCheck(){

	m_bShowStatus = m_oShowCheck.GetCheck();
}

//////////////////////////////////////////////////////////////////////////
//   显示数据信息
BOOL CDESDlg::SetRecHelpInfo( int	nDataLen, BOOL	bRecv){

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
//  连接状态
void CDESDlg::OnConnectButton(){

	CTEServerDlg	 *pTEServerDlg = (CTEServerDlg*)AfxGetApp()->m_pMainWnd;

	if ( pTEServerDlg ){
		
		m_oStopSocketDlg.m_oTcpCheck.SetCheck( pTEServerDlg->m_oDESTCPServer.m_bStart );
		m_oStopSocketDlg.m_oUDPCheck.SetCheck( pTEServerDlg->m_oDESUDPServer.m_bStart );
		if ( m_oStopSocketDlg.IsWindowEnabled() ){
			m_oStopSocketDlg.ShowWindow(SW_SHOW);
		}
	}	
}
//***************************************************************************
//***************************************************************************
//***************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CDES2Dlg dialog
/////////////////////////////////////////////////////////////////////////////
// CDES2Dlg message handlers
CDES2Dlg::CDES2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDES2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDES2Dlg)
	m_strRecePack = _T("0");
	m_strSendPack = _T("0");
	m_strState = _T("");
	m_strRumTime = _T("");
	m_strLocalIP = _T("");
	//}}AFX_DATA_INIT
	m_nRecvPackage = 0;
	m_nSendPackage = 0;
	m_nConnectNumber = 0;
	m_bStart = FALSE;
	m_bShowStatus = FALSE;
}


void CDES2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDES2Dlg)
	DDX_Control(pDX, IDC_SHOW_CHECK, m_oShowCheck);
	DDX_Control(pDX, IDC_PROPERTY_BUTTON, m_oPropertyButton);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_oConnectButton);
	DDX_Text(pDX, IDC_RECE_PACKATGE, m_strRecePack);
	DDX_Text(pDX, IDC_SEND_PACKATGE, m_strSendPack);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDX_Text(pDX, IDC_RUM_TIME, m_strRumTime);
	DDX_Text(pDX, IDC_LOCAL_IP, m_strLocalIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDES2Dlg, CDialog)
	//{{AFX_MSG_MAP(CDES2Dlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PROPERTY_BUTTON, OnPropertyButton)
	ON_BN_CLICKED(IDC_SHOW_CHECK, OnShowCheck)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDES2Dlg message handlers

BOOL CDES2Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_oConnectButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	m_oPropertyButton.SetXButtonStyle(BS_XT_SEMIFLAT|BS_XT_WINXP_COMPAT|BS_XT_XPFLAT);
	InitLabel( );
	UpdateWindowState();
	UpdateData(FALSE);
	SetTimer(IDC_TIMER_UPDATE,1000,NULL);
	m_oStopSocketDlg.Create(IDD_STOPSOCKET_DLG, this );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////
//
void	CDES2Dlg::SetDES2ComPort( int nCOMPort, UINT nCOMBaudRate ){
	
	m_nCOMPort = nCOMPort;
	m_nCOMBaudRate = nCOMBaudRate;
}
//////////////////////////////////////////////////////////////////////////
//
void  CDES2Dlg::InitLabel( void ){
	
	CString		strData;
	
	GetDlgItem( IDC_STATIC_CONN_GROUP )->SetWindowText( GetLangStr(TEXT_CONNECT) );
	GetDlgItem( IDC_STATIC_STATE )->SetWindowText( GetLangStr(TEXT_STATUS) );
	GetDlgItem( IDC_STATIC_RUM_TIME )->SetWindowText( GetLangStr(TEXT_RUN) );
	GetDlgItem( IDC_STATIC_ACTIVITY )->SetWindowText( GetLangStr(TEXT_ACTIVITY) );
	GetDlgItem( IDC_STATIC_SEND )->SetWindowText( GetLangStr(TEXT_SEND) );
	GetDlgItem( IDC_STATIC_RECV )->SetWindowText( GetLangStr(TEXT_RECEIVE) );
	GetDlgItem( IDC_STATIC_SEND_PACKATGE )->SetWindowText( GetLangStr(TEXT_DATA_PACKATGE) );
	GetDlgItem( IDC_SHOW_CHECK )->SetWindowText( GetLangStr(TEXT_SHOW) );
	GetDlgItem( IDC_PROPERTY_BUTTON )->SetWindowText( GetLangStr(TEXT_PROPERTY) );
	GetDlgItem( IDC_CONNECT_BUTTON )->SetWindowText( GetLangStr(TEXT_START) );
	GetDlgItem(IDC_STATIC_LOCAL_IP)->SetWindowText( "COM Port:" );
}
//////////////////////////////////////////////////////////////////////////
//	更新窗口状态
void  CDES2Dlg::UpdateWindowState( void ){

	CTEServerDlg	*pDlg = (CTEServerDlg*)AfxGetApp()->m_pMainWnd;

	m_strRecePack.Format("%d", m_nRecvPackage);
	m_strSendPack.Format("%d", m_nSendPackage );
}

//////////////////////////////////////////////////////////////////////////
//  连接状态
void CDES2Dlg::IsConnectionStatus( BOOL  bStateus ){

	CString				strData;
	CString				strTime;
	CTime				oTime;
	
	oTime = CTime::GetCurrentTime();
	m_strRumTime = oTime.Format("%B %d, %Y %H:%M:%S");
	if( bStateus ){
		m_bStart = TRUE;
		m_strState = "DES_COMServer Run...";
		m_strLocalIP.Format("COM%d BAUD Rate:%d", m_nCOMPort, m_nCOMBaudRate );
		strData.Format("&%s", GetLangStr(TEXT_STOP));
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);		
	}
	else{
		m_bStart = FALSE;
		m_strState = "DES_COMerver Stop!";
		strData.Format("&%s", GetLangStr(TEXT_START));
		//m_strLocalIP.Format("COM%d", m_nCOMPort );
		m_strLocalIP.Format("COM%d  BAUD Rate:%d", m_nCOMPort, m_nCOMBaudRate );
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(strData);
	}
	UpdateData(FALSE);
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CDES2Dlg::OnTimer(UINT nIDEvent) 
{
	if ( IsWindowVisible()){
		UpdateWindowState();
		UpdateData(FALSE);
	}		
	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//
void CDES2Dlg::OnClose(){

	KillTimer(IDC_TIMER_UPDATE);
	CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 属性
void CDES2Dlg::OnPropertyButton(){

	CString			strData;	
	CComConfigDlg	 oDlg;
	
	oDlg.SetBaudrate( m_nCOMBaudRate );
	oDlg.SetCOMPort( m_nCOMPort);
	if ( oDlg.DoModal() == IDOK ){
		if ( m_bStart ){
			strData.Format("DES_COMServer %s", GetLangStr( TEXT_RUN_PAEASE_STOP_FIRST ) );
 			MessageBox( strData, GetLangStr(TEXT_WARNING), MB_ICONWARNING );
 		}
		else{
			m_nCOMBaudRate = oDlg.GetBaudrate();
			m_nCOMPort = oDlg.GetComPort();
			AfxGetApp()->m_pMainWnd->SendMessage(WM_SAVE_CONFIG, NULL, NULL );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
void CDES2Dlg::OnShowCheck(){

	m_bShowStatus = m_oShowCheck.GetCheck();
}

//////////////////////////////////////////////////////////////////////////
//   显示数据信息
BOOL CDES2Dlg::SetRecHelpInfo( int	nDataLen, BOOL	bRecv){

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
//  连接状态
void CDES2Dlg::OnConnectButton(){

	CTEServerDlg	 *pTEServerDlg = (CTEServerDlg*)AfxGetApp()->m_pMainWnd;

	if ( pTEServerDlg ){		
		pTEServerDlg->SendMessage( WM_SOCKET_STARTSTOP, 5, 0);
	}	
	return;
}
