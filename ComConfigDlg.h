#if !defined(AFX_COMCONFIGDLG_H__1ECC2CEC_DFF7_42F1_9131_DE79A7A2928A__INCLUDED_)
#define AFX_COMCONFIGDLG_H__1ECC2CEC_DFF7_42F1_9131_DE79A7A2928A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg dialog

class CComConfigDlg : public CDialog
{
// Construction
public:
	CComConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComConfigDlg)
	enum { IDD = IDD_COM_CONFIG_DLG };
	CComboBox	m_ctlPort;
	CComboBox	m_ctlBaudrate;
	int		m_nPort;
	int		m_nBaudrate;
	//}}AFX_DATA
public:
	int		m_nCOMnBuadrate;
public:
	void	InitBaudrate( int	nBaudrate );
	void	SetBaudrate( int nBuadrate ) { m_nCOMnBuadrate = nBuadrate; }
	void	SetCOMPort( int  nPort ){ m_nPort = nPort-1 ; }

	int		GetBaudrate( void );
	int		GetComPort( void );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMCONFIGDLG_H__1ECC2CEC_DFF7_42F1_9131_DE79A7A2928A__INCLUDED_)
