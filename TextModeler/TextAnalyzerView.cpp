
// TextAnalyzerView.cpp : implementation of the CTextAnalyzerView class
//

#include "stdafx.h"
#include "TextAnalyzer.h"

#include "TextAnalyzerDoc.h"
#include "TextAnalyzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextAnalyzerView
CTextAnalyzerView* pView=0;

IMPLEMENT_DYNCREATE(CTextAnalyzerView, CFormView)

BEGIN_MESSAGE_MAP(CTextAnalyzerView, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BN_START, &CTextAnalyzerView::OnBnClickedBnStart)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_TREE_VIEW, &CTextAnalyzerView::OnTreeView)
	ON_BN_CLICKED(IDC_BN_TICKNESS, &CTextAnalyzerView::OnBnClickedBnTickness)
	ON_BN_CLICKED(IDC_BN_VOL_ONOFF, &CTextAnalyzerView::OnBnClickedBnVolOnoff)
	ON_BN_CLICKED(IDC_BN_MARKER, &CTextAnalyzerView::OnBnClickedBnMarker)
	ON_BN_CLICKED(IDC_BN_TERONOFF, &CTextAnalyzerView::OnBnClickedBnTeronoff)
	ON_BN_CLICKED(IDC_BN_OBJ_EXPORT, &CTextAnalyzerView::OnBnClickedBnObjExport)
	ON_BN_CLICKED(IDC_BN_SP_TICKNESS, &CTextAnalyzerView::OnBnClickedBnSpTickness)
	ON_BN_CLICKED(IDC_BN_SP_VOL_ONOFF, &CTextAnalyzerView::OnBnClickedBnSpVolOnoff)
	ON_BN_CLICKED(IDC_BN_SP_NORM_ONOFF, &CTextAnalyzerView::OnBnClickedBnSpNormOnoff)
	ON_BN_CLICKED(IDC_BN_SPONOFF, &CTextAnalyzerView::OnBnClickedBnSponoff)
	ON_BN_CLICKED(IDC_BN_SP_OBJ_EXPORT, &CTextAnalyzerView::OnBnClickedBnSpObjExport)
	ON_BN_CLICKED(IDC_BN_OBJ_EXPORT_ALL, &CTextAnalyzerView::OnBnClickedBnObjExportAll)
	ON_BN_CLICKED(IDC_BN_DEM_DEPTH, &CTextAnalyzerView::OnBnClickedBnDemDepth)
//	ON_EN_CHANGE(IDC_EDIT_TREE_LEVEL, &CTextAnalyzerView::OnEnChangeEditTreeLevel)
//	ON_BN_CLICKED(IDC_BN_TICKNESS2, &CTextAnalyzerView::OnBnClickedBnTickness2)
ON_BN_CLICKED(IDC_BN_CHANGE_LEVEL, &CTextAnalyzerView::OnBnClickedBnChangeLevel)
ON_EN_CHANGE(IDC_EDIT1, &CTextAnalyzerView::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_BN_TREES_ONOFF, &CTextAnalyzerView::OnBnClickedBnTreesOnoff)
ON_BN_CLICKED(IDC_CHECK_OVERLAY, &CTextAnalyzerView::OnBnClickedCheckOverlay)
END_MESSAGE_MAP()

// CTextAnalyzerView construction/destruction

CTextAnalyzerView::CTextAnalyzerView()
	: CFormView(CTextAnalyzerView::IDD)
	, m_strInput(_T("Write or copy & paste sentances and press start button.\r\n"))
	, m_intTreeTick(5)
	, m_intSpTick(10)
	, m_intDemDepth(100)
	, m_nTreeLevel(5)
	, m_bChkOverlay(FALSE)
{
	// TODO: add construction code here
	m_pTextBuffer = 0;
	m_pDSMImg = 0;
	m_pSphereChart = 0;

	pView = this;
	m_pCharData=0;

}

CTextAnalyzerView::~CTextAnalyzerView()
{
}

void CTextAnalyzerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
	DDX_Control(pDX, IDC_BN_START, m_ctrlStartBtn);
	DDX_Text(pDX, IDC_EDIT_TICKNESS, m_intTreeTick);
	DDV_MinMaxShort(pDX, m_intTreeTick, 1, 255);
	DDX_Text(pDX, IDC_EDIT_TICKNESS2, m_intSpTick);
	DDV_MinMaxShort(pDX, m_intSpTick, 1, 255);
	DDX_Text(pDX, IDC_EDIT_DEM_DEPTH, m_intDemDepth);
	DDV_MinMaxLong(pDX, m_intDemDepth, 1, 99999);
	DDX_Text(pDX, IDC_EDIT_TREE_LEVEL, m_nTreeLevel);
	DDV_MinMaxShort(pDX, m_nTreeLevel, 1, 999);
	DDX_Check(pDX, IDC_CHECK_OVERLAY, m_bChkOverlay);
}

