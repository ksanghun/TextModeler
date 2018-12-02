#pragma once
#include "data_type.h"

class CLetterData
{
public:
	CLetterData(void);
	~CLetterData(void);

	void InitData();
	void ResetMapData();

	void UpdateData(POINT3D pos, POINT3D color, unsigned long nCode);
	void AddPriviousCode(unsigned long nCode);
	void AddNextCode(unsigned long nCode);
//	void GetNextCode()

	void SetNameByChar(wchar_t strCode);
	void SetNameByString(CString str);

	CString GetName() {return m_strName;};

	// Attribution //
	POINT3D m_position;
	POINT3D m_color;
	unsigned long m_code;
	unsigned long m_nRefCnt;

	unsigned long				m_nCntFirst;		// count number that previous character is space.
	unsigned long				m_nCntEnd;			// count number that next character is space.

//private:
	std::map<unsigned long, unsigned long> m_mapPreCode;
	std::map<unsigned long, unsigned long> m_mapNextCode;

private:
	CString						m_strName;

};
