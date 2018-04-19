// TEServer.h : main header file for the TESERVER application
//

#if !defined(AFX_TESERVER_H__96068DDF_E99E_429F_A505_733529FFA096__INCLUDED_)
#define AFX_TESERVER_H__96068DDF_E99E_429F_A505_733529FFA096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
/*
版本记录:
date		version			description
2010-2-26	DES_V2.05		解决TCP方式下。设备发数据时断链的问题
2011-03-10	DES_V2.14       解决SQL存数据慢的问题,分开存数据
2011-03-10	DES_V2.15       解决SQL存数据采用多线程连接存储数据
2011-03-16	DES_V2.16		改了GPS数据计算距离, 从TES程序计算,不要SQL服务器计算
2011-03-18	DES_V2.17		解决了盲点数据计算距离, 从TES程序计算,不要SQL服务器计算
2011-05-19	DES_V2.18		解决拍照时,掉包的情况下,时间为零,获取当前时间
2011-06-15	DES_V2.19		OnAccept增加了,申请SOCKET内存做了处理
2011-12-29  DES_V2.21       修改了 DEProtocolF 里的报警状态
2012-02-02  DES_V2.22		修改了油量阻值,油路报警值
2012-05-25  DES_V2.23		关SOCKET的时候，做了异常处理
2013-01-10  DES_V2.24       增加了湿度和加速感应数据存储 
							SQL数据库表 温度.湿度.油路.加速感应 没数据为-1
*/

/////////////////////////////////////////////////////////////////////////////
// CTEServerApp:
// See TEServer.cpp for the implementation of this class
//

class CTEServerApp : public CWinApp
{
public:
	CTEServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTEServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESERVER_H__96068DDF_E99E_429F_A505_733529FFA096__INCLUDED_)
