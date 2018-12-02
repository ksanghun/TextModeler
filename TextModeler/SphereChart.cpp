// SphereChart.cpp : implementation file
//

#include "stdafx.h"
//#include "CharDSM.h"
#include "SphereChart.h"
#include "TextAnalyzerView.h"


// SphereChart

IMPLEMENT_DYNAMIC(SphereChart, COGLWnd)

SphereChart::SphereChart()
{
	m_letterData = 0;
	m_IsShowSphere = true;
	m_IsNormalMode = false;
	m_IsVolume = false;

	memset(&m_LogFont, 0, sizeof(m_LogFont));
	strcpy((char*)m_LogFont.lfFaceName, "돋움");
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfHeight = -10;
	m_LogFont.lfWidth = 0;
	//	m_LogFont.lfWeight = FW_BOLD;

	m_pBmpInfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256));

	m_cubeTichness = 10.0f;

	m_IsZoom = false;
	m_letterData = NULL;
	m_pVertex = NULL;
}

SphereChart::~SphereChart()
{
	if(m_letterData !=0)
	{
		for(int i=0; i<65536; i++)
		{
			m_letterData[i].ResetMapData();
		}
		delete [] m_letterData;
	}

	delete m_pBmpInfo;
}


BEGIN_MESSAGE_MAP(SphereChart, COGLWnd)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// SphereChart message handlers
void SphereChart::OnOffSpherer()
{
	if(m_IsShowSphere)
		m_IsShowSphere = false;
	else
		m_IsShowSphere = true;

	Render();
}

void SphereChart::Render()
{
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
//	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);
//	glClearColor(0.3f, 0.40f, 0.65f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	//glColor3f(1,0,0);
	//glBegin(GL_LINE_STRIP);
	//glVertex3f(0,0,0);
	//glVertex3f(0,260, 0);
	//glVertex3f(260, 260, 0);
	//glVertex3f(260, 0, 0);
	//glVertex3f(0,0,0);
	//glEnd();


//	Render2D();
//	DrawDsmTerrain();





	if(m_IsNormalMode)
	{
		if(m_IsVolume==true)
			DrawVolumeNormLink();
		else
			DrawNormLinks();
	}
	else
	{
		if(m_IsVolume==true)
			DrawVolumeLink();
		else
			DrawLetterLinks();
	}

	if(m_IsShowSphere)
	{
		DrawSphere();
	}


	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//m_testCube.DrawCube(m_cubeIndex);

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	Render2D();

	SwapBuffers( m_CDCPtr->GetSafeHdc() );		// Swap the buffers (glFlush)
}

