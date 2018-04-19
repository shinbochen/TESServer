#if !defined(AFX_CSSDLG_H__148BF389_0ED6_4959_A2C9_9B10364FBBB8__INCLUDED_)
#define AFX_CSSDLG_H__148BF389_0ED6_4959_A2C9_9B10364FBBB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #include "DBDataDlg.h": header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDBDataDlg dialog

class CDBDataDlg : public CDialog
{
// Construction
public:
	CDBDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDBDataDlg)
	enum { IDD = IDD_DBDATA_DLG };
	CXTButton	m_oConnectButton;	
	CString		m_strDatabaseName;
	CString		m_strServerName;
	CString		m_strStausInfo;
	CString		m_strUserName;
	CString		m_strUserPsd;
	//}}AFX_DATA
public:
	BOOL	    m_bConnection;
public:
	void		InitLabel( void );
	// 服务器名
	void		SetServerName( CString strServerName ) { m_strServerName = strServerName;}
	CString		GetServerName( void ) { return m_strServerName; }
	// 数据库名
	void		SetDatabaseName( CString strDatabaseName ) { m_strDatabaseName = strDatabaseName; }
	CString		GetDatabaseName( void ) { return m_strDatabaseName; }
	// 用户名
	void		SetUserName( CString  strUserName ) { m_strUserName = strUserName; }
	CString		GetUserName( void ) { return m_strUserName; }
	// 密码
	void		SetUserPsd( CString strUserPsd ) {  m_strUserPsd = strUserPsd; }
	CString		GetUserPsd( void ) { return m_strUserPsd ;}
	//  是否连接成功	
	void		IsConnectionStatus( BOOL  bFlag );

public:	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDBDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnConnectButton();
	afx_msg void OnChangeServerName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSSDLG_H__148BF389_0ED6_4959_A2C9_9B10364FBBB8__INCLUDED_)
