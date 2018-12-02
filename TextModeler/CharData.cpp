#include "StdAfx.h"
#include "CharData.h"
//#include "TBContainerView.h"


#define QRGB_R(color) ((color&0x00FF0000)>>16)
#define QRGB_G(color) ((color&0x0000FF00)>>8)
#define QRGB_B(color) ((color&0x000000FF)>>0)

#define BRENCH_LENGTH 20

CCharData::CCharData(void)
{
	m_pArrayHChar = new CHangulChar[65536];
	m_nNumChar = 0;
	m_nTop20Value = 0;
	m_fXOffset = 0.0f;
	m_nTop20Word = 0;

	m_pAllChar = 0;
	m_pAllCharInv = 0;

	m_charAddCnt=0;
	m_forwardTreeDepth = 0;
	m_backwardTreeDepth = 0;

	memset(m_treeBuffer, 0x00, sizeof(m_treeBuffer));


	float upSize = 300;
	for(int i=0;i<20; i++)
	{
		m_fFLevelSize[i] = upSize;
		upSize-=12;
	}

	upSize = 100;
	for(int i=0;i<20; i++)
	{
		m_fBLevelSize[i] = upSize;
		upSize*=1.2;
	}

	m_nMaxTreeLevel = 7;
	m_nLastNodeCnt=0;
}

CCharData::~CCharData(void)
{
	
}

void CCharData::ReleaseData()
{
	if(m_pAllChar != 0)
	{
		delete [] m_pAllChar;
		m_pAllChar = 0;
	}

// 	if(m_pArrayHChar !=0)
// 		delete [] m_pArrayHChar;

	if(m_pAllCharInv != 0)
	{
		delete [] m_pAllCharInv;
		m_pAllCharInv = 0;
	}

	// 	std::map<unsigned long, CHangulChar*>::iterator it;
	// 	it = m_mapChar.begin();
	// 	for(;it!=m_mapChar.end(); it++)
	// 	{
	// 		delete it->second;
	// 		it->second = NULL;
	// 	}

	m_mapChar.clear();
	m_sortVec.clear();
	m_mapHWord.clear();
	m_sortHWord.clear();
	m_vecTopHWord.clear();

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

POINT3D CCharData::GetColor(unsigned long nCode)
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
	else if((i>=1024) && (i<1280))
	{
		R=255-(i-1024); G = 0; B=(i-512);
	}


	POINT3D vColor;
	mtSetPoint3D(&vColor, R*0.00390625f,  G*0.00390625f,  B*0.00390625f);
	return vColor;
}

void CCharData::InitCharData()
{
	m_fXOffset = 160.0f;
	for(int i=0; i<65536; i++)
	{
		m_pArrayHChar[i].InitDataSet();
		m_pArrayHChar[i].m_vImgPos.x = i/256;
		m_pArrayHChar[i].m_vImgPos.y = i%256;
		m_pArrayHChar[i].m_vImgPos.z = 0;

		m_pArrayHChar[i].m_vPos.x = (float)(i)/50.0f + m_fXOffset;
		m_pArrayHChar[i].m_vPos.y = 0.0f;
		m_pArrayHChar[i].m_vPos.z = 0.0f;

		m_pArrayHChar[i].m_vCirclePos.x = 0.0f;
		m_pArrayHChar[i].m_vCirclePos.y = 0.0f;
		m_pArrayHChar[i].m_vCirclePos.z = 0.0f;

		
		unsigned long colorCode= i/52;
		//if((i/1024) % 2 ==0)
		//{
		//	colorCode = i%1024;
		//}
		//else
		//{
		//	colorCode = 1024 - (i%1024);
		//}

		
		m_pArrayHChar[i].m_vColor = GetColor(colorCode);

//		TRACE(L"Color Table [%d]: %d, %d, %d\n", i, (int)(m_pArrayHChar[i].m_vColor.x*255), (int)(m_pArrayHChar[i].m_vColor.y*255), (int)(m_pArrayHChar[i].m_vColor.z*255));



		//m_pArrayHChar[i].m_vColor.y		= (float)(QRGB_R(colorCode)) * 0.00390625f;
		//m_pArrayHChar[i].m_vColor.x		= (float)(QRGB_G(colorCode)) * 0.00390625f;
		//m_pArrayHChar[i].m_vColor.z		= (float)(QRGB_B(colorCode)) * 0.00390625f;

		//m_pArrayHChar[i].GetColor(i);

	}

	// Make Circle //
	float cid = 0.0f;
	float fRadious = 150;
//	for(int i=44000; i<55200; i++)
	for(int i=0; i<65536; i++)
	{
		double fAngle = (cid / 65536.0f)*360;
		double rad = fAngle*3.141529f / 180.0f;


		
		m_pArrayHChar[i].m_vCirclePos.x = fRadious * cos(rad);
		m_pArrayHChar[i].m_vCirclePos.y = fRadious * sin(rad);

		cid+= 1.0f;

	}



	m_pArrayHChar[10].m_strName = L"Enter";
	m_pArrayHChar[32].m_strName = L"Space";



	m_mapChar.clear();
	m_sortVec.clear();
	m_nNumChar = 0;

}

int CCharData::GetTotalNumber()
{
	return m_nNumChar;
}

int CCharData::GetRealNumber()
{
	return (int)m_mapChar.size();
}

void CCharData::LoadBookData(CString strFile)
{
// 	if(pView->IsShowLogDlg()==false)
// 	{
// 		pView->ActivateLogDialog();
// 	}

	InitCharData();

	USES_CONVERSION;
	wchar_t* sz = (strFile.GetBuffer());

	//USES_CONVERSION;
	//char* sz = T2A(strFile);
	//FILE* fp = fopen(sz, "r");
	FILE* fp = _wfopen(sz, L"rt,ccs=UTF-8");


	if(fp)
	{
		WCHAR rbuf[10240];		
		memset(rbuf, '\0', sizeof(WCHAR)*10240);
		int pageNum=0;
		int lineCnt=0;
		m_nNumChar = 0;
		m_charAddCnt=0;
		m_totlaHangulCnt = 0;
		m_hangulRatio = 0;
		m_totalCnt = 0;

		fseek(fp, 0, SEEK_SET);
		while( fgetws(rbuf, sizeof(WCHAR)*10240, fp) != NULL )
		{
		//	wchar_t* readBuf = CharToWChar(rbuf);
			PreProcessHangulChar(rbuf, pageNum);

			//if(rbuf !=0)
			//{
			//	delete [] rbuf;
			//}
			//
			lineCnt++;
			if(lineCnt>100)
			{
				pageNum++;
				lineCnt = 0;
			}				
		}

		// 처음으로 되돌림 //
		memset(rbuf, '\0', sizeof(WCHAR)*10240);
		if(m_nNumChar>0)
		{	
			fseek(fp, 0, SEEK_SET);
			m_pAllChar = new unsigned short[m_nNumChar];
			m_pAllCharInv = new unsigned short[m_nNumChar];
			while( fgetws(rbuf, sizeof(WCHAR)*10240, fp) != NULL )
			{			
				RestoreAllChar(rbuf);		
			}
		}

		fclose(fp);
		MakeStatisticInfo();
	}	
	// Test here //
	// testFunc();


//	strInfo.Format(L"Number of letter : Total: %d   -   Used: %d   -   Korean (%d%s)", m_pCharData->GetTotalNumber(), m_pCharData->GetRealNumber(), m_pCharData->m_hangulRatio, strPercent);
	m_hangulRatio = ((float)(m_totlaHangulCnt) / (float)(m_totalCnt))*100;



	CString strPercent = L"%";
	CString strLog;

	int iPos = strFile.ReverseFind('\\');

	CString strFileName = strFile.Right(strFile.GetLength() - iPos -1);



	int realNum = (int)m_mapChar.size();
// 	strLog.Format(L"%s		%d	%d	%d%s",strFileName,  m_nNumChar, realNum, m_hangulRatio, strPercent);
// 	pView->AddLogString(strLog);
}

