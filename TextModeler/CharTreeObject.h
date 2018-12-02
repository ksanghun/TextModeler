#pragma once
#include "data_type.h"
#include "math_tool.h"

typedef struct BRENCH_CUBE
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
			vtxColor[i*4+0] = _color0.r+(float)i*0.1f;
			vtxColor[i*4+1] = _color0.g+(float)i*0.1f;
			vtxColor[i*4+2] = _color0.b+(float)i*0.1f;
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

}BRENCH_CUBE;

class CCharData;
class CCharTreeObject
{
public:
	CCharTreeObject(void);
	~CCharTreeObject(void);

	void ReleaseTreeData();
	void MakingTreeObject(CCharData* pChartData, short nTickness);
	void Draw3DTree();
//	void DrawTreeText(CCharData* pChartData, LOGFONT logFont, BITMAPINFO *pBmpInfo, CDC* pCDCPtr);
	void DrawTreeNode(float fScale, float fAlpha);
	void DrawTreeLine();
//	void SetTreeData(CCharData* pChartData);
	void MakeTreeCube(short nTickness);

	void WriteObjVertex(FILE* fp);
	int WriteObjIndex(FILE* fp, int& nIndex);

	
	unsigned short m_nSeedCode[2]; // first-second;
	POINT3D m_vPosition;
	std::vector<BRENCH_CUBE> m_linkCube;
	float m_cubeTichness;
	unsigned long m_cubeIndex[36];
	bool m_IsVolumn;

	CString m_strSeedName;


	unsigned short m_nMaxLevel;
	unsigned long m_nTotalCnt;
	std::vector<_CHAR_TREE*> m_forwardTree;
	std::vector<_CHAR_TREE*> m_backwardTree;

};
