
// Draw2.h : main header file for the Draw2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDraw2App:
// See Draw2.cpp for the implementation of this class
//

class CDraw2App : public CWinApp
{
public:
	CDraw2App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDraw2App theApp;