void CCharData::SetData(WCHAR* pData, int _nSize)
{
	InitCharData();

	int pageNum=0;
	int lineCnt=0;
	m_nNumChar = 0;
	m_charAddCnt=0;
	m_totlaHangulCnt = 0;
	m_hangulRatio = 0;
	m_totalCnt = 0;

	PreProcessHangulChar(pData, _nSize);
	lineCnt++;
	if(lineCnt>100)
	{
		pageNum++;
		lineCnt = 0;
	}


	m_pAllChar = new unsigned short[m_nNumChar];
	m_pAllCharInv = new unsigned short[m_nNumChar];
	RestoreAllChar(pData);		

	MakeStatisticInfo();



}

void CCharData::RestoreAllChar(wchar_t* _cSrc)
{
	int nLen = wcslen(_cSrc);
	for(int i=0; i<nLen; i++)
	{
		unsigned short nCode = _cSrc[i];
		if((nCode==10)||(nCode==13))
		{
			continue;
		}

		m_pAllChar[m_charAddCnt] = nCode;
		m_charAddCnt++;

		if((nCode > 0xAC00)&&(nCode < 0xD7A3))
			m_totlaHangulCnt++;

		if((nCode > 20000)&&(nCode < 0xD7A3))
			m_totalCnt++;

	}

	
}


void CCharData::PreProcessHangulChar(wchar_t* _cSrc, int pageNum)
{
	int nLen = wcslen(_cSrc);

//	CString strWord;
//	strWord = L"";
	wchar_t cWord[100];
	int offset=0;
	m_nNumChar = 0;
	memset(cWord, 0x00, sizeof(cWord));
	for(int i=0; i<nLen; i++)
	{
		unsigned long nCode = _cSrc[i];	
		if((nCode==10)||(nCode==13))
		{
			continue;
		}
		
		m_pArrayHChar[nCode].SetColor(nCode, _cSrc[i]);
		m_pArrayHChar[nCode].m_nCntRef++;

		if(i>0)
		{
			unsigned long nPreCode = _cSrc[i-1];
			m_pArrayHChar[nCode].AddPriviousCode(nPreCode);
		}
		if(i<(nLen-1))
		{
			unsigned long nNextCode = _cSrc[i+1];
			m_pArrayHChar[nCode].AddNextCode(nNextCode);
		}

#if 0
		// Extract Word ============================//
 		if((nCode == 32) || (nCode == 10) || (offset>10))
 		{
 		//	strWord = L"";
 			unsigned long hCode = getHashCode(cWord,offset);
 
 			if(m_mapHWord.find(hCode)==m_mapHWord.end())
 			{
 				_HANWORD hWord;
 				memset(hWord.seqCnt, 0, sizeof(hWord.seqCnt));
 				hWord.cnt=1;
 				hWord.strName = cWord;
 				hWord.seqCnt[pageNum]=1;
 				hWord.nCode = nCode;
 				m_mapHWord[hCode] = hWord;
 			}
 			else
 			{
 				m_mapHWord[hCode].cnt++;
 				m_mapHWord[hCode].seqCnt[pageNum]++;
 			}			
 
 			memset(cWord, 0x00, sizeof(cWord));			
 			offset = 0;			
 		}
 		else
 		{
 		//	strWord += _cSrc[i];
 		//	getHashCode(strWord, 1);
 			wcsncpy(cWord+offset, &_cSrc[i], 1);
 			offset++;				
 		}
		//============================================//
#endif
		m_nNumChar++;
	}


	
}

wchar_t* CCharData::CharToWChar(const char* pstrSrc)
{
	int nLen = strlen(pstrSrc)+1;	
	wchar_t* UniCode = new wchar_t[nLen];
	memset( UniCode, 0, sizeof( UniCode ) );

	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pstrSrc, -1, UniCode, nLen );


	//ASSERT(pstrSrc);
	//int nLen = strlen(pstrSrc)+1;

	//wchar_t* pwstr      = (LPWSTR) malloc ( sizeof( wchar_t )* nLen);
	//mbstowcs(pwstr, pstrSrc, nLen);

	return UniCode;
}


void CCharData::MakeStatisticInfo()
{

	m_maxCnt = 0;
	for(int i=0; i<65536; i++)
	{
		if(m_pArrayHChar[i].m_nCntRef>0)
		{			
			m_pArrayHChar[i].UpdatePositon();
			m_mapChar[i] = &m_pArrayHChar[i];

			unsigned long code = i;
			unsigned long cnt = m_pArrayHChar[i].m_nCntRef;
			m_sortVec.push_back(std::pair<unsigned long, unsigned long>(code, cnt));

			if((code>100) && (cnt>m_maxCnt))
			{
				m_maxCnt = cnt;
			}
		}		
	}


	std::sort(m_sortVec.begin(), m_sortVec.end(),  compare_pair_second<std::greater>());

	int vSize = m_sortVec.size();
	if(vSize>20)
	{
		m_nTop20Value = m_sortVec[20].second*100/m_maxCnt;
	}
	else
	{
		m_nTop20Value = m_sortVec[vSize-1].second*100/m_maxCnt;
	}

	
	// 카운트를 백분율로 표시 //
	std::map<unsigned long, unsigned long>::iterator itCode;
	for(int i=0; i<65536; i++)
	{
		if(m_pArrayHChar[i].m_nCntRef>0)
		{			
			m_pArrayHChar[i].m_nCntRef = m_pArrayHChar[i].m_nCntRef*100/m_maxCnt;

			for(itCode = m_pArrayHChar[i].m_mapPreCode.begin(); itCode != m_pArrayHChar[i].m_mapPreCode.end(); itCode++)
			{
				itCode->second = (itCode->second*100 / m_maxCnt)+1;
			}

			for(itCode = m_pArrayHChar[i].m_mapNextCode.begin(); itCode != m_pArrayHChar[i].m_mapNextCode.end(); itCode++)
			{
				itCode->second = (itCode->second*100 / m_maxCnt)+1;
			}
		}		
	}



#if 0


	// Sort HWord //
	std::map<unsigned long, _HANWORD>::iterator it;
	it = m_mapHWord.begin();
	for(;it!=m_mapHWord.end(); it++)
	{
		unsigned long code = it->first;
		unsigned long cnt = it->second.cnt;
		m_sortHWord.push_back(std::pair<unsigned long, unsigned long>(code, cnt));
	}

	std::sort(m_sortHWord.begin(), m_sortHWord.end(),  compare_pair_second<std::greater>());
	vSize = m_sortHWord.size();
	if(vSize>20)
	{
		m_nTop20Word = m_sortHWord[20].second;
	}
	else
	{
		m_nTop20Word = m_sortHWord[vSize-1].second;
	}


	//Get Draw Word Info===============//
	int vecNum = m_sortHWord.size();
	if(vecNum>20)
		vecNum = 20;

	for(int i=0; i<vecNum; i++)
	{
		unsigned long nCode = m_sortHWord[i].first;
		if(m_mapHWord.find(nCode) != m_mapHWord.end())
		{
			_HANWORD pWord =  m_mapHWord[nCode];	
			if(pWord.strName != L"")
			{
				m_vecTopHWord.push_back(pWord);
			}			
		}	
	}
	//====================================//

#endif

	// All Character Inverse //
	for(int i=0; i<m_charAddCnt; i++)
	{
		m_pAllCharInv[i] = m_pAllChar[m_charAddCnt-(i+1)];
	}






}

