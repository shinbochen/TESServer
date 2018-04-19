//////////////////////////////////////////////////////////////////////////
// file Name:  public.h
// Time     :  2008-11-19
//author    :  xxx

#include "StdAfx.h"
#include "TerminalData.h"

// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
CTerminalData::CTerminalData(){
	
	m_strDESUID="";
	m_nWorkMode = TERMINAL_WORK_UDP;
	m_pClientSocket = NULL;
}
//////////////////////////////////////////////////////////////////////
//
CTerminalData::~CTerminalData(){
	FreeMemory( );	
}

//////////////////////////////////////////////////////////////////////
//
void CTerminalData::FreeMemory(){

	StopSocket( );
}


//////////////////////////////////////////////////////////////////////
//
void CTerminalData::StopSocket( void ){
	
	try{
		if( m_pClientSocket ){

			m_pClientSocket->ShutDown(2);
			m_pClientSocket->Close();
			delete m_pClientSocket;
			m_pClientSocket = NULL;
		}
	}
	catch(...){
		return;
	}
	return;
}