BOOL CTextAnalyzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTextAnalyzerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CRect cRect;
	GetWindowRect(&cRect);
	m_pDSMImg = new CDSMImg;
	m_pDSMImg->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, cRect, this, 0x01);
	m_pDSMImg->InitGLview(500, 500);

	m_pSphereChart = new SphereChart;
	m_pSphereChart->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, cRect, this, 0x02);
	m_pSphereChart->InitGLview(500, 500);

	if((m_pDSMImg!=0)&&(m_pSphereChart!=0))
	{
		int nWidth = m_nScreenWidth/2;
		m_ctrlEdit.MoveWindow(0, m_nScreenHeight-150, m_nScreenWidth-150, 150);
		m_ctrlStartBtn.MoveWindow(m_nScreenWidth-150, m_nScreenHeight-150, 150, 150);

		int lefeWidth = nWidth + 200;
		int righWidth = m_nScreenWidth - lefeWidth;
		m_pDSMImg->MoveWindow(110, 0, lefeWidth, m_nScreenHeight - 150);
		m_pSphereChart->MoveWindow(lefeWidth + 110, 0, righWidth - 110, m_nScreenHeight - 150);
	}

//	m_ctrlStartBtn.ShowWindow(SW_HIDE);

	m_pDlgTree = new CDlgTree;
	m_pDlgTree->Create(this);
	m_pDlgTree->ShowWindow(SW_HIDE);



}


// CTextAnalyzerView diagnostics

#ifdef _DEBUG
void CTextAnalyzerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTextAnalyzerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

// CTextAnalyzerDoc* CTextAnalyzerView::GetDocument() const // non-debug version is inline
// {
// 	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextAnalyzerDoc)));
// 	return (CTextAnalyzerDoc*)m_pDocument;
// }
#endif //_DEBUG


// CTextAnalyzerView message handlers

void CTextAnalyzerView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_nScreenWidth = cx;
	m_nScreenHeight = cy;

	if((m_pDSMImg!=0)&&(m_pSphereChart!=0))
	{
		int nWidth = m_nScreenWidth / 2;
		m_ctrlEdit.MoveWindow(0, m_nScreenHeight-150, m_nScreenWidth-150, 150);
		m_ctrlStartBtn.MoveWindow(m_nScreenWidth-150, m_nScreenHeight-150, 150, 150);
	
		
		int lefeWidth = nWidth + 200;
		int righWidth = m_nScreenWidth - lefeWidth;
		m_pDSMImg->MoveWindow(110, 0, lefeWidth , m_nScreenHeight-150);
		m_pSphereChart->MoveWindow(lefeWidth +110, 0, righWidth -110, m_nScreenHeight-150);

		
	}


}

void CTextAnalyzerView::OnBnClickedBnStart()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	CString strText;
	GetDlgItemText(IDC_EDIT1, strText);

//	strText+=L"\r\n";

	int nStrLen = strText.GetLength();
	if(nStrLen>0)
	{
		if(m_pTextBuffer != 0)
		{
			delete [] m_pTextBuffer;
			m_pTextBuffer = 0;
		}

		m_pTextBuffer = new WCHAR[nStrLen+1];
		memset(m_pTextBuffer, 0x00, nStrLen+1);
		wsprintf(m_pTextBuffer, strText.GetBuffer());

		if(m_pCharData != 0)
		{
			m_pCharData->ReleaseData();
			delete m_pCharData;
			m_pCharData=0;
		}
		m_pCharData = new CCharData;
		m_pCharData->SetData(m_pTextBuffer, nStrLen);


		if(m_pSphereChart)
		{
		//	m_pSphereChart->SetDSM(m_pTextBuffer, nStrLen);
			m_pSphereChart->SetData(m_pTextBuffer, nStrLen);
			m_pSphereChart->Render();
		}

		if(m_pDSMImg)
		{
			UpdateData(TRUE);
			m_pDSMImg->SetChartData(m_pCharData);
			m_pDSMImg->SetDSM(m_pTextBuffer, nStrLen);

			m_pDSMImg->MakeTrees(m_intTreeTick, m_nTreeLevel);

			m_pDSMImg->Render();
		}

		

	}
}

void CTextAnalyzerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTextAnalyzerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CTextAnalyzerView::PreTranslateMessage(MSG* pMsg)
{
	if(IsDialogMessage(pMsg))           

		return TRUE;

	else  

		return CWnd::PreTranslateMessage(pMsg);   


	// TODO: Add your specialized code here and/or call the base class
	//if(pMsg->message == WM_KEYDOWN)
	//{
	//	if(pMsg->wParam==17)
	//	{
	//		if(m_pDSMImg)
	//		{
	//			m_pDSMImg->SetZoomMode(true);
	//		}
	//		if(m_pSphereChart)
	//		{
	//			m_pSphereChart->SetZoomMode(true);
	//		}
	//	}
	//}

	//if(pMsg->message == WM_KEYUP)
	//{
	//	if(pMsg->wParam==17)
	//	{
	//		if(m_pDSMImg)
	//		{				
	//			m_pDSMImg->SetZoomMode(false);
	//		}
	//		if(m_pSphereChart)
	//		{
	//			m_pSphereChart->SetZoomMode(false);
	//		}
	//	}
	//}

//	return CFormView::PreTranslateMessage(pMsg);
}