void CCharData::DrawLink()
{
	std::map<unsigned long, CHangulChar*>::iterator it;
	it = m_mapChar.begin();
	for(;it!=m_mapChar.end(); it++)
	{
		CHangulChar* pHChar = it->second;
	//	if(pHChar->m_nCode > 40000)
		drawForwordLink(pHChar);
	}
}

void CCharData::DrawWordCntInfo()
{
	glBegin(GL_POINTS);
	for(int i=0; i<m_vecTopHWord.size(); i++)
	{
		_HANWORD hWord = m_vecTopHWord[i];
		for(int j=0; j<500; j++)
		{
			float fAlpha = hWord.seqCnt[j]*0.1f;
			glColor4f(0.0f, 1.0f, 0.0f, fAlpha);
			glVertex3f(j, -i, 0);
		}			
	}
	glEnd();
}

void CCharData::drawForwordLink(CHangulChar* pChar)
{
	//if(pChar->m_nCode < 100)
	//	return;

	int drawcnt = 0;
	std::map<unsigned long, unsigned long>::iterator it = pChar->m_mapPreCode.begin();

	glBegin(GL_LINES);
	for(;it!=pChar->m_mapPreCode.end(); it++)
	{
		unsigned long nCode = it->first;
		//if((nCode>100))
		//{

			unsigned long nCnt = it->second;
			float aValue = (float)nCnt*0.02f;

			CHangulChar* pNext = &m_pArrayHChar[nCode];
			glColor4f(pChar->m_vColor.x, pChar->m_vColor.y, pChar->m_vColor.z, aValue);
			glVertex3f(pChar->m_vCirclePos.x, pChar->m_vCirclePos.y, 0.0f);
			glColor4f(pNext->m_vColor.x, pNext->m_vColor.y, pNext->m_vColor.z, aValue);
			glVertex3f(pNext->m_vCirclePos.x, pNext->m_vCirclePos.y, 0.0f);
//		}


		drawcnt++;
		if(drawcnt>50)
			break;
	}
	glEnd();
	

}

unsigned int CCharData::getHashCode(wchar_t* str, unsigned len)
{
	unsigned int hash = 5381;
	unsigned int i    = 0;

	for(i = 0; i < len; str++, i++)
	{
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;

}


void CCharData::FindTreeByChar(unsigned short nCode, int nTreeLevel)
{
	m_nMaxTreeLevel = nTreeLevel;

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


	_CHAR_TREE* tmp = new _CHAR_TREE;
	tmp->nLevel = 0;
	tmp->nCode = nCode;
	tmp->pParents = 0;
	tmp->nCnt = m_pArrayHChar[nCode].m_nCntRef*m_maxCnt/100.0f;
	tmp->fWeight = 1.0f;
	tmp->fAnglePos = 0.0f;
	tmp->fAngle3D= 0.0f;
	tmp->nChildCnt=1;
	mtSetPoint3D(&tmp->vTargetPos, 0,nTreeLevel*15,0);
	mtSetPoint3D(&tmp->vPos, 0,nTreeLevel*15,0);
	mtSetPoint3D(&tmp->vNodeDir, 0,1,0);
	m_forwardTree.push_back(tmp);


 	_CHAR_TREE* tmpb = new _CHAR_TREE;
 	tmpb->nLevel = 0;
 	tmpb->nCode = nCode;
 	tmpb->pParents = 0;
 	tmpb->nCnt = m_pArrayHChar[nCode].m_nCntRef*m_maxCnt/100.0f;
	tmpb->fWeight = 1.0f;
	tmpb->fAnglePos = 0.0f;
	tmpb->fAngle3D= 0.0f;
 	tmpb->nChildCnt = 0;
 	mtSetPoint3D(&tmpb->vTargetPos, 0,-0,0);
	mtSetPoint3D(&tmpb->vPos, 0,-0,0);
	mtSetPoint3D(&tmpb->vNodeDir, 0,-1,0);
 	m_backwardTree.push_back(tmpb);



	int nextLevel = tmp->nLevel+1;

// 	CString strChar = m_pArrayHChar[tmp->nCode].m_strName;
// 	TRACE(L"AddChild %s (%d)\n", strChar, 0);

	m_forwardTreeDepth=0;
	FindChildRecursive(tmp,tmp, nextLevel, m_forwardTree, m_pAllChar, m_forwardTreeDepth);
	m_backwardTreeDepth=0;
	FindChildRecursive(tmp,tmpb, nextLevel, m_backwardTree, m_pAllCharInv, m_backwardTreeDepth);
	//	RemoveDuplicate();


	//CString strChar;
	//for(int i=0; i<m_forwardTree.size(); i++){
	//	int nCode = m_forwardTree[i]->nCode;
	//	int nLevel = m_forwardTree[i]->nLevel;
	//	int nCnt = m_forwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Forward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	//}
	//for(int i=0; i<m_backwardTree.size(); i++){
	//	int nCode = m_backwardTree[i]->nCode;
	//	int nLevel = m_backwardTree[i]->nLevel;
	//	int nCnt = m_backwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Backward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);
	//}


//	MakeDummyChild(m_forwardTree);
//	MakeDummyChild(m_backwardTree);


//	POINT3D vDir;
//	mtSetPoint3D(&vDir, 0,1,0);
//	SetTreePosition(m_forwardTree, -60.0f, 120.0f, m_forwardTreeDepth, m_fFLevelSize, 30);
//	mtSetPoint3D(&vDir, 0,-1,0);
//	SetTreePosition(m_backwardTree, 105.0f, 150.0f, m_backwardTreeDepth, m_fBLevelSize, 30);


//	ApplyWeightPosition(m_forwardTree, 70);
//	ApplyWeightPosition(m_backwardTree, 70);



	float fPI = 3.141529f;
	FindNodeWeight(m_forwardTree);
	FineTreePosition(m_forwardTree, 50, 1,m_forwardTree[0], 0.2f);

	FindNodeWeight(m_backwardTree);
	FineTreePosition(m_backwardTree, 30, 1,m_backwardTree[0], -0.1f);



}

void CCharData::ApplyWeightPosition(std::vector<_CHAR_TREE*> &vecTree, float fLevelHeight)
{
	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		float fLevelCoff = 1.0f  - ((float)level / (float)m_nMaxTreeLevel)*0.3f;
//		float fLevelCoff = 1.0f;
		float fWeightCoff =  (float)m_nLastNodeCnt / (float)vecTree.size();
		float fAngleDiff = cos(vecTree[i]->fAnglePos);
		if(fAngleDiff<0)
			fAngleDiff *= -1.0f;
		
//		float fLength = fLevelHeight*fLevelCoff + vecTree[i]->fWeight*fWeightCoff;
		float fGravityCoff = 0.0f;
		float fLength = 0;
		_CHAR_TREE * pNode=vecTree[i];
		for(int nLevel=level; nLevel>0; nLevel--)
		{
		//	fLength += (fLevelHeight*fLevelCoff + pNode->fWeight*fWeightCoff*2.0f);
		//	fLength += (fLevelHeight);
			pNode=pNode->pParents;

			fGravityCoff += fLength*fAngleDiff*(1.0f-fLevelCoff);
		}

		fLength = (fLevelHeight);

		if(level >0)
		{
				//vecTree[i]->vTargetPos.x = vecTree[i]->pParents->vTargetPos.x + vecTree[i]->vNodeDir.x*fLength;
				//vecTree[i]->vTargetPos.y = vecTree[i]->pParents->vTargetPos.y + vecTree[i]->vNodeDir.y*fLength;
				//vecTree[i]->vTargetPos.z = 0.0f;

			vecTree[i]->vTargetPos.x = vecTree[i]->vPos.x + vecTree[i]->vNodeDir.x*fLength;
			vecTree[i]->vTargetPos.y = vecTree[i]->vPos.y + vecTree[i]->vNodeDir.y*fLength;
			vecTree[i]->vTargetPos.z = 0.0f;

			vecTree[i]->vPos =vecTree[i]->vTargetPos;

		}
	}

}

