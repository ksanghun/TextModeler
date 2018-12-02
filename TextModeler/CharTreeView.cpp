#include "StdAfx.h"
#include "CharTreeView.h"
#include "CharData.h"

CCharTreeView::CCharTreeView(void)
{
	memset(&m_LogFont, 0, sizeof(m_LogFont));
	strcpy((char*)m_LogFont.lfFaceName, "돋움");
	m_LogFont.lfCharSet = ANSI_CHARSET;
	m_LogFont.lfHeight = -12;
	m_LogFont.lfWidth = 0;
	//	m_LogFont.lfWeight = FW_BOLD;

	m_pBmpInfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD)*256));

	m_nWidth = 0;
	m_nHeight = 0;

	m_dragOper.init();
	m_mouseMode = 0;

	m_pCharData = 0;
	m_AniCnt = 0;

	m_cubeTichness = 10.0f;
	m_IsVolumn = true;
	
	
}

CCharTreeView::~CCharTreeView(void)
{
	delete m_pBmpInfo;
}

BEGIN_MESSAGE_MAP(CCharTreeView, COGLWnd)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// CCharView 메시지 처리기입니다.
void CCharTreeView::InitGLview(int _nWidth, int _nHeight)
{
	m_nWidth = _nWidth;
	m_nHeight = _nHeight;

	//	m_pCharData = _pData;


	m_lookAt.x = 0;
	m_lookAt.y = 150;
	m_lookAt.z = 0;
	m_cameraPri.InitializeCamera(45.0f, 0.0f, 0.0f, m_lookAt, _nWidth, _nHeight);
	m_cameraPri.SetInitLevelHeight(1500);

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	SetTimer(100, 200, NULL);

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
}


void CCharTreeView::SetCharData(CCharData* _pData)
{
	m_pCharData = _pData;
	MakingTreeObject();
}

