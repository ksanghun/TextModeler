// DSMImg.cpp : implementation file
//

#include "stdafx.h"
#include "DSMImg.h"
#include "TextAnalyzerView.h"
#include "CharData.h"


// CDSMImg
#define DEM_GRID_SIZE 1024

IMPLEMENT_DYNAMIC(CDSMImg, COGLWnd)

CDSMImg::CDSMImg()
{
	m_pVertex = 0;
	m_pColor = 0;
	m_pTexcoord = 0;
	m_pIndex = 0;
//	m_vertexNum = 26;
	m_pVertexBottom = 0;
	m_pIndexBottom = 0;

	m_vtxBottomNum = 0;
	m_idxBottomNum = 0;
	m_nDemDepth = 100;

	m_IsZoom = false;
	m_pTrees = 0;
	m_nTreeNum = 0;
	m_pChartData = 0;

	memset(&m_LogFont, 0, sizeof(m_LogFont));
	strcpy((char*)m_LogFont.lfFaceName, "Arial");
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfHeight = -18;
	m_LogFont.lfWidth = 0;
	//	m_LogFont.lfWeight = FW_BOLD;

	memset(&m_LogFontB, 0, sizeof(m_LogFontB));
	strcpy((char*)m_LogFontB.lfFaceName, "Arial");
	m_LogFontB.lfCharSet = ANSI_CHARSET;
	m_LogFontB.lfHeight = -18;
	m_LogFontB.lfWidth = 0;
	m_LogFontB.lfWeight = FW_BOLD;

	m_pBmpInfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256));



	m_bDemOnOff = true;
	m_VolumnOnOff = true;
	m_MarkerOnOff = false;
	m_bTreeOnOff = true;
	m_showOverlay = FALSE;
}

CDSMImg::~CDSMImg()
{
	if(m_pVertex != 0)			delete [] m_pVertex;
	if(m_pTexcoord !=0)			delete [] m_pTexcoord;
	if(m_pIndex !=0)			delete [] m_pIndex;

	delete m_pBmpInfo;

}



BEGIN_MESSAGE_MAP(CDSMImg, COGLWnd)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CDSMImg message handlers

void CDSMImg::SetChartData(CCharData* pChartData)
{
	m_pChartData = pChartData;
}

