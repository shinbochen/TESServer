// TEServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "TEServerDlg.h"
#include "TerminalData.h"
#include "nb30.h"
#include "..\public\Encrypt.h"
#include "..\public\EncryptionData.h"
#include "..\public\MacIP.h"


#pragma comment(lib,"netapi32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// 
const double	EARTH_RADIUS = 637.8137;
const double	PI = 3.1414926;


//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
#define BoolType(b) b?true:false
//----------------
const int g_nMainMenuID[][2] = {
	IDC_EDIE_CLEAR,			TEXT_DELETE_ALL,
	0,						0,
	IDC_DES_SERVER,			TEXT_DES_SERVER,
	IDC_CSS_SERVER,			TEXT_CSS_SERVER,
//	IDC_DBS_SERVER,			TEXT_DBS_SERVER,
	IDC_COM_SERVER,			TEXT_COM_SERVER,
	IDC_DATABASE_CONNECT,	TEXT_DATABASE_CONNECT,
	0,						0,
	IDC_SUBMENU,			TEXT_THEME,
	-1,						-1
};
//------------------------------
const int g_nCommMenuID[][2] = {
	ID_THEME_OFFICE,			TEXT_THEME_OFFICE,
	ID_THEME_OFFICE,			TEXT_THEME_OFFICE_XP,
	ID_THEME_OFFICE2003,		TEXT_THEME_OFFICE2003,
	ID_THEME_WINXP,				TEXT_THEME_WINXP,
	ID_THEME_GRIPPERED,			TEXT_THEME_GRIPPERED,
	ID_THEME_OFFICE2007,		TEXT_THEME_OFFIC2007,
	ID_THEME_WORD2007,			TEXT_THEME_WORD2007,	
	-1,							-1,
};

//////////////////////////////////////////////////////////////////////////
//
typedef	struct _STSMSHELPDATA{
	
	CTEServerDlg*			m_pDlg;
	char*					m_pPhone;
	int						m_nPhoneLen;
	char*					m_pBuf;
	int						m_nLen;
	int						m_nMR;
	BOOL					m_bFlag;
}STSMSHELPDATA;
//////////////////////////////////////////////////////////////////////////
//
UINT  AddHelpDataThread( LPVOID lpParm ){
	
	STSMSHELPDATA			*pHelpData = (STSMSHELPDATA*)lpParm;
	CString				strData;
	
	if( pHelpData ){
		if( pHelpData->m_bFlag ){
			strData.Format("Recv:%s\r\n", pHelpData->m_pPhone );
		}
		else{
			strData.Format( "Send:%s\r\n MR:%d\r\n", pHelpData->m_pPhone, pHelpData->m_nMR );
		}
		strData += "Data:";
		strData += HexToCString( pHelpData->m_pBuf, pHelpData->m_nLen);
		strData += "\r\n";
		pHelpData->m_pDlg->AddData( strData );
		
		free(pHelpData->m_pPhone );
		free( pHelpData->m_pBuf );
		pHelpData->m_pPhone = NULL;
		pHelpData->m_pBuf = NULL;
		pHelpData->m_pDlg = NULL;
		delete pHelpData;
		pHelpData = NULL;
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
void ADDHelpData( CTEServerDlg *pDlg, CString	strPhone, LPVOID pBuf, int	nLen, int	nMR ,BOOL bRecv ){
	
	STSMSHELPDATA			*pHelpData;
	
	pHelpData = new STSMSHELPDATA;
	pHelpData->m_pDlg = pDlg;
	// 号码
	pHelpData->m_nPhoneLen = strPhone.GetLength();
	pHelpData->m_pPhone = (char*)malloc( pHelpData->m_nPhoneLen+1 );
	memset( pHelpData->m_pPhone, 0x00, pHelpData->m_nPhoneLen );
	memcpy( pHelpData->m_pPhone, strPhone.GetBuffer(0), pHelpData->m_nPhoneLen );
	strPhone.ReleaseBuffer();
	pHelpData->m_pPhone[pHelpData->m_nPhoneLen] = 0x00;
	
	pHelpData->m_pBuf = (char*)malloc( nLen );
	memcpy( pHelpData->m_pBuf, (char*)pBuf, nLen );
	pHelpData->m_nLen = nLen;
	pHelpData->m_bFlag = bRecv;
	pHelpData->m_nMR = nMR;
	
	AfxBeginThread( AddHelpDataThread, pHelpData );
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTEServerDlg dialog

CTEServerDlg::CTEServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTEServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTEServerDlg)
		m_strDataInfo = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_strServerIP = "";			  //远程IP地址	
	m_bConfigIP = FALSE;			  //修改远程IP内容

	//  DES 监听UDP端口号
	m_nDESUDPPort = 8868;
	//  DES 监听TCP端口号
	m_nDESTCPPort = 8868;
	//  DES 监听终端IP地址
	m_strDESTCPIP = "192.168.1.111";
	//  CSS  TCP端口号
	m_nCSSTCPPort = 9090; 
	//  CSS  TCP IP地址
	m_strCSSTCPIP = "192.168.1.111";
	//  BS TCP端口
	m_nBSSTCPPort = 9099;
	//  BS TCPIP地址
	m_strBSSTCPIP = "192.168.1.111";
	//  数据库名
	m_strDatabaseName = "MapTrack_DB";
	//  服务器名
	m_strServerName = "(local)";
	//  用户名
	m_strUserName = "sa";
	//  用户密码
	m_strUserPsd = "sa";
	// 数据库连接
	m_bDatabaseStart = FALSE;
	//  自动连接标志
	m_nConnectEnable = 0xFF;

	// DES 工作方式
	m_nDESWorkMode = 0;/*DES_WORKSMS;*/

	m_nControlMode = CONTROL_GPRS,

	m_nCOMPort = 1;
	m_nCOMBaudRate = 9600;

	m_hDBThread = INVALID_HANDLE_VALUE;
	m_oMapDEUIDToGPSData.RemoveAll();
	m_oMapDEUIDToBlindGPSData.RemoveAll();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_oMapTCPDESInfo.RemoveAll();
	m_oMapUDPDESInfo.RemoveAll();
	m_oStrArrayTime.RemoveAll();
	m_oMapDEUIDToSIM.RemoveAll();
	srand((unsigned)time(0)); 
	int			nCnt = 0;
	
	//----------submenu1----
	nCnt = 0;
	m_oSubMenu1.CreatePopupMenu();
	while( 1 ){
		if( g_nCommMenuID[nCnt][0] == -1 ){
			break;
		}
		if( g_nCommMenuID[nCnt][0] > 0 ){			
			m_oSubMenu1.AppendMenu(MF_STRING, g_nCommMenuID[nCnt][0], GetLangStr( g_nCommMenuID[nCnt][1] ) );
		}
		else if( g_nCommMenuID[nCnt][0] == 0 ){
			m_oSubMenu1.AppendMenu(MF_SEPARATOR, 0, "" );
		}
		nCnt++;
	}

	int			nSubMenu = 0;

	nCnt = 0;	
	m_oMainMenu.CreateMenu();
	while( 1 ){
		if( g_nMainMenuID[nCnt][0] == -1 ){
			break;
		}
		if( g_nMainMenuID[nCnt][0] == IDC_SUBMENU){
			switch( nSubMenu ){
			case 0:
				m_oMainMenu.InsertMenu(nCnt, MF_POPUP|MF_BYPOSITION, (UINT)(m_oSubMenu1.m_hMenu), GetLangStr(g_nMainMenuID[nCnt][1]) );
				break;
			default:
				break;
			}
			nSubMenu++;
		}
		else if( g_nMainMenuID[nCnt][0] > 0 ){			
			m_oMainMenu.AppendMenu(MF_STRING, g_nMainMenuID[nCnt][0], GetLangStr(g_nMainMenuID[nCnt][1]) );
		}
		else if( g_nMainMenuID[nCnt][0] == 0 ){
			m_oMainMenu.AppendMenu(MF_SEPARATOR, 0, "" );
		}
		nCnt++;
	}
}

void CTEServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTEServerDlg)
	DDX_Text(pDX, IDC_EDIT_DATA_INFO,	 m_strDataInfo);
	DDX_Control(pDX, IDC_EDIT_DATA_INFO, m_oDataInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTEServerDlg, CDialog)
	//{{AFX_MSG_MAP(CTEServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()	
	ON_WM_TIMER()
	ON_COMMAND( IDC_EDIE_CLEAR, OnEditClear)
	ON_COMMAND( IDC_DES_SERVER, OnDESServer)
	ON_COMMAND( IDC_DBS_SERVER, OnDBSServer)
	ON_COMMAND( IDC_DATABASE_CONNECT, OnDataBaseConnect)
	ON_COMMAND(ID_THEME_OFFICE, OnThemeOffice)
	ON_COMMAND(ID_THEME_OFFICE2003, OnThemeOffice2003)
	ON_COMMAND(ID_THEME_OFFICE2007, OnThemeOffice2007)
	ON_COMMAND(ID_THEME_SHORTCUTBAR2003, OnThemeShortcutbar2003)
	ON_COMMAND(ID_THEME_WINXP, OnThemeWinxp)
	ON_COMMAND(ID_THEME_WORD2007, OnThemeWord2007)
	ON_COMMAND(ID_THEME_GRIPPERED, OnThemeGrippered)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(IDC_CSS_SERVER, OnCssServer)
	ON_COMMAND( IDC_COM_SERVER, OnComServer )

	ON_UPDATE_COMMAND_UI(IDC_COM_SERVER, OnUpdateComServer )
	ON_UPDATE_COMMAND_UI( IDC_DES_SERVER, OnUpdateDESServer )
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

	ON_MESSAGE(WM_SOCKET_SENDDATA,				OnSocketSendData )
	ON_MESSAGE(WM_SOCKET_SEND_DATATODE_FMCS,	OnSendDataToDESFmCSS )
	ON_MESSAGE(WM_ADDDESINFO,					OnAddDESInfo)
	ON_MESSAGE(WM_SAVE_CONFIG,					SaveParameterConfig )
	ON_MESSAGE(WM_SOCKET_CLOSE,					OnSockeClose)
	ON_MESSAGE(WM_SOCKET_CONNECT,				OnSocketConnect )
	ON_MESSAGE(WM_SOCKET_STARTSTOP,				OnStartStopServer)
	ON_MESSAGE(WM_SOFTWART_ERROR_EXIT,			OnSoftwartErrorEixt )
	ON_MESSAGE(WM_RECVCOMDATA,					OnRecvCOMData )
	ON_MESSAGE(WM_SENDCOMDATARESULT,			OnSendCOMDataResult )
	ON_MESSAGE( WM_SOCKET_SEND_ERROR,			OnSocketSendError )
	ON_MESSAGE(WM_SQL_DISCONNECT,				OnSQLDisconnect )
	ON_MESSAGE( WM_RESULT_GG_ADDR,				OnResultAddrMsg )
	ON_MESSAGE( WM_QUERY_GG_ADDR,				OnQueryGGAddr )
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//
static UINT indicators[] ={		

	ID_TIME_DATE,
	ID_TIME_APP,	
};
//////////////////////////////////////////////////////////////////////////
//
static UINT icoArray[] ={
	IDR_MAINFRAME,
};


//********************************************************************
//