void CCharData::MakeDummyChild(std::vector<_CHAR_TREE*> &vecTree)
{
	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		if(level<=m_nMaxTreeLevel)
		{
			if(vecTree[i]->pParents)
			{
				vecTree[i]->pParents->nChildCnt++;
			}
		}
	}

	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		if((level > 0)&&(level<m_nMaxTreeLevel))
		{
			if(vecTree[i]->nChildCnt==0)
			{
				
				_CHAR_TREE* ppTree = vecTree[i];
				for(int j=vecTree[i]->nLevel; j<m_nMaxTreeLevel; j++)
				{
					_CHAR_TREE* tmpb = new _CHAR_TREE;
					tmpb->nLevel = j+1;
					tmpb->nCode = 65535;		// DUMMY
					//	tmpb->nPCode = 0;
					tmpb->pParents = ppTree;
					tmpb->nCnt = 1;	
					tmpb->nChildCnt = 1;
					if(tmpb->nLevel==m_nMaxTreeLevel)
					{
						tmpb->nChildCnt=0;
					}
					mtSetPoint3D(&tmpb->vTargetPos, 0,-50,0);
					vecTree.push_back(tmpb);		

					ppTree ->nChildCnt=1;
					ppTree = tmpb;
					
				}
			}
		}
	}

}

void CCharData::FindNodeWeight(std::vector<_CHAR_TREE*> &vecTree)
{
	_CHAR_TREE* pNode = NULL;
	_CHAR_TREE* ppNode = NULL;
	for(int i=0; i<vecTree.size(); i++)
	{
		pNode = vecTree[i];
		if(pNode->pParents!=NULL)
		{
			ppNode = pNode->pParents;
			while(ppNode != NULL)
			{	
				ppNode->fWeight+=1.0f;
				ppNode = ppNode->pParents;
			}
		}
	}
}

POINT3D CCharData::RoteteVecOnAxis(POINT3D _vLocalO, POINT3D _vInput, float _fAngle, float _x, float _y, float _z)
{
	_vInput.x	-= _vLocalO.x;
	_vInput.y	-= _vLocalO.y;
	_vInput.z   -= _vLocalO.z;


	GLdouble m_mat[16];
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();

//	glTranslatef(-_vLocalO.x, -_vLocalO.y, -_vLocalO.z);
	
	glRotatef(_fAngle, _x, _y, _z);

//	glTranslatef(_vLocalO.x, _vLocalO.y, _vLocalO.z);	
	
	//glRotatef(_fAngle, 0.0f, 0.0f, _z);
	//glRotatef(_fAngle, 0.0f, _y, 0.0f);
	//glRotatef(_fAngle, _x, 0.0f, 0.0f);
	
	
	glGetDoublev(GL_MODELVIEW_MATRIX, m_mat);
	glPopMatrix();

	double inVec[4];
	double outVec[4];

	inVec[0] = _vInput.x;	inVec[1] = _vInput.y;	inVec[2] = _vInput.z;	inVec[3] = 0;
	outVec[0] = 0;			outVec[1] = 0;			outVec[2] = 0;			outVec[3] = 0;

	mtMultMatrixVecf(m_mat, inVec, outVec);
	
	outVec[0]	+= _vLocalO.x;
	outVec[1]	+= _vLocalO.y;
	outVec[2] += _vLocalO.z;

	POINT3D vRes;
	mtSetPoint3D(&vRes, (float)outVec[0], (float)outVec[1], (float)outVec[2]);
	return vRes;

}



