// TEServerDlg.h : header file
//

#if !defined(AFX_TESERVERDLG_H__75637894_E3B5_4A67_9ADB_C21053EC0D02__INCLUDED_)
#define AFX_TESERVERDLG_H__75637894_E3B5_4A67_9ADB_C21053EC0D02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 

#include "DESDlg.h"
#include "CSSDlg.h"
#include "DBDataDlg.h"
#include "BSSDlg.h"
#include "TerminalData.h"
#include "Resource.h"
#include "SocketSendErrorDlg.h"

#include "..\public\ServerSocket.h"
#include "..\public\QuerySocketObject.h"
#include "..\public\Structs.h"
#include "..\public\ProtocolData.h"
#include "..\public\SoftKey.h"
#include "..\public\Comm.h"
#include "..\public\QueryGGAddr.h"
#include <list>


#define    USER_LIMIT_VEHICLE_0						0
#define    USER_LIMIT_VEHICLE_200					200
#define    USER_LIMIT_VEHICLE_500					500
#define    USER_LIMIT_VEHICLE_5000					5000

//////////////////////////////////////////////////////////////////////////
// 两点最大充许距离30000
#define   MAX_SPACEMILEAGE	30000
#define   MIN_SPACEMILEAGE	255

//////////////////////////////////////////////////////////////////////////
//
enum{
	IDC_SUBMENU	= 5000,
	//==============================
	IDC_EDIE_CLEAR,
	//==============================
	IDC_DES_SERVER,
	IDC_CSS_SERVER,
	IDC_DBS_SERVER,
	IDC_DATABASE_CONNECT,
	//==============================
	ID_THEME_OFFICE,
	ID_THEME_OFFICE2003,
	ID_THEME_WINXP,
	ID_THEME_GRIPPERED,
	ID_THEME_SHORTCUTBAR2003,
	ID_THEME_OFFICE2007,
	ID_THEME_WORD2007
};

//////////////////////////////////////////////////////////////////////////
//	服务器的控制方式
enum{
	CONTROL_SMS = 0x00,
	CONTROL_GPRS,
	CONTROL_SMS_GPRS,
};

class CTerminalData;
class CSocketThread;

/////////////////////////////////////////////////////////////////////////////
// CTEServerDlg dialog

class CTEServerDlg : public CDialog
{
// Construction
public:
	CTEServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTEServerDlg)
	enum { IDD = IDD_TESERVER_DIALOG };
	CString		m_strDataInfo;
	CEdit		m_oDataInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEServerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CXTTrayIcon				m_TrayIcon;
	CMenu					m_oMainMenu;
	CMenu					m_oSubMenu1;
public: //(2013-07-12)
	CString					m_strServerIP;			  //远程IP地址	
	BOOL					m_bConfigIP;			  //修改远程IP内容

	void					SetServerIP( unsigned char *pBuf,  int  nLen );
	CString					GetServerIP( void ){ return  m_strServerIP; }

	void					SetAutoConfigIP( BOOL bConfigIP ) { m_bConfigIP = bConfigIP; }
	BOOL				    IsAutoConfigIP(){ return m_bConfigIP; }
public:
	CString					m_strVersion;

	int						m_nVersionTime;

public:
	//  DES 监听UDP端口号
	UINT					m_nDESUDPPort;
	//  DES 监听TCP端口号
	UINT					m_nDESTCPPort;
	//  DES 监听终端IP地址
	CString					m_strDESTCPIP;
	//  CSS  TCP端口号
	UINT					m_nCSSTCPPort;
	//  CSS  TCP IP地址
	CString					m_strCSSTCPIP;
	//  BSS  TCP端口号
	UINT					m_nBSSTCPPort;
	//  BSS	 TCP IP地址
	CString				    m_strBSSTCPIP;
	//  Local IP
	CString					m_strlocalIP;
	//  数据库名
	CString					m_strDatabaseName;
	//  服务器名
	CString					m_strServerName;
	//  用户名
	CString					m_strUserName;
	//  用户密码
	CString					m_strUserPsd;
	//  授权码
	CString					m_strAuthor;
	
	// DES工作模式:
	// 0:1	DES_WORKTCPUDP / DES_WORKSMS
	int						m_nDESWorkMode;
	
	int						m_nCOMPort;
	int						m_nCOMBaudRate;
	// 设备控制模式
	int						m_nControlMode;
		
	CString					m_strImagePPath;
	CString					m_strImageVPath;