void CDSMImg::Render()
{
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClearColor(0.9411f, 0.9411f, 0.9411f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if (m_bTreeOnOff == true)
		Render2D();

	//glColor3f(1,0,0);
	//glBegin(GL_LINE_STRIP);
	//glVertex3f(0,0,0);
	//glVertex3f(0,260, 0);
	//glVertex3f(260, 260, 0);
	//glVertex3f(260, 0, 0);
	//glVertex3f(0,0,0);
	//glEnd();


//	Render2D();

	float fAlpah = 0.3f;

	if(m_bDemOnOff)
	{
		DrawImgPlan();
	}
	
	//	DrawDsmTerrain();
	if(m_bTreeOnOff){
		if(m_VolumnOnOff)	{
			for(int i=0; i<m_nTreeNum; i++)	{
				m_pTrees[i].Draw3DTree();
			}
		}
		else{
			for(int i=0; i<m_nTreeNum; i++){
				m_pTrees[i].DrawTreeLine();
			}
		}
		glDisable(GL_DEPTH_TEST);
		float fLevelScale = 1.0f / (m_cameraPri.GetLevelHeight()*0.001f);
		for(int i=0; i<m_nTreeNum; i++)	{
			m_pTrees[i].DrawTreeNode(fLevelScale, fAlpah);
		}

		if(m_MarkerOnOff)	{
			for(int i=0; i<m_nTreeNum; i++)	{
				//	m_pTrees[i].DrawTreeText(m_pChartData);
				DrawTreeText(&m_pTrees[i]);
			}
		}
	}

	else
	{
		glDisable(GL_DEPTH_TEST);
		DrawTextOnDem();
	}


	if(m_showOverlay)
		OverlayImage();
	glEnable(GL_DEPTH_TEST);


	

//	DrawTrees();
	

	SwapBuffers( m_CDCPtr->GetSafeHdc() );		// Swap the buffers (glFlush)
}

void CDSMImg::DrawTextOnDem()
{
	glColor4f(1.0f,1.0f,1.0f, 0.9f);
	for(int i=0; i<m_nTreeNum; i++)
	{
		POINT3D tPos;
		mtSetPoint3D(&tPos, m_pTrees[i].m_vPosition.x, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
		//mtSetPoint3D(&tPos, 1000, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
		gl_DrawText(tPos, m_pTrees[i].m_strSeedName, m_LogFontB, 1, m_pBmpInfo, m_CDCPtr);

	}
}

void CDSMImg::DrawTrees()
{
	//TEST //


 	glColor4f(1.0f,1.0f,0.0f,0.3f);
 	glBegin(GL_LINES);
 
 	for(int i=0; i<m_nTreeNum; i++)
 	{
 		glVertex3f(m_pTrees[i].m_vPosition.x, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
 		//glVertex3f(m_pTrees[i].m_vPosition.x, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
 		glVertex3f(1000, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
 
 
 	}
 
 	glEnd();



	glDisable(GL_DEPTH_TEST);
	glColor4f(1.0f,1.0f,1.0f, 0.9f);
	for(int i=0; i<m_nTreeNum; i++)
	{
		POINT3D tPos;
		//mtSetPoint3D(&tPos, m_pTrees[i].m_vPosition.x, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z+200);
		mtSetPoint3D(&tPos, 1000, m_pTrees[i].m_vPosition.y, m_pTrees[i].m_vPosition.z);
		gl_DrawText(tPos, m_pTrees[i].m_strSeedName, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);

	}
	glEnable(GL_DEPTH_TEST);


}


void CDSMImg::DrawDsmTerrain()
{
	glPointSize(3);
	glBegin(GL_POINTS);


	//std::map<unsigned long, CODEPOSITION>::iterator it;
	//it = m_dsmInfo.begin();
	//for(;it!=m_dsmInfo.end(); it++)
	//{
	//	POINT3D pos = it->second.pos;
	//	POINT3D color = it->second.vColor;	

	//	glColor3f(color.x, color.y, color.z);
	//	glVertex3f(pos.x, pos.y, pos.z);

	//}


	glEnd();
}

void CDSMImg::DrawImgPlan()
{
	if(m_pVertex==0)
		return;


	int iGridNum = m_dsmRange.size();

	glColor3f(1.0f,1.0f,1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, m_pTexcoord);
	glVertexPointer(3, GL_FLOAT, 0, m_pVertex);
	glColorPointer(3, GL_FLOAT, 0, m_pColor);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_bgTex);
	glDrawElements(GL_TRIANGLES, (iGridNum-1)*(iGridNum-1)*6, GL_UNSIGNED_INT, m_pIndex);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);



	glColor3f(0.5f, 0.5f, 0.5f);
	glVertexPointer(3, GL_FLOAT, 0, m_pVertexBottom);
	glDrawElements(GL_TRIANGLES, m_idxBottomNum, GL_UNSIGNED_INT, m_pIndexBottom);


	glDisableClientState(GL_VERTEX_ARRAY);



	//glPointSize(5);
	//glColor3f(1,0,0);
	//glBegin(GL_POINTS);
	//for(int i=0; i<m_vtxBottomNum; i++)
	//{
	//	glVertex3f(m_pVertexBottom[i*3+0], m_pVertexBottom[i*3+1], m_pVertexBottom[i*3+2]);
	//}
	//glEnd();



	//glColor3f(1.0f, 1.0f, 0.0f);

	//int idx=0;
	//
	//for(int y=0; y<iGridNum-1; y++)
	//{
	//	for(int x=0; x<iGridNum-1; x++)
	//	{
	//	//	glBegin(GL_LINE_STRIP);
	//		glBegin(GL_LINE_STRIP);
	//		idx = y*iGridNum+x;
	//		glVertex3f(m_pVertex[idx*3+0], m_pVertex[idx*3+1], m_pVertex[idx*3+2]);
	//		idx = (y+1)*iGridNum+x;
	//		glVertex3f(m_pVertex[idx*3+0], m_pVertex[idx*3+1], m_pVertex[idx*3+2]);
	//		idx = (y+1)*iGridNum+x+1;
	//		glVertex3f(m_pVertex[idx*3+0], m_pVertex[idx*3+1], m_pVertex[idx*3+2]);
	//		idx = (y)*iGridNum+x+1;
	//		glVertex3f(m_pVertex[idx*3+0], m_pVertex[idx*3+1], m_pVertex[idx*3+2]);
	//		idx = (y)*iGridNum+x;
	//		glVertex3f(m_pVertex[idx*3+0], m_pVertex[idx*3+1], m_pVertex[idx*3+2]);

	//		glEnd();

	//	}
	//}
	



}

void CDSMImg::Render2D()
{
	gl_PushOrtho(0,m_nWidth,0, m_nHeight);

	if(m_IsZoom)
	{
		glLineWidth(2);
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(0,0);
		glVertex2f(m_nWidth,0);
		glVertex2f(m_nWidth, m_nHeight);
		glVertex2f(0,m_nHeight);
		glVertex2f(0,0);
		glEnd();
		glLineWidth(1);
	}



	glBegin(GL_QUADS);
	glColor3f(0.59f, 0.83f,0.99f);
	glVertex3f(0,0,-1);
	glVertex3f(m_nWidth,0,-1);
	glColor3f(0.24f, 0.41f,0.52f);
	glVertex3f(m_nWidth, m_nHeight,-1);
	glVertex3f(0,m_nHeight,-1);
	glEnd();



	gl_PopOrtho();


//	glEnableClientState(GL_VERTEX_ARRAY);
////	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 0, m_pVertex);
//
//	glPointSize(3);
//	glColor3f(1.0f, 1.0f, 0.0f);
////	glDrawArrays(GL_POINTS, 0, m_vertexNum*m_vertexNum);
//
//
//	glDisableClientState(GL_VERTEX_ARRAY);
}


void CDSMImg::OverlayImage()
{
	gl_PushOrtho(0, m_nWidth, 0, m_nHeight);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_overlayTextureID);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glTexCoord2f(0.0f, 0.0);  glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0); glVertex3f(m_nWidth, 0, 0);
	glTexCoord2f(1.0f, 1.0); glVertex3f(m_nWidth, m_nHeight, 0);
	glTexCoord2f(0.0f, 1.0); glVertex3f(0, m_nHeight, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	gl_PopOrtho();
}

void CDSMImg::InitData()
{
	/*
	int gridNum = 27;
	m_pVertex = new float[gridNum*gridNum*3];
	m_pTexcoord = new float[gridNum*gridNum*2];
	m_pIndex = new unsigned long[(gridNum-1)*(gridNum-1)*6];
	//m_pIndexForLines = new unsigned long[(gridNum-1)*(gridNum-1)*8];


	// Make Vertex, Texture-Coord Array //
	int idx = 0;
	int idxLine =0;
	for(int y=0; y<gridNum; y++)
	{
		for(int x=0; x<gridNum; x++)
		{
			m_pVertex[(y*gridNum+x)*3+0] = x*10.0f;
			m_pVertex[(y*gridNum+x)*3+1] = y*10.0f;
			m_pVertex[(y*gridNum+x)*3+2] = 0.0f;

			m_pTexcoord[(y*gridNum+x)*2+0] = (float)x/(float)(gridNum-1);
			m_pTexcoord[(y*gridNum+x)*2+1] = (float)y/(float)(gridNum-1);

			if((y != (gridNum-1))&&(x != (gridNum-1)))
			{
				// Make Index List //
				m_pIndex[idx*6+0] = y*gridNum+x;
				m_pIndex[idx*6+1] = (y+1)*gridNum+(x+1);
				m_pIndex[idx*6+2] = (y+1)*gridNum+x;

				m_pIndex[idx*6+3] = y*gridNum+x;
				m_pIndex[idx*6+4] = y*gridNum+(x+1);
				m_pIndex[idx*6+5] =  (y+1)*gridNum+(x+1);
				idx++;

		
				m_pIndexForLines[idxLine] = new unsigned long[5];
				m_pIndexForLines[idxLine][0]= y*gridNum+x;
				m_pIndexForLines[idxLine][1]= (y+1)*gridNum+(x);
				m_pIndexForLines[idxLine][2]= (y+1)*gridNum+(x+1);
				m_pIndexForLines[idxLine][3]= (y)*gridNum+(x+1);
				m_pIndexForLines[idxLine][4]= y*gridNum+x;

// 				m_pIndexForLines[idxLine][0]= y*gridNum+x;
// 				m_pIndexForLines[idxLine][1]= (y+1)*gridNum+(x+1);
// 				m_pIndexForLines[idxLine][2]= (y+1)*gridNum+x;
// 				m_pIndexForLines[idxLine][3]= y*gridNum+x;
// 				m_pIndexForLines[idxLine][4]= y*gridNum+x;
// 				m_pIndexForLines[idxLine][5]= y*gridNum+(x+1);
// 				m_pIndexForLines[idxLine][6]= (y+1)*gridNum+(x+1);
// 				m_pIndexForLines[idxLine][7]= y*gridNum+x;
				idxLine++;		

			}


		}
	}
	*/

	


}

void CDSMImg::InitGLview(int _nWidth, int _nHeight)
{
	m_nWidth = _nWidth;
	m_nHeight = _nHeight;

	m_lookAt.x = 512.0f;
	m_lookAt.y = 512.0f;
	m_lookAt.z = 0;
	m_cameraPri.InitializeCamera(45.0f, 60.0f, 0.0f, m_lookAt, _nWidth, _nHeight);
	m_cameraPri.SetInitLevelHeight(1000);

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glEnable(GL_POINT_SMOOTH);

	m_bgTex = LoadTgaTexture(L"./bg.tga");
	m_overlayTextureID = LoadTgaTexture(L"./overlay.tga");
//	m_bgTex = LoadBMPTexture(L"./face.bmp");
//	InitData();

}

void CDSMImg::MouseWheel(short zDelta)
{
	if(m_IsZoom==false)
		return;


	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	


	float zoomValue = m_cameraPri.GetLevelHeight()*0.1f;
	if(zDelta>0)
		zoomValue = -zoomValue;
	m_cameraPri.SetLevelHeight(zoomValue);	
	m_cameraPri.SetModelViewMatrix(m_lookAt, 0, 0);
	Render();

}
void CDSMImg::OnSize(UINT nType, int cx, int cy)
{
	COGLWnd::OnSize(nType, cx, cy);

	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
	m_nWidth = cx;
	m_nHeight = cy;

	m_cameraPri.SetProjectionMatrix(45.0f, 0.0f,0.0f,cx,cy);	
	m_cameraPri.SetModelViewMatrix(m_cameraPri.GetLookAt(), 0.0f, 0.0f);

	Render();

	// TODO: Add your message handler code here
//	m_nWidth = cx;
//	m_nHeight = cy;
//	InitGLview(cx, cy);
}


void CDSMImg::SetDSMTerrain(WCHAR* pData, int _nSize)
{
	if(_nSize < 2)
		return;

	m_dsmRange.clear();
	m_dsmInfo.clear();

	// Find Code Range ==========================//
	memset(m_sCodeBase, 0x00, sizeof(short)*65536);
	unsigned short nCode, nextCode;
	for(int i=0; i<_nSize; i++)
	{
		//if((pData[i]==10)||(pData[i]==13)||(pData[i]==32))
		//	continue;

		nCode = pData[i];
		m_sCodeBase[nCode] = 1;
	}
	m_sCodeBase[0]=1;
	m_sCodeBase[1]=1;
	m_sCodeBase[2]=1;

	m_sCodeBase[65533] = 1;
	m_sCodeBase[65534] = 1;
	m_sCodeBase[65535] = 1;

	// Find Code Range and rearrange coordination ========//
	unsigned short index=0;
//	m_dsmRange[65534] = index;			index++;
	for(int i=0; i<65536; i++)
	{
		if(m_sCodeBase[i]==1)
		{
			m_dsmRange[i] = index;
			index++;
		}
	}
	
//	m_dsmRange[65535] = index;
	//================================//


	unsigned long  cpos = 0;
	byte keyCode[4];

	for(int i=0; i<_nSize-1; i++)
	{
		if((pData[i]==10)||(pData[i]==13)||(pData[i]==32))
			continue;

		if((pData[i+1]==10)||(pData[i+1]==13)||(pData[i+1]==32))
			continue;

		nCode = pData[i];
		nextCode = pData[i+1];

		memset(keyCode, 0x00, 4);
		memcpy(keyCode, &nCode, sizeof(short));
		memcpy(keyCode+2, &nextCode, sizeof(short));
		memcpy(&cpos, keyCode, 4);

		std::map<unsigned long, CODEPOSITION>::iterator it = m_dsmInfo.find(cpos);
		if(it == m_dsmInfo.end())
		{
			CODEPOSITION codeinfo;
// 			codeinfo.pos.x = m_dsmRange[nCode];
// 			codeinfo.pos.y = m_dsmRange[nextCode];
// 			codeinfo.pos.z = 1.0f;

			codeinfo.xCode = m_dsmRange[nCode];
			codeinfo.yCode = m_dsmRange[nextCode];
			codeinfo.sCnt = 1;

			codeinfo.vColor.x = 0.4f;
			codeinfo.vColor.y = 0.4f;
			codeinfo.vColor.z = 0.4f;

	//		codeinfo.sCnt = 0;
			codeinfo.nRealCode[0] = nCode;
			codeinfo.nRealCode[1] = nextCode;

			m_dsmInfo[cpos] = codeinfo;
		}
		else
		{
			it->second.sCnt++;
			//float fColor = (float)it->second.sCnt*0.1f;
			//it->second.vColor.x = fColor;
			//it->second.vColor.y = fColor;
			//it->second.vColor.z = fColor;
		}
	}
	//==================================//


	



	int gridNum = m_dsmRange.size();
	MakeTerrainGrid(gridNum);
	SetDSMGridZValue(gridNum);

	MakeTerrainBottom(m_nDemDepth);
}

void CDSMImg::ReleaseTerrainDataSets()
{
	int iGridNum = m_dsmRange.size();

	if(m_pVertex != 0)
	{			
		delete [] m_pVertex;
		m_pVertex = 0;
	}

	if(m_pColor !=0)
	{
		delete [] m_pColor;
		m_pColor = 0;
	}

	if(m_pTexcoord !=0)
	{
		delete [] m_pTexcoord;
		m_pTexcoord = 0;
	}
	if(m_pIndex !=0)
	{
		delete [] m_pIndex;
		m_pIndex = 0;
	}
	//if(m_pIndexForLines !=0)
	//{
	//	for(int i=0; i<(iGridNum-1)*(iGridNum-1); i++)
	//	{
	//		delete [] m_pIndexForLines[i];
	//		m_pIndexForLines[i] = 0;
	//	}
	//}


	if(m_pVertexBottom !=0)
	{
		delete [] m_pVertexBottom;
		m_pVertexBottom = 0;
	}

	if(m_pIndexBottom !=0)
	{
		delete [] m_pIndexBottom;
		m_pIndexBottom = 0;
	}

}

void CDSMImg::ChangeDemDempth(int nDepth)
{
	MakeTerrainBottom(nDepth);
	Render();
}

void CDSMImg::MakeTerrainBottom(int nDepth)
{
	unsigned short sGrinNum = m_dsmRange.size();
	if(m_pVertex==0)
		return;


	if(m_pVertexBottom !=0)
	{
		delete [] m_pVertexBottom;
		m_pVertexBottom = 0;
	}

	if(m_pIndexBottom !=0)
	{
		delete [] m_pIndexBottom;
		m_pIndexBottom = 0;
	}

	int vNum = 8;
	int vid=0;
	m_pVertexBottom = new float[vNum*3];

	m_pVertexBottom[vid*3+0] = m_pVertex[0];		m_pVertexBottom[vid*3+1] = m_pVertex[1];		m_pVertexBottom[vid*3+2] = m_pVertex[2];		vid++;
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum-1)*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum-1)*3+1];		m_pVertexBottom[vid*3+2] = m_pVertex[(sGrinNum-1)*3+2];		vid++;
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum*sGrinNum-1)*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum*sGrinNum-1)*3+1];		m_pVertexBottom[vid*3+2] = m_pVertex[(sGrinNum*sGrinNum-1)*3+2];		vid++;
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum*(sGrinNum-1))*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum*(sGrinNum-1))*3+1];		m_pVertexBottom[vid*3+2] = m_pVertex[(sGrinNum*(sGrinNum-1))*3+2];		vid++;

	m_pVertexBottom[vid*3+0] = m_pVertex[0];		m_pVertexBottom[vid*3+1] = m_pVertex[1];		m_pVertexBottom[vid*3+2] = -nDepth;				vid++;	
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum-1)*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum-1)*3+1];		m_pVertexBottom[vid*3+2] = -nDepth;				vid++;	
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum*sGrinNum-1)*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum*sGrinNum-1)*3+1];		m_pVertexBottom[vid*3+2] = -nDepth;				vid++;	
	m_pVertexBottom[vid*3+0] = m_pVertex[(sGrinNum*(sGrinNum-1))*3];		m_pVertexBottom[vid*3+1] = m_pVertex[(sGrinNum*(sGrinNum-1))*3+1];		m_pVertexBottom[vid*3+2] = -nDepth;				vid++;

	m_vtxBottomNum = vNum;
	m_idxBottomNum = (vid/2)*6+6;  // +6 for bottom quad //

	m_pIndexBottom = new unsigned long[m_idxBottomNum];
	m_pIndexBottom[0] = 0;		m_pIndexBottom[1] = 4;		m_pIndexBottom[2] = 5;
	m_pIndexBottom[3] = 0;		m_pIndexBottom[4] = 5;		m_pIndexBottom[5] = 1;

	m_pIndexBottom[6] = 1;		m_pIndexBottom[7] = 5;		m_pIndexBottom[8] = 6;
	m_pIndexBottom[9] = 1;		m_pIndexBottom[10] = 6;		m_pIndexBottom[11] = 2;

	m_pIndexBottom[12] = 2;		m_pIndexBottom[13] = 6;		m_pIndexBottom[14] = 7;
	m_pIndexBottom[15] = 2;		m_pIndexBottom[16] = 7;		m_pIndexBottom[17] = 3;

	m_pIndexBottom[18] = 3;		m_pIndexBottom[19] = 7;		m_pIndexBottom[20] = 4;
	m_pIndexBottom[21] = 3;		m_pIndexBottom[22] = 4;		m_pIndexBottom[23] = 0;

	// Bottom Quad //
	m_pIndexBottom[24] = 4;		m_pIndexBottom[25] = 6;		m_pIndexBottom[26] = 5;
	m_pIndexBottom[27] = 4;		m_pIndexBottom[28] = 7;		m_pIndexBottom[29] = 6;


	/*
	int vNum = 4*(sGrinNum-1);
	m_pVertexBottom = new float[vNum*3*2];
	m_pIndexBottom = new unsigned long[vNum*6];

	m_vtxBottomNum = vNum*2;
	m_idxBottomNum = vNum*6;


	// Extract Vertex From m_pVertex //
	int vid=0;
	for(int i=0; i<sGrinNum-1; i++)
	{
		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = m_pVertex[i*3+2];
		vid++;

		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = -fDepth;
		vid++;
	}

	for(int i=(sGrinNum-1); i<sGrinNum*(sGrinNum-1); i+=sGrinNum)
	{
		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = m_pVertex[i*3+2];
		vid++;

		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = -fDepth;
		vid++;
	}

	for(int i=(sGrinNum*sGrinNum-1); i>sGrinNum*(sGrinNum-1); i--)
	{
		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = m_pVertex[i*3+2];
		vid++;

		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = -fDepth;
		vid++;
	}

	for(int i=sGrinNum*(sGrinNum-1); i>0; i-=sGrinNum)
	{
		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = m_pVertex[i*3+2];
		vid++;

		m_pVertexBottom[vid*3+0] = m_pVertex[i*3+0];
		m_pVertexBottom[vid*3+1] = m_pVertex[i*3+1];
		m_pVertexBottom[vid*3+2] = -fDepth;
		vid++;
	}
	*/


}
void CDSMImg::MakeTerrainGrid(unsigned short sGridNum)
{
	ReleaseTerrainDataSets();


	int gridNum = sGridNum;
	m_pVertex = new float[gridNum*gridNum*3];
	m_pColor = new float[gridNum*gridNum*3];
	m_pTexcoord = new float[gridNum*gridNum*2];
	m_pIndex = new unsigned long[(gridNum-1)*(gridNum-1)*6];
	//m_pIndexForLines = new unsigned long[(gridNum-1)*(gridNum-1)*8];


	// Make Vertex, Texture-Coord Array //
	int idx = 0;
	int idxLine =0;
	float  fGridSize = DEM_GRID_SIZE/sGridNum;
	for(int y=0; y<gridNum; y++)
	{
		for(int x=0; x<gridNum; x++)
		{
			m_pVertex[(y*gridNum+x)*3+0] = x*fGridSize;
			m_pVertex[(y*gridNum+x)*3+1] = y*fGridSize;			
			m_pVertex[(y*gridNum+x)*3+2] = 0.0f;

			m_pColor[(y*gridNum+x)*3+0] = 0.5f;
			m_pColor[(y*gridNum+x)*3+1] = 0.5f;			
			m_pColor[(y*gridNum+x)*3+2] = 0.5f;


			m_pTexcoord[(y*gridNum+x)*2+0] = (float)x/(float)(gridNum-1);
			m_pTexcoord[(y*gridNum+x)*2+1] = (float)y/(float)(gridNum-1);

			if((y != (gridNum-1))&&(x != (gridNum-1)))
			{
				// Make Index List //
				m_pIndex[idx*6+0] = y*gridNum+x;
				m_pIndex[idx*6+1] = (y+1)*gridNum+(x+1);
				m_pIndex[idx*6+2] = (y+1)*gridNum+x;

				m_pIndex[idx*6+3] = y*gridNum+x;
				m_pIndex[idx*6+4] = y*gridNum+(x+1);
				m_pIndex[idx*6+5] =  (y+1)*gridNum+(x+1);
				idx++;


				//m_pIndexForLines[idxLine] = new unsigned long[5];
				//m_pIndexForLines[idxLine][0]= y*gridNum+x;
				//m_pIndexForLines[idxLine][1]= (y+1)*gridNum+(x);
				//m_pIndexForLines[idxLine][2]= (y+1)*gridNum+(x+1);
				//m_pIndexForLines[idxLine][3]= (y)*gridNum+(x+1);
				//m_pIndexForLines[idxLine][4]= y*gridNum+x;

				idxLine++;		

			}


		}
	}

}


