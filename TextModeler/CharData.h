#pragma once
#include "HangulChar.h"


template<template <typename> class P = std::less >
struct compare_pair_second 
{
	template<class T1, class T2> bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right) {

		return P<T2>()(left.second, right.second);

	}
};





class CCharData
{
public:
	CCharData(void);
	~CCharData(void);

	void ReleaseData();
	void PreProcessHangulChar(wchar_t* _cSrc, int pageNum);
	void RestoreAllChar(wchar_t* _cSrc);
	void LoadBookData(CString strFile);
	int  GetTotalNumber();
	int	 GetRealNumber();
	void DrawLink();
	void ExtractWord();
	void DrawWordCntInfo();
	void FindTreeByChar(unsigned short nCode, int nTreeLevel);
	void SetData(WCHAR* pData, int _nSize);
	CString FindStringByCode(unsigned short nCode);
	void FindTreeBySeedChar(unsigned short nCode1, unsigned short nCode2, int nTreeLevel);
	void FindTreeBySeedChar2(unsigned short nCode1, unsigned short nCode2, int nTreeLevel);
	void FindTreeBySeedChar3(unsigned short nCode1, unsigned short nCode2, int nTreeLevel, std::vector<_CHAR_TREE*>&pforwardTree, std::vector<_CHAR_TREE*>&pBackwardTree);

	std::map<unsigned long, CHangulChar*> m_mapChar;
	std::vector< std::pair <unsigned long, unsigned long>> m_sortVec;

	std::map<unsigned long, _HANWORD> m_mapHWord;		// Code - String
	std::vector< std::pair <unsigned long, unsigned long>> m_sortHWord;// Count  - code



	std::vector<_HANWORD> m_vecTopHWord;


	CHangulChar*	m_pArrayHChar;
	int				m_nTop20Value;
	int				m_nTop20Word;
	float			m_fXOffset ;
	int				m_maxCnt;

	unsigned long	m_charAddCnt;
	unsigned long	m_totlaHangulCnt;
	unsigned long	m_totalCnt;
	unsigned long	m_hangulRatio;
	unsigned short	*m_pAllChar;
	unsigned short	*m_pAllCharInv;

	std::vector<_CHAR_TREE*> m_forwardTree;
	std::vector<_CHAR_TREE*> m_backwardTree;

	float			m_fFLevelSize[20];
	float			m_fBLevelSize[20];

	unsigned short FindCodeByString(CString strName);
	int			m_nMaxTreeLevel;

	POINT3D RoteteVecOnAxis(POINT3D _vLocalO, POINT3D _vInput, float _fAngle, float _x, float _y, float _z);

private:
	
	void drawForwordLink(CHangulChar* pChar);
	void MakeStatisticInfo();
	void InitCharData();
	POINT3D GetColor(unsigned long nCode);
	wchar_t* CharToWChar(const char* pstrSrc);
	unsigned int getHashCode(wchar_t* str, unsigned len);

	void testFunc();
	bool FindChildRecursive(_CHAR_TREE* pTree, _CHAR_TREE* pParents, unsigned int nLevel, std::vector<_CHAR_TREE*> &vecTree, unsigned short* pAllChar, int& maxLevel);
	void RemoveDuplicate();
	bool CheckDuplication(_CHAR_TREE*pTree, std::vector<_CHAR_TREE*> &vecTree);
	void SetTreePosition(std::vector<_CHAR_TREE*> &vecTree, float fStartAngle, float fAngleRange, int &maxLevel, float* levelLength, float fLevelHeight);
	bool FindParentPosRCV(std::vector<_CHAR_TREE*> &vecTree, int nChildLevel, float fLevelHeight, POINT3D vBasePos);
	void MakeDummyChild(std::vector<_CHAR_TREE*> &vecTree);
	void ApplyWeightPosition(std::vector<_CHAR_TREE*> &vecTree, float fLevelHeight);


	void FineTreePosition(std::vector<_CHAR_TREE*> &vecTree, float fLevelLength, int nIndex, _CHAR_TREE* ppNode, float fGravity);
	void FindNodeWeight(std::vector<_CHAR_TREE*> &vecTree);


	

	

	
	int			m_nLastNodeCnt;

		
	int				m_nNumChar;

	
	int						 m_forwardTreeDepth;	
	int						 m_backwardTreeDepth;

	_CHAR_TREE				 m_treeBuffer[200];
	
	

	
};