void CTextAnalyzerView::SetFocusOfGLView(unsigned short _nviewId)
{
	switch(_nviewId)
	{
	case 0:		// img view
		m_pDSMImg->SetZoomMode(true);
		m_pSphereChart->SetZoomMode(false);
		m_pDSMImg->Render();
		m_pSphereChart->Render();

		break;
	case 1:		// spere view
		m_pDSMImg->SetZoomMode(false);
		m_pSphereChart->SetZoomMode(true);
		m_pDSMImg->Render();
		m_pSphereChart->Render();
		break;
	}
}


BOOL CTextAnalyzerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if(m_pDSMImg)
	{
		m_pDSMImg->MouseWheel(zDelta);
	}
	if(m_pSphereChart)
	{
		m_pSphereChart->MouseWheel(zDelta);
	}

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CTextAnalyzerView::OnTreeView()
{
	// TODO: Add your command handler code here
	m_pDlgTree->ShowWindow(SW_SHOW);
	if(m_pCharData!=0)
	{
		m_pDlgTree->SetChartData(m_pCharData);
	}
}

void CTextAnalyzerView::OnBnClickedBnTickness()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDSMImg)
	{
		m_pDSMImg->ChangeTreeTickness(m_intTreeTick);
		m_pDSMImg->Render();
	}
}

void CTextAnalyzerView::OnBnClickedBnVolOnoff()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->SetVolumnOnOff(true);
	}
}

void CTextAnalyzerView::OnBnClickedBnMarker()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->SetMarkerOnOff(true);
	}
}

void CTextAnalyzerView::OnBnClickedBnTeronoff()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->SetDemOnOff(true);
	}
}

void CTextAnalyzerView::OnBnClickedBnObjExport()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->ExportObj();		// DEM
		m_pDSMImg->ExportObjTrees();		// Trees
		AfxMessageBox(L"Files are saved", MB_OK, NULL);
	}
}

void CTextAnalyzerView::OnBnClickedBnSpTickness()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pSphereChart)
	{
		m_pSphereChart->m_cubeTichness = (float)m_intSpTick;
		m_pSphereChart->MakeLinkObject();
		
	}
}

void CTextAnalyzerView::OnBnClickedBnSpVolOnoff()
{
	// TODO: Add your control notification handler code here
	if(m_pSphereChart)
	{
		m_pSphereChart->OnOffVolumn();
	}
}

void CTextAnalyzerView::OnBnClickedBnSpNormOnoff()
{
	// TODO: Add your control notification handler code here
	if(m_pSphereChart)
	{
		m_pSphereChart->ModeNormalVec();
	}
}

void CTextAnalyzerView::OnBnClickedBnSponoff()
{
	// TODO: Add your control notification handler code here
	if(m_pSphereChart)
	{
		m_pSphereChart->OnOffSpherer();
	}
}

void CTextAnalyzerView::OnBnClickedBnSpObjExport()
{
	// TODO: Add your control notification handler code here
	if(m_pSphereChart)
	{
		m_pSphereChart->ExportObj();
		AfxMessageBox(L"Files are saved", MB_OK, NULL);
	}
}

void CTextAnalyzerView::OnBnClickedBnObjExportAll()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->ExportAll();
		AfxMessageBox(L"File is saved", MB_OK, NULL);

	}
}

void CTextAnalyzerView::OnBnClickedBnDemDepth()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDSMImg)
	{
		m_pDSMImg->ChangeDemDempth(m_intDemDepth);
	}
}



// void CTextAnalyzerView::OnBnClickedBnTickness2()
// {
// 	// TODO: Add your control notification handler code here
// }

void CTextAnalyzerView::OnBnClickedBnChangeLevel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pDSMImg)
	{
		m_pDSMImg->MakeTrees(m_intTreeTick, m_nTreeLevel);
		m_pDSMImg->Render();
	}
}

void CTextAnalyzerView::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CTextAnalyzerView::OnBnClickedBnTreesOnoff()
{
	// TODO: Add your control notification handler code here
	if(m_pDSMImg)
	{
		m_pDSMImg->SetTreeOnOff();
	}
}


void CTextAnalyzerView::OnBnClickedCheckOverlay()
{
	// TODO: Add your control notification handler code here	
	UpdateData(TRUE);

	if (m_pDSMImg){
		m_pDSMImg->ShowOverlay(m_bChkOverlay);
	}
}
