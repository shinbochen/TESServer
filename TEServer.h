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
�汾��¼:
date		version			description
2010-2-26	DES_V2.05		���TCP��ʽ�¡��豸������ʱ����������
2011-03-10	DES_V2.14       ���SQL��������������,�ֿ�������
2011-03-10	DES_V2.15       ���SQL�����ݲ��ö��߳����Ӵ洢����
2011-03-16	DES_V2.16		����GPS���ݼ������, ��TES�������,��ҪSQL����������
2011-03-18	DES_V2.17		�����ä�����ݼ������, ��TES�������,��ҪSQL����������
2011-05-19	DES_V2.18		�������ʱ,�����������,ʱ��Ϊ��,��ȡ��ǰʱ��
2011-06-15	DES_V2.19		OnAccept������,����SOCKET�ڴ����˴���
2011-12-29  DES_V2.21       �޸��� DEProtocolF ��ı���״̬
2012-02-02  DES_V2.22		�޸���������ֵ,��·����ֵ
2012-05-25  DES_V2.23		��SOCKET��ʱ�������쳣����
2013-01-10  DES_V2.24       ������ʪ�Ⱥͼ��ٸ�Ӧ���ݴ洢 
							SQL���ݿ�� �¶�.ʪ��.��·.���ٸ�Ӧ û����Ϊ-1
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