void SphereChart::DrawVolumeLink()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	for(int i=0; i<m_linkCube.size(); i++)
	{
		m_linkCube[i].DrawCube(m_cubeIndex);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

void SphereChart::DrawVolumeNormLink()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	for(int i=0; i<m_linkCube.size(); i++)
	{
		m_normLinkCube[i].DrawCube(m_cubeIndex);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

void SphereChart::DrawSphere()
{
	if(m_pVertex==0)
		return;

//	glPolygonMode(GL_FRONT, GL_LINE);

	int iGridNum =256;

	glColor3f(1.0f,1.0f,1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//	glTexCoordPointer(2, GL_FLOAT, 0, m_pTexcoord);
	glVertexPointer(3, GL_FLOAT, 0, m_pVertex);
	glColorPointer(4, GL_FLOAT, 0, m_pColor);

//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, m_bgTex);
	glDrawElements(GL_TRIANGLES, (iGridNum-1)*(iGridNum-1)*6, GL_UNSIGNED_INT, m_pIndex);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


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
	


//	glPolygonMode(GL_FRONT, GL_FILL);


}

void SphereChart::DrawNormLinks()
{
	glDisable(GL_DEPTH_TEST);
	for(int i=0; i<m_normVector.size(); i++)
	{
		glBegin(GL_LINES);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glColor4f(m_normVector[i].color.r, m_normVector[i].color.g, m_normVector[i].color.b, m_normVector[i].color.a);
		glVertex3f(m_normVector[i].v.x, m_normVector[i].v.y, m_normVector[i].v.z);

		glEnd();
	}
	glEnable(GL_DEPTH_TEST);


}

void SphereChart::DrawLetterLinks()
{
	if (m_letterData == NULL)	return;


	POINT3D tPos;
	float fAlpha[2];
	glDisable(GL_DEPTH_TEST);
	for(int i=0; i<65536; i++)
	{
		if(m_letterData[i].m_nRefCnt > 0)
		{
			std::map<unsigned long, unsigned long>::iterator it = m_letterData[i].m_mapNextCode.begin();
			for(;it!=m_letterData[i].m_mapNextCode.end(); it++)
			{
				unsigned long nx = it->first;
				fAlpha[0] = (float)m_letterData[i].m_nRefCnt * 0.01f + 0.1f;
				fAlpha[1] = (float)it->second*0.01f + 0.1f;


				glBegin(GL_LINES);
				glColor4f(m_letterData[i].m_color.x, m_letterData[i].m_color.y, m_letterData[i].m_color.z, fAlpha[0]);
				glVertex3f(m_letterData[i].m_position.x, m_letterData[i].m_position.y, m_letterData[i].m_position.z);

				glColor4f(m_letterData[nx].m_color.x, m_letterData[nx].m_color.y, m_letterData[nx].m_color.z, fAlpha[1]);
				glVertex3f(m_letterData[nx].m_position.x, m_letterData[nx].m_position.y, m_letterData[nx].m_position.z);
				glEnd();

			}
		}

		if(m_letterData[i].m_nRefCnt>20)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			mtSetPoint3D(&tPos, m_letterData[i].m_position.x, m_letterData[i].m_position.y, m_letterData[i].m_position.z );
			gl_DrawText(tPos, m_letterData[i].GetName(), m_LogFont,1, m_pBmpInfo, m_CDCPtr);

		}
	}
	glEnable(GL_DEPTH_TEST);
}

void SphereChart::Render2D()
{
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
	glColor3f(0.63f, 0.63f,0.63f);
	glVertex3f(0,0,-1);
	glVertex3f(m_nWidth,0,-1);
	glColor3f(0.14f, 0.14f,0.14f);
	glVertex3f(m_nWidth, m_nHeight,-1);
	glVertex3f(0,m_nHeight,-1);
	glEnd();


	gl_PopOrtho();
}

void SphereChart::ReleaseDataSets()
{

}

void SphereChart::InitData()
{
	m_letterData = new CLetterData[65536];

	int gridNum = 256;
	m_pVertex = new float[gridNum*gridNum*3];
	m_pColor = new float[gridNum*gridNum*4];
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
			//m_pVertex[(y*gridNum+x)*3+0] = (float)x*1.4117647f ;  //
			//m_pVertex[(y*gridNum+x)*3+1] = (float)y*0.705882f ;
			m_pVertex[(y*gridNum+x)*3+0] = (float)x*1.4117647f;  //
			m_pVertex[(y*gridNum+x)*3+1] = (float)y*0.686274f + 2.5f ;
			m_pVertex[(y*gridNum+x)*3+2] = 0.0f;

			unsigned long colorCode= (y*gridNum+x)/64;
			POINT3D vColor = GetColor(colorCode);

			m_pColor[(y*gridNum+x)*4+0] = vColor.x;
			m_pColor[(y*gridNum+x)*4+1] = vColor.y;			
			m_pColor[(y*gridNum+x)*4+2] = vColor.z;
			m_pColor[(y*gridNum+x)*4+3] = 0.3f;

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

	// Make Cube Index //

	// Bottom //
	m_cubeIndex[0] = 0;		m_cubeIndex[1] = 3;		m_cubeIndex[2] = 2;
	m_cubeIndex[3] = 0;		m_cubeIndex[4] = 2;		m_cubeIndex[5] = 1;

	// Top //
	m_cubeIndex[6] = 4;		m_cubeIndex[7] = 5;		m_cubeIndex[8] = 6;
	m_cubeIndex[9] = 4;		m_cubeIndex[10] = 6;		m_cubeIndex[11] = 7;

	// Side //
	m_cubeIndex[12] = 0;		m_cubeIndex[13] = 1;		m_cubeIndex[14] = 4;
	m_cubeIndex[15] = 1;		m_cubeIndex[16] = 5;		m_cubeIndex[17] = 4;

	m_cubeIndex[18] = 1;		m_cubeIndex[19] = 2;		m_cubeIndex[20] = 5;
	m_cubeIndex[21] = 2;		m_cubeIndex[22] = 6;		m_cubeIndex[23] = 5;

	m_cubeIndex[24] = 2;		m_cubeIndex[25] = 3;		m_cubeIndex[26] = 6;
	m_cubeIndex[27] = 3;		m_cubeIndex[28] = 7;		m_cubeIndex[29] = 6;

	m_cubeIndex[30] = 3;		m_cubeIndex[31] = 0;		m_cubeIndex[32] = 7;
	m_cubeIndex[33] = 0;		m_cubeIndex[34] = 4;		m_cubeIndex[35] = 7;



	// Cube Test //

	POINT3D v0, v1;
	mtSetPoint3D(&v0, 0,0,0);
	mtSetPoint3D(&v1, 0, 20, 20);
	GLES_COLOR c0, c1;
	c0.r = 1.0f;	c0.g = 0.0f;	c0.b = 0.0f; c0.a = 0.7f;
	c1.r = 0.0f;	c1.g = 1.0f;	c1.b = 0.0f; c1.a = 1.0f;

	m_testCube.MakeCube(v0, v1, c0, c1, 20, 20);


}

void SphereChart::InitLetterData()
{
	// For letter data //
	for(int i=0; i<65536; i++)
	{
		POINT3D pos, color;
		pos.x = m_pVertex[i*3+0];
		pos.y = m_pVertex[i*3+1];
		pos.z = m_pVertex[i*3+2];

		color.x = m_pColor[i*4+0];
		color.y = m_pColor[i*4+1];
		color.z = m_pColor[i*4+2];

		m_letterData[i].InitData();
		m_letterData[i].UpdateData(pos, color, i);
	}
}

POINT3D SphereChart::convertVecToCentroidCoord(const float & longi, const float & lati, const float & radious)
{
	POINT3D cPos;

	// 	cPos.x = longi;
	// 	cPos.y = lati;
	// 	cPos.z = 0.0f;
	// 	return cPos;


	double dPI    = 3.1415926535897932384626433832795028841968;
//	double DegToM = 360000.0f;
	double DegToM = 1.0;

	double longiR = (longi/DegToM)*dPI/180.0;
	double latiR  = ((lati/DegToM))*dPI/180.0;

	double sinvalue1 = sin(latiR);
	double sinvalue2 = sin(longiR);
	double cosvalue1 = cos(latiR);
	double cosvalue2 = cos(longiR);



	cPos.x = radious*sinvalue1*cosvalue2;
	cPos.z = radious*sinvalue1*sinvalue2;
	cPos.y = radious*cosvalue1;

	return cPos;
}

void SphereChart::MakeSphere(float fRadious)
{
	int vNum = 256*256;
	for(int i=0; i<vNum; i++)
	{
		float fLati = m_pVertex[i*3+0];
		float fLongi = m_pVertex[i*3+1];

		POINT3D sPos = convertVecToCentroidCoord(fLati, fLongi, fRadious);

		m_pVertex[i*3+0] = sPos.x;
		m_pVertex[i*3+1] = sPos.y;
		m_pVertex[i*3+2] = sPos.z;

	}


}

void SphereChart::InitGLview(int _nWidth, int _nHeight)
{
	m_nWidth = _nWidth;
	m_nHeight = _nHeight;

	m_lookAt.x = 0.0f;
	m_lookAt.y = 0.0f;
	m_lookAt.z = 0;
	m_cameraPri.InitializeCamera(45.0f, 60.0f, 0.0f, m_lookAt, _nWidth, _nHeight);
	m_cameraPri.SetInitLevelHeight(2000);

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	InitData();
	MakeSphere(500);
	InitLetterData();

}

void SphereChart::MouseWheel(short zDelta)
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
void SphereChart::OnSize(UINT nType, int cx, int cy)
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


void SphereChart::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 1;
	m_mousedown = point;
	//if(m_IsZoom==false)
	//	IDragMap(point.x, point.y, 0);
	pView->SetFocusOfGLView(1);

	SetCapture();


	COGLWnd::OnLButtonDown(nFlags, point);
}

BOOL SphereChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return COGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void SphereChart::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 0;
	if(m_IsZoom==false)
		IDragMap(point.x, point.y, 2);
	ReleaseCapture();

	COGLWnd::OnLButtonUp(nFlags, point);
}

