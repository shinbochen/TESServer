//////////////////////////////////////////////////////////////////////////
// file Name:  Thread.h
// Time     :  2008-11-19
//author    :  xxx

#include "StdAfx.h"
#include "Thread.h"
#include "TEServerDlg.h"
#include "..\public\ProtocolData.h"

//////////////////////////////////////////////////////////////////////////
//   接收DE SUDP数据
UINT  RecvDESUDPDataProcess( LPVOID lParam ){

	unsigned int		nResult = 0;	
	PSTGPSDATA			pGpsData = NULL;
	PSTQUERYADDR		pQueryAddr = NULL;
	PSTSOCKETDATA		pRecvDESData = NULL;
	CTerminalData		*pTerminalData = NULL;
	CData				oData;
	CDESProtocolData	oDESProtocolData;
	
	CTEServerDlg	    *pDlg = (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;
	pRecvDESData  = (PSTSOCKETDATA)lParam;
	
	pDlg->SetDESUDPHelpInfo(pRecvDESData->m_pDataBuf, 
							pRecvDESData->m_nDataLen, 
							TRUE, 
							&pRecvDESData->m_oSOCKADDR );	

	if( oDESProtocolData.ParseDESData( (unsigned char*)pRecvDESData->m_pDataBuf,
									    pRecvDESData->m_nDataLen ) ){	

		pTerminalData = new CTerminalData;
		if( pTerminalData ){

			pTerminalData->SetWorkMode( TERMINAL_WORK_UDP );
			pTerminalData->SetTermialUID( oDESProtocolData.GetDESUID());
			pTerminalData->SetTerminalSocket( NULL );
			pTerminalData->CopySockAddr( &pRecvDESData->m_oSOCKADDR );
		
			pDlg->SendMessage( WM_ADDDESINFO, 0, (LPARAM)pTerminalData );
		}
		
		std::list<int>::iterator   it;
		STSOCKETDATA		oSendData;

		it = oDESProtocolData.m_lstSendMode.begin();
		while ( it != oDESProtocolData.m_lstSendMode.end() ){			
			CData		oData;
			
			oDESProtocolData.GetTLSendData( &oData );
			oSendData.m_pDataBuf = oData.GetDataBuf();
			oSendData.m_nDataLen = oData.GetDataLen();
			oSendData.m_pSocket = NULL;
			if ( *it == DES_TO_CS ){	
				pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_CS, (LPARAM)&oSendData );
			}
			else if ( *it == DES_TO_DE ){
				memcpy( &oSendData.m_oSOCKADDR, &pRecvDESData->m_oSOCKADDR, sizeof(SOCKADDR));				
				pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_DE_UDP, (LPARAM)&oSendData);
				
#if( AUTO_CONFIG_IP )
				if( pDlg->IsAutoConfigIP() ){
										
					CDEProtocolB  oProtocolB;
					CData1		  oData1;

					oProtocolB.ComposeSetupServer( &oData1,
												   11, 
												   oDESProtocolData.GetDESUID(), 
												   8868, 
												   pDlg->GetServerIP() );
					
					oSendData.m_pDataBuf = oData1.GetDataBuf();
					oSendData.m_nDataLen = oData1.GetDataLen();
					memcpy( &oSendData.m_oSOCKADDR, &pRecvDESData->m_oSOCKADDR, sizeof(SOCKADDR));

					pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_DE_UDP, (LPARAM)&oSendData);
				}
#endif //AUTO_CONFIG_IP
			}
			// 收到IP地址
#if( AUTO_CONFIG_IP )
			else if( *it == DES_AUTO_CONFIG_IP ){
				
				pDlg->SetAutoConfigIP( TRUE );
				pDlg->SetServerIP( (unsigned char*)oData.GetDataBuf(), oData.GetDataLen() );
				TRACE("Server: %s\r\n", pDlg->GetServerIP() );
			}
#endif //AUTO_CONFIG_IP
			oDESProtocolData.m_lstSendMode.pop_front();
			it = oDESProtocolData.m_lstSendMode.begin();
		}	
		//
		std::list<CData1*>::iterator		it1;

		it1 = oDESProtocolData.m_lstDeviceData.begin();
		while( it1 != oDESProtocolData.m_lstDeviceData.end() ){

			delete *it1;
			it1++;
		}
		oDESProtocolData.m_lstDeviceData.clear();

		// 查询地址
		it1 = oDESProtocolData.m_lstQueryGGAddr.begin();
		while( it1 != oDESProtocolData.m_lstQueryGGAddr.end() ){
			
			pGpsData = (PSTGPSDATA)(*it1)->GetDataBuf();
			pQueryAddr = new STQUERYADDR;
			ASSERT( pQueryAddr );
			memset( pQueryAddr, 0x00, sizeof(STQUERYADDR) );
			memcpy( pQueryAddr->m_nDEUID, pGpsData->m_nDEUID, sizeof(STDEUID)-1 );
			pQueryAddr->m_dbLat = pGpsData->m_dbLat;
			pQueryAddr->m_dbLng = pGpsData->m_dbLon;
			pQueryAddr->m_nSpeed = pGpsData->m_nSpeed;
			pQueryAddr->m_LangMode = (*it1)->GetDataType();

			pDlg->PostMessage( WM_QUERY_GG_ADDR, NULL, (long)pQueryAddr );

			delete *it1;
			oDESProtocolData.m_lstQueryGGAddr.pop_front();
			it1 = oDESProtocolData.m_lstQueryGGAddr.begin();
		}

		//	保存GPS到SQL数据库
		it1 = oDESProtocolData.m_lstDBData.begin();
		while ( it1 != oDESProtocolData.m_lstDBData.end() ){
			
			CData1	*pData = NULL;

			pData = new CData1();
			pData->SetDataType( (*it1)->GetDataType() );
			pData->AddData( (*it1)->GetDataBuf(), (*it1)->GetDataLen() );
			
			AfxBeginThread( SaveDBSQLThread, pData  );

		//	pDlg->PushGPSToDBlist( (*it1)->GetDataType(), 
		//						   (*it1)->GetDataBuf(), 
		//						   (*it1)->GetDataLen() );
			delete *it1;
			oDESProtocolData.m_lstDBData.pop_front();
			it1 = oDESProtocolData.m_lstDBData.begin();
		}
	}

	free( pRecvDESData->m_pDataBuf );
	delete pRecvDESData;
	pRecvDESData = NULL;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//   接收DES TCP数据