void CCharData::FineTreePosition(std::vector<_CHAR_TREE*> &vecTree, float fLevelLength, int nIndex, _CHAR_TREE* ppNode, float fGravity)
{	
	int nTotalCnt = vecTree.size();
	_CHAR_TREE* pNode=NULL;
	std::vector<_CHAR_TREE*> tmpNode;

	float fBaseAngle = ppNode->fAnglePos;
	if(fBaseAngle==0.0f)
		fBaseAngle+=0.05f;
	float fAngleDir = 2.0f / (float)m_nMaxTreeLevel;
	//if(fBaseAngle>0)
	fAngleDir = 1.0f;

	POINT3D vBaseVec, vTmpVec, vPependiVec;
	mtSetPoint3D(&vBaseVec, 0,0,1);

	for(int i=nIndex; i<nTotalCnt; i++)
	{
		pNode = vecTree[i];
		if(pNode->pParents == ppNode)
		{
			pNode->nIndex = i;
			tmpNode.push_back(pNode);
		}	
	}

	int cnt = tmpNode.size();
	if(cnt==0)
	{
		ppNode->nChildCnt = 0;
		return;
	}

	if(cnt==1)		// It has one branch.
	{
		tmpNode[0]->vPos.x = ppNode->vPos.x + ppNode->vNodeDir.x*fLevelLength;
		tmpNode[0]->vPos.y = ppNode->vPos.y + ppNode->vNodeDir.y*fLevelLength;
		tmpNode[0]->vPos.z = ppNode->vPos.z + ppNode->vNodeDir.z*fLevelLength;
		tmpNode[0]->vPos.y -= fLevelLength*fGravity;

		// Update Node Diretion //
		tmpNode[0]->vNodeDir = tmpNode[0]->vPos - ppNode->vPos;
		tmpNode[0]->vNodeDir = mtNormalize(tmpNode[0]->vNodeDir);



		//float fWeightCoff = tmpNode[0]->fWeight / (float)nTotalCnt;
		//float fLength = fLevelLength + (fLevelLength*fWeightCoff*fWl);
		//tmpNode[0]->fAnglePos = (fBaseAngle + (fBaseAngle*fWa));

		//tmpNode[0]->fAngle3D =0.0f;

		//tmpNode[0]->vPos.x = ppNode->vPos.x + sin(tmpNode[0]->fAnglePos)*fLength;
		//tmpNode[0]->vPos.y = ppNode->vPos.y + cos(tmpNode[0]->fAnglePos)*fLength;
		//tmpNode[0]->vPos.z = 0.0f;

		//tmpNode[0]->vNodeDir = tmpNode[0]->vPos - ppNode->vPos;
		//tmpNode[0]->vNodeDir = mtNormalize(tmpNode[0]->vNodeDir);

		// Z-Positioning //		
// 		tmpNode[0]->vPos = RoteteVecOnAxis(ppNode->vPos, tmpNode[0]->vPos, tmpNode[0]->fAngle3D, ppNode->vNodeDir.x, ppNode->vNodeDir.y, ppNode->vNodeDir.z);
// 
// 		// Update Node Diretion //
// 		tmpNode[0]->vNodeDir = tmpNode[0]->vPos - ppNode->vPos;
// 		tmpNode[0]->vNodeDir = mtNormalize(tmpNode[0]->vNodeDir);
		


		FineTreePosition(vecTree, fLevelLength*0.9f, tmpNode[0]->nIndex, tmpNode[0], fGravity);
		
	} 

	else		// It has many branches.
	{		
		vTmpVec.x = ppNode->vPos.x + ppNode->vNodeDir.x*fLevelLength;
		vTmpVec.y = ppNode->vPos.y + ppNode->vNodeDir.y*fLevelLength;
		vTmpVec.z = ppNode->vPos.z + ppNode->vNodeDir.z*fLevelLength;

		vPependiVec = mtCross(vBaseVec, ppNode->vNodeDir);
		vPependiVec = mtNormalize(vPependiVec);

		vTmpVec.x = vTmpVec.x + vPependiVec.x*fLevelLength*0.5f;
		vTmpVec.y = vTmpVec.y + vPependiVec.y*fLevelLength*0.5f;
		vTmpVec.z = vTmpVec.z + vPependiVec.z*fLevelLength*0.5f;

		float fAngleStep = (360.0f) / (float)(cnt);
		float fAnglePos3D = 0.0f;

		for(int i=0; i<tmpNode.size(); i++)
		{
			fAnglePos3D = (fAngleStep*i);
			tmpNode[i]->vPos = RoteteVecOnAxis(ppNode->vPos, vTmpVec, fAnglePos3D, ppNode->vNodeDir.x, ppNode->vNodeDir.y, ppNode->vNodeDir.z);
			tmpNode[i]->vPos.y -= fLevelLength*fGravity;


			// Update Node Diretion //
			tmpNode[i]->vNodeDir = tmpNode[i]->vPos - ppNode->vPos;
			tmpNode[i]->vNodeDir = mtNormalize(tmpNode[i]->vNodeDir);
		
			



			float fWeightCoff = tmpNode[i]->fWeight / (float)nTotalCnt;
			//float fAngleStep = (fAngleRange) / (float)(cnt-1);
			//float fAnglePos = ((fBaseAngle - fAngleRange*0.5f + fBaseAngle*fWeightCoff*fAngleDir) + fAngleStep*i);
			//float fLength = fLevelLength + (fLevelLength*fWeightCoff*2.0f);

			//float fAngleStep3D = (float)(360)/ (float)(cnt-1);			// 3D Angle for z-positioning //
			//float fAnglePos3D = (fAngleStep3D*i);


			//tmpNode[i]->fAnglePos = fAnglePos;
			//tmpNode[i]->fAngle3D = fAnglePos3D;

			//tmpNode[i]->vPos.x = ppNode->vPos.x + sin(tmpNode[i]->fAnglePos)*fLength;
			//tmpNode[i]->vPos.y = ppNode->vPos.y + cos(tmpNode[i]->fAnglePos)*fLength;
			//tmpNode[i]->vPos.z = 0.0f;

			//tmpNode[0]->vNodeDir = tmpNode[0]->vPos - ppNode->vPos;
			//tmpNode[0]->vNodeDir = mtNormalize(tmpNode[0]->vNodeDir);

// 			// Z-Positioning //		
// 			tmpNode[0]->vPos = RoteteVecOnAxis(ppNode->vPos, tmpNode[0]->vPos, tmpNode[0]->fAngle3D, ppNode->vNodeDir.x, ppNode->vNodeDir.y, ppNode->vNodeDir.z);
// 
// 			// Update Node Diretion //
// 			tmpNode[0]->vNodeDir = tmpNode[0]->vPos - ppNode->vPos;
// 			tmpNode[0]->vNodeDir = mtNormalize(tmpNode[0]->vNodeDir);

			FineTreePosition(vecTree, fLevelLength*0.9f, tmpNode[i]->nIndex, tmpNode[i], fGravity);
		}
		
	}


	tmpNode.clear();


		//float fLength = level*fLevelHeight;
		//vecTree[i]->vPos = vecTree[0]->vTargetPos;
		//vecTree[i]->nChildCnt = 0;
		//vecTree[i]->fWeight = 0.0f;
		//vecTree[i]->fAnglePos = 0.0f;
		//if(level==nNodeLevel)
		//{
		//	nLastNodeCnt++;
		//}
		//vBasePos = vecTree[0]->vTargetPos;
	

}

void CCharData::SetTreePosition(std::vector<_CHAR_TREE*> &vecTree, float fStartAngle, float fAngleRange, int &maxLevel, float* levelLength, float fLevelHeight)
{

	// initialize node position ===================//
	int nNodeLevel = m_nMaxTreeLevel;
	int nLastNodeCnt=0;
	POINT3D vBasePos;
	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		float fLength = level*fLevelHeight;
		vecTree[i]->vPos = vecTree[0]->vTargetPos;
		vecTree[i]->nChildCnt = 0;
		vecTree[i]->fWeight = 0.0f;
		vecTree[i]->fAnglePos = 0.0f;
		vecTree[i]->fAngle3D = 0.0f;
		if(level==nNodeLevel)
		{
			nLastNodeCnt++;
		}
		vBasePos = vecTree[0]->vTargetPos;
	}

	m_nLastNodeCnt = nLastNodeCnt;
	//==================================//

	// Define the last node's position ==============//
	float fPI = 3.141529f;
	float fAngleStep = (fAngleRange *fPI/180.0f)/ (float)nLastNodeCnt;	
	float fPosAngle = (fStartAngle*fPI/180.0f);

	float fAngleStep3D = (360 *fPI/180.0f)/ (float)nLastNodeCnt;			// 3D Angle for z-positioning //
	float fPosAngle3D=0.0f;
	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		if(level == nNodeLevel)
		{
			float fLength = level*fLevelHeight;
			vecTree[i]->vNodeDir.x = sin(fPosAngle);
			vecTree[i]->vNodeDir.y = cos(fPosAngle);
			vecTree[i]->vNodeDir.z = 0.0f;


			vecTree[i]->vPos = vBasePos;


			vecTree[i]->vTargetPos.x = vBasePos.x + vecTree[i]->vNodeDir.x*fLength;
			vecTree[i]->vTargetPos.y = vBasePos.y + vecTree[i]->vNodeDir.y*fLength;
			vecTree[i]->vTargetPos.z = 0.0f;
			vecTree[i]->fAnglePos = fPosAngle;
			vecTree[i]->fAngle3D = fPosAngle3D;
			vecTree[i]->fWeight = 1.0f;

			vecTree[i]->vPos = vecTree[i]->vTargetPos;

			fPosAngle += fAngleStep;
			fPosAngle3D += fAngleStep3D;
		}
	}


	if(FindParentPosRCV(vecTree, nNodeLevel, fLevelHeight, vBasePos)==false)
	{
		return;
	}

	return;




	/*
	for(int level=1; level<maxLevel; level++)
	{
		float fLevelSize=0;
		if(level<19)
		{
			fLevelSize = levelLength[level];
		}
		else
		{
			fLevelSize = levelLength[19];
		}

		int levelCnt = 0;
		for(int i=0; i<vecTree.size(); i++)
		{
			if(level==vecTree[i]->nLevel)
				levelCnt++;			
		}

		if(levelCnt>0)
		{
			float levelStep = fLevelSize/levelCnt;	
			float levelHeight = fLevelHeight * (level+2);
			float xPos = -fLevelSize*0.5f;

			for(int i=0; i<vecTree.size(); i++)
			{
				if(level==vecTree[i]->nLevel)
				{
					vecTree[i]->vTargetPos.x = xPos;
					vecTree[i]->vTargetPos.y = levelHeight*vDir.y;
					vecTree[i]->vTargetPos.z = 0.0f;
					xPos+=levelStep;
				}
			}

		}			
	}
	*/

}

