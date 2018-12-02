#pragma once

#include "data_type.h"
#include "math_tool.h"

class CHangulChar
{
public:
	CHangulChar(void);
	~CHangulChar(void);


	POINT3D GetColor(unsigned long nCode);
	void SetColor(unsigned long _nCode, wchar_t strCode);
	void UpdatePositon();
	void AddPriviousCode(unsigned long nCode);
	void AddNextCode(unsigned long nCode);
	void InitDataSet();


	POINT3D						m_vColor;		
	POINT3D						m_vPos;
	POINT3D						m_vCirclePos;

	unsigned long				m_nCode;
	std::map<unsigned long, unsigned long>	m_mapPreCode;
	std::map<unsigned long, unsigned long>	m_mapNextCode;  // code - count



	unsigned long				m_nCntRef;			// define x position

	unsigned long				m_nCntFirst;		// count number that previous character is space.
	unsigned long				m_nCntEnd;			// count number that next character is space.
	CString						m_strName;

	POINT3D						m_vImgPos;

private:

};