UINT  RecvDESTCPDataProcess( LPVOID lParam ){
	
	CData				oData;
	CDESProtocolData	oDESProtocolData;
	PSTGPSDATA			pGpsData = NULL;
	PSTQUERYADDR		pQueryAddr = NULL;
	PSTSOCKETDATA		pRecvDESData = NULL;
	CTerminalData		*pTerminalData = NULL;	
	
	CTEServerDlg       *pDlg =  (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;
	pRecvDESData = (PSTSOCKETDATA)lParam;	
	
	pDlg->SetDESTCPHelpInfo(pRecvDESData->m_pDataBuf, 
							pRecvDESData->m_nDataLen, 
							TRUE, 
							&pRecvDESData->m_oSOCKADDR );			
	if( oDESProtocolData.ParseDESData( (unsigned char*)pRecvDESData->m_pDataBuf, 
										pRecvDESData->m_nDataLen ) ){
		 
		if( pRecvDESData->m_bFirstRecv ){
			
			pTerminalData = new CTerminalData;
			
			if( pTerminalData ){
				pTerminalData->SetWorkMode( TERMINAL_WORK_TCP );
				pTerminalData->SetTermialUID( oDESProtocolData.GetDESUID( ) );
				pTerminalData->SetTerminalSocket( pRecvDESData->m_pSocket );
				pTerminalData->CopySockAddr( &pRecvDESData->m_oSOCKADDR );
				
				pDlg->SendMessage( WM_ADDDESINFO, 1, (LPARAM)pTerminalData );
			}
		}

		std::list<int>::iterator   it;
		STSOCKETDATA		oSendData;
		
		it = oDESProtocolData.m_lstSendMode.begin();
		while ( it != oDESProtocolData.m_lstSendMode.end() ){			
			CData		oData;
			
			oDESProtocolData.GetTLSendData( &oData );
			oSendData.m_pDataBuf = oData.GetDataBuf();
			oSendData.m_nDataLen = oData.GetDataLen();
			oSendData.m_pSocket = NULL;
			if ( *it == DES_TO_CS ){	
				pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_CS, (LPARAM)&oSendData );
			}
			else if ( *it == DES_TO_DE ){

				oSendData.m_pSocket = pRecvDESData->m_pSocket;
				memcpy( &oSendData.m_oSOCKADDR, &pRecvDESData->m_oSOCKADDR, sizeof(SOCKADDR));
				pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_DE_TCP, (LPARAM)&oSendData);

#if( AUTO_CONFIG_IP )
				if( pDlg->IsAutoConfigIP() ){
										
					CDEProtocolB  oProtocolB;
					CData1		  oData1;

					oProtocolB.ComposeSetupServer( &oData1,
												   11, 
												   oDESProtocolData.GetDESUID(), 
												   8868, 
												   pDlg->GetServerIP() );
					
					oSendData.m_pSocket = pRecvDESData->m_pSocket;
					oSendData.m_pDataBuf = oData1.GetDataBuf();
					oSendData.m_nDataLen = oData1.GetDataLen();
					memcpy( &oSendData.m_oSOCKADDR, &pRecvDESData->m_oSOCKADDR, sizeof(SOCKADDR));

					pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_DE_TCP, (LPARAM)&oSendData);
				}
#endif //AUTO_CONFIG_IP
			}
			// 收到IP地址
#if( AUTO_CONFIG_IP )
			else if( *it == DES_AUTO_CONFIG_IP ){
				
				pDlg->SetAutoConfigIP( TRUE );				
				pDlg->SetServerIP( (unsigned  char*)oData.GetDataBuf(), oData.GetDataLen() );
				TRACE("Server: %s\r\n", pDlg->GetServerIP() );
			}
#endif //AUTO_CONFIG_IP
			oDESProtocolData.m_lstSendMode.pop_front();
			it = oDESProtocolData.m_lstSendMode.begin();			
		}

		std::list<CData1*>::iterator		it1;
		
		it1 = oDESProtocolData.m_lstDeviceData.begin();
		while( it1 != oDESProtocolData.m_lstDeviceData.end() ){
			
			delete *it1;
			it1++;
		}
		oDESProtocolData.m_lstDeviceData.clear();

		// 查询地址
		it1 = oDESProtocolData.m_lstQueryGGAddr.begin();
		while( it1 != oDESProtocolData.m_lstQueryGGAddr.end() ){
			
			pGpsData = (PSTGPSDATA)(*it1)->GetDataBuf();
			pQueryAddr = new STQUERYADDR;
			ASSERT( pQueryAddr );
			memset( pQueryAddr, 0x00, sizeof(STQUERYADDR) );
			memcpy( pQueryAddr->m_nDEUID, pGpsData->m_nDEUID, sizeof(STDEUID)-1 );
			pQueryAddr->m_dbLat = pGpsData->m_dbLat;
			pQueryAddr->m_dbLng = pGpsData->m_dbLon;
			pQueryAddr->m_nSpeed = pGpsData->m_nSpeed;
			pQueryAddr->m_LangMode = (*it1)->GetDataType();
			
			pDlg->PostMessage( WM_QUERY_GG_ADDR, NULL, (long)pQueryAddr );
			
			delete *it1;
			oDESProtocolData.m_lstQueryGGAddr.pop_front();
			it1 = oDESProtocolData.m_lstQueryGGAddr.begin();
		}

		//	保存GPS到SQL数据库
		it1 = oDESProtocolData.m_lstDBData.begin();
		while ( it1 != oDESProtocolData.m_lstDBData.end() ){
			
			CData1		*pData = NULL;

			pData = new CData1();
			pData->SetDataType( (*it1)->GetDataType() );
			pData->AddData( (*it1)->GetDataBuf(), (*it1)->GetDataLen() );
			
			AfxBeginThread( SaveDBSQLThread, pData  );

		//	pDlg->PushGPSToDBlist( (*it1)->GetDataType(), 
		//						   (*it1)->GetDataBuf(), 
		//						   (*it1)->GetDataLen() );
			delete *it1;
			oDESProtocolData.m_lstDBData.pop_front();
			it1 = oDESProtocolData.m_lstDBData.begin();
		}
	}
	//test
	//pRecvDESData->m_pSocket->Send("0123456789", 10);
	//////////////////////////////////////////////////////////////////////////
	free( pRecvDESData->m_pDataBuf );
	delete pRecvDESData;
	pRecvDESData = NULL;
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//   接收DES TCP数据
UINT  RecvDESSMSDataProcess( LPVOID lParam ){
	
	CData				oData;
	CDESProtocolData	oDESProtocolData;
	PSTPDUDATA			pRecvPDUData = NULL;
	CTerminalData		*pTerminalData = NULL;	
	
	CTEServerDlg       *pDlg =  (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;
	pRecvPDUData = (PSTPDUDATA)lParam;	
	

	pDlg->SetDESRecvSMSHelpInfo( pRecvPDUData->m_nReceiver, (char*)pRecvPDUData->m_pTPUDBuf, pRecvPDUData->m_nTPUDBufLen);			
	
	if( oDESProtocolData.ParseDESData( (unsigned char*)pRecvPDUData->m_pTPUDBuf, 
										pRecvPDUData->m_nTPUDBufLen ) ){
		
		// 保存并更新DESIM与DEUID
		pDlg->SaveDESIMandDEUID( oDESProtocolData.GetDESUID(), pRecvPDUData->m_nReceiver );

		std::list<int>::iterator   it;
		STSOCKETDATA		oSendData;
		
		it = oDESProtocolData.m_lstSendMode.begin();
		while ( it != oDESProtocolData.m_lstSendMode.end() ){			
			CData		oData;
			
			oDESProtocolData.GetTLSendData( &oData );
			oSendData.m_pDataBuf = oData.GetDataBuf();
			oSendData.m_nDataLen = oData.GetDataLen();
			oSendData.m_pSocket = NULL;
			if ( *it == DES_TO_CS ){	
				pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_CS, (LPARAM)&oSendData );
			}
			else if ( *it == DES_TO_DE ){
				pDlg->SendSMSDataToDE( oDESProtocolData.GetDESUID(), oSendData.m_pDataBuf, oSendData.m_nDataLen );
			}
			oDESProtocolData.m_lstSendMode.pop_front();
			it = oDESProtocolData.m_lstSendMode.begin();			
		}
		
		//	保存GPS到SQL数据库
		std::list<CData1*>::iterator		it1;
		it1 = oDESProtocolData.m_lstDBData.begin();
		while ( it1 != oDESProtocolData.m_lstDBData.end() ){
			
			CData1		*pData = NULL;
			
			pData = new CData1();
			pData->SetDataType( (*it1)->GetDataType() );
			pData->AddData( (*it1)->GetDataBuf(), (*it1)->GetDataLen() );
			
			AfxBeginThread( SaveDBSQLThread, pData  );
			
		//		pDlg->PushGPSToDBlist( (*it1)->GetDataType(), 
		//							   (*it1)->GetDataBuf(), 
		//							   (*it1)->GetDataLen() );
			delete *it1;
			oDESProtocolData.m_lstDBData.pop_front();
			it1 = oDESProtocolData.m_lstDBData.begin();
		}
	}
	delete pRecvPDUData;
	pRecvPDUData = NULL;
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//  接收CSS  TCP数据
UINT RecvCSSTCPDataProcess( LPVOID lParam ){

	BOOL						bFind = FALSE;	
	int							nResultLen = 0;
	CDESProtocolData			oDESProtocolData;
	CComposeData				*pComposeData = NULL;
	char						*lpBuf = NULL;	
	PSTSOCKETDATA				pRecvData = NULL;
	CTerminalData				*pTerminalData = NULL;

	CTEServerDlg				*pDlg = (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;
		
	pRecvData = (PSTSOCKETDATA)lParam;
	pDlg->SetCSSTCPHelpInfo( pRecvData->m_pDataBuf, pRecvData->m_nDataLen,
		TRUE, &pRecvData->m_oSOCKADDR );
	try{
		if( pRecvData->m_bFirstRecv ){			
			pComposeData = new CComposeData();
			pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
			pDlg->m_oQuerySocketObject.AddSocketToCompose( pRecvData->m_pSocket, pComposeData );
			pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
		}
	}
	catch(...){
		if( pRecvData->m_pDataBuf){
			free( pRecvData->m_pDataBuf );
		}
		delete pRecvData;
		pRecvData = NULL;
		return 0;
	}
	
	pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
	pComposeData = (CComposeData*)pDlg->m_oQuerySocketObject.QuerySocketToCompose( pRecvData->m_pSocket );
	if (pComposeData != NULL){
		pComposeData->AddRecvData( pRecvData->m_pDataBuf, pRecvData->m_nDataLen );
		bFind = TRUE;
	}
	else{
		bFind = FALSE; 
	}
	pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
	while ( bFind ){			
		TRACE("Receive CSS OK\r\n");
		pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
		pComposeData = (CComposeData*)pDlg->m_oQuerySocketObject.QuerySocketToCompose( pRecvData->m_pSocket );
		if ( pComposeData != NULL){
			lpBuf = pComposeData->GetOutData( nResultLen );
		}
		pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
		if ( lpBuf == NULL ){
			break;
		}
		else{			
			STSOCKETDATA				oSendData;
			std::list<int>::iterator	it;

			// 协议解析		
			oDESProtocolData.ParseCSSRecvData((unsigned char*)lpBuf, nResultLen );			
			it = oDESProtocolData.m_lstSendMode.begin();
			while ( it != oDESProtocolData.m_lstSendMode.end() ){
				
				CData		oData;
				
				oDESProtocolData.GetTLSendData( &oData );				
				if ( *it == DES_TO_CS ){					
					oSendData.m_pDataBuf = oData.GetDataBuf();
					oSendData.m_nDataLen = oData.GetDataLen();
					oSendData.m_pSocket = NULL;
					pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_CS, (LPARAM)&oSendData );
				}
				if ( *it == DES_TO_DE ){	
					STSENDDEData		oSendDEData;
					CString				strDEUID;	

					memset( &oSendDEData, 0x00, sizeof(STSENDDEData) );
					strDEUID = oDESProtocolData.GetDESUID();
					strncpy( oSendDEData.m_nDEUID, strDEUID.GetBuffer(0), sizeof(STDEUID)-1 );
					strDEUID.ReleaseBuffer();
					oSendDEData.m_pBuf = (unsigned char*)oData.GetDataBuf();
					oSendDEData.m_nLen = oData.GetDataLen();
					pDlg->SendMessage( WM_SOCKET_SEND_DATATODE_FMCS,  NULL, (LPARAM)&oSendDEData );
				}
				oDESProtocolData.m_lstSendMode.pop_front();
				it = oDESProtocolData.m_lstSendMode.begin();
			}	
			free( lpBuf );
			lpBuf = NULL;
		}		
	}
	free( pRecvData->m_pDataBuf );
	delete pRecvData;
	pRecvData = NULL;
	
	return 0;
} 


//////////////////////////////////////////////////////////////////////////
//   接收BSS TCP数据
UINT RecvBSSTCPDataProcess( LPVOID lParam ){

	BOOL						bFind = FALSE;	
	int							nResultLen = 0;
	CDESProtocolData			oDESProtocolData;
	CComposeData				*pComposeData = NULL;
	char						*lpBuf = NULL;	
	PSTSOCKETDATA				pRecvData = NULL;
	CTerminalData				*pTerminalData = NULL;
	
	CTEServerDlg				*pDlg = (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;
	
	pRecvData = (PSTSOCKETDATA)lParam;
	pDlg->SetCSSTCPHelpInfo( pRecvData->m_pDataBuf, pRecvData->m_nDataLen,
		TRUE, &pRecvData->m_oSOCKADDR );
	if( pRecvData->m_bFirstRecv ){			
		pComposeData = new CComposeData();
		pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
		pDlg->m_oQuerySocketObject.AddSocketToCompose( pRecvData->m_pSocket, pComposeData );
		pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
	}	
	
	pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
	pComposeData = (CComposeData*)pDlg->m_oQuerySocketObject.QuerySocketToCompose( pRecvData->m_pSocket );
	if (pComposeData != NULL){
		pComposeData->AddRecvData( pRecvData->m_pDataBuf, pRecvData->m_nDataLen );
		bFind = TRUE;
	}
	else{
		bFind = FALSE; 
	}
	pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
	while ( bFind ){			
		TRACE("Receive CSS OK\r\n");
		pDlg->m_oQuerySocketObject.ObjectEnterCriticalSection();
		pComposeData = (CComposeData*)pDlg->m_oQuerySocketObject.QuerySocketToCompose( pRecvData->m_pSocket );
		if ( pComposeData != NULL){
			lpBuf = pComposeData->GetOutData( nResultLen );
		}
		pDlg->m_oQuerySocketObject.ObjectLeaveCriticalSection();
		if ( lpBuf == NULL ){
			break;
		}
		else{			
			STSOCKETDATA				oSendData;
			std::list<int>::iterator	it;

			// 协议解析		
			oDESProtocolData.ParseCSSRecvData((unsigned char*)lpBuf, nResultLen );			
			it = oDESProtocolData.m_lstSendMode.begin();
			while ( it != oDESProtocolData.m_lstSendMode.end() ){				
				CData		oData;
				
				oDESProtocolData.GetTLSendData( &oData );				
				if ( *it == DES_TO_CS ){					
					oSendData.m_pDataBuf = oData.GetDataBuf();
					oSendData.m_nDataLen = oData.GetDataLen();
					oSendData.m_pSocket = NULL;
					pDlg->SendMessage( WM_SOCKET_SENDDATA, DES_TO_CS, (LPARAM)&oSendData );
				}
				if ( *it == DES_TO_DE ){	
					CString				strDEUID;
					STSENDDEData		oSendDEData;
					
					memset( &oSendDEData, 0x00, sizeof(STSENDDEData) );
					strDEUID = oDESProtocolData.GetDESUID();
					strncpy( oSendDEData.m_nDEUID, strDEUID.GetBuffer(0), sizeof(STDEUID)-1 );
					strDEUID.ReleaseBuffer();
					oSendDEData.m_pBuf = (unsigned char*)oData.GetDataBuf();
					oSendDEData.m_nLen = oData.GetDataLen();
					pDlg->SendMessage( WM_SOCKET_SEND_DATATODE_FMCS,  NULL, (LPARAM)&oSendDEData );
				}
				oDESProtocolData.m_lstSendMode.pop_front();
				it = oDESProtocolData.m_lstSendMode.begin();
			}	
			free( lpBuf );
			lpBuf = NULL;
		}		
	}
	free( pRecvData->m_pDataBuf );
	delete pRecvData;
	pRecvData = NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//
UINT StartSocketThread( LPVOID lParam ){
	
	unsigned char	nCnt = 0;
	BOOL            bDatabaseFlag = FALSE;
	CTEServerDlg	*pDlg = (CTEServerDlg	*)AfxGetApp()->m_pMainWnd;
	
	while(1){
		
		if( pDlg->m_nConnectEnable & 0x01 ){
			if( !pDlg->IsCSSTCPClientConnect( ) ){
				pDlg->SendMessage( WM_SOCKET_STARTSTOP, 0, 0);
				Sleep( 200 );
			}
		}
		if( pDlg->IsWorkTCPUDP() ){
			if( pDlg->m_nConnectEnable & 0x02 ){
				if( !pDlg->IsDESTCPServerConnect( ) ){
					pDlg->SendMessage( WM_SOCKET_STARTSTOP, 1, 0);
					Sleep( 200 );
				}
			}
			if( pDlg->m_nConnectEnable & 0x04 ){
				if( !pDlg->IsDESUDPServerConnect( ) ){
					pDlg->SendMessage( WM_SOCKET_STARTSTOP, 2, 0);
					Sleep( 200 );
				}
			}	
		}
		if ( bDatabaseFlag == FALSE ){
			if( nCnt++ > 100 ){
				bDatabaseFlag = TRUE;
			}
			if( pDlg->m_nConnectEnable & 0x08 ){
				if( !pDlg->IsDatabaseConnect( ) ){
					pDlg->SendMessage( WM_SOCKET_STARTSTOP, 3, 0);
					Sleep( 200 );
				}
			}
		}
		if ( pDlg->m_nConnectEnable & 0x10 ){
			if ( !pDlg->IsBSSTCPServerConnect() ){
				pDlg->SendMessage( WM_SOCKET_STARTSTOP, 4, 0);
				Sleep(200);
			}
		}
		if( pDlg->IsWorkSMSMode() ){
			if ( pDlg->m_nConnectEnable & 0x20 ){
				if ( !pDlg->IsCOMServerConnect() ){
					pDlg->SendMessage( WM_SOCKET_STARTSTOP, 5, 0);
					Sleep(200);
				}
			}
		}
		Sleep(30*1000);	
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//  接收数据显示数据
UINT AddAppHelpDataThread( LPVOID lParam ){

	CTEServerDlg		*pTEServerDlg = (CTEServerDlg*)AfxGetApp()->m_pMainWnd;
	STHELPDATA			*pHelpData = (STHELPDATA*)lParam;
	CString				strData;
	CString				strReceFlag;
	CString				strReceIP;
	CString				strRecvData;
	char				chAppName[30];

	if ( pHelpData ){
		/*	if (pHelpData->pDlg->m_strDataInfo.GetLength() > 2048){
			pHelpData->pDlg->m_strDataInfo.Empty();
			pHelpData->pDlg->GetDlgItem(IDC_EDIT_DATA_INFO)->SetWindowText("");
		}
		if ( pHelpData->m_nAppLen >= 30){
			pHelpData->m_nAppLen = 28;
		}
		memcpy(chAppName, pHelpData->m_pAppName, pHelpData->m_nAppLen );
		chAppName[pHelpData->m_nAppLen] = 0;
		
		if ( pHelpData->m_bRecFlag){
			strReceFlag = "Receive";
		}
		else{
			strReceFlag = "Send";
		}	
		strReceIP = GetIPFormSOCKADDR(&pHelpData->m_oSockAddr);
		strRecvData = HexToCString( pHelpData->m_pBuf, pHelpData->m_nDataLen);
		strData.Format("%s  %s  %s\r\nData:%s\r\n", chAppName, strReceFlag, strReceIP, strRecvData);
		pHelpData->pDlg->m_strDataInfo += strData;
		int nLen = pHelpData->pDlg->m_strDataInfo.GetLength();
		pHelpData->pDlg->GetDlgItem(IDC_EDIT_DATA_INFO)->SetWindowText( pTEServerDlg->m_strDataInfo );
	*/
	}	
	
	free( pHelpData->m_pBuf );
	pHelpData->m_pBuf = NULL;
	free( pHelpData->m_pAppName );
	pHelpData->m_pAppName = NULL;
	delete pHelpData;
	pHelpData = NULL;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// 线程的创建 
DWORD WINAPI DBMainThread(LPVOID lpparam){
	
	CTEServerDlg					*pDlg = (CTEServerDlg *)lpparam;	
	STGPSDATA						oldGPSData;
	PSTGPSDATA						pGPSData = NULL;
	int								nMileage = 0;
	int								nType;
	std::list<CData1*>::iterator    it;

	while( 1 ){

		it = pDlg->m_lstDBData.begin();
		while( it != pDlg->m_lstDBData.end() ){
			nType = (*it)->GetDataType();
			switch ( nType ){
			case TYPE_GPSDATA:
				pGPSData = (PSTGPSDATA)(*it)->GetDataBuf();
				if( pGPSData ){
					nMileage = pDlg->SaveGpsData( pGPSData );
					pGPSData->m_nMileage = nMileage;
					CDBData::DBSaveGPSData( pGPSData );
					//CDBData::DBSaveGPSData( (PSTGPSDATA)(*it)->GetDataBuf() );	
				}				
				break;
			case TYPE_TESETUP:
				CDBData::DBModifyDeviceSetup( (PSTDEVICESETUP)(*it)->GetDataBuf() );
				break;
			case TYPE_IMGDATA:
				CDBData::SaveImageData( (PSTIMAGEPATHINFO)(*it)->GetDataBuf() );
				break;
			case TYPE_BLINDGPSDATA:					
				pGPSData = (PSTGPSDATA)(*it)->GetDataBuf();
				if( pGPSData ){
					nMileage = 0;
					memset( &oldGPSData, 0x00, sizeof(STGPSDATA) );						
					// 1:如果之前没有旧数据返回为假。否则取出最后数据并返回为真
					// 2:如果传入数据与最后数据相差时间太长(数小时)返回为假。并记录传入数据为最后数据
					if( pDlg->GetBeforeBlinkGPSData( oldGPSData,  pGPSData ) ){

						// 传入数据与最后的距离大于30000公里,赋值255米，为以后查询方便
						nMileage = pDlg->CalculateDistance( oldGPSData.m_dbLat, 
														    oldGPSData.m_dbLon,
															pGPSData->m_dbLat,
															pGPSData->m_dbLon );
						if( nMileage > MAX_SPACEMILEAGE ){
							nMileage = MIN_SPACEMILEAGE;
						}
					}
					pGPSData->m_nMileage = nMileage;
					CDBData::DBSaveBlindGPSData( pGPSData );		
					//CDBData::DBSaveBlindGPSData( (PSTGPSDATA)(*it)->GetDataBuf() );		
				}
				break;
			case TYPE_GASDATA:
				CDBData::DBSaveGasData( (PSTGASDATA)(*it)->GetDataBuf() );			
				break;
			case TYPE_TEMPERATURE_DATA:		
				CDBData::DBSaveTemperatureData( (PSTTEMPERATUREDATA)(*it)->GetDataBuf() );
				break;
			case TYPE_QUERY_GG_ADDR_CN:
				CDBData::DBSaveGPSData( (PSTGPSDATA)(*it)->GetDataBuf() );
				break;
			case TYPE_QUERY_GG_ADDR_EN:
				CDBData::DBSaveGPSData( (PSTGPSDATA)(*it)->GetDataBuf() );
				break;
			default:		
				break;
			}
			delete *it;
			pDlg->m_lstDBData.pop_front();
			it = pDlg->m_lstDBData.begin();
		}
		Sleep(1);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//  保存数据库线程的创建 
UINT  SaveDBSQLThread( LPVOID lParam ){

	CData1				*pData = NULL;
	int					nType = 0;
	int					nMileage = 0;
	CDBData				oDBData;
	PSTGPSDATA			pGPSData = NULL;
	STGPSDATA			oldGPSData;

	CTEServerDlg		*pDlg = (CTEServerDlg*)AfxGetApp( )->m_pMainWnd;

	pData = (CData1*)lParam;
	if( pData == NULL ){
		return 0;
	}
	//////////////////////////////////////////////////////////////////////////
	// 连接数据库
	if ( oDBData.Connect2( pDlg->m_strServerName, 
						   pDlg->m_strDatabaseName, 
						   pDlg->m_strUserName, 
						   pDlg->m_strUserPsd ) ){

		nType = pData->GetDataType();
		switch ( nType ){
		case TYPE_GPSDATA:
			pGPSData = (PSTGPSDATA)pData->GetDataBuf();
			if( pGPSData ){
				TRACE("%d\r\n", pGPSData->m_oTime );
				nMileage = pDlg->SaveGpsData( pGPSData );
				pGPSData->m_nMileage = nMileage;			
				oDBData.DBSaveGPSData2( pGPSData );	
			}	
			break;
		case TYPE_TESETUP:
			oDBData.DBModifyDeviceSetup2( (PSTDEVICESETUP)pData->GetDataBuf() );
			break;
		case TYPE_IMGDATA:
			oDBData.SaveImageData2( (PSTIMAGEPATHINFO)pData->GetDataBuf() );
			break;
		case TYPE_BLINDGPSDATA:
			pGPSData = (PSTGPSDATA)pData->GetDataBuf();
			if( pGPSData ){
				nMileage = 0;
				memset( &oldGPSData, 0x00, sizeof(STGPSDATA) );						
				// 1:如果之前没有旧数据返回为假。否则取出最后数据并返回为真
				// 2:如果传入数据与最后数据相差时间太长(数小时)返回为假。并记录传入数据为最后数据
				if( pDlg->GetBeforeBlinkGPSData( oldGPSData,  pGPSData ) ){
					
					// 传入数据与最后的距离大于30000公里,赋值255米，为以后查询方便
					nMileage = pDlg->CalculateDistance( oldGPSData.m_dbLat, 
														oldGPSData.m_dbLon,
														pGPSData->m_dbLat,
														pGPSData->m_dbLon );
					if( nMileage > MAX_SPACEMILEAGE ){
						nMileage = MIN_SPACEMILEAGE;
					}
				}
				pGPSData->m_nMileage = nMileage;
				oDBData.DBSaveBlindGPSData2( pGPSData );			
			}		
			break;
		case TYPE_GASDATA:
			oDBData.DBSaveGasData2( (PSTGASDATA)pData->GetDataBuf() );
			break;
		case TYPE_GSMDATA:
			oDBData.DBSaveGSMData2( (PSTGSMDATA)pData->GetDataBuf() );
			break;
		case TYPE_TEMPERATURE_DATA:		
			oDBData.DBSaveTemperatureData2( (PSTTEMPERATUREDATA)pData->GetDataBuf() );
			break;
		case TYPE_HUMIDITY_DATA:	//湿度
			oDBData.DBSaveHumidityData2( (PSTHUMIDITYDATA)pData->GetDataBuf() );
			break;
		case TYPE_DMA_DATA:			//加速感应器
			oDBData.DBDMAData2( (PSTDMADATA)pData->GetDataBuf() );
			break;
		case TYPE_QUERY_GG_ADDR_CN:
			oDBData.DBSaveGPSData2( (PSTGPSDATA)pData->GetDataBuf() );
			break;
		case TYPE_QUERY_GG_ADDR_EN:
			oDBData.DBSaveGPSData2( (PSTGPSDATA)pData->GetDataBuf() );
			break;
		default:		
			break;
		}
		oDBData.DisConnect2();
	}
	delete pData;
	pData = NULL;

	return 0;
}