void SphereChart::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 2;
	if(m_IsZoom==false)
		IDragMap(point.x, point.y, 0);
	SetCapture();

	COGLWnd::OnRButtonDown(nFlags, point);
}

void SphereChart::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_mouseMode = 0;
	if(m_IsZoom==false)
		IDragMap(point.x, point.y, 2);
	ReleaseCapture();

	COGLWnd::OnRButtonUp(nFlags, point);
}

void SphereChart::OnMouseMove(UINT nFlags, CPoint point)
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
//		else
//		{
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
//		}
		Render();
	}	

	COGLWnd::OnMouseMove(nFlags, point);
}


void SphereChart::IDragMap(int x, int y, short sFlag)
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
				mtSetPoint3D(&pN, 0, 0, 1);
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
				//	ptLookAt.z = 0;
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



void SphereChart::SetData(WCHAR* pData, int _nSize)
{

#ifdef _DEBUG
	_tsetlocale(LC_ALL, _T("korean"));
#endif

	CString strCode;
	for(int i=0; i<65536; i++)
	{
		m_letterData[i].ResetMapData();
	}

	for(int i=0; i<_nSize; i++)
	{
		unsigned long nCode = pData[i];		

		strCode = pData[i];
//		TRACE(_T("%s - %d\n"), strCode, nCode);
//		TRACE(_T("%06d"), nCode);

		if(nCode==0)
			continue;

		m_letterData[nCode].m_nRefCnt++;

		if(i>0)
		{
			unsigned long nPreCode = pData[i-1];
			if(nPreCode>0)
				m_letterData[nCode].AddPriviousCode(nPreCode);
		}
		if(i<(_nSize-1))
		{
			unsigned long nNextCode = pData[i+1];
			if(nNextCode>0)
				m_letterData[nCode].AddNextCode(nNextCode);
		}

		m_letterData[nCode].SetNameByChar(pData[i]);
	}


	// Exception //
	m_letterData[10].SetNameByString(L"Enter");
	m_letterData[32].SetNameByString(L"Space");


	MakeLinkObject();
	m_IsNormalMode = false;
}

