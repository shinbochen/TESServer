#if !defined(AFX_DBSDLG_H__DD8EE1EF_2D0A_495C_AD93_529B10C7E8E9__INCLUDED_)
#define AFX_DBSDLG_H__DD8EE1EF_2D0A_495C_AD93_529B10C7E8E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// "CSSDlg.h" : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCSSDlg dialog

class CCSSDlg : public CDialog
{
// Construction
public:
	CCSSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCSSDlg)
	enum { IDD = IDD_CSS_DLG };
	CButton		m_oShowCheck;
	CXTButton	m_oPropertyButton;
	CXTButton	m_oConnectionButton;
	CString	m_strConnNumber;
	CString	m_strLocalIP;
	CString	m_strRecePack;
	CString	m_strRumTime;
	CString	m_strSendPack;
	CString	m_strState;
	//}}AFX_DATA
public:
	BOOL			m_bStart;
	// 接收包
	int				m_nRecvPackage;
	// 发送包
	int				m_nSendPackage;
	// 连接数
	int				m_nConnectNumber;
	// DBS 端口号 
	UINT			m_nTCPPort;
	// DBS IP
	CString			m_strTCPIP;
	//  是否显示状态
	BOOL			m_bShowStatus;
public:
	void	SetCSSTCPPort( UINT nPort ) { m_nTCPPort = nPort; }
	void    SetLoaclIP( CString strIP)  { m_strLocalIP = strIP; }
	void	SetCSSTCPIP( CString strIP ) { m_strTCPIP = strIP; }

	UINT    GetCSSTCPPort( void ) { return  m_nTCPPort; }
	CString	GetCSSTCPIP( void )   { return  m_strTCPIP;}
public:
	void    InitLabel( void );
	void    IsConnectionStatus( BOOL  bStateus );
	void	UpdateWindowState( void );
	BOOL	SetRecHelpInfo( int	nDataLen, BOOL	bRecv);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSSDlg)
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

#endif // !defined(AFX_DBSDLG_H__DD8EE1EF_2D0A_495C_AD93_529B10C7E8E9__INCLUDED_)
