#if !defined(AFX_BSSDLG_H__56DD8077_1944_4E3B_BEA5_4765BCD4B0A9__INCLUDED_)
#define AFX_BSSDLG_H__56DD8077_1944_4E3B_BEA5_4765BCD4B0A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BSSDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBSSDlg dialog

class CBSSDlg : public CDialog
{
// Construction
public:
	CBSSDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBSSDlg)
	enum { IDD = IDD_BSS_DLG };
	CButton		m_oShowCheck;
	CXTButton	m_oPropertyButton;
	CXTButton	m_oConnectionButton;
	CString		m_strConnNumber;
	CString		m_strLocalIP;
	CString		m_strRecePack;
	CString		m_strRumTime;
	CString		m_strSendPack;
	CString		m_strState;
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
	void		SetBSSTCPPort( UINT nTCPPort ) { m_nTCPPort = nTCPPort; }
	UINT		GetBSSTCPPort( void )		   { return m_nTCPPort; }

	void		SetBSSTCPIP( CString strTCPIP )   { m_strTCPIP = strTCPIP; }
	CString		GetBSSTCPIP( void )            { return m_strTCPIP; }

	void		SetLocalIP( CString  strLocalIP )   { m_strLocalIP = strLocalIP; }
public:
	void        InitLabel( void );
	void        UpdateWindowState( void );
	void        IsConnectionStatus( BOOL  bStateus );
	BOOL        SetRecHelpInfo( int	nDataLen, BOOL	bRecv);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBSSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBSSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPropertyButton();
	afx_msg void OnConnectButton();
	afx_msg void OnClose();
	afx_msg void OnShowCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BSSDLG_H__56DD8077_1944_4E3B_BEA5_4765BCD4B0A9__INCLUDED_)
