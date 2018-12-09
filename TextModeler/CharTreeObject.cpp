#include "StdAfx.h"
#include "CharTreeObject.h"
#include "CharData.h"

CCharTreeObject::CCharTreeObject(void)
{

	m_nTotalCnt = 0;
	m_nMaxLevel = 0;
	m_cubeTichness = 5.0f;
	m_IsVolumn = true;

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

CCharTreeObject::~CCharTreeObject(void)
{
	ReleaseTreeData();
}


void CCharTreeObject::ReleaseTreeData()
{
	for(int i=0; i<m_forwardTree.size(); i++)
	{
		delete m_forwardTree[i];
	}
	for(int i=0; i<m_backwardTree.size(); i++)
	{
		delete m_backwardTree[i];
	}
	m_forwardTree.clear();
	m_backwardTree.clear();
}



void CCharTreeObject::MakeTreeCube(short nTickness)
{
//	int nMaxLevel=pChartData->m_nMaxTreeLevel;
	m_linkCube.clear();

	m_cubeTichness = nTickness;
	

	_CHAR_TREE* pTreeNode;
	_CHAR_TREE* pParents;

	glLineWidth(1);
	glColor4f(0.3f, 0.2f, 0.1f, 0.4f);
	//	glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
	GLES_COLOR color0, color1;
	POINT3D v0, v1;

	for(int i=0; i<m_forwardTree.size(); i++)
	{
		pTreeNode = m_forwardTree[i];
		pParents = pTreeNode->pParents;
		if((pParents) &&(pTreeNode->nCode != 65535))
		{
			// !!!!! Change Axis !!! Y-Z -> Z-Y //
			mtSetPoint3D(&v0, pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
			mtSetPoint3D(&v1, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);

			float fAlpah0 = (float)(m_nMaxLevel-pParents->nLevel)/(float)m_nMaxLevel;
			float fAlpah1 = (float)(m_nMaxLevel-pTreeNode->nLevel)/(float)m_nMaxLevel;
			//	fAlpah1 = fAlpah0;

			color0.r = 0.3;		color0.g = 0.2f;		color0.b = 0.1f;		color0.a = fAlpah0;
			color1.r = 0.3;		color1.g = 0.2f;		color1.b = 0.1f;		color1.a = fAlpah1;

			BRENCH_CUBE nCube;
			nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
			m_linkCube.push_back(nCube);
		}			
	}

	for(int i=0; i<m_backwardTree.size(); i++)
	{
		pTreeNode = m_backwardTree[i];
		pParents = pTreeNode->pParents;
		if((pParents) &&(pTreeNode->nCode != 65535))
		{
			mtSetPoint3D(&v0, pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
			mtSetPoint3D(&v1, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);

			float fAlpah0 = (float)(m_nMaxLevel-pParents->nLevel)/(float)m_nMaxLevel;
			float fAlpah1 = (float)(m_nMaxLevel-pTreeNode->nLevel)/(float)m_nMaxLevel;
			//		fAlpah1 = fAlpah0;

			color0.r = 0.7;		color0.g = 0.5f;		color0.b = 0.4f;		color0.a = fAlpah0;
			color1.r = 0.7;		color1.g = 0.5f;		color1.b = 0.4f;		color1.a = fAlpah1;


			if(pTreeNode->nLevel>5)
			{
				int a=0;
			}

			BRENCH_CUBE nCube;
			nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
			m_linkCube.push_back(nCube);
		}			
	}

	if(m_forwardTree.size()>0)
	{
		mtSetPoint3D(&v0,m_forwardTree[0]->vPos.x, m_forwardTree[0]->vPos.z, m_forwardTree[0]->vPos.y);
		mtSetPoint3D(&v1,0.0f,0.0f,0.0f);

		float fAlpah0 = 1.0f;
		float fAlpah1 = 1.0f;

		color0.r = 0.3;		color0.g = 0.2f;		color0.b = 0.1f;		color0.a = fAlpah0;
		color1.r = 0.3;		color1.g = 0.2f;		color1.b = 0.1f;		color1.a = fAlpah1;

		BRENCH_CUBE nCube;
		nCube.MakeCube(v0, v1, color0, color1, m_cubeTichness*fAlpah0, m_cubeTichness*fAlpah1);
		m_linkCube.push_back(nCube);
	}


}

void CCharTreeObject::MakingTreeObject(CCharData* pChartData, short nTickness)
{
	if(pChartData)
	{
		m_nMaxLevel = pChartData->m_nMaxTreeLevel;
		m_nTotalCnt = pChartData->m_totalCnt;
		MakeTreeCube(nTickness);

	}

}

void CCharTreeObject::Draw3DTree()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();
	glTranslatef(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	for(int i=0; i<m_linkCube.size(); i++)
	{
		m_linkCube[i].DrawCube(m_cubeIndex);
	}
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}


void CCharTreeObject::DrawTreeLine()
{
	glPushMatrix();
	glTranslatef(m_vPosition.x, m_vPosition.y, m_vPosition.z);


	int nMaxLevel = m_nMaxLevel;


//	if(m_pCharData)
	{
		_CHAR_TREE* pTreeNode;
		_CHAR_TREE* pParents;

		glLineWidth(2);
		glColor4f(0.99f, 0.99f, 0.1f, 0.99f);
		//	glColor4f(0.0f, 0.0f, 0.0f, 0.9f);

		for(int i=0; i<m_forwardTree.size(); i++)
		{
			pTreeNode = m_forwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
			//	glLineWidth(nMaxLevel-pTreeNode->nLevel);
				glBegin(GL_LINES);
				//glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.y, pTreeNode->vPos.z);
				//glVertex3f(pParents->vPos.x, pParents->vPos.y, pParents->vPos.z);

				glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				glVertex3f(pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				glEnd();
			}			
		}

		glColor4f(0.99f, 0.5f, 0.1f, 0.99f);
		for(int i=0; i<m_backwardTree.size(); i++)
		{
			pTreeNode = m_backwardTree[i];
			pParents = pTreeNode->pParents;
			if((pParents) &&(pTreeNode->nCode != 65535))
			{
			//	glLineWidth(nMaxLevel-pTreeNode->nLevel);
				glBegin(GL_LINES);
				//glVertex3f(pTreeNode->vPos.x, -pTreeNode->vPos.y, pTreeNode->vPos.z);
				//glVertex3f(pParents->vPos.x, -pParents->vPos.y, pParents->vPos.z);
				glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
				glVertex3f(pParents->vPos.x, pParents->vPos.z, pParents->vPos.y);
				glEnd();
			}			
		}





		if(m_forwardTree.size()>0)
		{
		//	glLineWidth(nMaxLevel);
		//	glColor4f(0.3f, 0.2f, 0.1f, 0.5f);
			glColor4f(0.99f, 0.99f, 0.1f, 0.99f);
			glBegin(GL_LINES);
			glVertex3f(m_forwardTree[0]->vPos.x, m_forwardTree[0]->vPos.z, m_forwardTree[0]->vPos.y);
			//	glVertex3f(m_pCharData->m_forwardTree[0]->vPos.x, m_pCharData->m_forwardTree[0]->vPos.y, m_pCharData->m_forwardTree[0]->vPos.z);
			//	glVertex3f(m_pCharData->m_backwardTree[0]->vPos.x, m_pCharData->m_backwardTree[0]->vPos.y, m_pCharData->m_backwardTree[0]->vPos.z);
			glVertex3f(0,0,0);
			glEnd();
		}

		glLineWidth(1);


	}

	glPopMatrix();
}

void CCharTreeObject::DrawTreeNode(float fScale, float fAlpha)
{
	glPushMatrix();
	glTranslatef(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	if(fScale>1.0f)
		fScale = 1.0f;

	//int iSizeB = (int)(15.0f*fScale);
	//int iSizeS = (int)(8.0f*fScale);
	//int iSizeO = iSizeB+2;

	int iSizeS = 1;
	int nNodeSize=1;

//	if(m_pCharData)
	{

//		float fTotal = m_nTotalCnt;


		_CHAR_TREE* pTreeNode;
		_CHAR_TREE* pParents;



		for(int i=1; i<m_forwardTree.size(); i++)
		{

			pTreeNode = m_forwardTree[i];	
	//		nNodeSize = (float)pTreeNode->nCnt*500 / fTotal;
	//		if(nNodeSize<8)
				nNodeSize = 8;
	//		if (nNodeSize > 15)
	//			nNodeSize = 15;
			

			glPointSize(nNodeSize);

			//if(pTreeNode->nChildCnt>0)
			//{
			//	continue;
			//}

			glColor4f(0.2f, 0.99f, 0.2f, fAlpha);
			if(pTreeNode->nCode==32)
			{
				glPointSize(iSizeS);
				glColor4f(0.99f, 0.4f, 0.2f, fAlpha);
			}
			glBegin(GL_POINTS);
			//	glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.y, pTreeNode->vPos.z);	
			glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);	
			glEnd();
		}

#if 0
		for(int i=1; i<m_backwardTree.size(); i++)
		{

			pTreeNode = m_backwardTree[i];	
			nNodeSize = (float)pTreeNode->nCnt*500 / fTotal;
			if(nNodeSize<8)
				nNodeSize = 8;

			glPointSize(nNodeSize);
			//if(pTreeNode->nChildCnt>0)
			//{
			//	continue;
			//}

			glColor4f(0.2f, 0.99f, 0.2f, 0.5f);
			if(pTreeNode->nCode==32)
			{
				glPointSize(iSizeS);
				glColor4f(0.99f, 0.4f, 0.2f, 0.5f);
			}
			glBegin(GL_POINTS);
			//	glVertex3f(pTreeNode->vPos.x, -pTreeNode->vPos.y, pTreeNode->vPos.z);	
			glVertex3f(pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);	
			glEnd();
		}		
#endif

	}



	glPopMatrix();
}
// 
// void CCharTreeObject::DrawTreeText(CCharData* pChartData, LOGFONT logFont, BITMAPINFO *pBmpInfo, CDC* pCDCPtr)
// {
// //	if(m_pCharData)
// 	{
// 		_CHAR_TREE* pTreeNode;
// 		CString strChar;
// 		POINT3D tPos;
// 
// 
// 		glColor4f(0.0f, 0.0f, 0.0f, 0.9f);
// 		for(int i=1; i<m_forwardTree.size(); i++)
// 		{			
// 			pTreeNode = m_forwardTree[i];			
// 			if(pTreeNode->nCode!=32)
// 			{
// 				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
// 				strChar = pChartData->m_pArrayHChar[pTreeNode->nCode].m_strName;
// 				gl_DrawText(tPos, strChar, logFont, 1, pBmpInfo, pCDCPtr);
// 			}					
// 		}
// 
// 		for(int i=0; i<m_backwardTree.size(); i++)
// 		{
// 			pTreeNode = m_backwardTree[i];				
// 			if(pTreeNode->nCode!=32)
// 			{
// 				// 				tPos = pTreeNode->vPos;
// 				// 				tPos.y = -tPos.y;
// 				mtSetPoint3D(&tPos, pTreeNode->vPos.x, pTreeNode->vPos.z, pTreeNode->vPos.y);
// 				strChar = pChartData->m_pArrayHChar[pTreeNode->nCode].m_strName;
// 				gl_DrawText(tPos, strChar, m_LogFont, 1, m_pBmpInfo, m_CDCPtr);
// 			}					
// 		}		
// 	}
// }


void CCharTreeObject::WriteObjVertex(FILE* fp)
{
	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	int linkNum = m_linkCube.size();
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<8; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %3.2f %3.2f %3.2f", "v", m_linkCube[i].vertices[j*3+0]+m_vPosition.x, m_linkCube[i].vertices[j*3+1]+m_vPosition.y, m_linkCube[i].vertices[j*3+2]+m_vPosition.z);
			fwrite(buff, sizeof(buff), 1, fp);
		}
	}
}

int CCharTreeObject::WriteObjIndex(FILE* fp, int& nIndex)
{
	char buff[64];
	memset(buff, 0x00, sizeof(buff));
	int linkNum = m_linkCube.size();
	for(int i=0; i<linkNum; i++)
	{
		for(int j=0; j<12; j++)
		{
			memset(buff, 0x00, sizeof(buff));
			sprintf(buff, "\n%s %d %d %d", "f", m_cubeIndex[j*3+0]+nIndex,m_cubeIndex[j*3+1]+nIndex, m_cubeIndex[j*3+2]+nIndex);
			fwrite(buff, sizeof(buff), 1, fp);
		}
		nIndex+=8;
	}

	return nIndex;
}