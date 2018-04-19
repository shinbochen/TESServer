#if !defined(AFX_CONFIGDLG_H__D514F5D2_72D5_4CF9_8BAF_562443C4724C__INCLUDED_)
#define AFX_CONFIGDLG_H__D514F5D2_72D5_4CF9_8BAF_562443C4724C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG_DLG };
	CXTButton	m_oOn;
	CXTButton	m_oCancel;
	CString	m_strIPName;
	CString	m_strNamePort1;
	CString	m_strPort2;
	CString	m_strIP;
	UINT	m_nPort1;
	UINT	m_nPort2;
	//}}AFX_DATA
public:
	int			m_nTypeMode;
public:
	void   InitLabel( void );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__D514F5D2_72D5_4CF9_8BAF_562443C4724C__INCLUDED_)
