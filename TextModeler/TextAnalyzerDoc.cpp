
// TextAnalyzerDoc.cpp : implementation of the CTextAnalyzerDoc class
//

#include "stdafx.h"
#include "TextAnalyzer.h"

#include "TextAnalyzerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextAnalyzerDoc

IMPLEMENT_DYNCREATE(CTextAnalyzerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextAnalyzerDoc, CDocument)
END_MESSAGE_MAP()


// CTextAnalyzerDoc construction/destruction

CTextAnalyzerDoc::CTextAnalyzerDoc()
{
	// TODO: add one-time construction code here

}

CTextAnalyzerDoc::~CTextAnalyzerDoc()
{
}

BOOL CTextAnalyzerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTextAnalyzerDoc serialization

void CTextAnalyzerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTextAnalyzerDoc diagnostics

#ifdef _DEBUG
void CTextAnalyzerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextAnalyzerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTextAnalyzerDoc commands
