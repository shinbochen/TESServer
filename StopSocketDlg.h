#if !defined(AFX_STOPSOCKETDLG_H__9F393BC3_7E20_4802_87E9_3492C646A08F__INCLUDED_)
#define AFX_STOPSOCKETDLG_H__9F393BC3_7E20_4802_87E9_3492C646A08F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StopSocketDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStopSocketDlg dialog

class CStopSocketDlg : public CDialog
{
// Construction
public:
	CStopSocketDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStopSocketDlg)
	enum { IDD = IDD_STOPSOCKET_DLG };
	CButton	m_oUDPCheck;
	CButton	m_oTcpCheck;
	BOOL	m_bTCP;
	BOOL	m_bUDP;
	//}}AFX_DATA
public:
	BOOL		m_bCOMMode;
public:
	void	SetCOMConfig( BOOL	nCOMMode ) { m_bCOMMode = nCOMMode; }
	void	InitLabel( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopSocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStopSocketDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPSOCKETDLG_H__9F393BC3_7E20_4802_87E9_3492C646A08F__INCLUDED_)
