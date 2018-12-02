
// TextAnalyzer.h : main header file for the TextAnalyzer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTextAnalyzerApp:
// See TextAnalyzer.cpp for the implementation of this class
//

class CTextAnalyzerApp : public CWinApp
{
public:
	CTextAnalyzerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTextAnalyzerApp theApp;