/*
at+cgrg=2
at+cgrg?
at+cops=0,2
at+cops?
mobile_country_code  :460,mobile_network_code :0

#include "httpData.h"
#include "httpSocket.h"
{
"version": "1.1.0" ,
"host": "maps.google.com",
"access_token": "2:k7j3G6LaL6u_lafw:4iXOeOpTh1glSXe",
"home_mobile_country_code": 460,
"home_mobile_network_code":0,
"address_language": "zh_CN",
"radio_type": "gsm",
"request_address": true ,
"cell_towers":[
{
"cell_id":36526,
"location_area_code":14556,
"mobile_country_code":460,
"mobile_network_code":0,
"timing_advance":5555
}]
}

CString    getStringHTML(){

	CString   strEntity = "";
	CString	  strData = "maps.google.com";

	//strEntity.Format("{version : '1.1.0' , host	: '%s',access_token	: '2:k7j3G6LaL6u_lafw:4iXOeOpTh1glSXe', home_mobile_country_code	: 460,home_mobile_network_code	:0,address_language	: 'zh_CN',radio_type: 'gsm',request_address: true ,cell_towers:[{cell_id:36526,location_area_code :14556,  mobile_country_code  :460,mobile_network_code :0,timing_advance:5555}]}", strData);
	strEntity.Format("{version : '1.1.0' , host	: '%s',access_token	: '2:k7j3G6LaL6u_lafw:4iXOeOpTh1glSXe_22', home_mobile_country_code	: 460,home_mobile_network_code	:0,address_language	: 'zh_CN',radio_type: 'gsm',request_address: true ,cell_towers:[{cell_id:3693,location_area_code :9360,  mobile_country_code  :460,mobile_network_code :0,timing_advance:5555}]}", strData);

	return strEntity;
}

//////////////////////////////////////////////////////////////////////////
// 主线程
CString queryAddress(){

	CString							    strSrc = "http://www.google.com/loc/json";
	CHttpSocket							*pSocket = NULL;
	CHttpData							httpData;

	pSocket = new CHttpSocket;
	if( pSocket == NULL ){
		return "";
	}
	pSocket->SetThreadID( 0 );	
	httpData.SetSocket( pSocket );

	httpData.GetHTMLCode( strSrc, getStringHTML() );

	AfxMessageBox( httpData.m_strCode);
	//////////////////////////////////////////////////////////////////////////
	//
	if( pSocket != NULL ){		
		pSocket->Close();
		delete pSocket;
	}
	return "";
}
*/
//***********************************************************************

/////////////////////////////////////////////////////////////////////////////
// CTEServerDlg message handlers

BOOL CTEServerDlg::OnInitDialog(){

	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL){
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()){
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	//////////////////////////////////////////////////////////////////////////
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);
	
	m_paneManager.UseSplitterTracker(FALSE);
	m_paneManager.SetShowContentsWhileDragging(TRUE);
	m_paneManager.SetAlphaDockingContext(TRUE);

	m_pWndDESPane = m_paneManager.CreatePane(
		IDR_DESPANE, CRect(0, 0,300, 120), xtpPaneDockLeft);

	m_pWndCSSPane = m_paneManager.CreatePane(
		IDR_CSSPANE, CRect(0, 0,300, 120), xtpPaneDockLeft);

	m_pWndBSSPane = m_paneManager.CreatePane(
		IDR_DBSPANE, CRect(0, 0,300, 120), xtpPaneDockLeft);

	m_pWndDBDataPane = m_paneManager.CreatePane(
		IDR_DATABASEPANE, CRect(0, 0,300, 120), xtpPaneDockLeft);

	m_pWndCOMPane = m_paneManager.CreatePane( 
		IDR_COMPANE, CRect(0,0,300,1200), xtpPaneDockLeft );

	
	//////////////////////////////////////////////////////////////////////////
	// 读取日志
	GetInitData();
	GetInitStatusBar( );
	GetInitConfig();
	GetPrivateData();
	InitApp();
			
	m_paneManager.AttachPane(m_pWndCOMPane,m_pWndDESPane);
	m_paneManager.AttachPane(m_pWndCSSPane,m_pWndDESPane);
	m_paneManager.AttachPane(m_pWndBSSPane,m_pWndDESPane);
	m_paneManager.AttachPane(m_pWndDBDataPane,m_pWndDESPane);

	int nIDIcons[] = {IDR_DESPANE, IDR_CSSPANE,IDR_DBSPANE, IDR_DATABASEPANE,IDR_COMPANE};
	m_paneManager.SetIcons(IDB_APP_BMP, nIDIcons,
		_countof(nIDIcons), RGB(0, 255, 0));
		
	XTPPaintManager()->SetTheme( xtpThemeOffice2003 );

	m_pWndBSSPane->Close();
	//////////////////////////////////////////////////////////////////////////
	m_oSocketSendErrorDlg.Create( IDD_SOCKET_ERROR_DLG, this );

	//////////////////////////////////////////////////////////////////////////
	// 读取日志
	//GetInitData();
	//GetInitStatusBar( );
	//GetInitConfig();
	//GetPrivateData();
	//AfxBeginThread( StartSocketThread, NULL );

	CString			strWindowName;
	CString			strWindowName2;
	strWindowName.LoadString(IDS_VERSION);
	GetWindowText( strWindowName2 );	
	strWindowName2 +=" " + strWindowName;
	SetWindowText( strWindowName2 );

	// 创建托盘
	if (!m_TrayIcon.Create(
							_T("TEServer"),				// Toolktip text
							this,                       // Parent window
							IDR_MAINFRAME,              // Icon resource ID
							IDR_TRAY_MENU,              // Resource ID of popup menu
							IDR_MAINFRAME,              // Default menu item for popup menu
							false))                     // True if default menu item is located by position
	{
		TRACE0("Failed to create tray icon\n");
		return -1;
	}	
	m_TrayIcon.SetAnimationIcons( icoArray, _countof(icoArray));
	
	CString	 strTmp;

	#ifdef	TRACK_EARTH_SERVER
		CString  strAuthor;
		CString  strNetwork;		
		
		#ifdef  TEXT_MACIP
			char		chBuf[128];
			
			memset( chBuf, 0x00, 128);
			
			if( GetMacByCmd( chBuf ) ){
				
				strNetwork.Format("%s", chBuf );
				strAuthor = Decode( strNetwork, 12 );
			}
		#else	
			strNetwork = GetMacAddr();
			strAuthor = Decode( strNetwork, 12 );
		#endif //TEXT_MACIP

		
		if (  strAuthor != m_strAuthor ){
			strTmp.Format( "%s %s:%s", GetLangStr(TEXT_WARING_NOT_AUTHORIZE), TEXT_AUTHOR_EMAIL, strNetwork );
			AfxMessageBox( strTmp );
			PostMessage(WM_COMMAND, IDOK, 0 );
			return FALSE;
		}
		CDBData::SetSoftwareVehicleTotal( USER_LIMIT_VEHICLE_5000 );
		SetWorkTCPUDP();
		CDESProtocolData::SetProtocol( PROTOCOL_ASICO ); 
		CDESProtocolData::SetProtocol( GBU_PROTOCOL );
	#else
		//////////////////////////////////////////////////////////////////////////
		BOOL		bTimeFlag = TRUE;

		if( (m_oSoftKey.Ini( ) && m_oSoftKey.YCheckKey()) == FALSE ){		
			CTime		oCurrentTime = CTime::GetCurrentTime();
			int			nCurrentTime;
			int			nTime;	
			int			nTmp;
						
			strTmp.Format("%d", oCurrentTime );
			nCurrentTime = 	atoi(strTmp.GetBuffer(0));
			strTmp.ReleaseBuffer();
			if( m_oStrArrayTime.GetSize() > 0 ){
				for ( int nCnt = 0; nCnt < m_oStrArrayTime.GetSize(); nCnt++ ){
					strTmp = m_oStrArrayTime.GetAt(nCnt);
					if( nCnt == 0 ){			//时间						
						nTime = atoi( strTmp.GetBuffer(0));
						strTmp.ReleaseBuffer();
					}
					else if( nCnt == 1 ){		//数量
						nTmp = atoi( strTmp.GetBuffer(0) );
						strTmp.ReleaseBuffer();
						if( nTmp == 0 ){
							CDBData::SetSoftwareVehicleTotal(USER_LIMIT_VEHICLE_0);	
						}
						else if( nTmp == 1 ){
							CDBData::SetSoftwareVehicleTotal(USER_LIMIT_VEHICLE_200);	
						}
						else if( nTmp == 2){
							CDBData::SetSoftwareVehicleTotal(USER_LIMIT_VEHICLE_500);	
						}
						else if( nTmp == 3 ){
							CDBData::SetSoftwareVehicleTotal(USER_LIMIT_VEHICLE_5000);
						}					
					}				
				}				
				//	时间比较
				if( nTime > nCurrentTime  ){
					bTimeFlag = FALSE;
				}
			}
			if( bTimeFlag ){
				strTmp.Format("%s%s", GetLangStr(TEXT_WARING_NOT_AUTHORIZE), TEXT_AUTHOR_EMAIL );
				MessageBox( strTmp ,GetLangStr(TEXT_WARNING), MB_OK|MB_ICONWARNING );
				PostMessage(WM_COMMAND, IDOK, 0 );
				return FALSE;
			}
		}	
		DWORD		dwVersion;	
		
		if( bTimeFlag ){
			dwVersion = m_oSoftKey.GetIDVersion();
			if( ComparisonVersionID( dwVersion) == FALSE ){
				strTmp.Format("%s%s", GetLangStr(TEXT_WARING_NOT_AUTHORIZE), TEXT_AUTHOR_EMAIL );
				MessageBox( strTmp ,GetLangStr(TEXT_WARNING), MB_OK|MB_ICONWARNING );
				PostMessage(WM_COMMAND, IDOK, 0 );
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////
			switch( m_oSoftKey.GetUserLimit() ){
			case USER_LIMIT_0:
				CDBData::SetSoftwareVehicleTotal( USER_LIMIT_VEHICLE_0 );
				break;
			case USER_LIMIT_200:
				CDBData::SetSoftwareVehicleTotal( USER_LIMIT_VEHICLE_200 );
				break;
			case USER_LIMIT_500:
				CDBData::SetSoftwareVehicleTotal( USER_LIMIT_VEHICLE_500 );
				break;
			case USER_NO_LIMIT:
				CDBData::SetSoftwareVehicleTotal( USER_LIMIT_VEHICLE_5000 );
				break;
			}
			if( m_oSoftKey.IsASICOProtocolValid() ){
				CDESProtocolData::SetProtocol( PROTOCOL_ASICO );
			}
			if( m_oSoftKey.IsGBUProtocolValid() ){
				CDESProtocolData::SetProtocol( GBU_PROTOCOL );
			}
			//////////////////////////////////////////////////////////////////////////
			//  工作模式
			if( m_oSoftKey.IsSMSAndTCPUPDMode() ){
				SetWorkSMSMode(); 
				SetWorkTCPUDP();
			}
			else{
				if( m_oSoftKey.IsWorkSMSMode() ){
					SetWorkSMSMode(); 
				}
				if( m_oSoftKey.IsWorkTCPUDP() ){
					SetWorkTCPUDP();
				}
			}
			char		chBuf[20];
			
			strcpy( chBuf, "MapTrack" );
			m_oSoftKey.YCompareString( chBuf, "MapTrack", strlen(chBuf) );		
			if( m_oSoftKey.YGetLastError() != 0 ){
				strTmp.Format("%s%s", GetLangStr(TEXT_WARING_NOT_AUTHORIZE), TEXT_AUTHOR_EMAIL );
				MessageBox( strTmp ,GetLangStr(TEXT_WARNING), MB_OK|MB_ICONWARNING );
				PostMessage(WM_COMMAND, IDOK, 0 );
				return FALSE;
			}
			//	第一次十分钟检测
			SetTimer(WM_TIMER_CHECK, 1000*60*10 , NULL );
		}
	#endif
		//  时间设置
#if(TIME_CONFIG_VERSION)
		
		CTime			oTime = CTime::GetCurrentTime();
		CString			strData;
		
		strData.Format("%d", oTime );
		int  nCurTime = atoi( strData.GetBuffer(0) );
		strData.ReleaseBuffer();
		if( nCurTime >= TIME_CONFIG_TIME ){		
			PostMessage(WM_COMMAND, IDOK, 0 );
			return FALSE;
		}
#endif 
	//////////////////////////////////////////////////////////////////////////
	InitWindowShow( );
	//////////////////////////////////////////////////////////////////////////
	AfxBeginThread( StartSocketThread, NULL );
	InitializeCriticalSection( &m_hCriticalSection );
	InitializeCriticalSection( &m_hSocketCriticalSection );

	SetTimer(WM_TIMER_UPDATA, 3000, NULL );
		
	m_oQueryGGAdd.Open( this );
	// 创建线程
	//m_hDBThread = CreateThread( NULL, 0, DBMainThread, this, 0, NULL);

	//queryAddress();
	return TRUE;  		
}