POINT3D SphereChart::GetColor(unsigned long nCode)
{
	float R,G,B;
	int i = nCode;

	if(i<256)
	{
		R=0; G=(i); B = 255; 
	}

	else if((i>=256)&&(i<512))
	{ 
		R=0; G=255; B=(255-(i-256)); 
	}
	else if((i>=512)&&(i<768))
	{ 
		R = (i-512); G=255; B=0;
	}
	else if((i>=768) && (i<1024))
	{ 
		R=255; G = 255-(i-768); B=0;	
	}
	//else if((i>=1024) && (i<1280))
	//{
	//	R=255-(i-1024); G = 0; B=(i-512);
	//}


	POINT3D vColor;
	mtSetPoint3D(&vColor, R*0.00390625f,  G*0.00390625f,  B*0.00390625f);
	return vColor;
}

void SphereChart::ModeNormalVec()
{
	if(m_IsNormalMode==false)
		m_IsNormalMode = true;
	else
		m_IsNormalMode = false;

	Render();
}

void SphereChart::MakeLinkObject()
{
	float fNormLength=500.0f;
	m_normVector.clear();
	m_linkCube.clear();
	m_normLinkCube.clear();

	POINT3D V0, V1, nV, oV;
	GLES_COLOR color0, color1;
	mtSetPoint3D(&oV, 0.0f, 0.0f, 0.0f);

	for(int i=0; i<65536; i++)
	{
		if(m_letterData[i].m_nRefCnt > 0)
		{
			std::map<unsigned long, unsigned long>::iterator it = m_letterData[i].m_mapNextCode.begin();
			for(;it!=m_letterData[i].m_mapNextCode.end(); it++)
			{

				unsigned long nx = it->first;
				mtSetPoint3D(&V0, m_letterData[i].m_position.x, m_letterData[i].m_position.y, m_letterData[i].m_position.z);
				mtSetPoint3D(&V1, m_letterData[nx].m_position.x, m_letterData[nx].m_position.y, m_letterData[nx].m_position.z);
				nV = V1 - V0;
 				nV = mtNormalize(nV);
 				nV.x = nV.x*fNormLength;
 				nV.y = nV.y*fNormLength;
 				nV.z = nV.z*fNormLength;
				
				color0.r = m_letterData[i].m_color.x;
				color0.g = m_letterData[i].m_color.y;
				color0.b = m_letterData[i].m_color.z;
				color0.a = (float)it->second*0.01f + 0.1f;

				color1.r = m_letterData[nx].m_color.x;
				color1.g = m_letterData[nx].m_color.y;
				color1.b = m_letterData[nx].m_color.z;
				color1.a = (float)it->second*0.01f + 0.1f;

				COLOR_VEC3D nVector;
				nVector.v = nV;
				nVector.color = color1;

				

				m_normVector.push_back(nVector);

			//	float fCubeSize = 10.0f;

				CUBE3D linkCube;
				linkCube.MakeCube(V0, V1, color0, color1, m_cubeTichness, m_cubeTichness);
				m_linkCube.push_back(linkCube);

				CUBE3D normLinkCube;
				normLinkCube.MakeCube(oV, nV, color0, color1, m_cubeTichness, m_cubeTichness);
				m_normLinkCube.push_back(normLinkCube);




				

			}
		}
	}

//	m_IsNormalMode = true;
	Render();

}

