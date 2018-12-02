
// TextAnalyzerView.h : interface of the CTextAnalyzerView class
//


#pragma once

#include "DSMImg.h"
#include "SphereChart.h"
#include "afxwin.h"
#include "resource.h"

#include "DlgTree.h"
#include "CharData.h"

class CTextAnalyzerView : public CFormView
{
protected: // create from serialization only
	CTextAnalyzerView();
	DECLARE_DYNCREATE(CTextAnalyzerView)

	CDlgTree*	  m_pDlgTree;
	CCharData* m_pCharData;

public:
	enum{ IDD = IDD_TEXTANALYZER_FORM };

// Attributes
public:
//	CTextAnalyzerDoc* GetDocument() const;

// Operations
public:
	void	SetFocusOfGLView(unsigned short _nviewId);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

	

// Implementation
public:
	virtual ~CTextAnalyzerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDSMImg*			m_pDSMImg;
	SphereChart*		m_pSphereChart;

	short					m_nScreenWidth;
	short					m_nScreenHeight;
	WCHAR*			m_pTextBuffer;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString m_strInput;
	afx_msg void OnBnClickedBnStart();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CEdit m_ctrlEdit;
	CButton m_ctrlStartBtn;
	afx_msg void OnTreeView();
	short m_intTreeTick;
	short m_intSpTick;
	afx_msg void OnBnClickedBnTickness();
	afx_msg void OnBnClickedBnVolOnoff();
	afx_msg void OnBnClickedBnMarker();
	afx_msg void OnBnClickedBnTeronoff();
	afx_msg void OnBnClickedBnObjExport();
	afx_msg void OnBnClickedBnSpTickness();
	afx_msg void OnBnClickedBnSpVolOnoff();
	afx_msg void OnBnClickedBnSpNormOnoff();
	afx_msg void OnBnClickedBnSponoff();
	afx_msg void OnBnClickedBnSpObjExport();
	afx_msg void OnBnClickedBnObjExportAll();
	long m_intDemDepth;
	afx_msg void OnBnClickedBnDemDepth();

//	afx_msg void OnBnClickedBnTickness2();
	afx_msg void OnBnClickedBnChangeLevel();
	short m_nTreeLevel;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedBnTreesOnoff();
	afx_msg void OnBnClickedCheckOverlay();
	BOOL m_bChkOverlay;
};

//#ifndef _DEBUG  // debug version in TextAnalyzerView.cpp
// inline CTextAnalyzerDoc* CTextAnalyzerView::GetDocument() const
//    { return reinterpret_cast<CTextAnalyzerDoc*>(m_pDocument); }
// #endif

