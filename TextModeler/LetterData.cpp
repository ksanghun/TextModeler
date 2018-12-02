#include "StdAfx.h"
#include "LetterData.h"

CLetterData::CLetterData(void)
{
	InitData();
}

CLetterData::~CLetterData(void)
{
}

void CLetterData::InitData()
{
	m_position.x = 0;
	m_position.y = 0;
	m_position.z = 0;

	m_color.x = 0;
	m_color.y = 0;
	m_color.z = 0;

	m_code = 0;
	m_nRefCnt = 0;
	m_nCntFirst = 0;
	m_nCntEnd = 0;

	ResetMapData();
}

void CLetterData::ResetMapData()
{
	m_mapNextCode.clear();
	m_mapPreCode.clear();
	m_nRefCnt = 0;
	m_nCntFirst = 0;
	m_nCntEnd = 0;
}

void CLetterData::UpdateData(POINT3D pos, POINT3D color, unsigned long nCode)
{
	m_position = pos;
	m_color = color;
	m_code = nCode;

}
void CLetterData::AddPriviousCode(unsigned long nCode)
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

	if(m_nCntFirst>100)
		m_nCntFirst  = 100;


}
void CLetterData::AddNextCode(unsigned long nCode)
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

	if(m_nCntEnd>100)
		m_nCntEnd  = 100;

}

void CLetterData::SetNameByChar(wchar_t strCode)
{
	CString tmpStr(strCode);
	m_strName = tmpStr;
}
void CLetterData::SetNameByString(CString str)
{
	m_strName = str;
}