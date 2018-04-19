#if !defined(AFX_DBSDLG_H__DD8EE1EF_2D0A_495C_AD93_529B10C7E8E9__INCLUDED_)
#define AFX_DBSDLG_H__DD8EE1EF_2D0A_495C_AD93_529B10C7E8E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBSDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDBSDlg dialog

class CDBSDlg : public CDialog
{
// Construction
public:
	CDBSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDBSDlg)
	enum { IDD = IDD_DBS_DLG };
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
	void	SetDBSTCPPort( UINT nPort ) { m_nTCPPort = nPort; }
	void    SetLoaclIP( CString strIP)  { m_strLocalIP = strIP; }
	void	SetDBSTCPIP( CString strIP ) { m_strTCPIP = strIP; }

	UINT    GetCSSTCPPort( void ) { return  m_nTCPPort; }
	CString	GetCSSTCPIP( void )   { return  m_strLocalIP ;}
public:
	void    IsConnectionStatus( BOOL  bStateus );
	void	UpdateWindowState( void );
	BOOL	SetRecHelpInfo( int	nDataLen, BOOL	bRecv);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDBSDlg)
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