void CDSMImg::SetDSM(WCHAR* pData, int _nSize)
{
	SetDSMTerrain(pData, _nSize);

/*
	memset(m_matDsm, 0x00, sizeof(int)*27*27);

	unsigned short nCode, nextCode;
	m_totalCnt=0;
	for(int i=0; i<_nSize-1; i++)
	{
		nCode		= GetDSMCode(pData[i]);
		nextCode	= GetDSMCode(pData[i+1]);

		if((nextCode != 0) && (nCode != 0) && (nCode != 100))
		{

			if(nextCode==100)		// space!!
			{
			//	m_matDsm[nCode][0] ++;
			//	m_totalCnt++;
			}
			else
			{
				m_matDsm[nCode][nextCode] ++;
				m_totalCnt++;
			}
		}		
	}
	SetDSMGridZValue();
	*/
}

void CDSMImg::SetDSMGridZValue(unsigned short sGridNum)
{
	float fGridSize = DEM_GRID_SIZE / sGridNum;
	int x=0, y=0, cnt=0;
	std::map<unsigned long, CODEPOSITION>::iterator it;
	it = m_dsmInfo.begin();

	for(;it!=m_dsmInfo.end(); it++)
	{
		x = it->second.xCode;
		y = it->second.yCode;
		cnt = it->second.sCnt;
		m_pVertex[(y*sGridNum+x)*3+2] = cnt*fGridSize*10;		
		
	}
 
 	for(int i=0; i<5; i++)
  	{
  		SmoothTerrain(sGridNum);
  	}
	

	// For the Trees===================================================//
	int nSeedNum = m_dsmInfo.size();
	if(m_pTrees !=0)
	{
		for(int i=0; i<m_nTreeNum; i++)
		{
			m_pTrees[i].ReleaseTreeData();
		}
		delete [] m_pTrees;
		m_pTrees = 0;
	}

	m_nTreeNum = nSeedNum;
	m_pTrees = new CCharTreeObject[m_nTreeNum];
	memset(m_pTrees, 0x00, sizeof(m_pTrees));
	
	int idx=0;
	it = m_dsmInfo.begin();
	for(;it!=m_dsmInfo.end(); it++)
//	for(int i=0; i<nSeedNum; i++)
	{
		m_pTrees[idx].m_nSeedCode[0] = it->second.nRealCode[0];
		m_pTrees[idx].m_nSeedCode[1] = it->second.nRealCode[1];

		
		x = it->second.xCode;
		y = it->second.yCode;
		m_pTrees[idx].m_vPosition.z = m_pVertex[(y*sGridNum+x)*3+2];
		m_pTrees[idx].m_vPosition.x =(float)x*fGridSize;
		m_pTrees[idx].m_vPosition.y =(float)y*fGridSize;

		if(m_pChartData!=0)
		{
			CString strCodeName = m_pChartData->FindStringByCode(m_pTrees[idx].m_nSeedCode[0]);
			strCodeName += m_pChartData->FindStringByCode(m_pTrees[idx].m_nSeedCode[1]);
			m_pTrees[idx].m_strSeedName = strCodeName;
		}
		

		idx++;
	}



	//for(int y=0; y<sGridNum; y++)
	//{
	//	for(int x=0; x<sGridNum; x++)
	//	{
	//		if(m_totalCnt>0)
	//			m_pVertex[(y*27+x)*3+2] = ((float)m_matDsm[x][y]/(float)m_totalCnt)*100.0f;
	//		//	m_pVertex[(y*27+x)*3+2] = (float)m_matDsm[x][y]*2;
	//		else
	//			m_pVertex[(y*27+x)*3+2] = 0.0f;

	//	}
	//}


	// Adjust ZValue //
	//float effectArea=0;
	//int vNum = sGridNum*sGridNum;
	//for(int y=0; y<sGridNum; y++)
	//{
	//	for(int x=0; x<sGridNum; x++)
	//	{
	//		if(m_matDsm[x][y] > 0)
	//		{
	//			int i = (y*sGridNum+x);
	//			POINT3D V0, V1;
	//			V0.x = m_pVertex[i*3+0];
	//			V0.y = m_pVertex[i*3+1];
	//			V0.z = m_pVertex[i*3+2];

	//			effectArea = V0.z*10.0f*0.2f;  // 10.0 grig scale

	//			for(int j=0; j<vNum; j++)
	//			{
	//				V1.x = m_pVertex[j*3+0];
	//				V1.y = m_pVertex[j*3+1];
	//				V1.z = m_pVertex[j*3+2];

	//				float deltaZ = V0.z - V1.z;

	//					float fDist = mtDistanceXY(V0, V1);
	//					if((fDist < effectArea)&&(j != i))
	//					{					
	//						float fWeight = ((fDist*fDist)/(effectArea*effectArea))*0.85f;
	//						// 					if(fWeight<0.05)
	//						// 						fWeight = 0.0f;

	//						m_pVertex[j*3+2] = V1.z + (deltaZ * (1.0f-fWeight));
	//					}		

	//			}
	//		}
	//	}
	//}

}


