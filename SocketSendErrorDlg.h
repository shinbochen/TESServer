#if !defined(AFX_SOCKETSENDERRORDLG_H__6DD2F043_4280_4A83_966C_F7FE9EF7702E__INCLUDED_)
#define AFX_SOCKETSENDERRORDLG_H__6DD2F043_4280_4A83_966C_F7FE9EF7702E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketSendErrorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSocketSendErrorDlg dialog

class CSocketSendErrorDlg : public CDialog
{
// Construction
public:
	CSocketSendErrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSocketSendErrorDlg)
	enum { IDD = IDD_SOCKET_ERROR_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void	SetErrorCode( int	iErrorCode );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketSendErrorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSocketSendErrorDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETSENDERRORDLG_H__6DD2F043_4280_4A83_966C_F7FE9EF7702E__INCLUDED_)
