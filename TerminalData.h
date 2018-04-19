//////////////////////////////////////////////////////////////////////////
// file Name:  public.h
// Time     :  
//author    :  

#ifndef		TERMINALDATA_H
#define     TERMINALDATA_H

enum{
	TERMINAL_WORK_TCP = 0,
	TERMINAL_WORK_UDP=1
};

class CTerminalData : public CObject{
	
public:
	CTerminalData( );
	virtual ~CTerminalData();
	
public:
	CString				m_strDESUID;
	int					m_nWorkMode;
	CAsyncSocket		*m_pClientSocket;
	// ∂‘∑Ωsockµÿ÷∑
	SOCKADDR			m_oPeerSockAddr;	
	
public:
	void		FreeMemory( void );
	void		SetTermialUID( CString str) { m_strDESUID = str;}
	void		SetWorkMode( int nMode ){ASSERT((nMode==TERMINAL_WORK_TCP)||(nMode==TERMINAL_WORK_UDP)); m_nWorkMode=nMode;  }
	BOOL		IsTCPWORKMode( void ){ return (m_nWorkMode==TERMINAL_WORK_TCP) ? TRUE:FALSE;}
	CString		GetTerminalUID( void ){ return m_strDESUID;}
	void		SetTerminalSocket( CAsyncSocket *pSocket ){ m_pClientSocket = pSocket; }
	
	void		StopSocket( void );
	
	SOCKADDR*	GetSOCKADDR( void ){ return &m_oPeerSockAddr;}
	void		CopySockAddr( SOCKADDR *pSockAddr ){ memcpy( &m_oPeerSockAddr, pSockAddr,sizeof(SOCKADDR) );}
};
#endif		//TERMINALDATA_H