void CCharTreeView::Render()
{
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	

//	glClearColor(0.4f, 0.6f, 0.8f, 1.0f);

	glClearColor(0.99f, 0.99f, 0.99f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	float fLevelScale = 1.0f / (m_cameraPri.GetLevelHeight()*0.001f);

	// Draw Base Line //
	//glLineWidth(4);
//	glColor4f(0.2f, 0.2f, 0.4f, 0.4f);
	
	glColor4f(0.7f, 0.7f, 0.7f, 0.3f);
	glLineWidth(1);
	glBegin(GL_LINES);
	for(int i=0; i<21; i++)
	{
		glVertex3f(1000,100*i-1000,0);
		glVertex3f(-1000,100*i-1000,0);

		glVertex3f(100*i-1000, -1000, 0);
		glVertex3f(100*i-1000, 1000, 0);
	}
	glEnd();


	if(m_IsVolumn)
	{
		Draw3DTree();
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
		DrawTreeLine();
	}


	DrawTreeNode(fLevelScale);
	if(fLevelScale>1.0f)
		DrawTreeText();
	glEnable(GL_DEPTH_TEST);
	
	
	SwapBuffers( m_CDCPtr->GetSafeHdc() );		// Swap the buffers (glFlush)
}

void CCharTreeView::MakingTreeObject()
{
	m_linkCube.clear();

	if(m_pCharData)
	{
		int nMaxLevel=m_pCharData->m_nMaxTreeLevel;

		_CHAR_TREE* pTreeNode;
		_CHAR_TREE* pParents;

		glLineWidth(1);
		glColor4f(0.3f, 0.2f, 0.1f, 0.4f);
		//	glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
		GLES_COLOR color0, color1;
		POINT3D v0, v1;

		for(int i=0; i<m_pCharData->m_forwardTree.size(); i++)
		{
			pTreeNode = m_pCharData->m_forwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
				// !!!!! Change Axis !!! Y-Z -> Z-Y //
				mtSetPoint3D(&v0, pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				mtSetPoint3D(&v1, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);

				float fAlpah0 = (float)(nMaxLevel-pParents->nLevel)/(float)nMaxLevel;
				float fAlpah1 = (float)(nMaxLevel-pTreeNode->nLevel)/(float)nMaxLevel;
			//	fAlpah1 = fAlpah0;

				color0.r = 0.3;		color0.g = 0.2f;		color0.b = 0.1f;		color0.a = fAlpah0;
				color1.r = 0.3;		color1.g = 0.2f;		color1.b = 0.1f;		color1.a = fAlpah1;

				CUBE3D nCube;
				nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
				m_linkCube.push_back(nCube);
			}			
		}

		for(int i=0; i<m_pCharData->m_backwardTree.size(); i++)
		{
			pTreeNode = m_pCharData->m_backwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
				mtSetPoint3D(&v0, pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				mtSetPoint3D(&v1, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);

				float fAlpah0 = (float)(nMaxLevel-pParents->nLevel)/(float)nMaxLevel;
				float fAlpah1 = (float)(nMaxLevel-pTreeNode->nLevel)/(float)nMaxLevel;
		//		fAlpah1 = fAlpah0;

				color0.r = 0.3;		color0.g = 0.2f;		color0.b = 0.1f;		color0.a = fAlpah0;
				color1.r = 0.3;		color1.g = 0.2f;		color1.b = 0.1f;		color1.a = fAlpah1;


				if(pTreeNode->nLevel>5)
				{
					int a=0;
				}

				CUBE3D nCube;
				nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
				m_linkCube.push_back(nCube);
			}			
		}

		if(m_pCharData->m_forwardTree.size()>0)
		{
			mtSetPoint3D(&v0,m_pCharData->m_forwardTree[0]->vPos.x, m_pCharData->m_forwardTree[0]->vPos.z, m_pCharData->m_forwardTree[0]->vPos.y);
			mtSetPoint3D(&v1,0.0f,0.0f,0.0f);

			float fAlpah0 = 1.0f;
			float fAlpah1 = 1.0f;

			color0.r = 0.3;		color0.g = 0.2f;		color0.b = 0.1f;		color0.a = fAlpah0;
			color1.r = 0.3;		color1.g = 0.2f;		color1.b = 0.1f;		color1.a = fAlpah1;

			CUBE3D nCube;
			nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
			m_linkCube.push_back(nCube);
		}
	}

}

void CCharTreeView::Draw3DTree()
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

void CCharTreeView::DrawTreeLine()
{
	int nMaxLevel = 0;
	if(m_pCharData)
	{
		nMaxLevel=m_pCharData->m_nMaxTreeLevel-2;
	}

	if(m_pCharData)
	{
		_CHAR_TREE* pTreeNode;
		_CHAR_TREE* pParents;

		glLineWidth(1);
		glColor4f(0.3f, 0.2f, 0.1f, 0.4f);
	//	glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
		
		for(int i=0; i<m_pCharData->m_forwardTree.size(); i++)
		{
			pTreeNode = m_pCharData->m_forwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
				glLineWidth(nMaxLevel-pTreeNode->nLevel);
				glBegin(GL_LINES);
				//glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.y, pTreeNode->vPos.z);
				//glVertex3f(pParents->vPos.x, pParents->vPos.y, pParents->vPos.z);

				glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				glVertex3f(pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				glEnd();
			}			
		}

		glColor4f(0.3f, 0.2f, 0.1f, 0.2f);
		for(int i=0; i<m_pCharData->m_backwardTree.size(); i++)
		{
			pTreeNode = m_pCharData->m_backwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
				glLineWidth(nMaxLevel-pTreeNode->nLevel);
				glBegin(GL_LINES);
				//glVertex3f(pTreeNode->vPos.x, -pTreeNode->vPos.y, pTreeNode->vPos.z);
				//glVertex3f(pParents->vPos.x, -pParents->vPos.y, pParents->vPos.z);
				glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				glVertex3f(pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				glEnd();
			}			
		}





		if(m_pCharData->m_forwardTree.size()>0)
		{
			glLineWidth(nMaxLevel);
			glColor4f(0.3f, 0.2f, 0.1f, 0.5f);
			glBegin(GL_LINES);
			glVertex3f(m_pCharData->m_forwardTree[0]->vPos.x, m_pCharData->m_forwardTree[0]->vPos.z, m_pCharData->m_forwardTree[0]->vPos.y);
		//	glVertex3f(m_pCharData->m_forwardTree[0]->vPos.x, m_pCharData->m_forwardTree[0]->vPos.y, m_pCharData->m_forwardTree[0]->vPos.z);
		//	glVertex3f(m_pCharData->m_backwardTree[0]->vPos.x, m_pCharData->m_backwardTree[0]->vPos.y, m_pCharData->m_backwardTree[0]->vPos.z);
			glVertex3f(0,0,0);
			glEnd();
		}

		glLineWidth(1);

		
	}
}

void CCharTreeView::DrawTreeNode(float fScale)
{
	
	if(fScale>1.0f)
		fScale = 1.0f;

	//int iSizeB = (int)(15.0f*fScale);
	int iSizeS = (int)(8.0f*fScale);
	//int iSizeO = iSizeB+2;

	int nNodeSize=1;

	if(m_pCharData)
	{

		float fTotal = m_pCharData->m_totalCnt;


		_CHAR_TREE* pTreeNode;
		_CHAR_TREE* pParents;

		
		
		for(int i=1; i<m_pCharData->m_forwardTree.size(); i++)
		{
			
			pTreeNode = m_pCharData->m_forwardTree[i];	
			nNodeSize = (float)pTreeNode->nCnt*500 / fTotal;
			if(nNodeSize<8)
				nNodeSize = 8;

			glPointSize(nNodeSize);

			//if(pTreeNode->nChildCnt>0)
			//{
			//	continue;
			//}

			glColor4f(0.2f, 0.99f, 0.2f, 0.99f);
			if(pTreeNode->nCode==32)
			{
				glPointSize(iSizeS);
				glColor4f(0.99f, 0.4f, 0.2f, 0.99f);
			}
			glBegin(GL_POINTS);
		//	glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.y, pTreeNode->vPos.z);	
			glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);	
			glEnd();
		}

		for(int i=1; i<m_pCharData->m_backwardTree.size(); i++)
		{
			
			pTreeNode = m_pCharData->m_backwardTree[i];	
			nNodeSize = (float)pTreeNode->nCnt*500 / fTotal;
			if(nNodeSize<8)
				nNodeSize = 8;

			glPointSize(nNodeSize);
			//if(pTreeNode->nChildCnt>0)
			//{
			//	continue;
			//}

			glColor4f(0.2f, 0.99f, 0.2f, 0.99f);
			if(pTreeNode->nCode==32)
			{
				glPointSize(iSizeS);
				glColor4f(0.99f, 0.4f, 0.2f, 0.99f);
			}
			glBegin(GL_POINTS);
		//	glVertex3f(pTreeNode->vPos.x, -pTreeNode->vPos.y, pTreeNode->vPos.z);	
			glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);	
			glEnd();
		}		

		
	}
}