void CDSMImg::MakeTrees(short nTickness, unsigned short nTreeLevel)
{
	if(m_pChartData==0)
		return;

	for(int i=0; i<m_nTreeNum; i++)
	{
	//	m_pChartData->FindTreeBySeedChar(m_pTrees[i].m_nSeedCode[0], m_pTrees[i].m_nSeedCode[1], m_pChartData->m_nMaxTreeLevel);

	//	m_pChartData->FindTreeBySeedChar2(m_pTrees[i].m_nSeedCode[0], m_pTrees[i].m_nSeedCode[1], m_pChartData->m_nMaxTreeLevel);
	//	m_pChartData->FindTreeBySeedChar3(m_pTrees[i].m_nSeedCode[0], m_pTrees[i].m_nSeedCode[1], m_pChartData->m_nMaxTreeLevel, m_pTrees[i].m_forwardTree, m_pTrees[i].m_backwardTree);
		m_pChartData->FindTreeBySeedChar3(m_pTrees[i].m_nSeedCode[0], m_pTrees[i].m_nSeedCode[1], nTreeLevel, m_pTrees[i].m_forwardTree, m_pTrees[i].m_backwardTree);
		m_pTrees[i].MakingTreeObject(m_pChartData, nTickness);	
	}
}

void CDSMImg::ChangeTreeTickness(short nTickness)
{
	for(int i=0; i<m_nTreeNum; i++)
	{
//		m_pChartData->FindTreeBySeedChar2(m_pTrees[i].m_nSeedCode[0], m_pTrees[i].m_nSeedCode[1], m_pChartData->m_nMaxTreeLevel);
//		m_pTrees[i].MakingTreeObject(nTickness);	
		m_pTrees[i].MakeTreeCube(nTickness);
	}

}