bool CCharData::FindParentPosRCV(std::vector<_CHAR_TREE*> &vecTree, int nChildLevel, float fLevelHeight, POINT3D vBasePos)
{
	int nParentsLevel = nChildLevel-1;
	if(nParentsLevel==0)
		return false;

	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		if(level == nChildLevel)
		{
			vecTree[i]->pParents->fAnglePos += vecTree[i]->fAnglePos;
			vecTree[i]->pParents->fWeight+= vecTree[i]->fWeight;
			vecTree[i]->pParents->nChildCnt++;
		}
	}

	// Find Parents Position============= //
	float fPI = 3.141529f;
	for(int i=0; i<vecTree.size(); i++)
	{
		int level = vecTree[i]->nLevel;
		float fLength = level*fLevelHeight;
		if(level == nParentsLevel)
		{
			if(vecTree[i]->nChildCnt>0)
			{
				vecTree[i]->fAnglePos = vecTree[i]->fAnglePos / (float)vecTree[i]->nChildCnt;
				vecTree[i]->vNodeDir.x = sin(vecTree[i]->fAnglePos);
				vecTree[i]->vNodeDir.y = cos(vecTree[i]->fAnglePos);
				vecTree[i]->vNodeDir.z = 0.0f;

				vecTree[i]->vPos = vBasePos;

 				vecTree[i]->vTargetPos.x = vBasePos.x + vecTree[i]->vNodeDir.x*fLength;
 				vecTree[i]->vTargetPos.y = vBasePos.y + vecTree[i]->vNodeDir.y*fLength;
 				vecTree[i]->vTargetPos.z = 0.0f;
				vecTree[i]->fWeight += 1.0f;

				vecTree[i]->vPos = vecTree[i]->vTargetPos;
			}

			else
			{
				TRACE("ERROR\n");
			}



		}
	}

	FindParentPosRCV(vecTree, nParentsLevel, fLevelHeight, vBasePos);
}

