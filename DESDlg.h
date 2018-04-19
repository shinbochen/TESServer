#if !defined(AFX_DESDLG_H__ADB43FE0_5B7E_4EBB_92AC_9CE4802028AA__INCLUDED_)
#define AFX_DESDLG_H__ADB43FE0_5B7E_4EBB_92AC_9CE4802028AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DESDlg.h : header file
//
#include "resource.h"
#include "StopSocketDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CDESDlg dialog

class CDESDlg : public CDialog
{
// Construction
public:
	CDESDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDESDlg)
	enum { IDD = IDD_DES_DLG };
	CButton		m_oShowCheck;
	CXTButton	m_oPropertyButton;
	CXTButton	m_oConnectButton;
	CString		m_strRecePack;
	CString		m_strSendPack;
	CString		m_strState;
	CString		m_strRumTime;
	CString		m_strLocalIP;
	CString		m_strConnNumber;
	//}}AFX_DATA
public:
	BOOL			m_bStart;
	// 接收包
	int				m_nRecvPackage;
	// 发送包
	int				m_nSendPackage;
	// 连接数
	int				m_nConnectNumber;
	//  UDP端口
	UINT			m_nUDPPort;
	//  TCP端口
	UINT			m_nTCPPort;
	//  是否显示状态
	BOOL			m_bShowStatus;
public:
	CStopSocketDlg	 m_oStopSocketDlg;
public:
	void	SetDESUDPPort( UINT  nPort ){ m_nUDPPort = nPort; }
	void    SetDESTCPPort( UINT  nPort ) { m_nTCPPort = nPort;}
	void    SetDESLocalIP( CString strIP ) { m_strLocalIP = strIP;}

	UINT	GetDESUDPPort( void ) { return m_nUDPPort; }
	UINT	GetDESTCPPort( void ) { return m_nTCPPort; }
	CString	GetDESTCPIP( void ) { return m_strLocalIP;}
public:
	void    InitLabel( void );
	void    IsConnectionStatus( BOOL  bStateus );
	void	UpdateWindowState( void );
	BOOL	SetRecHelpInfo( int	nDataLen, BOOL	bRecv);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDESDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDESDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnPropertyButton();
	afx_msg void OnShowCheck();
	afx_msg void OnConnectButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CDES2Dlg dialog
class CDES2Dlg : public CDialog
{
	// Construction
public:
	CDES2Dlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDES2Dlg)
	enum { IDD = IDD_DES_DLG2 };
	CButton		m_oShowCheck;
	CXTButton	m_oPropertyButton;
	CXTButton	m_oConnectButton;
	CString		m_strRecePack;
	CString		m_strSendPack;
	CString		m_strState;
	CString		m_strRumTime;
	CString		m_strLocalIP;
	//}}AFX_DATA
public:
	BOOL			m_bStart;
	// 接收包
	int				m_nRecvPackage;
	// 发送包
	int				m_nSendPackage;
	// 连接数
	int				m_nConnectNumber;
	// 串口
	int				m_nCOMPort;
	// 
	UINT			m_nCOMBaudRate;
	//  是否显示状态
	BOOL			m_bShowStatus;
public:
	CStopSocketDlg	 m_oStopSocketDlg;
public:
	void	SetDES2ComPort( int	nCOMPort,	UINT		nCOMBaudRate );
	int		GetCOMPort( void ) { return m_nCOMPort; }
	UINT	GetCOMBaudRate( void ){ return  m_nCOMBaudRate; }

public:
	void    InitLabel( void );
	void    IsConnectionStatus( BOOL  bStateus );
	void	UpdateWindowState( void );
	BOOL	SetRecHelpInfo( int	nDataLen, BOOL	bRecv);
	BOOL	IsShowStatus() { return m_bShowStatus; }
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDES2Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDES2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnPropertyButton();
	afx_msg void OnShowCheck();
	afx_msg void OnConnectButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESDLG_H__ADB43FE0_5B7E_4EBB_92AC_9CE4802028AA__INCLUDED_)
