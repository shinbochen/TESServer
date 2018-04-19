// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8437BE71_F637_42B7_8B5E_27A4ED3B76DA__INCLUDED_)
#define AFX_STDAFX_H__8437BE71_F637_42B7_8B5E_27A4ED3B76DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <XTToolkitPro.h>   // Codejock Software Components
#include <afxsock.h>		// MFC socket extensions

#include "Thread.h"
#include "..\public\public.h"
#include "..\public\SQLDBData.h"
#include "..\public\Language.h"

#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>		// MFC template classes
#include <locale.h>			// MFC local

//////////////////////////////////////////////////////////////////////////
//
enum{
	IDC_TIMER_UPDATE = WM_USER + 1, 
	WM_UPDATE_TIME,
	WM_SAVE_CONFIG,
	WM_SOCKET_CLOSE,
	WM_SOCKET_CONNECT,
	WM_SOCKET_STARTSTOP,
	WM_ADDDESINFO,
	WM_SOCKET_SENDDATA,
	WM_SOCKET_SEND_DATATODE_FMCS,
	WM_SOFTWART_ERROR_EXIT,
	WM_TIMER_UPDATA,
	WM_TIMER_CHECK,
	WM_SOCKET_SEND_ERROR,
	WM_RESULT_GG_ADDR,
	WM_QUERY_GG_ADDR
};

#define		_SINGLE_SOFTWARE	

#define		_NEED_CSPROTOCOL		1
#define		_NEED_DESPROTOCOL		1
//#define		_NEED_CLPROTOCOL		1

#define		TEXT_AUTHOR_EMAIL		"Author"//trackearth@live.com"
#define     TRACK_EARTH_SERVER

// 另外一种加密方式
//#define		TEXT_MACIP	


#define     TIME_CONFIG_TIME            1404637512			
#define     TIME_CONFIG_VERSION			0
#define     AUTO_CONFIG_IP				0

// 可拷贝数组模板定义
template <class TYPE, class ARG_TYPE>
class CArrayExt : public CArray<TYPE, ARG_TYPE>
{
public:
	CArrayExt()		// 缺省构造函数
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = m_nGrowBy = 0;
	}
	CArrayExt(CArrayExt<TYPE, ARG_TYPE>& a)		// 拷贝构造函数
	{
		SetSize(a.GetSize());
		for (int i=0; i<a.GetSize(); i++)
			ElementAt(i) = a[i];
	}
	const CArrayExt<TYPE, ARG_TYPE>& operator=(const CArrayExt<TYPE, ARG_TYPE>& a)	//重载赋值运算符
	{
		if (m_pData!=a.m_pData)
		{
			SetSize(a.GetSize());
			for (int i=0; i<a.GetSize(); i++)
				ElementAt(i) = a[i];
		}
		return *this;
	}
	int AddMany(TYPE* pAddElements, int size)
	{
		int nOldSize = m_nSize;
		SetSize(m_nSize + size);
		CopyElements<TYPE>(m_pData + nOldSize, pAddElements, size);
		return nOldSize;
	}
};

typedef CArrayExt<BYTE, BYTE>		CAByte;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8437BE71_F637_42B7_8B5E_27A4ED3B76DA__INCLUDED_)
