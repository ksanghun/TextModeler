// DlgTree.cpp : implementation file
//

#include "stdafx.h"
#include "TextAnalyzer.h"
#include "DlgTree.h"
#include "CharData.h"


// CDlgTree dialog

IMPLEMENT_DYNAMIC(CDlgTree, CDialog)

CDlgTree::CDlgTree(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTree::IDD, pParent)
	, m_strInputChar(_T(""))
	, m_intTreeLevel(0)
	, m_iTickness(0)
{
	m_listAddCnt=0;
	m_pCharData = 0;
	m_cx=0;
	m_cy=0;

	m_strInputChar = "";
	m_intTreeLevel = 7;
	m_pViewTree = NULL;
}

CDlgTree::~CDlgTree()
{
}

void CDlgTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHAR, m_strInputChar);
	DDX_Text(pDX, IDC_EDIT_TREE_LEVEL, m_intTreeLevel);
	DDV_MinMaxInt(pDX, m_intTreeLevel, 0, 9999);
	DDX_Text(pDX, IDC_EDIT_TICKNESS, m_iTickness);
	DDV_MinMaxInt(pDX, m_iTickness, 0, 9999);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CDlgTree, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgTree::OnNMDblclkList1)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BN_TREE, &CDlgTree::OnBnClickedBnTree)
	ON_BN_CLICKED(IDC_BN_EXPORT, &CDlgTree::OnBnClickedBnExport)
	ON_BN_CLICKED(IDC_BN_ONOFFVOL, &CDlgTree::OnBnClickedBnOnoffvol)
	ON_BN_CLICKED(IDC_BN_CHANGETICK, &CDlgTree::OnBnClickedBnChangetick)
END_MESSAGE_MAP()


// CDlgTree message handlers

BOOL CDlgTree::Create(CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::Create(IDD, pParentWnd);
}

void CDlgTree::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;

		CString strItem = m_listCtrl.GetItemText( pNMIA->iItem, 0);	
		unsigned short nCode = _wtoi(strItem.GetBuffer(strItem.GetLength()));

		if(m_pCharData!=0)
		{
			m_pCharData->FindTreeByChar(nCode, m_intTreeLevel);
			//	m_pViewTree->StartAnimation();
			m_pViewTree->MakingTreeObject();
		}

	}
	*pResult = 0;

}

void CDlgTree::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDialog::OnSize(nType, cx, cy);

	m_cx = cx;
	m_cy = cy;
	if(m_pViewTree)
	{
		m_pViewTree->MoveWindow(150, 50, cx-150, cy-50);
		m_listCtrl.MoveWindow(0,0,150,m_cy);
	}
}

BOOL CDlgTree::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CDlgTree::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow();

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listCtrl.InsertColumn(0, L"Code.", LVCFMT_LEFT, 50, 0);
	m_listCtrl.InsertColumn(1, L"Char.",  LVCFMT_LEFT, 42, 1);
	m_listCtrl.InsertColumn(2, L"Cnt.",  LVCFMT_LEFT, 50, 2);


	CRect cRect;
	GetWindowRect(&cRect);
	m_pViewTree = new CCharTreeView;
	//	m_pCharData = new CCharData;
	m_pViewTree->Create(NULL, NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, cRect, this, 0x07);
	m_pViewTree->InitGLview(m_cx-150, m_cy);	
	m_pViewTree->MoveWindow(150, 50, m_cx-150, m_cy-50);
	m_listCtrl.MoveWindow(0,0,150,m_cy);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTree::ResetListCtrl()
{
	m_listAddCnt=0;
	m_listCtrl.DeleteAllItems();
	UpdateData(TRUE);
}

void CDlgTree::SetChartData(CCharData* pCharData)
{
	m_pCharData = pCharData;
	if(m_pViewTree)
	{
		m_pViewTree->SetCharData(pCharData);
	}

	// Add List Item //
	ResetListCtrl();

	CString strItem;
	int cnt = m_pCharData->m_sortVec.size();
	// 	if(cnt>20)
	// 		cnt = 20;
	unsigned long code;
	unsigned long charCnt;
	CString strTmp;
	m_listAddCnt=0;
	for(int i=0; i<cnt; i++)
	{
		code = m_pCharData->m_sortVec[i].first;
		if((code == 10) || (code==32))
			continue;

		strItem.Format(L"%d", code);
		m_listCtrl.InsertItem(m_listAddCnt, strItem, 0);

		strItem = m_pCharData->m_pArrayHChar[code].m_strName;
		m_listCtrl.SetItemText(m_listAddCnt, 1, strItem);	

		//if(i==2)
		//{
		//	m_strInputChar = strItem;
		//}
		//
		charCnt = m_pCharData->m_sortVec[i].second;
		strItem.Format(L"%d", charCnt);
		m_listCtrl.SetItemText(m_listAddCnt, 2, strItem);	


		m_listAddCnt++;
	}

	UpdateData(FALSE);

}
void CDlgTree::OnBnClickedBnTree()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	unsigned short nCode = m_pCharData->FindCodeByString(m_strInputChar);
	if(nCode !=0)
	{
		m_pCharData->FindTreeByChar(nCode, m_intTreeLevel);
		m_pViewTree->MakingTreeObject();
	}
	else
	{
		AfxMessageBox(L"해당 글자가 없습니다.");
	}
}

void CDlgTree::OnBnClickedBnExport()
{
	// TODO: Add your control notification handler code here
	if(m_pViewTree)
		m_pViewTree->ExportObj();
	AfxMessageBox(L"Exported", NULL, NULL);
}

void CDlgTree::OnBnClickedBnOnoffvol()
{
	// TODO: Add your control notification handler code here
	if(m_pViewTree)
		m_pViewTree->VolumnMode();
}

void CDlgTree::OnBnClickedBnChangetick()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pViewTree)
	{
		//m_pViewTree->m_cubeTichness = (float)m_iTickness;
		m_pViewTree->ChangeTickness(m_iTickness);
	}
}
