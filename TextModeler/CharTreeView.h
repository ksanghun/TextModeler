#pragma once
#include "oglwnd.h"



class CCharData;
class CCharTreeView :
	public COGLWnd
{
public:
	CCharTreeView(void);
	virtual ~CCharTreeView(void);

	void Render();
	void Render2D();
	void InitGLview(int _nWidth, int _nHeight);
	void SetCharData(CCharData* _pData);
	void MouseWheel(short zDelta);
	void DrawTreeLine();
	void Draw3DTree();
	void DrawTreeNode(float fScale);
	void DrawTreeText();
	void StartAnimation();
	void UpdateTreeNodePosition();
	void MakingTreeObject();
	void ExportObj();
	void VolumnMode();
	void ChangeTickness(int iTickness);

	void IDragMap(int x, int y, short sFlag);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);


	
	std::vector<CUBE3D> m_linkCube;
	float m_cubeTichness;
	unsigned long m_cubeIndex[36];
	bool m_IsVolumn;

private:
	CPoint m_mousedown;
	short m_mouseMode;
	POINT3D m_lookAt;
	VA_DRAG_OPER m_dragOper;

	LOGFONT		m_LogFont;
	BITMAPINFO* m_pBmpInfo;

	int			m_nWidth;
	int			m_nHeight;

	CCharData* m_pCharData;
	int			m_AniCnt;

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