void CDSMImg::SmoothTerrain(unsigned short sGridNum)
{
	// smooth Z-Value //

//	m_pVertex = new float[sGridNum*sGridNum*3];


	float effectArea=0;
	int vNum = sGridNum*sGridNum;
	float fSumZ=0.0f;
	for(int y=1; y<sGridNum-1; y++)
	{
		for(int x=1; x<sGridNum-1; x++)
		{
			int i = (y*sGridNum+x);
			fSumZ = 0.0f;
			fSumZ += m_pVertex[((y)*sGridNum+(x))*3+2];
			fSumZ += m_pVertex[((y-1)*sGridNum+(x))*3+2];
			fSumZ += m_pVertex[((y-1)*sGridNum+(x-1))*3+2];
			fSumZ += m_pVertex[((y)*sGridNum+(x-1))*3+2];
			fSumZ += m_pVertex[((y+1)*sGridNum+(x-1))*3+2];
			fSumZ += m_pVertex[((y+1)*sGridNum+(x))*3+2];
			fSumZ += m_pVertex[((y+1)*sGridNum+(x+1))*3+2];
			fSumZ += m_pVertex[((y)*sGridNum+(x+1))*3+2];
			fSumZ += m_pVertex[((y-1)*sGridNum+(x+1))*3+2];


			m_pVertex[((y)*sGridNum+(x))*3+2] = fSumZ/9.0f;


			float fColor = (float)fSumZ*0.005f + 0.4f;
			m_pColor[(y*sGridNum+x)*3+0] = fColor+0.2f;
			m_pColor[(y*sGridNum+x)*3+1] = fColor+0.4f;
			m_pColor[(y*sGridNum+x)*3+2] = fColor+0.2f;


		}
	}

}