public:
	CStringArray			m_oStrArrayVersion;
	CStringArray			m_oStrArrayTime;
public:								
	HANDLE					m_hDBThread;
	std::list<CData1*>		m_lstDBData;				//保存GPS数据SQL(2011-03-09)
	CMapStringToPtr			m_oMapDEUIDToGPSData;		//通过DEUID查询GPS数示

	CMapStringToPtr			m_oMapDEUIDToBlindGPSData;	//保存盲点数据

public:													
	//GPS内存数据的管理
	int						SaveGpsData( PSTGPSDATA  pGpsData );
	PSTGPSDATA				FindGpsData( CString	  strDEUID );
	void					DelGpsData(CString		  strDEUID );
	void					DelAllGpsData( void );
	
	// 盲点数据
	BOOL					GetBeforeBlinkGPSData( STGPSDATA	&oldGPSData, PSTGPSDATA		pStGPSData );
	void					DelBlinkGpsData( CString		strDEUID );
	void					DelAllBlinkpsData( void );	

	double					Rad(double d);
	double					CalculateDistance(double lat1, double lng1, double lat2, double lng2);
public:
	// 维护终端IP地址，工作模式等数据
	CMapStringToOb			m_oMapTCPDESInfo;
	CMapStringToOb			m_oMapUDPDESInfo;
	CMapStringToString		m_oMapDEUIDToSIM;
	int						m_nConnectEnable;
	BOOL					m_bDatabaseStart;

public:
	CXTPDockingPane*		m_pWndDESPane;
	CXTPDockingPane*		m_pWndCSSPane;
	CXTPDockingPane*		m_pWndBSSPane;
	CXTPDockingPane*		m_pWndDBDataPane;
	CXTPDockingPane*		m_pWndCOMPane;
	CXTPDockingPaneManager  m_paneManager;
public:
	CDESDlg					m_oDESDlg;
	CDES2Dlg				m_oDES2Dlg;
	CCSSDlg					m_oCSSDlg;
	CBSSDlg					m_oBSSDlg;
	CDBDataDlg				m_oDBDataDlg;
	SoftKey					m_oSoftKey;
	CSocketSendErrorDlg		m_oSocketSendErrorDlg;
public:
	// Socket UDP DES Server
	CSocketThread			m_oDESUDPServer;
	// Socket TCP DES Server
	CSocketThread			m_oDESTCPServer;
	// COM DES Server
	CComm					m_oDESCOMServer;
	// query address Server
	CQueryGGAddr			m_oQueryGGAdd;

	// Socket TCP BSS Server
	CSocketThread			m_oBSSTCPServer;
	// Socket TCP CSS Server
	CSocketThread			m_oCSSTCPClient;
public:
	CQuerySocketObject	    m_oQuerySocketObject;
public:			
	CXTPStatusBar		   m_wndStatusBar;
public:
	CRITICAL_SECTION		m_hCriticalSection;
	// 处理SOCKET是否关闭
	CRITICAL_SECTION		m_hSocketCriticalSection;

	
	void					AddData(CString strData);
public:
	void					InitApp( void );
	void					InitWindowShow();
	void					GetInitStatusBar( void );
	void					GetInitConfig( void );
	CString					GetMacAddr( );
	CString					Decode( CString	strSrc, int nKey );
public:	 
	void					SetWorkSMSMode( void ){ m_nDESWorkMode |= DES_WORKSMS; }
	void					SetWorkTCPUDP( void ){ m_nDESWorkMode |= DES_WORKTCPUDP; }
	
	BOOL					IsWorkSMSMode( void ) { return (m_nDESWorkMode&DES_WORKSMS)?TRUE:FALSE; }	
	BOOL					IsWorkTCPUDP( void ){ return (m_nDESWorkMode&DES_WORKTCPUDP)?TRUE:FALSE; }
	// 把数据压入列表
	void					PushGPSToDBlist( int  nDataType, char	*lpBuf, int	nDataLen  );
	
