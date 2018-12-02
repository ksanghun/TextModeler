#pragma once
#include "afxcmn.h"

#include "CharTreeView.h"
// CDlgTree dialog
class CCharData;
class CDlgTree : public CDialog
{
	DECLARE_DYNAMIC(CDlgTree)

public:
	CDlgTree(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTree();

// Dialog Data
	enum { IDD = IDD_DIALOG_TREE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strInputChar;
	int m_intTreeLevel;
	int m_iTickness;
	CListCtrl m_listCtrl;

	virtual BOOL Create(CWnd* pParentWnd = NULL);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnInitDialog();

	void ResetListCtrl();
	void SetChartData(CCharData* pChartData);

private:
	
	CCharData* m_pCharData;
	CCharTreeView* m_pViewTree;

	int m_listAddCnt;
	int m_cx;
	int m_cy;
public:
	afx_msg void OnBnClickedBnTree();
	afx_msg void OnBnClickedBnExport();
	afx_msg void OnBnClickedBnOnoffvol();
	afx_msg void OnBnClickedBnChangetick();
};