unsigned short CDSMImg::GetDSMCode(unsigned short _nCode)
{
	//if(_nCode<128)
	//	return m_uCodeMapTable[_nCode];
	//else
	//	return 0;

	if((_nCode>=65) && (_nCode<91))
	{
		return _nCode-64;
	}

	if((_nCode>=97) && (_nCode<123))
	{
		return _nCode-96;
	}

	if(_nCode==32)
		return 100;

	return 0;
}
void CDSMImg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 1;
	m_mousedown = point;
	//if(m_IsZoom==false)
	//{
		IDragMap(point.x, point.y, 0);
	//}

	pView->SetFocusOfGLView(0);
	
	SetCapture();


	COGLWnd::OnLButtonDown(nFlags, point);
}

BOOL CDSMImg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return COGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CDSMImg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 0;
// 	if(m_IsZoom==false)
// 	{
		IDragMap(point.x, point.y, 2);
// 	}
	ReleaseCapture();

	COGLWnd::OnLButtonUp(nFlags, point);
}

void CDSMImg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 2;
// 	if(m_IsZoom==false)
 		IDragMap(point.x, point.y, 0);
	SetCapture();

	COGLWnd::OnRButtonDown(nFlags, point);
}

void CDSMImg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 0;
//	if(m_IsZoom==false)
		IDragMap(point.x, point.y, 2);
	ReleaseCapture();

	COGLWnd::OnRButtonUp(nFlags, point);
}

