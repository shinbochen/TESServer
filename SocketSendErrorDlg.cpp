// SocketSendErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "SocketSendErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketSendErrorDlg dialog


CSocketSendErrorDlg::CSocketSendErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketSendErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSocketSendErrorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSocketSendErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketSendErrorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSocketSendErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketSendErrorDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketSendErrorDlg message handlers

void	CSocketSendErrorDlg::SetErrorCode( int iErrorCode ){

	CString		strData;

	strData.Format("Error: %d!", iErrorCode );
	GetDlgItem(IDC_ERROR_CODE)->SetWindowText( strData );
}