//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::InitWindowShow( void ){

	if( IsWorkSMSMode()){
		m_paneManager.ShowPane(IDR_COMPANE);		
	}
	else{
		m_pWndCOMPane->Close();
	}
	if( IsWorkTCPUDP() ){
		m_paneManager.ShowPane( IDR_DESPANE );		
	}
	else{
		m_pWndDESPane->Close();
	}
}

//////////////////////////////////////////////////////////////////////////
//  初始化参数配置
void CTEServerDlg::GetInitConfig( void ){

	m_strlocalIP =	GetLocalIP();	
	m_strDESTCPIP = m_strlocalIP;
	// DES 窗口
	m_oDESDlg.SetDESLocalIP( m_strDESTCPIP );
	m_oDESDlg.SetDESUDPPort( m_nDESUDPPort );
	m_oDESDlg.SetDESTCPPort( m_nDESTCPPort );
	
	// DES 窗口2
	m_oDES2Dlg.SetDES2ComPort( m_nCOMPort, m_nCOMBaudRate );

	// 数据库库接
	m_oDBDataDlg.SetServerName( m_strServerName );
	m_oDBDataDlg.SetDatabaseName( m_strDatabaseName );
	m_oDBDataDlg.SetUserName( m_strUserName );
	m_oDBDataDlg.SetUserPsd( m_strUserPsd );
	
	// CSS 窗口
	m_oCSSDlg.SetLoaclIP( m_strlocalIP );
	m_oCSSDlg.SetCSSTCPPort( m_nCSSTCPPort );
	m_oCSSDlg.SetCSSTCPIP( m_strlocalIP );
	m_strCSSTCPIP = m_strlocalIP;
	//m_oCSSDlg.SetCSSTCPIP( m_strCSSTCPIP );

	// BSS 窗口
	m_oBSSDlg.SetLocalIP( m_strlocalIP );
	m_oBSSDlg.SetBSSTCPPort( m_nBSSTCPPort );
	m_oBSSDlg.SetBSSTCPIP( m_strBSSTCPIP );

}

//////////////////////////////////////////////////////////////////////////
//  保存端口配置
void CTEServerDlg::SaveParameterConfig( WPARAM wParam, LPARAM  lParam ){

	// DES 窗口
	m_strDESTCPIP = m_oDESDlg.GetDESTCPIP();
	m_nDESUDPPort = m_oDESDlg.GetDESUDPPort();
	m_nDESTCPPort = m_oDESDlg.GetDESTCPPort();

	// DES 窗口2
	m_nCOMPort = m_oDES2Dlg.GetCOMPort();
	m_nCOMBaudRate = m_oDES2Dlg.GetCOMBaudRate();

	// 数据库库接
	m_strServerName = m_oDBDataDlg.GetServerName();
	m_strDatabaseName = m_oDBDataDlg.GetDatabaseName();
	m_strUserName  = m_oDBDataDlg.GetUserName();
	m_strUserPsd   = m_oDBDataDlg.GetUserPsd();
	// CSS 窗口
	m_nCSSTCPPort = m_oCSSDlg.GetCSSTCPPort();	
	m_strCSSTCPIP = m_oCSSDlg.GetCSSTCPIP();
	//  BSS窗口
	m_nBSSTCPPort = m_oBSSDlg.GetBSSTCPPort();
	m_strBSSTCPIP = m_oBSSDlg.GetBSSTCPIP();

	// 保存配置信息
	SaveInitData();
}
//////////////////////////////////////////////////////////////////////////
//  初始状态条
void CTEServerDlg::GetInitStatusBar( void ){

	CString     strTime;
	CString		strData;
	CTime		oTime;
	
	m_wndStatusBar.SetPaneWidth(0, 360);
	m_wndStatusBar.SetPaneWidth(1, 175);	
	strData.Format(" %s:%s ", GetLangStr(TEXT_VERSION),m_strVersion);
	m_wndStatusBar.SetPaneText(0, strData);
	oTime   = CTime::GetCurrentTime();		
	strData = oTime.Format("%Y-%m-%d %H:%M:%S ");
	strTime.Format("%s:%s", GetLangStr(TEXT_TIME), strData);
	m_wndStatusBar.SetPaneText(1,strTime);	
	SetTimer(WM_UPDATE_TIME,1000,NULL);	
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTEServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTEServerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rcClient(0, 0, cx, cy);
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, 0, 0, &rcClient);
	
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rcClient, &rcClient);
	
	if (m_oDataInfo.GetSafeHwnd()){	
		
		m_oDataInfo.MoveWindow(rcClient);
	}	
}