void CDSMImg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
	if(GetCapture())
	{
// 		if(m_IsZoom==true)
// 		{
// 			short zDelta = m_mousedown.y - point.y;
// 			MouseWheel(zDelta);
// 			m_mousedown = point;
// 		}
// 		else
// 		{
			if(m_mouseMode==1)
			{
				float xAngle = (point.y-m_mousedown.y)/3.6;
				float zAngle = (point.x-m_mousedown.x)/3.6;		
				m_mousedown = point;	

				m_cameraPri.SetModelViewMatrix(m_lookAt, xAngle, zAngle);
			}
			else if(m_mouseMode==2)
			{
				IDragMap(point.x, point.y, 1);	
			}		
	//	}
		Render();
	}	

	COGLWnd::OnMouseMove(nFlags, point);
}


void CDSMImg::IDragMap(int x, int y, short sFlag)
{
	POINT3D curPos, prePos, transPos, ptLookAt;
	BOOL res = FALSE;
	int dx=0, dy=0;


	switch(sFlag){
		case 0:		// DOWN
			m_dragOper.init();
			m_dragOper.IsDrag = true;
			m_dragOper.px = x;
			m_dragOper.py = y;	
			break;

		case 1:		// MOVE
			if(m_dragOper.IsDrag){

				POINT3D pO, pN;
				mtSetPoint3D(&pO, 0, 0, 0);
		//		mtSetPoint3D(&pN, 0, 0, 1);
				pN = m_cameraPri.GetEye() - m_lookAt;
				pN = mtNormalize(pN);

				curPos = m_cameraPri.ScreenToWorld(x,y, pO, pN);
				prePos = m_cameraPri.ScreenToWorld(m_dragOper.px, m_dragOper.py, pO, pN);
				transPos = prePos  - curPos;
				ptLookAt = m_cameraPri.GetLookAt();

				//if(m_level<0){
				//	ptLookAt.x += transPos.x*1.5f;
				//	ptLookAt.y += transPos.y*1.5f;
				//	//	ptLookAt.z = 0;
				//}
				//else{
				ptLookAt.x += transPos.x;
				ptLookAt.y += transPos.y;
				ptLookAt.z += transPos.z;
				//}

				m_dragOper.px = x;
				m_dragOper.py = y;	


				//	m_DemProj->ProjectPoint(&ptLookAt, &m_cameraPri.m_currentBlockid);
				m_cameraPri.SetModelViewMatrix(ptLookAt, 0, 0);
				m_lookAt = ptLookAt;
				//	UpdateCamera(ptLookAt, 0,0);
			}
			break;

		case 2:		// UP
			ReleaseCapture();
			m_dragOper.init();
			break;
	}

}

void CDSMImg::ExportObjTrees()
{
	FILE* fp = 0;
	fp = fopen("./tree3d.obj", "w");

	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%s", "//Volumn-Links by Sanghun and Wayne");
	fwrite(buff, sizeof(buff), 1, fp);

	for(int i=0; i<m_nTreeNum; i++)
	{
		m_pTrees[i].WriteObjVertex(fp);
	}

	int iCurrPos = 1;
	for(int i=0; i<m_nTreeNum; i++)
	{
		m_pTrees[i].WriteObjIndex(fp, iCurrPos);
	}

	fclose(fp);
}

void CDSMImg::ExportObj()
{
	int gridNum = m_dsmRange.size();

	//m_pVertex = new float[gridNum*gridNum*3];
	//m_pIndex = new unsigned long[(gridNum-1)*(gridNum-1)*6];


	FILE* fp = 0;
	fp = fopen("./terrain.obj", "w");

	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%s", "//DSM-Terrain by Sanghun and Wayne");
	fwrite(buff, sizeof(buff), 1, fp);

	for(int i=0; i<gridNum*gridNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_pVertex[i*3+0],m_pVertex[i*3+1], m_pVertex[i*3+2]);
		fwrite(buff, sizeof(buff), 1, fp);
	}

	// Bottom Cube //
	for(int i=0; i<m_vtxBottomNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_pVertexBottom[i*3+0],m_pVertexBottom[i*3+1], m_pVertexBottom[i*3+2]);
		fwrite(buff, sizeof(buff), 1, fp);
	}


	for(int i=0; i<(gridNum-1)*(gridNum-1); i++)
	{

		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndex[i*6+0]+1,m_pIndex[i*6+1]+1, m_pIndex[i*6+2]+1);
		fwrite(buff, sizeof(buff), 1, fp);
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndex[i*6+3]+1,m_pIndex[i*6+4]+1, m_pIndex[i*6+5]+1);
		fwrite(buff, sizeof(buff), 1, fp);
	}


	// Bottom Cube //
	int iCurrPos = gridNum*gridNum+1;
	int btmIdxNum = m_idxBottomNum/6;
	for(int i=0; i<btmIdxNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndexBottom[i*6+0]+iCurrPos,m_pIndexBottom[i*6+1]+iCurrPos, m_pIndexBottom[i*6+2]+iCurrPos);
		fwrite(buff, sizeof(buff), 1, fp);
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndexBottom[i*6+3]+iCurrPos,m_pIndexBottom[i*6+4]+iCurrPos, m_pIndexBottom[i*6+5]+iCurrPos);
		fwrite(buff, sizeof(buff), 1, fp);
	}


	fclose(fp);


}
void CDSMImg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	COGLWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDSMImg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	COGLWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDSMImg::OnSetFocus(CWnd* pOldWnd)
{
	COGLWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}