public:
	void	RestoreInitData( void );
	void	SaveInitData( void );
	void	GetInitData( void );
	CString GetIniFileName( void );
	void	GetPrivateData();
	BOOL	ComparisonVersionID( DWORD	dwVersionID );
	void	OnTimingCheckKey();
public:
	// 建立SOCKET 联接
	BOOL	StartDESUDPServer( void );
	BOOL	StartDESTCPServer( void );	
	BOOL    StartDESCOMServer(void);
	BOOL	StartBSSTCPServer( void );
	BOOL	StartCSSTCPClient( void );
	BOOL    StartConnectDatabase(void);
	// 关闭SOCKET 联接
	void    StopDESUDPServer( void );
	void    StopDESTCPServer( void );
	void	StopDESCOMServer( void );
	void	StopBSSTCPServer( void );
	void    StopCSSTCPClient( void );
	void	StopConnectDataBase( void );
	//  释放所有数据
	void	FreeMomery( void );
public:
	//  获取控制模式
	int		GetControlMode( void ) { return m_nControlMode; }
	// DES 
	UINT	GetDESUDPPort( void ) { return m_nDESUDPPort; } 
	UINT	GetDESTCPPort( void ) { return m_nDESTCPPort; }	
	CString	GetDESTCPIP( void )   { return m_strDESTCPIP; }
	// CSS
	UINT    GetCSSTCPPort( void ) { return m_nCSSTCPPort; }	
	CString GetCSSTCPIP( void )   { return m_strCSSTCPIP; }
	// BSS
	UINT	GetBSSTCPPort( void ) { return m_nBSSTCPPort;}
	CString GetBSSTCPIP( void )   { return m_strBSSTCPIP;}
	// 建立临时的UDP端口
	CSocket* CreateUDPSocket(  ){ 
		CSocket	*pSocket = NULL;
		pSocket = new CSocket;
		pSocket->Create(0,SOCK_DGRAM);
		return pSocket;
	}
public:
	
	void  AddTCPDESInfo(CTerminalData *pTerminalData );
	
	void  AddUDPDESInfo(CTerminalData *pTerminalData );
	
	void  DeleteDESInfo(CMapStringToOb	*pMapData, 
			 		    CString			strDESUID, 
					    CTerminalData	*pTerminalData );
public:
	void ComposeTECmdResult( PSTSENDTECMDRESULT  pSendTECmdResult, CDESProtocolData	   &oDESProtocolData );
	void OnAddDESInfo(WPARAM wParam, LPARAM lParam );
	BOOL FindTCPDESData(CString strDESUID, CTerminalData *&pTerminalData );
	BOOL FindUDPDESData(CString strDESUID, CTerminalData *&pTerminalData );
	void PushDataToCSSSendBuf( char* lpBuf, int nLen, SOCKADDR *pPeerSockAddr = NULL );
	void PushDataToDESUDPSendBuf( char* lpBuf, int nLen, SOCKADDR *pPeerSockAddr = NULL );
	void PushDataToDESTCPSendBuf( CAsyncSocket *pSocket, char* lpBuf, int nLen, SOCKADDR *pPeerSockAddr = NULL );
	void PushDataToBSSTCPSendBuf( CAsyncSocket *pSocket, char *lpBuf, int nLen, SOCKADDR *pPeerSockAddr = NULL );
	BOOL GetDESIMByDEUID( CString strDEUID,  CString& strDESIM );
	BOOL SaveDESIMandDEUID( CString strDEUID, CString strDESIM );
	void SendSMSDataToDE( CString strDEUID, char *lpBuf, int nLen );
	
public:
	void OnSocketSendData( WPARAM wParam, LPARAM lParam);
	void OnSendDataToDESFmCSS(WPARAM wParam, LPARAM lParam );
	int	 GetDESTCPActiveConnect( void );
	
