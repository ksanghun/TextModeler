
// TextModelerView.cpp : implementation of the CTextModelerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TextModeler.h"
#endif

#include "TextModelerDoc.h"
#include "TextModelerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextModelerView

IMPLEMENT_DYNCREATE(CTextModelerView, CView)

BEGIN_MESSAGE_MAP(CTextModelerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTextModelerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTextModelerView construction/destruction

CTextModelerView::CTextModelerView()
{
	// TODO: add construction code here

}

CTextModelerView::~CTextModelerView()
{
}

BOOL CTextModelerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTextModelerView drawing

void CTextModelerView::OnDraw(CDC* /*pDC*/)
{
	CTextModelerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTextModelerView printing


void CTextModelerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTextModelerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTextModelerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTextModelerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTextModelerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTextModelerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTextModelerView diagnostics

#ifdef _DEBUG
void CTextModelerView::AssertValid() const
{
	CView::AssertValid();
}

void CTextModelerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTextModelerDoc* CTextModelerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextModelerDoc)));
	return (CTextModelerDoc*)m_pDocument;
}
#endif //_DEBUG


// CTextModelerView message handlers