//////////////////////////////////////////////////////////////////////////
// 初始化对像
void   CTEServerDlg::InitApp( void ){

	CString  strData;

	m_oDESDlg.Create(IDD_DES_DLG, this);	
	m_pWndDESPane->Attach(&m_oDESDlg);
	switch ( GetControlMode() ){
	case CONTROL_SMS_GPRS:
		strData.Format("DES %s GPRS/SMS", GetLangStr(TEXT_SERVER));
		break;
	case CONTROL_SMS:
		strData.Format("DES %s SMS", GetLangStr(TEXT_SERVER));
		break;
	default:
		strData.Format("DES %s", GetLangStr(TEXT_SERVER));
		break;
	}	
	m_pWndDESPane->SetTitle(strData);

	m_oCSSDlg.Create(IDD_CSS_DLG, this);
	m_pWndCSSPane->Attach(&m_oCSSDlg);
	strData.Format("CSS %s", GetLangStr(TEXT_SERVER));
	m_pWndCSSPane->SetTitle(strData);

	m_oBSSDlg.Create(IDD_BSS_DLG, this );
	m_pWndBSSPane->Attach(&m_oBSSDlg);
	strData.Format("BSS %s", GetLangStr(TEXT_SERVER));
	m_pWndBSSPane->SetTitle(strData);

	m_oDBDataDlg.Create(IDD_DBDATA_DLG, this); 	
	m_pWndDBDataPane->Attach(&m_oDBDataDlg);
	strData.Format("Database %s", GetLangStr(TEXT_SERVER));	
	m_pWndDBDataPane->SetTitle(strData );

	m_oDES2Dlg.Create( IDD_DES_DLG2, this );
	m_pWndCOMPane->Attach( &m_oDES2Dlg );
	switch ( GetControlMode() ){
	case CONTROL_SMS_GPRS:
		strData.Format("DES %s COM GPRS/SMS", GetLangStr(TEXT_SERVER));
		break;
	case CONTROL_SMS:
		strData.Format("DES %s COM SMS", GetLangStr(TEXT_SERVER));
		break;
	default:
		strData.Format("DES %s COM", GetLangStr(TEXT_SERVER));
		break;
	}
	m_pWndCOMPane->SetTitle( strData );	
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnClose(){	

	if( MessageBox(GetLangStr(TEXT_ARE_YOU_SURE), GetLangStr(TEXT_EXIT), MB_ICONASTERISK|MB_ICONINFORMATION|MB_YESNO) == IDYES ){
		FreeMomery();	
		// 释放组织数据
		m_oQuerySocketObject.DeleteComposeObject( );
		DeleteCriticalSection( &m_hCriticalSection );
		DeleteCriticalSection( &m_hSocketCriticalSection );
		KillTimer(WM_TIMER_UPDATA);
		KillTimer(WM_TIMER_CHECK);
		CDialog::OnClose();
	}
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnExit() 
{
	OnClose();
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////
//
LRESULT CTEServerDlg::OnKickIdle(WPARAM, LPARAM){
	
	m_paneManager.UpdatePanes();	
	m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI, TRUE);	
	return 0;
}
////////////////////////////////////////////////////////////////////
// 
CString CTEServerDlg::GetIniFileName(){

	CString			strResult;	

	strResult = GetModulePath( )+"\\DES.ini";
	return strResult;
}

////////////////////////////////////////////////////////////////////
//  
void CTEServerDlg::GetInitData(){
	
	CString			strAppName;
	CString			strKey;
	CString			strValue;
	CString			strData;
	
	
	strAppName = "DEServer";
	
	strKey="Version";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer();

	m_strVersion.LoadString(IDS_VERSION);
	// DES_V2.06_20100419
	strData = strValue.Left( 5 );
	//if( m_strVersion == strValue ){
	if( m_strVersion.Find( strData) == 0) {
		RestoreInitData();
	}
	else{
		strValue = m_strVersion;
		SaveInitData();
		WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	}
	return;
}

////////////////////////////////////////////////////////////////////
//  读取日志
void CTEServerDlg::RestoreInitData(){

	CString			strAppName;
	CString			strKey;
	CString			strValue;
	
	
	strAppName = "DEServer";
	
	strKey="DESUDPPort";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	m_nDESUDPPort = atoi( strValue.GetBuffer(0) );
	strValue.ReleaseBuffer( );
	
	strKey="DESTCPPort";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	m_nDESTCPPort = atoi( strValue.GetBuffer(0) );
	strValue.ReleaseBuffer( );

	strKey="DESCOMPort";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	m_nCOMPort = atoi( strValue.GetBuffer(0) );
	strValue.ReleaseBuffer( );
	
	strKey="DESCOMBaudRate";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	m_nCOMBaudRate = atoi( strValue.GetBuffer(0) );
	strValue.ReleaseBuffer( );
	
	//  服务器名
	strKey="ServerName";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer( );
	m_strServerName = strValue;

	//  数据库名
	strKey="DatabaseName";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer( );
	m_strDatabaseName = strValue;

	//  用户名
	strKey="UserName";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer( );
	m_strUserName = strValue;

	//  用户密码
	strKey="UserPsd";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer( );
	m_strUserPsd =	DecryptPassword(strValue);

	strKey="CSSIP";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer( );
	m_strCSSTCPIP = strValue;	
	
	strKey="CSSTCPPort";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	m_nCSSTCPPort = atoi( strValue.GetBuffer(0) );
	strValue.ReleaseBuffer( );

	// BS参数
	strKey = "BSIP";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer();
	m_strBSSTCPIP = strValue;

	strKey = "BSTCPPort";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer();
	m_nBSSTCPPort = atoi( strValue.GetBuffer(0));
	strValue.ReleaseBuffer();

	// 授权码
	strKey = "AuthorCode";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer();
	m_strAuthor = strValue;

	strKey = "ControlMode";
	GetPrivateProfileString( strAppName, strKey, "", strValue.GetBuffer(256), 255, GetIniFileName() );
	strValue.ReleaseBuffer();
	m_nControlMode = atoi( strValue.GetBuffer(0));
	strValue.ReleaseBuffer();

	return;
}

////////////////////////////////////////////////////////////////////
//  保存日志
void CTEServerDlg::SaveInitData(){

	
	CString			strAppName;
	CString			strKey;
	CString			strValue;
	
	strAppName = "DEServer";	

	strKey="DESUDPPort";
	strValue.Format("%d",m_nDESUDPPort);		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey="DESTCPPort";
	strValue.Format("%d",m_nDESTCPPort);		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	
	
	strKey="DESCOMPort";
	strValue.Format("%d",m_nCOMPort);		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	
	
	strKey="DESCOMBaudRate";
	strValue.Format("%d",m_nCOMBaudRate);		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	
	strKey="ServerName";		
	strValue = m_strServerName;
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey="DatabaseName";	
	strValue = m_strDatabaseName;
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey="UserName";		
	strValue = m_strUserName;
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey="UserPsd";
	strValue = EncryptPassword( m_strUserPsd );
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey="CSSIP";
	strValue = m_strCSSTCPIP;		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	
	strKey="CSSTCPPort";
	strValue.Format("%d",m_nCSSTCPPort);		
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	
	strKey = "BSIP";
	strValue = m_strBSSTCPIP;
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	strKey = "BSTCPPort";
	strValue.Format("%d", m_nBSSTCPPort);
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );

	// 授权码
	strKey = "AuthorCode";
	strValue = m_strAuthor;
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );


	strKey = "ControlMode";
	strValue.Format("%d", m_nControlMode );
	WritePrivateProfileString( strAppName, strKey, strValue, GetIniFileName() );
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnTimer(UINT nIDEvent){

	CString     strUDPStatus;
	CString     strTCPStatus;
	CString		strBSStatus;
	CString		strData;
	CString     strTime;
	CTime		oTime;
	static BOOL	bFlag = FALSE;
	
	if ( nIDEvent == WM_UPDATE_TIME){
		
		if ( m_oDESTCPServer.m_bStart ){
			strTCPStatus.Format("DES:TCP%s", GetLangStr(TEXT_ENABLE));
		}
		else{
			strTCPStatus.Format("DES:TCP%s", GetLangStr(TEXT_DISABLE));
		}
		if ( m_oDESUDPServer.m_bStart )	{
			strUDPStatus.Format("DES:UDP%s", GetLangStr(TEXT_ENABLE)); 
		}
		else{
			strUDPStatus.Format("DES:UDP%s", GetLangStr(TEXT_DISABLE));
		}
		if ( m_oBSSTCPServer.m_bStart ){
			strBSStatus.Format("BS:TCP%s", GetLangStr(TEXT_ENABLE)); 
		}
		else{
			strBSStatus.Format("BS:TCP%s", GetLangStr(TEXT_DISABLE));
		}
		strData.Format("%s:%s %s %s %s", GetLangStr(TEXT_VERSION),m_strVersion, strTCPStatus , strUDPStatus, strBSStatus);
		m_wndStatusBar.SetPaneText(0, strData);

		oTime = CTime::GetCurrentTime();		
		strData = oTime.Format("%Y-%m-%d %H:%M:%S ");
		strTime.Format("%s%s", GetLangStr(TEXT_TIME),strData);
		m_wndStatusBar.SetPaneText(1,strTime);		

		if( IsWorkSMSMode() ){
			// 串口是否初始化成功
			if( m_oDESCOMServer.IsInitCMMComplete() == FALSE ){	
				switch ( GetControlMode() ){
				case CONTROL_SMS_GPRS:
					strData.Format("DES %s COM GPRS/SMS Init", GetLangStr(TEXT_SERVER));
					break;
				case CONTROL_SMS:
					strData.Format("DES %s COM SMS Init", GetLangStr(TEXT_SERVER));
					break;
				default:
					strData.Format("DES %s COM Init", GetLangStr(TEXT_SERVER));
					break;
				}
				m_pWndCOMPane->SetTitle( strData );	
				bFlag = FALSE;
			}
			else{
				if( bFlag == FALSE ){
					bFlag = TRUE;
					switch ( GetControlMode() ){
					case CONTROL_SMS_GPRS:
						strData.Format("DES %s COM GPRS/SMS Succeed", GetLangStr(TEXT_SERVER));
						break;
					case CONTROL_SMS:
						strData.Format("DES %s COM SMS Succeed", GetLangStr(TEXT_SERVER));
						break;
					default:
						strData.Format("DES %s COM Succeed", GetLangStr(TEXT_SERVER));
						break;
					}
					m_pWndCOMPane->SetTitle( strData );	
				}
			}
		}
	}
	if( nIDEvent == WM_TIMER_UPDATA ) {
		UpdateData(FALSE);
	}
	if( nIDEvent == WM_TIMER_CHECK ){
		OnTimingCheckKey();
	}
	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////
// DES UDP SOCKET 联接
BOOL	CTEServerDlg::StartDESUDPServer( void ){

	return m_oDESUDPServer.StartServer( GetDESUDPPort(), 
		SOCK_DGRAM, RecvDESUDPDataProcess );
}

//////////////////////////////////////////////////////////////////////////
// DES TCP SOCKET 联接
BOOL	CTEServerDlg::StartDESTCPServer( void ){

	return m_oDESTCPServer.StartServer(GetDESTCPPort(),
		SOCK_STREAM, RecvDESTCPDataProcess );
}

//////////////////////////////////////////////////////////////////////////
// DES TCP SOCKET 联接
BOOL	CTEServerDlg::StartDESCOMServer( void ){	
	return m_oDESCOMServer.Open( this, m_nCOMPort, m_nCOMBaudRate );
}

//////////////////////////////////////////////////////////////////////////
// BS TCP Socket 联接
BOOL    CTEServerDlg::StartBSSTCPServer( void ){

	return m_oBSSTCPServer.StartServer( GetBSSTCPPort(),
										SOCK_STREAM,
										RecvBSSTCPDataProcess );	
}

//////////////////////////////////////////////////////////////////////////
// CSS TCP SOCKET 联接
BOOL	CTEServerDlg::StartCSSTCPClient( void ){

	BOOL			bResult;
	bResult = m_oCSSTCPClient.StartTCPClient(GetCSSTCPIP(),
		GetCSSTCPPort(), RecvCSSTCPDataProcess );
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//   Connect: Connect Database 
//////////////////////////////////////////////////////////////////////////
BOOL CTEServerDlg::StartConnectDatabase(){	
	
	StopConnectDataBase();	
	if ( CDBData::Connect( m_strServerName, m_strDatabaseName, m_strUserName, m_strUserPsd ) ){
		// 获取地址
		if( CDBData::DBGetImagePath( m_strImagePPath, m_strImageVPath ) ){
			CDESProtocolData::SetImagePath( m_strImagePPath, m_strImageVPath );
		}
		if( CDBData::GetVehicleCurrent() > CDBData::GetSoftwareVehicleTotal() ){
			SendMessage( WM_SOFTWART_ERROR_EXIT , 0, NULL );
		}
		return TRUE;
	}
	else{
		return FALSE;
	}	
}
//////////////////////////////////////////////////////////////////////////
//	车辆超出，退出系统
void	CTEServerDlg::OnSoftwartErrorEixt( WPARAM  wParam, LPARAM  lParam ){
	
	FreeMomery();
	CDialog::OnClose();
	CDialog::OnOK();
	if( wParam == 0 ){
		MessageBox( "Error: 1536!", GetLangStr(TEXT_WARNING), MB_OK|MB_ICONWARNING ); 	
	}
	else{
		MessageBox( "Error: 1537!", GetLangStr(TEXT_WARNING), MB_OK|MB_ICONWARNING ); 	
	}
}
//////////////////////////////////////////////////////////////////////////
//	收到COM数据
void	CTEServerDlg::OnRecvCOMData( WPARAM  wParam, LPARAM  lParam ){	
	AfxBeginThread( RecvDESSMSDataProcess, (LPVOID)lParam);
}
//////////////////////////////////////////////////////////////////////////
//	车辆超出，退出系统
// wparam = send result		0/1  send failed/ send success
// lparam = MR_CNT
void	CTEServerDlg::OnSendCOMDataResult( WPARAM  wParam, LPARAM  lParam ){
	SetDESSendSMSHelpInfo2( (int)lParam, ((int)wParam == 1)  ? TRUE :FALSE );

}
//////////////////////////////////////////////////////////////////////////
//  释放所有数据
void	CTEServerDlg::FreeMomery( void ){

	CTerminalData		*pData = NULL;
	CString				strKey;
	POSITION			pos;
		
	StopDESUDPServer();
	StopDESTCPServer();	
	StopCSSTCPClient();
	StopDESCOMServer();
	StopConnectDataBase();	
	
	KillTimer(WM_UPDATE_TIME);
	pos = m_oMapTCPDESInfo.GetStartPosition();
	while( pos != NULL){
		m_oMapTCPDESInfo.GetNextAssoc( pos, strKey, (CObject*&)pData );
		ASSERT( pData!= NULL);
		if( pData ){
			pData->FreeMemory();
			m_oMapTCPDESInfo.RemoveKey( strKey);
			delete pData;
			pData = NULL;
		}
	}
	m_oMapTCPDESInfo.RemoveAll();	
	
	pos = m_oMapUDPDESInfo.GetStartPosition();
	while( pos != NULL){
		m_oMapUDPDESInfo.GetNextAssoc( pos, strKey, (CObject*&)pData );
		ASSERT( pData!= NULL);
		if( pData ){
			pData->FreeMemory();
			m_oMapUDPDESInfo.RemoveKey( strKey);
			delete pData;
			pData = NULL;
		}
	}
	m_oMapUDPDESInfo.RemoveAll();
	m_oMapDEUIDToSIM.RemoveAll();

	//free data 2011-03-09
	// 终止线程
	if( m_hDBThread != INVALID_HANDLE_VALUE){

		::TerminateThread( m_hDBThread, 0 );
		m_hDBThread = INVALID_HANDLE_VALUE;
	}
	DelAllGpsData();	 //释放内存数据
	DelAllBlinkpsData(); //释放盲点数据

	std::list<CData1*>::iterator	it;
	//释放数据
	it = m_lstDBData.begin();
	while( it != m_lstDBData.end() ){
		
		delete *it;
		m_lstDBData.pop_front();
		it = m_lstDBData.begin();
	}
}
//////////////////////////////////////////////////////////////////////////
// 关闭 DES UDP SOCKET 联接
void    CTEServerDlg::StopDESUDPServer( void ){

	POSITION			pos = NULL;
	CString				strDESUID;
	CTerminalData		*pMapData = NULL;
	
	// 先关掉CS的服务器
	StopCSSTCPClient();
//	StopBSSTCPServer();
	m_oCSSDlg.IsConnectionStatus( FALSE );
	
	pos = m_oMapUDPDESInfo.GetStartPosition();
	while( pos ){
		m_oMapUDPDESInfo.GetNextAssoc(pos, strDESUID, (CObject*&)pMapData);
		if( pMapData ){
			DeleteDESInfo( &m_oMapUDPDESInfo, strDESUID, pMapData );			
		}
	}
	m_oDESUDPServer.Stop();
}

//////////////////////////////////////////////////////////////////////////
// 关闭 DES TCP SOCKET 联接
void    CTEServerDlg::StopDESTCPServer( void ){

	POSITION			pos = NULL;
	CString				strDESUID;
	CTerminalData		*pMapData = NULL;
	
	// 先关掉CS的服务器
	StopCSSTCPClient();
//	StopBSSTCPServer();
	m_oCSSDlg.IsConnectionStatus( FALSE );
	
	pos = m_oMapTCPDESInfo.GetStartPosition();
	while( pos ){
		m_oMapTCPDESInfo.GetNextAssoc(pos, strDESUID, (CObject*&)pMapData);
		if( pMapData ){
			//2011-11-09 修改
			m_oQuerySocketObject.ObjectEnterCriticalSection();
			m_oQuerySocketObject.DeleteComposeObject( pMapData->m_pClientSocket  );
			m_oQuerySocketObject.ObjectLeaveCriticalSection();

			DeleteDESInfo( &m_oMapTCPDESInfo, strDESUID, pMapData );			
		}
	}
	m_oDESTCPServer.Stop();
}


//////////////////////////////////////////////////////////////////////////
// DES TCP SOCKET 联接
void	CTEServerDlg::StopDESCOMServer( void ){	

	
	// 先关掉CS的服务器
	StopCSSTCPClient();
	//	StopBSSTCPServer();
	m_oCSSDlg.IsConnectionStatus( FALSE );

	// 不用清DEUID 与 DESIM的对应关系

	m_oDESCOMServer.Close();
	return ;
}

//////////////////////////////////////////////////////////////////////////
//  关闭 BSS TCP SOCKET 联接
void    CTEServerDlg::StopBSSTCPServer(void ){

	m_oBSSTCPServer.Stop();
}
//////////////////////////////////////////////////////////////////////////
// 关闭 CSS SOCKET 联接
void    CTEServerDlg::StopCSSTCPClient( void ){

	m_oCSSTCPClient.Stop();
}
//////////////////////////////////////////////////////////////////////////
// 
void  CTEServerDlg::StopConnectDataBase(){
	
	CDBData::DisConnect();
	return;
}
/////////////////////////////////////////////////////////////////////////////
//  
void CTEServerDlg::AddTCPDESInfo(CTerminalData *pTerminalData ){
	
	CString				strDESUID;
	CTerminalData		*pMapData = NULL;
	
	if( pTerminalData == NULL ){
		return;
	}
	strDESUID = pTerminalData->GetTerminalUID( );
	if( m_oMapTCPDESInfo.Lookup( strDESUID, (CObject*&)pMapData) ){
		if( pMapData->IsTCPWORKMode()){
			DeleteDESInfo( &m_oMapTCPDESInfo, strDESUID, pMapData  );
			m_oMapTCPDESInfo.SetAt( strDESUID, pTerminalData );
		}
		else{
			ASSERT(FALSE);
		}
	}
	else{
		TRACE( "DESTCPIncAccpetSocketsOK\r\n");
		
		// TETCP server已经STOP就不要处理
		if( m_oDESTCPServer.m_pSocket ){
			((CServerSocket*)(m_oDESTCPServer.m_pSocket))->IncAccpetSockets();
		}
		m_oMapTCPDESInfo.SetAt( strDESUID, pTerminalData );
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::AddUDPDESInfo(CTerminalData *pTerminalData ){
	
	CString				strDESUID;
	CTerminalData		*pMapData = NULL;
	
	
	if( pTerminalData == NULL ){
		return;
	}
	strDESUID = pTerminalData->GetTerminalUID( );
	if( m_oMapUDPDESInfo.Lookup( strDESUID, (CObject*&)pMapData) ){
		if( pMapData->IsTCPWORKMode()){				
			ASSERT(FALSE);
	//		pTerminalData->FreeMemory();
	//		delete pTerminalData;
	//		pTerminalData = NULL;
			//2011-11-09 修改
			m_oQuerySocketObject.ObjectEnterCriticalSection();
			m_oQuerySocketObject.DeleteComposeObject( pMapData->m_pClientSocket  );
			m_oQuerySocketObject.ObjectLeaveCriticalSection();

			EnterCriticalSection( &m_hSocketCriticalSection );
			DeleteDESInfo( &m_oMapTCPDESInfo, strDESUID, pMapData );
			LeaveCriticalSection( &m_hSocketCriticalSection );
		}
		else{
			if( memcmp( pMapData->GetSOCKADDR(), pTerminalData->GetSOCKADDR(), sizeof(SOCKADDR) ) != 0 ){
				DeleteDESInfo( &m_oMapUDPDESInfo, strDESUID, pMapData  );
				m_oMapUDPDESInfo.SetAt( strDESUID, pTerminalData );
			}
			else{
				pTerminalData->FreeMemory();
				delete pTerminalData;
				pTerminalData = NULL;
			}
		}
	}
	else{
		m_oMapUDPDESInfo.SetAt( strDESUID, pTerminalData );
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
//    
void  CTEServerDlg::DeleteDESInfo(  CMapStringToOb *pMapData, 
									 CString strDESUID, CTerminalData *pTerminalData ){
	
	pTerminalData->FreeMemory();
	pMapData->RemoveKey( strDESUID );
	delete pTerminalData;
	pTerminalData = NULL;
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnSockeClose( WPARAM wParam, LPARAM lParam ){

	CAsyncSocket			*pSocket=NULL;
	CAsyncSocket			*pSocket2=NULL;
	int						nCnt = 0;
	BOOL					bFind = FALSE;
	
	
	pSocket = (CAsyncSocket *)lParam;
	m_oQuerySocketObject.ObjectEnterCriticalSection();
	m_oQuerySocketObject.DeleteComposeObject( pSocket );
	m_oQuerySocketObject.ObjectLeaveCriticalSection();
	
	// 是否TETCP客户端关掉
	if( bFind == FALSE ){
		
		POSITION			pos;
		CString				strDESUID;
		CTerminalData		*pMapData = NULL;
				
		pos = m_oMapTCPDESInfo.GetStartPosition();
		while( pos ){
			m_oMapTCPDESInfo.GetNextAssoc(pos, strDESUID, (CObject*&)pMapData);
			if( pMapData->m_pClientSocket == pSocket ){
				// 包含关闭SOCKET
				// V2.05 modify
				// 2010-02-26修改
				// 原因:  在TCP方式下。设备发送数据后。TCP链接即时断链。造成服务器回发数据时找不到相应的SOCKET
				// 解决方案： 在删除SOCKET与发送数据之间加临界区处理
				EnterCriticalSection( &m_hSocketCriticalSection );
				DeleteDESInfo( &m_oMapTCPDESInfo, strDESUID, pMapData );
				LeaveCriticalSection( &m_hSocketCriticalSection );
				if( m_oDESTCPServer.m_pSocket ){
					TRACE( "DESTCPDecAcceptSocketOK\r\n");
					((CServerSocket*)(m_oDESTCPServer.m_pSocket))->DecAccpetSockets();
				}
				else{
					TRACE( "DESTCPDecAcceptSocketError\r\n");
				}
				bFind = TRUE;
				break;
			}
		}		
	}
	
	// 是否CSCLIENT连接断掉
	if( bFind == FALSE ){
		if( pSocket == m_oCSSTCPClient.m_pSocket ){
			StopCSSTCPClient();
			m_oCSSDlg.IsConnectionStatus( FALSE );
			bFind = TRUE;
		}
	}	
	if( bFind ==FALSE ){
		TRACE( "Not Found Socket\r\n" );
	}
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnSocketConnect( WPARAM wParam, LPARAM lParam ){

	SOCKET				hSocket;
	
	hSocket = (SOCKET)lParam;
	ASSERT( hSocket == m_oCSSTCPClient.m_pSocket->m_hSocket );
	// connect ok
	if( wParam ){
		m_oCSSTCPClient.m_bStart = TRUE;
		m_oCSSDlg.IsConnectionStatus( m_oCSSTCPClient.m_bStart);
	}
	else{
		m_oCSSTCPClient.m_bStart = FALSE;
		StopCSSTCPClient();
		m_oCSSDlg.IsConnectionStatus( m_oCSSTCPClient.m_bStart);
	}
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnAddDESInfo(WPARAM wParam, LPARAM lParam ){
	if( wParam == 0 ){
		AddUDPDESInfo( (CTerminalData*)lParam);
	}
	else{
		AddTCPDESInfo( (CTerminalData*)lParam);
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// 
BOOL CTEServerDlg::FindTCPDESData(CString strDESUID, CTerminalData *&pTerminalData ){
	
	BOOL				bResult = TRUE;
	
	if( m_oMapTCPDESInfo.Lookup(strDESUID, (CObject*&)pTerminalData) == FALSE ){
		pTerminalData = NULL;
		bResult = FALSE;
	}
	else{
		bResult = TRUE;
	}
	return	bResult;
}

/////////////////////////////////////////////////////////////////////////////
// 
BOOL CTEServerDlg::FindUDPDESData(CString strDESUID, CTerminalData *&pTerminalData ){
	
	BOOL				bResult = TRUE;
	
	if( m_oMapUDPDESInfo.Lookup(strDESUID, (CObject*&)pTerminalData) == FALSE ){
		pTerminalData = NULL;
		bResult = FALSE;
	}
	else{
		bResult = TRUE;
	}
	return	bResult;
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnSocketSendData( WPARAM wParam, LPARAM lParam){
	
	PSTSOCKETDATA		pSendData = (PSTSOCKETDATA)lParam;
	
	if( pSendData == NULL){
		return;
	}		
	switch( wParam ){
	case DES_TO_CS:		
		PushDataToCSSSendBuf( pSendData->m_pDataBuf, pSendData->m_nDataLen );
		break;		
	case DES_TO_DE_UDP:
		PushDataToDESUDPSendBuf( pSendData->m_pDataBuf, pSendData->m_nDataLen,  
			&pSendData->m_oSOCKADDR );
		break;		
	case DES_TO_DE_TCP: 
		PushDataToDESTCPSendBuf( pSendData->m_pSocket, pSendData->m_pDataBuf,
			pSendData->m_nDataLen, &pSendData->m_oSOCKADDR );
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return;
}
/////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::ComposeTECmdResult( PSTSENDTECMDRESULT  pSendTECmdResult,
									   CDESProtocolData	   &oDESProtocolData ){
	
	CData					oData;
	
	if ( pSendTECmdResult == NULL ){
		return ;
	}
	CAppData::ComposeData( &oData, 
							TYPE_SEND_TECMD_RESULT,
							(char*)pSendTECmdResult,
							sizeof( STSENDTECMDRESULT ));
	
	oDESProtocolData.AddAppSendData( &oData );
	oDESProtocolData.ComposeALData( CM_SEND_TECMD_RESULT );
}
//////////////////////////////////////////////////////////////////////////
// 发送数据到DES(来自CSS)
void CTEServerDlg::OnSendDataToDESFmCSS(WPARAM wParam, LPARAM lParam ){
		
	BOOL				 bSendFlag = FALSE;
	PSTSENDDEData		 pStSendDEData = NULL;
	CTerminalData		 *pTerminalData = NULL;
	

	pStSendDEData = (PSTSENDDEData)lParam;
	if ( pStSendDEData == NULL ){
		return ;
	}	
	//////////////////////////////////////////////////////////////////////////
	switch ( GetControlMode() ){
	case CONTROL_SMS:
		SendSMSDataToDE( pStSendDEData->m_nDEUID, (char*)pStSendDEData->m_pBuf, pStSendDEData->m_nLen );
		break;
	case CONTROL_GPRS:
		if( FindTCPDESData(pStSendDEData->m_nDEUID, pTerminalData)){
			ASSERT( pTerminalData != NULL );
			if( pTerminalData ){
				PushDataToDESTCPSendBuf( pTerminalData->m_pClientSocket,
					(char*)pStSendDEData->m_pBuf, 
					pStSendDEData->m_nLen,
					pTerminalData->GetSOCKADDR() );
				bSendFlag = TRUE;
			}
		}
		else if( FindUDPDESData(pStSendDEData->m_nDEUID, pTerminalData) ){
			ASSERT( pTerminalData != NULL );
			if( pTerminalData ){
				PushDataToDESUDPSendBuf( (char*)pStSendDEData->m_pBuf, 
					pStSendDEData->m_nLen,
					pTerminalData->GetSOCKADDR() );
				bSendFlag = TRUE;
			}
		}
		break;
	case CONTROL_SMS_GPRS:
		if( IsWorkTCPUDP() ){
			if( FindTCPDESData(pStSendDEData->m_nDEUID, pTerminalData)){
				ASSERT( pTerminalData != NULL );
				if( pTerminalData ){
					PushDataToDESTCPSendBuf( pTerminalData->m_pClientSocket,
						(char*)pStSendDEData->m_pBuf, 
						pStSendDEData->m_nLen,
						pTerminalData->GetSOCKADDR() );
					bSendFlag = TRUE;
				}
			}
			else if( FindUDPDESData(pStSendDEData->m_nDEUID, pTerminalData) ){
				ASSERT( pTerminalData != NULL );
				if( pTerminalData ){
					PushDataToDESUDPSendBuf( (char*)pStSendDEData->m_pBuf, 
						pStSendDEData->m_nLen,
						pTerminalData->GetSOCKADDR() );
					bSendFlag = TRUE;
				}
			}
		}
		if( IsWorkSMSMode() ){
			if( bSendFlag == FALSE ){
				SendSMSDataToDE( pStSendDEData->m_nDEUID, (char*)pStSendDEData->m_pBuf, pStSendDEData->m_nLen );
			}
		}
		break;
	}
	return;
}
//////////////////////////////////////////////////////////////////////////
// 通过Modem发送SMS数据到终端
void CTEServerDlg::SendSMSDataToDE( CString strDEUID, char *lpBuf, int nLen ){
	
	
	CString			strRecvNo;
	int				nSendNo;
	
	if( GetDESIMByDEUID( strDEUID, strRecvNo ) ){
		
		//初始化成功发送
		if( m_oDESCOMServer.IsInitCMMComplete() ){
			// 用8位编码发送		
			nSendNo = m_oDESCOMServer.SendPDUSMS( strRecvNo, lpBuf, nLen, 1 );
			// 请开线程更新数据。因为是父项是SENDMESSAGE送过来的
			SetDESSendSMSHelpInfo( strRecvNo, lpBuf, nLen, nSendNo );
		}
	}
	return;
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::PushDataToCSSSendBuf(char* lpBuf, int nLen, SOCKADDR *pPeerSockAddr /* = NULL  */){
	
	if( m_oCSSTCPClient.m_pSocket ){
		((CClientSocket*)(m_oCSSTCPClient.m_pSocket))->AddSendData( lpBuf, nLen, pPeerSockAddr );
		SetCSSTCPHelpInfo(lpBuf, nLen, FALSE );
	}
	return;
}
//////////////////////////////////////////////////////////////////////////
// 
void CTEServerDlg::PushDataToBSSTCPSendBuf(CAsyncSocket *pSocket, 
										   char         *lpBuf, 
										   int			nLen, 
										   SOCKADDR		*pPeerSockAddr /* = NULL  */){
	if ( pSocket ){
		((CClientSocket*)(pSocket))->AddSendData( lpBuf, nLen );
		SetBSTCPHelpInfo( lpBuf, nLen, FALSE, pPeerSockAddr );
	}
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::PushDataToDESUDPSendBuf(char* lpBuf, int nLen, SOCKADDR *pPeerSockAddr /* = NULL  */){
	
	if( m_oDESUDPServer.m_pSocket ){
		((CClientSocket*)(m_oDESUDPServer.m_pSocket))->AddSendData( lpBuf, nLen, pPeerSockAddr );
		SetDESUDPHelpInfo(lpBuf, nLen, FALSE, pPeerSockAddr);
	}
	return;
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::PushDataToDESTCPSendBuf(CAsyncSocket *pSocket, 
										   char         *lpBuf, 
										   int			nLen, 
										   SOCKADDR		*pPeerSockAddr /* = NULL  */){										   
	
	if( pSocket ){
		EnterCriticalSection( &m_hSocketCriticalSection );
		((CClientSocket*)(pSocket))->AddSendData( lpBuf, nLen, pPeerSockAddr );
		LeaveCriticalSection( &m_hSocketCriticalSection );
		SetDESTCPHelpInfo(lpBuf, nLen, FALSE, pPeerSockAddr);
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////
// DES　TCP Server的活动连接
int	CTEServerDlg::GetDESTCPActiveConnect( void ){ 
	
	if( m_oDESTCPServer.m_pSocket ){
		return ((CServerSocket*)(m_oDESTCPServer.m_pSocket))->GetAccpetSockets();
	}
	else{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//  DES SMS SHOW
//  parameter:
//		strRecvNo:  DE终端号码
//		lpBuf	 :  数据
//		nLen	 :  长度
// NG
void	CTEServerDlg::SetDESRecvSMSHelpInfo( CString	strRecvNo, 
											 char		*lpBuf,
											 int		nLen){

	if (m_oDES2Dlg.SetRecHelpInfo( nLen, TRUE ) ){		
		if( m_oDES2Dlg.IsShowStatus() ){
			ADDHelpData( this, strRecvNo, lpBuf, nLen, 0, TRUE );
		}
	}
	
}//////////////////////////////////////////////////////////////////////////
//  DES SMS SHOW
//  parameter:
//		strRecvNo:  DE终端号码
//		lpBuf	 :  数据
//		nLen	 :  长度
//		nMRCnt	  : 发往DE的数据参考号
// NG
void	CTEServerDlg::SetDESSendSMSHelpInfo( CString	strRecvNo, 
											char		*lpBuf,
											int			nLen,
											int			nMR ){
	
	if (m_oDES2Dlg.SetRecHelpInfo( nLen, FALSE ) ){
		if( m_oDES2Dlg.IsShowStatus() ){
			ADDHelpData( this, strRecvNo, lpBuf, nLen, nMR, FALSE );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::AddData(CString strData){
	
	int			nLen;
	
	nLen = strData.GetLength();	
	EnterCriticalSection(&m_hCriticalSection);
	m_strDataInfo += strData;
	m_strDataInfo = m_strDataInfo.Right( 500 );
	LeaveCriticalSection(&m_hCriticalSection);	
	return;
}
//////////////////////////////////////////////////////////////////////////
//  DES SMS SHOW
//  parameter:
//		nMR			: 发往DE的数据参考号
//		bSendResult	: 发送成功或失败
// NG
void	CTEServerDlg::SetDESSendSMSHelpInfo2(	int		nMR, BOOL bSendResult ){
		
	CString		strData;

	if( m_oDES2Dlg.IsShowStatus() ){
		strData.Format("MR_%d: %s", nMR, GetLangStr(TEXT_SEND_SUCCESS) );
		AddData( strData );
	}
}
//////////////////////////////////////////////////////////////////////////
//  DES UDP SHOW
void	CTEServerDlg::SetDESUDPHelpInfo(LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr /* = NULL  */){

	CString  strName;

	if (m_oDESDlg.SetRecHelpInfo( nDataLen, bRecvFlag ) ){

		strName = "DESUDP" ;
		ShowReceData( strName.GetBuffer(0), strName.GetLength(), lpBuf, nDataLen, bRecvFlag, pSockAddr  );
		strName.ReleaseBuffer();
	}
}
//////////////////////////////////////////////////////////////////////////
//
void    CTEServerDlg::SetBSTCPHelpInfo( LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr /* = NULL  */){
		
	CString		strName;

	if ( m_oBSSDlg.SetRecHelpInfo( nDataLen, bRecvFlag) ){
		strName = "BSUDP" ;
		ShowReceData( strName.GetBuffer(0), strName.GetLength(), lpBuf, nDataLen, bRecvFlag, pSockAddr  );
		strName.ReleaseBuffer();
	}
}
//////////////////////////////////////////////////////////////////////////
//  DES TCP SHOW
void	CTEServerDlg::SetDESTCPHelpInfo(LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr /* = NULL  */){

	CString  strName;

	if (m_oDESDlg.SetRecHelpInfo( nDataLen, bRecvFlag ) ){
		
		strName = "DESTCP" ;
		ShowReceData( strName.GetBuffer(0), strName.GetLength(), lpBuf, nDataLen, bRecvFlag, pSockAddr  );
		strName.ReleaseBuffer();
	}
}
//////////////////////////////////////////////////////////////////////////
//  CSS TCP SHOW
void	CTEServerDlg::SetCSSTCPHelpInfo(LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr /* = NULL  */){

	CString  strName;
	
	if (m_oCSSDlg.SetRecHelpInfo( nDataLen, bRecvFlag ) ){
		
		strName = "CSSTCP";
		ShowReceData( strName.GetBuffer(0), strName.GetLength(), lpBuf, nDataLen, bRecvFlag, pSockAddr  );
		strName.ReleaseBuffer( );
	}
}
//////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnStartStopServer(WPARAM wParam, LPARAM lParam){
		
	switch( wParam ){
	case 0: // CSS TCP
		if( m_oCSSTCPClient.m_bStart ){
			m_nConnectEnable &= ~0x01;
			StopCSSTCPClient();
			m_oCSSTCPClient.m_bStart = FALSE;
			m_oCSSDlg.IsConnectionStatus(m_oCSSTCPClient.m_bStart);			
		}
		else{			
			if ( StartCSSTCPClient() ){
				m_nConnectEnable |= 0x01;
			}			
		}
		break;
		
	case 1:  //DES TCP
		if( m_oDESTCPServer.m_bStart){
			m_nConnectEnable &= ~0x02;
			StopDESTCPServer();
			m_oDESTCPServer.m_bStart = FALSE;
			m_oDESDlg.IsConnectionStatus(m_oDESTCPServer.m_bStart);
		}
		else{
			m_nConnectEnable |= 0x02;
			m_oDESTCPServer.m_bStart = StartDESTCPServer();
			m_oDESDlg.IsConnectionStatus(m_oDESTCPServer.m_bStart);
		}
		break;
		
	case 2:  //DES UDP
		if( m_oDESUDPServer.m_bStart ){
			m_nConnectEnable &= ~0x04;
			StopDESUDPServer();
			m_oDESUDPServer.m_bStart = FALSE;
			m_oDESDlg.IsConnectionStatus(m_oDESUDPServer.m_bStart);
		}
		else{
			m_nConnectEnable |= 0x04;
			m_oDESUDPServer.m_bStart = StartDESUDPServer();
			m_oDESDlg.IsConnectionStatus( m_oDESUDPServer.m_bStart );
		}
		break;
	case 3: //DBData
		if( m_bDatabaseStart ){
			m_nConnectEnable &= ~0x08;	
			m_bDatabaseStart = FALSE;
			StopConnectDataBase( );
			m_oDBDataDlg.IsConnectionStatus( FALSE );			
		}
		else{			
			m_nConnectEnable |= 0x08;
			if( StartConnectDatabase() ){
				m_bDatabaseStart = TRUE;
			}			
			m_oDBDataDlg.IsConnectionStatus( m_bDatabaseStart );
		}
		break;
	case 4:	//BS 
// 		if ( m_oBSSTCPServer.m_bStart ){
// 			m_nConnectEnable &= ~0x10;
// 			StopBSSTCPServer();
// 			m_oBSSTCPServer.m_bStart = FALSE;
// 			m_oBSSDlg.IsConnectionStatus( m_oBSSTCPServer.m_bStart );
// 		}
// 		else{
// 			m_nConnectEnable |= 0x10;
// 			m_oBSSTCPServer.m_bStart = StartBSSTCPServer();
// 			m_oBSSDlg.IsConnectionStatus(m_oBSSTCPServer.m_bStart);
// 		}
		break;

	case 5:
		if( m_oDESCOMServer.m_bStart ){
			StopDESCOMServer( );		
			m_nConnectEnable &= ~0x20;
		}
		else{
			StartDESCOMServer( );			
			m_nConnectEnable |= 0x20;
		}
		m_oDES2Dlg.IsConnectionStatus( m_oDESCOMServer.m_bStart );
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::ShowReceData(char		*pAppName, 
								int			nAppLen,
								LPVOID		lpBuf, 
								int			nDataLen, 
								BOOL		bRecvFlag, 
								PSOCKADDR	pSockAddr /* = NULL  */){

	PSTHELPDATA	  pHelpData = NULL;

	pHelpData = new STHELPDATA;
	pHelpData->m_pBuf = (char*) malloc(nDataLen);
	pHelpData->m_pAppName = (char*) malloc( nAppLen );
	pHelpData->m_nDataLen = nDataLen;
	pHelpData->m_nAppLen = nAppLen;
	pHelpData->m_bRecFlag = bRecvFlag;
	pHelpData->pDlg = this;
	memcpy( pHelpData->m_pAppName, pAppName, nAppLen );
	memcpy( pHelpData->m_pBuf, lpBuf, nDataLen );
	if ( pSockAddr ){
		memcpy(&pHelpData->m_oSockAddr, pSockAddr, sizeof(SOCKADDR));
	}
	AfxBeginThread( AddAppHelpDataThread, pHelpData );
}

//////////////////////////////////////////////////////////////////////////
//
void  CTEServerDlg::OnEditClear(){

	m_strDataInfo.Empty();
	GetDlgItem(IDC_EDIT_DATA_INFO)->SetWindowText("");
}

//////////////////////////////////////////////////////////////////////////
//
void  CTEServerDlg::OnDESServer(){

	if( IsWorkTCPUDP() ){
		m_paneManager.ShowPane(IDR_DESPANE);	
	}
}
//////////////////////////////////////////////////////////////////////////
//
void  CTEServerDlg::OnDBSServer(){

	m_paneManager.ShowPane(IDR_DBSPANE);	
}
//////////////////////////////////////////////////////////////////////////
//
void  CTEServerDlg::OnDataBaseConnect(){

	m_paneManager.ShowPane(IDR_DATABASEPANE);	
}	
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnCssServer(){
	m_paneManager.ShowPane( IDR_CSSPANE );
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::OnComServer( ){

	if( IsWorkSMSMode()){
		m_paneManager.ShowPane( IDR_COMPANE );
	}
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::OnUpdateComServer( CCmdUI*	pCmdUI){

	if( IsWorkSMSMode() == FALSE ){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable(TRUE);
	}
}
//////////////////////////////////////////////////////////////////////////
//
void  CTEServerDlg::OnUpdateDESServer(CCmdUI* pCmdUI ){
	
	if( IsWorkTCPUDP()  == FALSE ){
		pCmdUI->Enable(FALSE);
	}
	else{
		pCmdUI->Enable(TRUE);
	}
}
//////////////////////////////////////////////////////////////////////////
//
BOOL CTEServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_RBUTTONDOWN){ 
		if(pMsg->hwnd==::GetDlgItem(m_hWnd,IDC_EDIT_DATA_INFO)){   
// 			CMenu menu;
// 			menu.LoadMenu(IDR_POP_MENU);
// 			CMenu* pMenu=menu.GetSubMenu(0);			
			
			CXTPCommandBars::TrackPopupMenu(&m_oMainMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pMsg->pt.x,pMsg->pt.y, this);			
			return   TRUE;   
		}   
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeOffice(){

	m_paneManager.SetTheme(xtpPaneThemeDefault);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeOffice2003(){
	
	m_paneManager.SetTheme(xtpPaneThemeOffice2003);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeOffice2007(){
	m_paneManager.SetTheme(xtpPaneThemeOffice2007);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeShortcutbar2003(){
	
	m_paneManager.SetTheme(xtpPaneThemeShortcutBar2003);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeWinxp(){
	m_paneManager.SetTheme(xtpPaneThemeNativeWinXP);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeWord2007(){
	m_paneManager.SetTheme(xtpPaneThemeVisio);
}
//////////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::OnThemeGrippered(){
	m_paneManager.SetTheme(xtpPaneThemeGrippered);
}
//////////////////////////////////////////////////////////////////////////
//
typedef		struct _ASTAT_{
	
	ADAPTER_STATUS	adapt;
	NAME_BUFFER		nameBuff[30];	
}ASTAT, *PASTAT;
//////////////////////////////////////////////////////////////////////////
//
CString	CTEServerDlg::GetMacAddr( ){
	
	LANA_ENUM		lana_enum;
	NCB				ncb;
	ASTAT			Adapter;	
	unsigned char	uRetCode;
	CString			strMAC;
	
	
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(lana_enum);
	uRetCode = Netbios(&ncb);
	
	if(uRetCode != NRC_GOODRET){
		return	"";
	}
	
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = lana_enum.lana[0];
	uRetCode = Netbios(&ncb);
	
	if(uRetCode != NRC_GOODRET){
		return	"";
	}
	
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char *)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);
	if(uRetCode != NRC_GOODRET){
		return	"";
	}
	
	strMAC.Format( "%02X%02X%02X%02X%02X%02X",
					Adapter.adapt.adapter_address[0],
					Adapter.adapt.adapter_address[1],
					Adapter.adapt.adapter_address[2],
					Adapter.adapt.adapter_address[3],
					Adapter.adapt.adapter_address[4],
					Adapter.adapt.adapter_address[5] );
	
	return strMAC;
}
//////////////////////////////////////////////////////////////////////////
//   
CString	 CTEServerDlg::Decode( CString	strSrc, int nKey ){
	
	char		*pEnd = NULL;
	CString		strResult;
	CString		strTmp;
	BOOL		bExit = FALSE;
	int			nCnt = 0;
	
	while ( !bExit ){
		unsigned long		ulTmp;
		char				nTmp[25];
		
		if( strSrc.GetLength() <= 4 ){
			bExit = TRUE;
			strTmp = strSrc;
		}
		else{
			strTmp = strSrc.Left(4);
			strSrc = strSrc.Mid(4);
		}
		
		ulTmp = strtoul( strTmp.GetBuffer(0), &pEnd, 16 );
		strTmp.ReleaseBuffer();
		ulTmp += nKey;
		ulTmp += nCnt++;
		
		itoa( ulTmp, nTmp, 8 );
		if( strResult.IsEmpty() == FALSE ){
			strResult += "-";
		}
		strResult += nTmp;
	}
	return strResult;
}
////////////////////////////////////////////////////////////////////
//
void CTEServerDlg::GetPrivateData(){
	
	CFile				oFile;	
	unsigned char		*lpDest = NULL;
	int					nLen = 0;
	CString				strData = "";
	
	
	if( oFile.Open( GetModulePath()+"\\1.dat", CFile::modeRead ) ){
		nLen = oFile.GetLength();
		lpDest = (unsigned char *)malloc( nLen );
		oFile.Read( lpDest, nLen );
		
		nLen = DeCodeData( lpDest, lpDest, nLen, 12 );		
		*(lpDest+nLen) = 0x00;
		strData = lpDest;
		free( lpDest );
		lpDest = NULL;
		oFile.Close();
	}
	GetStringArrayFromString( m_oStrArrayVersion, strData, ';' );

	GetStringArrayFromString( m_oStrArrayVersion, strData, ';' );
	
	if( oFile.Open( GetModulePath()+"\\2.dat", CFile::modeRead ) ){
		nLen = oFile.GetLength();
		lpDest = (unsigned char *)malloc( nLen );
		oFile.Read( lpDest, nLen );
		
		nLen = DeCodeData( lpDest, lpDest, nLen, 12 );		
		*(lpDest+nLen) = 0x00;
		strData = lpDest;
		free( lpDest );
		lpDest = NULL;
		oFile.Close();
	}
	GetStringArrayFromString( m_oStrArrayTime, strData, ';' );
	return;
}
//////////////////////////////////////////////////////////////////////////
//		比较版本ID号
BOOL	CTEServerDlg::ComparisonVersionID( DWORD	dwVersionID ){

	unsigned char		nSize = 0;
	CString				strVersion;
	CString				strTmp;
	BOOL				bResult = FALSE;
		
	strVersion.Format("%d", dwVersionID );
	for ( int nCnt = 0; nCnt < m_oStrArrayVersion.GetSize(); nCnt++ ){
		strTmp = m_oStrArrayVersion.GetAt( nCnt );
		if( strTmp == strVersion ){
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 由DEUID得到DESIM
// 如果查找不到，由数据库查询后。缓存
// NG
BOOL	CTEServerDlg::GetDESIMByDEUID( CString strDEUID,  CString& strDESIM){

	BOOL		bResult = TRUE;

	if ( m_oMapDEUIDToSIM.Lookup( strDEUID, (CString&)strDESIM) == FALSE ){
		bResult = CDBData::DBGetSIMByDEUID( strDEUID, strDESIM );
	}
	return bResult;
}
//////////////////////////////////////////////////////////////////////////
//
// NG
BOOL	CTEServerDlg::SaveDESIMandDEUID( CString strDEUID, CString strDESIM ){
	
	CString		strTmpDESIM;
	
	if ( m_oMapDEUIDToSIM.Lookup(strDEUID, (CString&)strTmpDESIM) ){
		if( strDESIM != strTmpDESIM ){
			m_oMapDEUIDToSIM.SetAt( strDEUID, strDESIM );
		}
	}
	else{
		m_oMapDEUIDToSIM.SetAt( strDEUID, strDESIM );
	}
	return FALSE;	
}
//////////////////////////////////////////////////////////////////////////
// 定时检测
void	CTEServerDlg::OnTimingCheckKey(){
	
	int			nRandomData;
	CString		strTmp;
	char		chBuf[20];
	
	KillTimer( WM_TIMER_CHECK );
	strcpy( chBuf, "MapTrack" );
	m_oSoftKey.YCompareString( chBuf, "MapTrack", strlen(chBuf) );		
	if( m_oSoftKey.YGetLastError() != 0 ){		
		SendMessage( WM_SOFTWART_ERROR_EXIT , 1, NULL );
		return;
	}	
	nRandomData = rand()%100;
	SetTimer(WM_TIMER_CHECK, 1000*60*nRandomData , NULL );
}
//////////////////////////////////////////////////////////////////////////
//	Socket 发送
void	CTEServerDlg::OnSocketSendError( WPARAM wParam, LPARAM lParam ){

	m_oSocketSendErrorDlg.SetErrorCode( wParam );
	m_oSocketSendErrorDlg.ShowWindow( SW_SHOW );
}
//////////////////////////////////////////////////////////////////////////
//
void   CTEServerDlg::OnSQLDisconnect( WPARAM	wParam, LPARAM  lParam ){
		
	m_nConnectEnable |= 0x08;	
	m_bDatabaseStart = FALSE;
	StopConnectDataBase( );
	m_oDBDataDlg.IsConnectionStatus( FALSE );
}
//////////////////////////////////////////////////////////////////////////
//  查询地图返回
void  CTEServerDlg::OnResultAddrMsg( WPARAM wParam,  LPARAM lParam ){

	BOOL				bSendFlag = FALSE;
	PSTQUERYADDR		pStQueryAddr = NULL;	
	CTerminalData		*pTerminalData = NULL;


	pStQueryAddr = (PSTQUERYADDR)lParam;
	if( pStQueryAddr == NULL ){
		return ;
	}	
	switch( GetControlMode() ){
	case CONTROL_SMS:
		SendSMSDataToDE( pStQueryAddr->m_nDEUID, (char*)pStQueryAddr->m_nMsg, pStQueryAddr->m_nMsgLen );
		break;
	case CONTROL_GPRS:
		if( FindTCPDESData( pStQueryAddr->m_nDEUID, pTerminalData)){
			ASSERT( pTerminalData != NULL );
			if( pTerminalData ){
				PushDataToDESTCPSendBuf( pTerminalData->m_pClientSocket,
										 (char*)pStQueryAddr->m_nMsg, 
										 pStQueryAddr->m_nMsgLen,
										 pTerminalData->GetSOCKADDR() );
				bSendFlag = TRUE;
			}
		}
		else if( FindUDPDESData( pStQueryAddr->m_nDEUID, pTerminalData) ){
			ASSERT( pTerminalData != NULL );
			if( pTerminalData ){
				PushDataToDESUDPSendBuf( (char*)pStQueryAddr->m_nMsg, 
										 pStQueryAddr->m_nMsgLen,
										 pTerminalData->GetSOCKADDR() );
				bSendFlag = TRUE;
			}
		}
		break;
	case CONTROL_SMS_GPRS:
		if( IsWorkTCPUDP() ){
			if( FindTCPDESData(pStQueryAddr->m_nDEUID, pTerminalData)){
				ASSERT( pTerminalData != NULL );
				if( pTerminalData ){
					PushDataToDESTCPSendBuf( pTerminalData->m_pClientSocket,
											(char*)pStQueryAddr->m_nMsg, 
											pStQueryAddr->m_nMsgLen,
											pTerminalData->GetSOCKADDR() );
					bSendFlag = TRUE;
				}
			}
			else if( FindUDPDESData(pStQueryAddr->m_nDEUID, pTerminalData) ){
				ASSERT( pTerminalData != NULL );
				if( pTerminalData ){
					PushDataToDESUDPSendBuf( (char*)pStQueryAddr->m_nMsg, 
										     pStQueryAddr->m_nMsgLen,
											 pTerminalData->GetSOCKADDR() );
					bSendFlag = TRUE;
				}
			}
		}
		if( IsWorkSMSMode() ){
			if( bSendFlag == FALSE ){
				SendSMSDataToDE( pStQueryAddr->m_nDEUID, (char*)pStQueryAddr->m_nMsg, pStQueryAddr->m_nMsgLen );
			}
		}
		break;
	}
	if( pStQueryAddr ){
		delete pStQueryAddr;
		pStQueryAddr = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
//  
void  CTEServerDlg::OnQueryGGAddr( WPARAM wParam, LPARAM lParam ){

	PSTQUERYADDR		pQueryAddr = NULL;
	
	pQueryAddr = (PSTQUERYADDR)lParam;

	if( pQueryAddr ){
		m_oQueryGGAdd.Push_back( pQueryAddr );
	}
	if( pQueryAddr ){
		delete pQueryAddr;
		pQueryAddr = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
//  保存GPS数据到LIST
void   CTEServerDlg::PushGPSToDBlist( int  nDataType, char	*lpBuf, int	nDataLen  ){
	
	CData1				*pData = NULL;
	
	if( lpBuf == NULL ){
		return;
	}
	pData = new CData1();
	ASSERT( pData );
	pData->AddData( (char*)lpBuf, nDataLen );	
	pData->SetDataType( nDataType );
	m_lstDBData.push_back( pData );
}
//////////////////////////////////////////////////////////////////////////
//
int	CTEServerDlg::SaveGpsData( PSTGPSDATA  pGpsData ){
	
	CString			strData;
	PSTGPSDATA		pTmpGpsData = NULL;
	int				nMileage = 0;
	
	ASSERT(pGpsData);
	if ( m_oMapDEUIDToGPSData.Lookup( pGpsData->m_nDEUID, (void*&)pTmpGpsData)){
		
		if( (pGpsData->m_bGPSValid) && (pGpsData->m_nSpeed >= 8) ){  //定位计算距离
			
			nMileage = CalculateDistance( pTmpGpsData->m_dbLat, 
										  pTmpGpsData->m_dbLon, 
										  pGpsData->m_dbLat, 
										  pGpsData->m_dbLon );

			pTmpGpsData->m_bGPSValid	= pGpsData->m_bGPSValid;
			pTmpGpsData->m_dbLat		= pGpsData->m_dbLat;
			pTmpGpsData->m_dbLon		= pGpsData->m_dbLon;
			pTmpGpsData->m_nAlarmState	= pGpsData->m_nAlarmState;
			pTmpGpsData->m_nCodeState	= pGpsData->m_nCodeState;
			pTmpGpsData->m_nDirection	= pGpsData->m_nDirection;
			pTmpGpsData->m_nDriverTime	= pGpsData->m_nDriverTime;
			pTmpGpsData->m_nHWState		= pGpsData->m_nHWState;
			pTmpGpsData->m_nMileage		= pGpsData->m_nMileage;
			pTmpGpsData->m_nSpeed		= pGpsData->m_nSpeed;
			pTmpGpsData->m_oTime		= pGpsData->m_oTime;
			if( nMileage >= MAX_SPACEMILEAGE){
				nMileage = MIN_SPACEMILEAGE;
			}
			
// 时间比较 时间大于GPS时间退出
#if ( TIME_CONFIG_VERSION )
		strData.Format("%d", pTmpGpsData->m_oTime );
		m_nVersionTime = atoi( strData.GetBuffer(0) );
		strData.ReleaseBuffer();
		if( m_nVersionTime >= TIME_CONFIG_TIME){
			PostMessage(WM_COMMAND, IDOK, 0 );
		}		
#endif
		}		
	}
	else{
		if( pGpsData->m_bGPSValid ){

			pTmpGpsData = new STGPSDATA;
			memset( pTmpGpsData, 0x00, sizeof(STGPSDATA));
			memcpy( pTmpGpsData, pGpsData, sizeof(STGPSDATA));		
			m_oMapDEUIDToGPSData.SetAt( pGpsData->m_nDEUID, pTmpGpsData );
		}
	}
	return nMileage;
}
//////////////////////////////////////////////////////////////////////////
//
PSTGPSDATA		CTEServerDlg::FindGpsData( CString	  strDEUID ){
	
	PSTGPSDATA		pGpsData = NULL;
	
	if ( m_oMapDEUIDToGPSData.Lookup( strDEUID, (void*&)pGpsData) ){
		return pGpsData;
	}
	else{
		return NULL;
	}
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::DelGpsData(CString		  strDEUID ){
	
	PSTGPSDATA		pGpsData = NULL;
	
	if ( m_oMapDEUIDToGPSData.Lookup( strDEUID, (void*&)pGpsData) ){
		
		if ( pGpsData != NULL ){
			delete pGpsData;
			pGpsData = NULL;
		}
		m_oMapDEUIDToGPSData.RemoveKey( strDEUID );
	}
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::DelAllGpsData( void ){
	
	CString			strKey;
	POSITION		pos = NULL;
	PSTGPSDATA		pGpsData = NULL;
	
	pos = m_oMapDEUIDToGPSData.GetStartPosition();
	while ( pos ){		
		m_oMapDEUIDToGPSData.GetNextAssoc( pos, strKey, (void*&)pGpsData );
		if ( pGpsData != NULL ){
			delete	pGpsData;
			pGpsData = NULL;
		}
	}
	m_oMapDEUIDToGPSData.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////
//
double  CTEServerDlg::Rad(double d){
	
	return d * PI / 180.0;
}
//////////////////////////////////////////////////////////////////////////
//   
double  CTEServerDlg::CalculateDistance(double lat1, double lng1, double lat2, double lng2){
	
	double radLat1 = Rad(lat1);
	double radLat2 = Rad(lat2);
	double a = radLat1 - radLat2;
	double b = Rad(lng1) - Rad(lng2);
	
	
	double s = 2 * asin( sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)) );
	s = s * EARTH_RADIUS;	
	
	s *= 10000;	
	return s;
}
//////////////////////////////////////////////////////////////////////////
// 盲点数据
BOOL	CTEServerDlg::GetBeforeBlinkGPSData( STGPSDATA		&oldGPSData, 
											 PSTGPSDATA		pStGPSData ){
	BOOL			bResult = FALSE;
	PSTGPSDATA		pTmpGpsData = NULL;
	
	ASSERT(pStGPSData);
	if ( m_oMapDEUIDToBlindGPSData.Lookup( pStGPSData->m_nDEUID, (void*&)pTmpGpsData)){
				
		if( (pStGPSData->m_bGPSValid) && (pStGPSData->m_nSpeed >= 8) ){  //定位计算距离			
			
			//时间大于1小时不处理
			if( ( pStGPSData->m_oTime - pTmpGpsData->m_oTime) < 1*60*60 ){
				bResult = TRUE;
			}
			memcpy( &oldGPSData, pTmpGpsData, sizeof(STGPSDATA) );
			pTmpGpsData->m_bGPSValid	= pStGPSData->m_bGPSValid;
			pTmpGpsData->m_dbLat		= pStGPSData->m_dbLat;
			pTmpGpsData->m_dbLon		= pStGPSData->m_dbLon;
			pTmpGpsData->m_nAlarmState	= pStGPSData->m_nAlarmState;
			pTmpGpsData->m_nCodeState	= pStGPSData->m_nCodeState;
			pTmpGpsData->m_nDirection	= pStGPSData->m_nDirection;
			pTmpGpsData->m_nDriverTime	= pStGPSData->m_nDriverTime;
			pTmpGpsData->m_nHWState		= pStGPSData->m_nHWState;
			pTmpGpsData->m_nMileage		= pStGPSData->m_nMileage;
			pTmpGpsData->m_nSpeed		= pStGPSData->m_nSpeed;
			pTmpGpsData->m_oTime		= pStGPSData->m_oTime;	
		}				
	}
	else{
		if( (pStGPSData->m_bGPSValid) && (pStGPSData->m_nSpeed >= 8) ){  //定位计算距离
			
			pTmpGpsData = new STGPSDATA;
			memset( pTmpGpsData, 0x00, sizeof(STGPSDATA));
			memcpy( pTmpGpsData, pStGPSData, sizeof(STGPSDATA));		
			m_oMapDEUIDToBlindGPSData.SetAt( pStGPSData->m_nDEUID, pTmpGpsData );
		}
	}	
	return bResult;
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::DelBlinkGpsData( CString		strDEUID ){
	
	PSTGPSDATA		pGpsData = NULL;
	
	if ( m_oMapDEUIDToBlindGPSData.Lookup( strDEUID, (void*&)pGpsData) ){
		
		if ( pGpsData != NULL ){
			delete pGpsData;
			pGpsData = NULL;
		}
		m_oMapDEUIDToBlindGPSData.RemoveKey( strDEUID );
	}
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::DelAllBlinkpsData( void ){
	
	CString			strKey;
	POSITION		pos = NULL;
	PSTGPSDATA		pGpsData = NULL;
	
	pos = m_oMapDEUIDToBlindGPSData.GetStartPosition();
	while ( pos ){		
		m_oMapDEUIDToBlindGPSData.GetNextAssoc( pos, strKey, (void*&)pGpsData );
		if ( pGpsData != NULL ){
			delete	pGpsData;
			pGpsData = NULL;
		}
	}
	m_oMapDEUIDToBlindGPSData.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////
//
void	CTEServerDlg::SetServerIP( unsigned char *pBuf,  int  nLen ){

	char			nBuf[21];
	
	if( nLen > 20 ){
		nLen = 20;
	}
	memset( nBuf, 0x00, 21 );
	memcpy( nBuf, pBuf, nLen );
	nBuf[nLen] = 0x00;
	m_strServerIP.Format("%s",  nBuf );
}