void CDSMImg::DrawTreeText(CCharTreeObject* pTree)
{
	glPushMatrix();
	glTranslatef(pTree->m_vPosition.x, pTree->m_vPosition.y, pTree->m_vPosition.z);
	//	if(m_pCharData)
	{
		_CHAR_TREE* pTreeNode;
		CString strChar;
		POINT3D tPos;


		glColor4f(0.99f, 0.99f, 0.99f, 0.9f);
		for (int i = 1; i < pTree->m_forwardTree.size(); i++)
		{
			pTreeNode = pTree->m_forwardTree[i];
			if((pTreeNode->nCode!=32)&&(pTreeNode->nChildCnt==0))
			{
				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				strChar = m_pChartData->m_pArrayHChar[pTreeNode->nCode].m_strName;
				gl_DrawText(tPos, strChar, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);
				//	}					
			}
		}

		glColor4f(0.99f, 0.99f, 0.0f, 0.9f);
		for (int i = 1; i < pTree->m_backwardTree.size(); i++)
		{
			pTreeNode = pTree->m_backwardTree[i];
			if ((pTreeNode->nCode != 32) && (pTreeNode->nChildCnt == 0)) {
				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				strChar = m_pChartData->m_pArrayHChar[pTreeNode->nCode].m_strName;
				gl_DrawText(tPos, strChar, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);
			}
		}


		glColor4f(0.99f, 0.0f, 0.0f, 0.9f);
		if (pTree->m_backwardTree.size() > 0) {
			pTreeNode = pTree->m_backwardTree[0];
			mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
			strChar = m_pChartData->m_pArrayHChar[pTreeNode->nCode].m_strName;
			gl_DrawText(tPos, strChar, m_LogFontB, 1, m_pBmpInfo, m_CDCPtr);
		}
	}

	glPopMatrix();
}


void CDSMImg::SetDemOnOff(bool IsOn)
{
	if(m_bDemOnOff==true)
	{
		m_bDemOnOff = false;
	}
	else
	{
		m_bDemOnOff = true;
	}

	Render();
}

void CDSMImg::SetVolumnOnOff(bool IsOn)
{
	if(m_VolumnOnOff==true)
	{
		m_VolumnOnOff = false;
	}
	else
	{
		m_VolumnOnOff = true;
	}

	Render();
}

void CDSMImg::SetTreeOnOff()
{
	if(m_bTreeOnOff==true)
	{
		m_bTreeOnOff = false;
	}
	else
	{
		m_bTreeOnOff = true;
	}
	Render();
}

void CDSMImg::SetMarkerOnOff(bool IsOn)
{
	if(m_MarkerOnOff==true)
	{
		m_MarkerOnOff = false;
	}
	else
	{
		m_MarkerOnOff = true;
	}

	Render();
}

void CDSMImg::ExportAll()
{
	int gridNum = m_dsmRange.size();
	int nVnum = gridNum*gridNum;

	//m_pVertex = new float[gridNum*gridNum*3];
	//m_pIndex = new unsigned long[(gridNum-1)*(gridNum-1)*6];


	FILE* fp = 0;
	fp = fopen("./Forest.obj", "w");

	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%s", "//Tree-Terrain by Sanghun and Wayne");
	fwrite(buff, sizeof(buff), 1, fp);

	for(int i=0; i<gridNum*gridNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_pVertex[i*3+0],m_pVertex[i*3+1], m_pVertex[i*3+2]);
		fwrite(buff, sizeof(buff), 1, fp);
	}
	// Bottom Cube //
	for(int i=0; i<m_vtxBottomNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_pVertexBottom[i*3+0],m_pVertexBottom[i*3+1], m_pVertexBottom[i*3+2]);
		fwrite(buff, sizeof(buff), 1, fp);
	}


	for(int i=0; i<(gridNum-1)*(gridNum-1); i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndex[i*6+0]+1,m_pIndex[i*6+1]+1, m_pIndex[i*6+2]+1);
		fwrite(buff, sizeof(buff), 1, fp);
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndex[i*6+3]+1,m_pIndex[i*6+4]+1, m_pIndex[i*6+5]+1);
		fwrite(buff, sizeof(buff), 1, fp);
	}
	// Bottom Cube//

	int iCurrPos = nVnum+1;

	int btmIdxNum = m_idxBottomNum/6;
	for(int i=0; i<btmIdxNum; i++)
	{
		memset(buff, 0x00, sizeof(buff));
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndexBottom[i*6+0]+iCurrPos,m_pIndexBottom[i*6+1]+iCurrPos, m_pIndexBottom[i*6+2]+iCurrPos);
		fwrite(buff, sizeof(buff), 1, fp);
		sprintf(buff, "\n%s %d %d %d", "f", m_pIndexBottom[i*6+3]+iCurrPos,m_pIndexBottom[i*6+4]+iCurrPos, m_pIndexBottom[i*6+5]+iCurrPos);
		fwrite(buff, sizeof(buff), 1, fp);
	}


	//=======TREES======================//
	for(int i=0; i<m_nTreeNum; i++)
	{
		m_pTrees[i].WriteObjVertex(fp);
	}

	iCurrPos = nVnum+m_vtxBottomNum+1;
	for(int i=0; i<m_nTreeNum; i++)
	{
		m_pTrees[i].WriteObjIndex(fp, iCurrPos);
	}


	fclose(fp);


}