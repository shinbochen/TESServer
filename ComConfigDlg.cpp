// ComConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEServer.h"
#include "ComConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg dialog


CComConfigDlg::CComConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComConfigDlg)
	m_nPort = -1;
	m_nBaudrate = -1;
	//}}AFX_DATA_INIT
}


void CComConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComConfigDlg)
	DDX_Control(pDX, IDC_COMBO_PORT, m_ctlPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ctlBaudrate);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_nPort);
	DDX_CBIndex(pDX, IDC_COMBO_BAUDRATE, m_nBaudrate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CComConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg message handlers

BOOL CComConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int		nTmp = 0;

	for( int nCnt = 0; nCnt<= 20; nCnt++){
		CString			str;
		
		str.Format("COM%d", nCnt+1 );
		m_ctlPort.AddString( str );
	}
	InitBaudrate( m_nCOMnBuadrate );
	UpdateData(FALSE);
	return TRUE; 
}
//////////////////////////////////////////////////////////////////////////
//
void	CComConfigDlg::InitBaudrate( int	nBaudrate ) {

	switch( nBaudrate ){
	case 4800:
		m_nBaudrate = 0;
		break;
	case 9600:
		m_nBaudrate = 1;
		break;
	case 14400:
		m_nBaudrate = 2;
		break;
	case 19200:
		m_nBaudrate = 3;
		break;
	case 38400:
		m_nBaudrate = 4;
		break;
	case 57600:
		m_nBaudrate = 5;
		break;
	case 115200:
		m_nBaudrate = 6;
		break;
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//
int		CComConfigDlg::GetBaudrate( void ) { 
	
	int		nResult = 9600;
	
	switch( m_nBaudrate){
	case 0:
		nResult = 4800;
		break;
	case 1:
		nResult = 9600;
		break;
	case 2:
		nResult = 14400;
		break;
	case 3:
		nResult = 19200;
		break;
	case 4:
		nResult = 38400;
		break;
	case 5:
		nResult = 57600;
		break;
	case 6:
		nResult = 115200;
		break;
	}
	return nResult; 
}
//////////////////////////////////////////////////////////////////////////
//
int		CComConfigDlg::GetComPort( void )  {

	return m_nPort+1;
 }

void CComConfigDlg::OnOK() 
{
	UpdateData(TRUE)	;
	CDialog::OnOK();
}