public:
	void	CTEServerDlg::SetDESRecvSMSHelpInfo( CString	strRecvNo, 
												 char		*lpBuf,
												 int		nLen);
	void	CTEServerDlg::SetDESSendSMSHelpInfo( CString	strRecvNo, 
												 char		*lpBuf,
												 int		nLen,
												 int		nMR );
	void	CTEServerDlg::SetDESSendSMSHelpInfo2(	int		nMR, BOOL bSendResult );

	void	SetDESUDPHelpInfo( LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr = NULL );
	void	SetDESTCPHelpInfo( LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr = NULL );
	void	SetCSSTCPHelpInfo( LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr = NULL );
	void	SetBSTCPHelpInfo(  LPVOID lpBuf, int nDataLen, BOOL bRecvFlag, PSOCKADDR pSockAddr = NULL );
	void    ShowReceData(char		*pAppName, 
						int			nAppLen,
						LPVOID		lpBuf, 
						int			nDataLen, 
						BOOL		bRecvFlag, 
						PSOCKADDR	pSockAddr  = NULL );

public:
	BOOL		IsCSSTCPClientConnect( ){ return m_oCSSTCPClient.m_bStart;}
	BOOL		IsDESTCPServerConnect( ){ return m_oDESTCPServer.m_bStart;}
	BOOL		IsDESUDPServerConnect( ){ return m_oDESUDPServer.m_bStart;}
	BOOL		IsBSSTCPServerConnect( ){ return m_oBSSTCPServer.m_bStart;}
	BOOL		IsDatabaseConnect()     { return m_bDatabaseStart;}
	BOOL		IsCOMServerConnect()	{ return m_oDESCOMServer.m_bStart;};
// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CTEServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg	void OnEditClear();
	afx_msg	void OnDESServer();
	afx_msg	void OnDBSServer();
	afx_msg	void OnDataBaseConnect();
	afx_msg void OnThemeOffice();
	afx_msg void OnThemeOffice2003();
	afx_msg void OnThemeOffice2007();
	afx_msg void OnThemeShortcutbar2003();
	afx_msg void OnThemeWinxp();
	afx_msg void OnThemeWord2007();
	afx_msg void OnThemeGrippered();
	afx_msg void OnExit();
	afx_msg void OnCssServer();
	afx_msg void OnComServer();
	afx_msg void OnUpdateComServer( CCmdUI*	pCmdUI);
	afx_msg void OnUpdateDESServer( CCmdUI*	pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	afx_msg void	SaveParameterConfig( WPARAM wParam, LPARAM lParam );
	afx_msg void	OnSockeClose( WPARAM wParam, LPARAM lParam );
	afx_msg void	OnSocketConnect( WPARAM wParam, LPARAM lParam );
	afx_msg void    OnStartStopServer(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnSoftwartErrorEixt( WPARAM  wParam, LPARAM  lParam );
	afx_msg void	OnRecvCOMData( WPARAM  wParam, LPARAM  lParam );
	afx_msg void	OnSendCOMDataResult( WPARAM  wParam, LPARAM  lParam );
	afx_msg void	OnSocketSendError( WPARAM wParam, LPARAM lParam );
	afx_msg void    OnSQLDisconnect( WPARAM	wParam, LPARAM  lParam );

	afx_msg void	OnResultAddrMsg( WPARAM wParam,  LPARAM lParam );
	afx_msg void    OnQueryGGAddr( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};

#ifndef  __STHELPDATA
#define  __STHELPDATA
typedef	struct _STHELPDATA{	
	char			 *m_pAppName;
	int				  m_nAppLen;
	char			 *m_pBuf;
	int				  m_nDataLen;
	BOOL			  m_bRecFlag;
	SOCKADDR		  m_oSockAddr;
	CTEServerDlg	  *pDlg;
}STHELPDATA, *PSTHELPDATA;
#endif  //__STHELPDATA
//////////////////////////////////////////////////////////////////////////
//
#ifndef   __STBSSSENDDATA
#define   __STBSSSENDDATA
typedef struct  _STBSSSENDDATA{
	
	CDESProtocolData	m_oDESProtocolData;
	STSOCKETDATA		m_oSendData;
}STBSSSENDDATA,*PSTBSSSENDDATA;
#endif  //__STBSSSENDDATA

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESERVERDLG_H__75637894_E3B5_4A67_9ADB_C21053EC0D02__INCLUDED_)
