#pragma once


// CDSMImg
#include "OGLWnd.h"
#include <map>

#include "CharTreeObject.h"

class CCharData;

typedef struct 
{
//	POINT3D pos;
	unsigned short xCode;
	unsigned short yCode;
	POINT3D vColor;
	unsigned short sCnt;

	unsigned short nRealCode[2];
	
}CODEPOSITION;


class CDSMImg : public COGLWnd
{
	DECLARE_DYNAMIC(CDSMImg)

public:
	CDSMImg();
	virtual ~CDSMImg();

	void SetChartData(CCharData* pChartData);
	void Render();
	void Render2D();
	void OverlayImage();
	void InitGLview(int _nWidth, int _nHeight);
	void MouseWheel(short zDelta);
	void InitData();
	void DrawImgPlan();
	void DrawDsmTerrain();
	void SetDSM(WCHAR* pData, int _nSize);
	void SetDSMTerrain(WCHAR* pData, int _nSize);
	void MakeTerrainGrid(unsigned short sGridNum);
	void MakeTerrainBottom(int fDepth);
	void ReleaseTerrainDataSets();
	unsigned short GetDSMCode(unsigned short _nCode);
	void SetDSMGridZValue(unsigned short sGridNum);
	void SmoothTerrain(unsigned short sGridNum);
	void IDragMap(int x, int y, short sFlag);
	void ExportObj();
	void ExportObjTrees();
	void SetZoomMode(bool IsZoom) { m_IsZoom = IsZoom;};
	void MakeTrees(short nTickness, unsigned short nTreeLevel);
	void ChangeTreeTickness(short nTickness);

	void DrawTrees();
	void DrawTreeText(CCharTreeObject* pTree);

	void SetDemOnOff(bool IsOn);
	void SetVolumnOnOff(bool IsOn);
	void SetMarkerOnOff(bool IsOn);
	void SetTreeOnOff();
	void ExportAll();
	void ChangeDemDempth(int nDepth);
	void DrawTextOnDem();

	void ShowOverlay(BOOL _isOK) { 
		m_showOverlay = _isOK; 
		Render();
	}

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

	bool m_IsZoom;
	GLuint		m_overlayTextureID;

	float *m_pVertex;
	float *m_pColor;
	float *m_pTexcoord;
	unsigned long *m_pIndex;
//	unsigned long *m_pIndexForLines[26*26];
//	int m_vertexNum;

	float *m_pVertexBottom;
	unsigned long* m_pIndexBottom;
	int m_vtxBottomNum;
	int m_idxBottomNum;
	int m_nDemDepth;


	int	m_matDsm[27][27];
	int m_totalCnt;

	GLuint m_bgTex;
	std::map<unsigned long, CODEPOSITION> m_dsmInfo;		// keycode, count (or structure with position) //
	std::map<unsigned short, unsigned short> m_dsmRange;  // code, position
	unsigned short m_sCodeBase[65536];



	// Trees on DSM //
	CCharData* m_pChartData;
	CCharTreeObject* m_pTrees;
	int m_nTreeNum;

	bool m_bDemOnOff;
	bool m_VolumnOnOff;
	bool m_MarkerOnOff;
	bool m_bTreeOnOff;

	BOOL m_showOverlay;
	

	
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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