void CCharTreeView::DrawTreeText()
{
	if(m_pCharData)
	{
		_CHAR_TREE* pTreeNode;
		CString strChar;
		POINT3D tPos;

	
		glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
		for(int i=1; i<m_pCharData->m_forwardTree.size(); i++)
		{			
			pTreeNode = m_pCharData->m_forwardTree[i];			
			if(pTreeNode->nCode!=32)
			{
				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				strChar = m_pCharData->m_pArrayHChar[pTreeNode->nCode].m_strName;
				gl_DrawText(tPos, strChar, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);
			}					
		}

 		for(int i=0; i<m_pCharData->m_backwardTree.size(); i++)
 		{
 			pTreeNode = m_pCharData->m_backwardTree[i];				
 			if(pTreeNode->nCode!=32)
 			{
// 				tPos = pTreeNode->vPos;
// 				tPos.y = -tPos.y;
				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
 				strChar = m_pCharData->m_pArrayHChar[pTreeNode->nCode].m_strName;
 				gl_DrawText(tPos, strChar, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);
 			}					
 		}		
	}
}

void CCharTreeView::OnSize(UINT nType, int cx, int cy)
{
	COGLWnd::OnSize(nType, cx, cy);

	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
	m_nWidth = cx;
	m_nHeight = cy;

	m_cameraPri.SetProjectionMatrix(45.0f, 0.0f,0.0f,cx,cy);	
	m_cameraPri.SetModelViewMatrix(m_cameraPri.GetLookAt(), 0.0f, 0.0f);

	
}