void SphereChart::OnOffVolumn()
{
	if(m_IsVolume==true)
		m_IsVolume = false;
	else
		m_IsVolume = true;

	Render();
}

void SphereChart::ExportObj()
{
	FILE* fp = 0;
	fp = fopen("./Sphere.obj", "w");

	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%s", "//Volumn-Links by Sanghun and Wayne");
	fwrite(buff, sizeof(buff), 1, fp);

	int linkNum = m_linkCube.size();
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<8; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_linkCube[i].vertices[j*3+0],m_linkCube[i].vertices[j*3+1], m_linkCube[i].vertices[j*3+2]);
			fwrite(buff, sizeof(buff), 1, fp);
		}
	}

	int iCurrPos = 1;
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<12; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %d %d %d", "f", m_cubeIndex[j*3+0]+iCurrPos,m_cubeIndex[j*3+1]+iCurrPos, m_cubeIndex[j*3+2]+iCurrPos);
			fwrite(buff, sizeof(buff), 1, fp);
		}
		iCurrPos+=8;
	}
	fclose(fp);


	// Normazed Link //
	fp = fopen("./NormSphere.obj", "w");

	memset(buff, 0x00, sizeof(buff));
	sprintf(buff, "%s", "//Volumn-Normalized-Links by Sanghun and Wayne");
	fwrite(buff, sizeof(buff), 1, fp);

	linkNum = m_normLinkCube.size();
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<8; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_normLinkCube[i].vertices[j*3+0],m_normLinkCube[i].vertices[j*3+1], m_normLinkCube[i].vertices[j*3+2]);
			fwrite(buff, sizeof(buff), 1, fp);
		}
	}

	iCurrPos = 1;
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<12; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %d %d %d", "f", m_cubeIndex[j*3+0]+iCurrPos,m_cubeIndex[j*3+1]+iCurrPos, m_cubeIndex[j*3+2]+iCurrPos);
			fwrite(buff, sizeof(buff), 1, fp);
		}
		iCurrPos+=8;
	}
	fclose(fp);


}