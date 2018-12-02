#pragma once


// COGLWnd
#include "Camera2Axis.h"



typedef struct CUBE3D
{
	float vertices[8*3];
	float vtxColor[8*4];
	POINT3D vColor;

	void MakeCube(POINT3D _v0, POINT3D _v1, GLES_COLOR _color0, GLES_COLOR _color1, float _fSize0, float _fSize1)
	{
		// calc. vector //
		POINT3D directionVector= _v1 - _v0;
		// calc. perpendicular vector //
		POINT3D tmpV;
		//	mtSetPoint3D(&tmpV, -directionVector.y, directionVector.x, directionVector.z);
		mtSetPoint3D(&tmpV, 0.0f, 1.0f, 0.0f);
		POINT3D perpendiVector0 = mtCross(tmpV, directionVector);
		POINT3D perpendiVector1 = mtCross(directionVector, perpendiVector0);

		perpendiVector0 = mtNormalize(perpendiVector0);
		perpendiVector1 = mtNormalize(perpendiVector1);

		// Decide Cube Vertices //
		vertices[0] = _v0.x+perpendiVector0.x*_fSize0;		vertices[1] = _v0.y+perpendiVector0.y*_fSize0;		vertices[2] = _v0.z+perpendiVector0.z*_fSize0;
		vertices[3] = _v0.x+perpendiVector1.x*_fSize0;		vertices[4] = _v0.y+perpendiVector1.y*_fSize0;		vertices[5] = _v0.z+perpendiVector1.z*_fSize0;
		vertices[6] = _v0.x-perpendiVector0.x*_fSize0;		vertices[7] = _v0.y-perpendiVector0.y*_fSize0;		vertices[8] = _v0.z-perpendiVector0.z*_fSize0;
		vertices[9] = _v0.x-perpendiVector1.x*_fSize0;		vertices[10] = _v0.y-perpendiVector1.y*_fSize0;		vertices[11] = _v0.z-perpendiVector1.z*_fSize0;

		vertices[12] = _v1.x+perpendiVector0.x*_fSize1;			vertices[13] = _v1.y+perpendiVector0.y*_fSize1;			vertices[14] = _v1.z+perpendiVector0.z*_fSize1;
		vertices[15] = _v1.x+perpendiVector1.x*_fSize1;			vertices[16] = _v1.y+perpendiVector1.y*_fSize1;			vertices[17] = _v1.z+perpendiVector1.z*_fSize1;
		vertices[18] = _v1.x-perpendiVector0.x*_fSize1;			vertices[19] = _v1.y-perpendiVector0.y*_fSize1;			vertices[20] = _v1.z-perpendiVector0.z*_fSize1;
		vertices[21] = _v1.x-perpendiVector1.x*_fSize1;			vertices[22] = _v1.y-perpendiVector1.y*_fSize1;			vertices[23] = _v1.z-perpendiVector1.z*_fSize1;


		// color of _v0
		for(int i=0; i<4; i++)
		{
			vtxColor[i*4+0] = _color0.r;
			vtxColor[i*4+1] = _color0.g;
			vtxColor[i*4+2] = _color0.b;
			vtxColor[i*4+3] = _color0.a;
		}
		// color of _v1
		for(int i=4; i<8; i++)
		{
			vtxColor[i*4+0] = _color1.r;
			vtxColor[i*4+1] = _color1.g;
			vtxColor[i*4+2] = _color1.b;
			vtxColor[i*4+3] = _color1.a;
		}

	};

	void DrawCube(unsigned long* pIndex)
	{
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glColorPointer(4, GL_FLOAT, 0, vtxColor);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, pIndex);
	};

}CUBE3D;

class COGLWnd : public CWnd
{
	DECLARE_DYNAMIC(COGLWnd)

public:
	COGLWnd();
	virtual ~COGLWnd();

	CDC* m_CDCPtr;	
	HGLRC		m_hRC;	
	HPALETTE	m_hPalette;	
	bool IsFirstSet;
	CCamera2Axis m_cameraPri;	



	// For Anti Aliasing ======//
	bool	arbMultisampleSupported;
	int		arbMultisampleFormat;

	
private:		
	bool WGLisExtensionSupported(const char *extension);
	bool InitMultisample(HDC hDC,PIXELFORMATDESCRIPTOR pfd);

	BOOL gl_init(HDC inhDC, HGLRC &outhRC, HPALETTE &outhPalette);
	HPALETTE gl_GetPalette(HDC inhDC);
	virtual void Render(void){};
	virtual void InitGLview(void){};
	virtual void InitCamera(void){};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	void MouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	void gl_PushOrtho(int x1, int x2, int y1, int y2);
	void gl_PopOrtho();

	void gl_DrawText(POINT3D pos, CString strText, LOGFONT font, short align, BITMAPINFO* pBmp, CDC* pDC);	
	GLuint LoadBMPTexture(CString fileName);
	GLuint LoadTgaTexture(CString filename);
};


