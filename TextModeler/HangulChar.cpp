#include "StdAfx.h"
#include "HangulChar.h"

CHangulChar::CHangulChar(void)
{

	m_nCode			=0;	
	m_nCntRef		=0;
	m_nCntFirst		=0;
	m_nCntEnd		=0;

	m_vColor.x		=0.0f;
	m_vColor.y		=0.0f;
	m_vColor.z		=0.0f;

	m_vPos.x		=0.0f;
	m_vPos.y		=0.0f;
	m_vPos.z		=0.0f;

}

CHangulChar::~CHangulChar(void)
{
 	m_mapPreCode.clear();
 	m_mapNextCode.clear();
}

void CHangulChar::InitDataSet()
{
	m_nCode			=0;	
	m_nCntRef		=0;
	m_nCntFirst		=0;
	m_nCntEnd		=0;

	m_vColor.x		=1.0f;
	m_vColor.y		=1.0f;
	m_vColor.z		=1.0f;

	m_vPos.x		=0.0f;
	m_vPos.y		=0.0f;
	m_vPos.z		=0.0f;

	m_mapPreCode.clear();
	m_mapNextCode.clear();

}

POINT3D CHangulChar::GetColor(unsigned long nCode)
{
	unsigned long rgb565 = nCode;
	unsigned short R,G,B;
	R=G=B=0;

	BYTE red = (rgb565 & 0xF800) >> 11;
	BYTE green = (rgb565 & 0x7E0) >> 5;
	BYTE blue = (rgb565 & 0x1F);

	BYTE r = red << 3;
	BYTE g = green << 2;
	BYTE b = blue << 3;

	POINT3D vColor;
	vColor.x = (float)r/255.0f;
	vColor.y = (float)g/255.0f;
	vColor.z = (float)b/255.0f;

	m_vColor = vColor;

	return vColor;
}

void CHangulChar::SetColor(unsigned long _nCode, wchar_t strCode)
{


	if(_nCode > 32 )
	{		
		float fCode		= (float)_nCode*0.9765625f;
		//m_vColor.x		= (fCode/1600.0f) * 0.025f;
		//m_vColor.y		= ((float)((int)fCode % 1600)/40.0f)*0.025f;
		//m_vColor.z		= ((int)(fCode) % 40)*0.025f;

		//m_vColor = GetColor(_nCode);


		CString tmpStr(strCode);
		m_strName = tmpStr;
	//	m_strName = CString(strCode);

	}
	else
	{
		int a=0;
	}

	m_nCode = _nCode;
}

void CHangulChar::UpdatePositon()
{
	float fScale = 0.02f;
	m_vPos.y = (float)(m_nCntFirst)*fScale;
	m_vPos.z = -(float)(m_nCntEnd)*fScale;
//	m_vPos.x = m_nCntRef*0.2f;
//	m_vPos.y = m_nCntFirst;
	//m_vPos.y = log((double)m_nCntFirst)*fScale;
	//m_vPos.z = -log((double)m_nCntEnd)*fScale;
//	m_vPos.y -= m_nCntEnd*fScale;
}


void CHangulChar::AddPriviousCode(unsigned long nCode)
{
	std::map<unsigned long, unsigned long>::iterator it = m_mapPreCode.find(nCode);
	if(it == m_mapPreCode.end())
	{
		m_mapPreCode[nCode] = 1;
	}
	else
	{
		it->second++;
	}

	switch(nCode)
	{
	case 10:
		m_nCntFirst ++;
		break;
	case 32:  // 
	case 46:  // .
	case 44:  // ,
	case 63:  // ?
	case 33:  // !
		m_nCntFirst ++;
		break;
	}


}
void CHangulChar::AddNextCode(unsigned long nCode)
{
	std::map<unsigned long, unsigned long>::iterator it = m_mapNextCode.find(nCode);
	if(it == m_mapNextCode.end())
	{
		m_mapNextCode[nCode] = 1;
	}
	else
	{
		it->second++;
	}

	switch(nCode)
	{
	case 10:
		m_nCntEnd ++;
		break;
	case 32:  // 
	case 46:  // .
	case 44:  // ,
	case 63:  // ?
	case 33:  // !
		m_nCntEnd ++;
		break;
	}

}