void CCharData::testFunc()
{
	_CHAR_TREE* tmp = new _CHAR_TREE;

	tmp->nLevel = 0;
	tmp->nCode = m_sortVec[2].first;
//	tmp->pParents = 0;
//	tmp->nPCode = 0;
	tmp->nCnt = 1;
	m_forwardTree.push_back(tmp);

	int nextLevel = tmp->nLevel+1;

	CString strChar = m_pArrayHChar[tmp->nCode].m_strName;
	TRACE(L"AddChild %s (%d)\n", strChar, 0);

//	m_forwardTreeDepth=0;
//	FindChildRecursive(tmp,nextLevel, m_forwardTree, m_pAllChar);
//	FindChildRecursive(tmp,nextLevel, m_backwardTree, m_pAllCharInv);
//	RemoveDuplicate();


	
	for(int i=0; i<m_forwardTree.size(); i++)
	{
		int nCode = m_forwardTree[i]->nCode;
		int nLevel = m_forwardTree[i]->nLevel;
		int nCnt = m_forwardTree[i]->nCnt;
		strChar = m_pArrayHChar[nCode].m_strName;

		TRACE(L"Forward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	}


	for(int i=0; i<m_backwardTree.size(); i++)
	{
		int nCode = m_backwardTree[i]->nCode;
		int nLevel = m_backwardTree[i]->nLevel;
		int nCnt = m_backwardTree[i]->nCnt;
		strChar = m_pArrayHChar[nCode].m_strName;

		TRACE(L"Backward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	}
	int a=0;
}

void CCharData::RemoveDuplicate()
{

	//std::vector<_CHAR_TREE*> tmpTree;
	//for(int i=0; i<m_forwordTree.size(); i++)
	//{
	//	tmpTree.push_back(m_forwordTree[i]);
	//}	
	//m_forwordTree.clear();

	//_CHAR_TREE* preTree=tmpTree[0];
	//m_forwordTree.push_back(tmpTree[0]);


	//for(int i=0; i<m_forwardTreeDepth; i++)
	//{
	//	for(int j=1; j<tmpTree.size(); j++)
	//	{
	//		if(tmpTree[j]->nLevel == i)
	//		{
	//			if((preTree->nCode==tmpTree[j]->nCode))
	//			{
	//				// Dublicate
	//			}
	//			else
	//			{
	//				m_forwordTree.push_back(tmpTree[j]);
	//				preTree = tmpTree[j];
	//			}			
	//		}			
	//		
	//	}
	//}
	
}

bool CCharData::FindChildRecursive(_CHAR_TREE* pTree, _CHAR_TREE* pParents, unsigned int nLevel, std::vector<_CHAR_TREE*> &vecTree, unsigned short* pAllChar, int& maxLevel)
{
 	if(maxLevel<nLevel)
 		maxLevel = nLevel;

	if(nLevel>m_nMaxTreeLevel)
		return false;

	CString strChar;
	// Find Next Step Parents //
	int nextLevel = nLevel+1;
//	_CHAR_TREE* pParents = new _CHAR_TREE[nextLevel];
	for(int j=0; j<nLevel; j++)
	{
		m_treeBuffer[j] = pTree[j];
	}
	
	char pIsOk[200];

	for(int i=nLevel; i<m_charAddCnt; i++)
	{		
		memset(pIsOk, 0x00, sizeof(pIsOk));
		for(int j=0; j<nLevel; j++)
		{
			unsigned short a = pTree[j].nCode;
			CString strCharA = m_pArrayHChar[a].m_strName;
			unsigned short b = pAllChar[i-(nLevel-j)];
			strChar = m_pArrayHChar[b].m_strName;
			if(a==b)
			{
				pIsOk[j] = 1;				
			}
		}


		bool IsChild = true;
		for(int j=0; j<nLevel; j++)
		{
			if(pIsOk[j] == 0)
			{
				IsChild = false;
				break;
			}
		}

	
		if(IsChild)
		{
			if((pAllChar[i] != 10) && (nLevel <m_nMaxTreeLevel))
		//	if( nLevel < 20)
			{
				_CHAR_TREE *pChild = new _CHAR_TREE;
				pChild->nLevel = nLevel;
				pChild->nCode = pAllChar[i];
			//	pChild->nPCode = pTree[nLevel-1].nCode;		
				pChild->pParents = pParents;
				pChild->nCnt=m_pArrayHChar[pChild->nCode].m_nCntRef*m_maxCnt/100.0f;;	
				pChild->nChildCnt=1;
				pChild->fAnglePos = 0.0f;
				pChild->fWeight = 1.0f;
				

				if(CheckDuplication(pChild, vecTree)==false)
				{
					vecTree.push_back(pChild);
					m_treeBuffer[nextLevel-1] = pChild[0];

				//	strChar = m_pArrayHChar[pChild->nCode].m_strName;
				//	TRACE(L"AddChild %s (%d)\n", strChar, nLevel);

					FindChildRecursive(m_treeBuffer, pChild, nextLevel, vecTree, pAllChar, maxLevel);
				}	
				else
				{
					delete pChild;
				}
			}		
			else
			{
			//	break;
			}
		}
	}

//	delete [] pParents;
	return true;
}


bool CCharData::CheckDuplication(_CHAR_TREE*pTree,  std::vector<_CHAR_TREE*> &vecTree)
{
	//if(pTree->nCode==32)
	//	return false;

	for(int i=0; i<vecTree.size(); i++)
	{
		
		if((pTree->nCode==vecTree[i]->nCode) &&
		   (pTree->nLevel==vecTree[i]->nLevel)&& 
		   (pTree->pParents == vecTree[i]->pParents))
		{		
//			vecTree[i]->nCnt++;
			return true;
		}
	}	
	return false;
}

unsigned short CCharData::FindCodeByString(CString strName)
{
	std::map<unsigned long, CHangulChar*>::iterator it;
	it = m_mapChar.begin();
	for(;it!=m_mapChar.end(); it++)
	{
		CHangulChar* pHChar = it->second;
		if(strName==pHChar->m_strName)
		{
			return pHChar->m_nCode;
		}

	}

	return 0;
}

CString CCharData::FindStringByCode(unsigned short nCode)
{
	return m_pArrayHChar[nCode].m_strName;
}

void CCharData::FindTreeBySeedChar(unsigned short nCode1, unsigned short nCode2, int nTreeLevel)
{


	m_nMaxTreeLevel = nTreeLevel;

	for(int i=0; i<m_forwardTree.size(); i++)
	{
		delete m_forwardTree[i];
	}
// 	for(int i=0; i<m_backwardTree.size(); i++)
// 	{
// 		delete m_backwardTree[i];
// 	}
	m_forwardTree.clear();
//	m_backwardTree.clear();

	float fMainLength = 10.0f;

	_CHAR_TREE* tmp = new _CHAR_TREE;
	tmp->nLevel = 0;
	tmp->nCode = nCode1;
	tmp->pParents = 0;
	tmp->nCnt = m_pArrayHChar[nCode1].m_nCntRef*m_maxCnt/100.0f;
	tmp->fWeight = 1.0f;
	tmp->fAnglePos = 0.0f;
	tmp->fAngle3D= 0.0f;
	tmp->nChildCnt=1;
	mtSetPoint3D(&tmp->vTargetPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vNodeDir, 0,1,0);
	m_forwardTree.push_back(tmp);




  	_CHAR_TREE* tmp2 = new _CHAR_TREE;
  	tmp2->nLevel = 1;
  	tmp2->nCode = nCode2;
  	tmp2->pParents = tmp;
  	tmp2->nCnt = m_pArrayHChar[nCode2].m_nCntRef*m_maxCnt/100.0f;
  	tmp2->fWeight = 1.0f;
  	tmp2->fAnglePos = 0.0f;
  	tmp2->fAngle3D= 0.0f;
  	tmp2->nChildCnt=1;
  	mtSetPoint3D(&tmp2->vTargetPos, 0,nTreeLevel*fMainLength,0);
  	mtSetPoint3D(&tmp2->vPos, 0,nTreeLevel*fMainLength,0);
  	mtSetPoint3D(&tmp2->vNodeDir, 0,1,0);


	
	m_forwardTree.push_back(tmp2);
	m_treeBuffer[0] = tmp[0];
	m_treeBuffer[1] = tmp2[0];

  	

	int nextLevel = tmp2->nLevel+1;
	

// 	_CHAR_TREE* tmpb = new _CHAR_TREE;
// 	tmpb->nLevel = 0;
// 	tmpb->nCode = nCode;
// 	tmpb->pParents = 0;
// 	tmpb->nCnt = m_pArrayHChar[nCode].m_nCntRef*m_maxCnt/100.0f;
// 	tmpb->fWeight = 1.0f;
// 	tmpb->fAnglePos = 0.0f;
// 	tmpb->fAngle3D= 0.0f;
// 	tmpb->nChildCnt = 0;
// 	mtSetPoint3D(&tmpb->vTargetPos, 0,-0,0);
// 	mtSetPoint3D(&tmpb->vPos, 0,-0,0);
// 	mtSetPoint3D(&tmpb->vNodeDir, 0,-1,0);
// 	m_backwardTree.push_back(tmpb);



	
	// 	CString strChar = m_pArrayHChar[tmp->nCode].m_strName;
	// 	TRACE(L"AddChild %s (%d)\n", strChar, 0);

	m_forwardTreeDepth=0;
	FindChildRecursive(m_treeBuffer, tmp2, nextLevel, m_forwardTree, m_pAllChar, m_forwardTreeDepth);
// 	m_backwardTreeDepth=0;
// 	FindChildRecursive(tmp,tmpb, nextLevel, m_backwardTree, m_pAllCharInv, m_backwardTreeDepth);
	//	RemoveDuplicate();


	//CString strChar;
	//for(int i=0; i<m_forwardTree.size(); i++){
	//	int nCode = m_forwardTree[i]->nCode;
	//	int nLevel = m_forwardTree[i]->nLevel;
	//	int nCnt = m_forwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Forward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	//}
	//for(int i=0; i<m_backwardTree.size(); i++){
	//	int nCode = m_backwardTree[i]->nCode;
	//	int nLevel = m_backwardTree[i]->nLevel;
	//	int nCnt = m_backwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Backward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);
	//}


	//	MakeDummyChild(m_forwardTree);
	//	MakeDummyChild(m_backwardTree);


	//	POINT3D vDir;
	//	mtSetPoint3D(&vDir, 0,1,0);
	//	SetTreePosition(m_forwardTree, -60.0f, 120.0f, m_forwardTreeDepth, m_fFLevelSize, 30);
	//	mtSetPoint3D(&vDir, 0,-1,0);
	//	SetTreePosition(m_backwardTree, 105.0f, 150.0f, m_backwardTreeDepth, m_fBLevelSize, 30);


	//	ApplyWeightPosition(m_forwardTree, 70);
	//	ApplyWeightPosition(m_backwardTree, 70);



	float fPI = 3.141529f;
	FindNodeWeight(m_forwardTree);
	FineTreePosition(m_forwardTree, BRENCH_LENGTH, 1,m_forwardTree[0], 0.2f);

//	FindNodeWeight(m_backwardTree);
//	FineTreePosition(m_backwardTree, 30, 1,m_backwardTree[0], -0.1f);



}

void CCharData::FindTreeBySeedChar3(unsigned short nCode1, unsigned short nCode2, int nTreeLevel, std::vector<_CHAR_TREE*>&pforwardTree, std::vector<_CHAR_TREE*>&pBackwardTree)
{
	m_nMaxTreeLevel = nTreeLevel;

	for(int i=0; i<pforwardTree.size(); i++)
	{
		delete pforwardTree[i];
	}
	for(int i=0; i<pBackwardTree.size(); i++)
	{
		delete pBackwardTree[i];
	}
	pforwardTree.clear();
	pBackwardTree.clear();


	float fMainLength = 10.0f;

	_CHAR_TREE* tmp = new _CHAR_TREE;
	tmp->nLevel = 0;
	tmp->nCode = nCode1;
	tmp->pParents = 0;
	tmp->nCnt = m_pArrayHChar[nCode1].m_nCntRef*m_maxCnt/100.0f;
	tmp->fWeight = 1.0f;
	tmp->fAnglePos = 0.0f;
	tmp->fAngle3D= 0.0f;
	tmp->nChildCnt=1;
	mtSetPoint3D(&tmp->vTargetPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vNodeDir, 0,1,0);
	pforwardTree.push_back(tmp);



	int nextLevel = tmp->nLevel+1;


	_CHAR_TREE* tmpb = new _CHAR_TREE;
	tmpb->nLevel = 0;
	tmpb->nCode = nCode2;
	tmpb->pParents = 0;
	tmpb->nCnt = m_pArrayHChar[nCode2].m_nCntRef*m_maxCnt/100.0f;
	tmpb->fWeight = 1.0f;
	tmpb->fAnglePos = 0.0f;
	tmpb->fAngle3D= 0.0f;
	tmpb->nChildCnt = 0;
	mtSetPoint3D(&tmpb->vTargetPos, 0,-0,0);
	mtSetPoint3D(&tmpb->vPos, 0,-0,0);
	mtSetPoint3D(&tmpb->vNodeDir, 0,-1,0);
	pBackwardTree.push_back(tmpb);




	// 	CString strChar = m_pArrayHChar[tmp->nCode].m_strName;
	// 	TRACE(L"AddChild %s (%d)\n", strChar, 0);

	m_forwardTreeDepth=0;
	FindChildRecursive(tmp, tmp, nextLevel, pforwardTree, m_pAllChar, m_forwardTreeDepth);
	m_backwardTreeDepth=0;
	FindChildRecursive(tmpb,tmpb, nextLevel, pBackwardTree, m_pAllCharInv, m_backwardTreeDepth);
	//	RemoveDuplicate();


	//CString strChar;
	//for(int i=0; i<m_forwardTree.size(); i++){
	//	int nCode = m_forwardTree[i]->nCode;
	//	int nLevel = m_forwardTree[i]->nLevel;
	//	int nCnt = m_forwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Forward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	//}
	//for(int i=0; i<m_backwardTree.size(); i++){
	//	int nCode = m_backwardTree[i]->nCode;
	//	int nLevel = m_backwardTree[i]->nLevel;
	//	int nCnt = m_backwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Backward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);
	//}


	//	MakeDummyChild(m_forwardTree);
	//	MakeDummyChild(m_backwardTree);


	//	POINT3D vDir;
	//	mtSetPoint3D(&vDir, 0,1,0);
	//	SetTreePosition(m_forwardTree, -60.0f, 120.0f, m_forwardTreeDepth, m_fFLevelSize, 30);
	//	mtSetPoint3D(&vDir, 0,-1,0);
	//	SetTreePosition(m_backwardTree, 105.0f, 150.0f, m_backwardTreeDepth, m_fBLevelSize, 30);


	//	ApplyWeightPosition(m_forwardTree, 70);
	//	ApplyWeightPosition(m_backwardTree, 70);


	float fPI = 3.141529f;

	if(pforwardTree.size()>0)
	{
		FindNodeWeight(pforwardTree);
		FineTreePosition(pforwardTree, BRENCH_LENGTH, 1,pforwardTree[0], 0.2f);
	}

	if(pBackwardTree.size()>0)
	{	
		FindNodeWeight(pBackwardTree);
		FineTreePosition(pBackwardTree, BRENCH_LENGTH, 1,pBackwardTree[0], -0.1f);
	}


}

void CCharData::FindTreeBySeedChar2(unsigned short nCode1, unsigned short nCode2, int nTreeLevel)
{



	m_nMaxTreeLevel = nTreeLevel;

	for(int i=0; i<m_forwardTree.size(); i++)
	{
		delete m_forwardTree[i];
	}
	// 	for(int i=0; i<m_backwardTree.size(); i++)
	// 	{
	// 		delete m_backwardTree[i];
	// 	}
	m_forwardTree.clear();
	//	m_backwardTree.clear();


	float fMainLength = 10.0f;

	_CHAR_TREE* tmp = new _CHAR_TREE;
	tmp->nLevel = 0;
	tmp->nCode = nCode1;
	tmp->pParents = 0;
	tmp->nCnt = m_pArrayHChar[nCode1].m_nCntRef*m_maxCnt/100.0f;
	tmp->fWeight = 1.0f;
	tmp->fAnglePos = 0.0f;
	tmp->fAngle3D= 0.0f;
	tmp->nChildCnt=1;
	mtSetPoint3D(&tmp->vTargetPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vPos, 0,nTreeLevel*fMainLength,0);
	mtSetPoint3D(&tmp->vNodeDir, 0,1,0);
	m_forwardTree.push_back(tmp);



	int nextLevel = tmp->nLevel+1;


	// 	_CHAR_TREE* tmpb = new _CHAR_TREE;
	// 	tmpb->nLevel = 0;
	// 	tmpb->nCode = nCode;
	// 	tmpb->pParents = 0;
	// 	tmpb->nCnt = m_pArrayHChar[nCode].m_nCntRef*m_maxCnt/100.0f;
	// 	tmpb->fWeight = 1.0f;
	// 	tmpb->fAnglePos = 0.0f;
	// 	tmpb->fAngle3D= 0.0f;
	// 	tmpb->nChildCnt = 0;
	// 	mtSetPoint3D(&tmpb->vTargetPos, 0,-0,0);
	// 	mtSetPoint3D(&tmpb->vPos, 0,-0,0);
	// 	mtSetPoint3D(&tmpb->vNodeDir, 0,-1,0);
	// 	m_backwardTree.push_back(tmpb);




	// 	CString strChar = m_pArrayHChar[tmp->nCode].m_strName;
	// 	TRACE(L"AddChild %s (%d)\n", strChar, 0);

	m_forwardTreeDepth=0;
	FindChildRecursive(tmp, tmp, nextLevel, m_forwardTree, m_pAllChar, m_forwardTreeDepth);
	// 	m_backwardTreeDepth=0;
	// 	FindChildRecursive(tmp,tmpb, nextLevel, m_backwardTree, m_pAllCharInv, m_backwardTreeDepth);
	//	RemoveDuplicate();


	//CString strChar;
	//for(int i=0; i<m_forwardTree.size(); i++){
	//	int nCode = m_forwardTree[i]->nCode;
	//	int nLevel = m_forwardTree[i]->nLevel;
	//	int nCnt = m_forwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Forward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);

	//}
	//for(int i=0; i<m_backwardTree.size(); i++){
	//	int nCode = m_backwardTree[i]->nCode;
	//	int nLevel = m_backwardTree[i]->nLevel;
	//	int nCnt = m_backwardTree[i]->nCnt;
	//	strChar = m_pArrayHChar[nCode].m_strName;
	//	TRACE(L"Backward**************Final AddChild %s (%d) - %d\n", strChar, nLevel, nCnt);
	//}


	//	MakeDummyChild(m_forwardTree);
	//	MakeDummyChild(m_backwardTree);


	//	POINT3D vDir;
	//	mtSetPoint3D(&vDir, 0,1,0);
	//	SetTreePosition(m_forwardTree, -60.0f, 120.0f, m_forwardTreeDepth, m_fFLevelSize, 30);
	//	mtSetPoint3D(&vDir, 0,-1,0);
	//	SetTreePosition(m_backwardTree, 105.0f, 150.0f, m_backwardTreeDepth, m_fBLevelSize, 30);


	//	ApplyWeightPosition(m_forwardTree, 70);
	//	ApplyWeightPosition(m_backwardTree, 70);


	float fPI = 3.141529f;
	FindNodeWeight(m_forwardTree);
	FineTreePosition(m_forwardTree, BRENCH_LENGTH, 1,m_forwardTree[0], 0.2f);

	//	FindNodeWeight(m_backwardTree);
	//	FineTreePosition(m_backwardTree, 30, 1,m_backwardTree[0], -0.1f);



}