void CCharTreeView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==100)
	{
		Render();
	}

	else if(nIDEvent==200)
	{
		UpdateTreeNodePosition();
		m_AniCnt++;
		if(m_AniCnt>20)
			KillTimer(200);
	}

	COGLWnd::OnTimer(nIDEvent);
}

void CCharTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_mouseMode = 1;
	m_mousedown = point;
	IDragMap(point.x, point.y, 0);
	SetCapture();
	

	COGLWnd::OnLButtonDown(nFlags, point);
}

void CCharTreeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_mouseMode = 0;
	IDragMap(point.x, point.y, 2);
	ReleaseCapture();
	

	COGLWnd::OnLButtonUp(nFlags, point);
}

void CCharTreeView::OnMouseMove(UINT nFlags, CPoint point)
{
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	
	if(GetCapture())
	{
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
		Render();
	}	

	COGLWnd::OnMouseMove(nFlags, point);
}

void CCharTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_mouseMode = 2;
	IDragMap(point.x, point.y, 0);
	SetCapture();	
	

	COGLWnd::OnRButtonDown(nFlags, point);
}

void CCharTreeView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_mouseMode = 0;
	IDragMap(point.x, point.y, 2);
	ReleaseCapture();
	

	COGLWnd::OnRButtonUp(nFlags, point);
}

void CCharTreeView::MouseWheel(short zDelta)
{
	wglMakeCurrent(m_CDCPtr->GetSafeHdc(), m_hRC);	


	float zoomValue = m_cameraPri.GetLevelHeight()*0.1f;
	if(zDelta>0)
		zoomValue = -zoomValue;
	m_cameraPri.SetLevelHeight(zoomValue);	
	m_cameraPri.SetModelViewMatrix(m_lookAt, 0, 0);
	Render();
}
BOOL CCharTreeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	MouseWheel(zDelta);
	return COGLWnd::OnMouseWheel(nFlags, zDelta, pt);
}

int CCharTreeView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	SetFocus();
	return COGLWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CCharTreeView::IDragMap(int x, int y, short sFlag)
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

void CCharTreeView::StartAnimation()
{
//	UpdateTreeNodePosition();
//  	KillTimer(200);
//  
//  	m_AniCnt = 0;
//  	SetTimer(200, 30, NULL);
}

void CCharTreeView::UpdateTreeNodePosition()
{
	POINT3D vDir;
	_CHAR_TREE* pTreeNode;
	float fSpeed  = 1.0f;


	for(int i=0; i<m_pCharData->m_forwardTree.size(); i++)
	{			
		pTreeNode = m_pCharData->m_forwardTree[i];		


		vDir = pTreeNode->vTargetPos - pTreeNode->vPos;
		pTreeNode->vPos.x = pTreeNode->vPos.x + vDir.x*fSpeed;
		pTreeNode->vPos.y = pTreeNode->vPos.y + vDir.y*fSpeed;
		pTreeNode->vPos.z = pTreeNode->vPos.z + vDir.z*fSpeed;
	}

 	for(int i=0; i<m_pCharData->m_backwardTree.size(); i++)
 	{
 		pTreeNode = m_pCharData->m_backwardTree[i];		
 		vDir = pTreeNode->vTargetPos - pTreeNode->vPos;
 		pTreeNode->vPos.x = pTreeNode->vPos.x + vDir.x*fSpeed;
 		pTreeNode->vPos.y = pTreeNode->vPos.y + vDir.y*fSpeed;
 		pTreeNode->vPos.z = pTreeNode->vPos.z + vDir.z*fSpeed;				
 	}


}

void CCharTreeView::ExportObj()
{
	FILE* fp = 0;
	fp = fopen("./tree3d.obj", "w");

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
}

void CCharTreeView::VolumnMode()
{
	if(m_IsVolumn)
		m_IsVolumn = false;
	else
		m_IsVolumn = true;
	Render();
}

void CCharTreeView::ChangeTickness(int iTickness)
{
	m_cubeTichness = iTickness;
	MakingTreeObject();
	Render();
}