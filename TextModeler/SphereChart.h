#pragma once


// SphereChart
#include "OGLWnd.h"
#include "LetterData.h"
#include <locale.h>



class SphereChart : public COGLWnd
{
	DECLARE_DYNAMIC(SphereChart)

public:
	SphereChart();
	virtual ~SphereChart();

	void Render();
	void Render2D();
	void InitGLview(int _nWidth, int _nHeight);
	void MouseWheel(short zDelta);
	void InitData();
	void ReleaseDataSets();
	void SetData(WCHAR* pData, int _nSize);
	void IDragMap(int x, int y, short sFlag);
	void DrawSphere();
	void DrawLetterLinks();
	void DrawNormLinks();
	void DrawVolumeNormLink();
	void DrawVolumeLink();


	void OnOffSpherer();
	void OnOffVolumn();

	POINT3D convertVecToCentroidCoord(const float & longi, const float & lati, const float & radious);
	void MakeSphere(float fRadious);
	POINT3D GetColor(unsigned long nCode);
	void InitLetterData();
	void MakeLinkObject();
	void ModeNormalVec();

	void ExportObj();

	void SetZoomMode(bool IsZoom) { m_IsZoom = IsZoom;};


	float m_cubeTichness;
	


private:
	CPoint m_mousedown;
	short m_mouseMode;
	POINT3D m_lookAt;
	VA_DRAG_OPER m_dragOper;

	LOGFONT		m_LogFont;
	LOGFONT		m_LogFontB;
	BITMAPINFO* m_pBmpInfo;
	int			m_nWidth;
	int			m_nHeight;

	float *m_pVertex;
	float *m_pColor;
	float *m_pTexcoord;
	unsigned long *m_pIndex;

	CLetterData* m_letterData;

	bool m_IsShowSphere;
	bool m_IsNormalMode;
	bool m_IsVolume;

	unsigned long m_cubeIndex[36];
	CUBE3D m_testCube;

	std::vector<COLOR_VEC3D> m_normVector;
	std::vector<CUBE3D> m_linkCube;
	std::vector<CUBE3D> m_normLinkCube;

	bool m_IsZoom;

	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


