//=============================================================================
//
// utility.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _UTILLTY_H_
#define _UTILLTY_H_

#include <stdio.h>
#include <tchar.h>
#include <bitset>
#include <vector>

//*********************************************************
//クラス
//*********************************************************
class Utility
{
public:
	//コンストラクタ
	Utility() {};

	//デストラクタ
	virtual ~Utility() {};
	
	template<typename T>
	static T DecimalConversion(T nNumber, T nDecimal1, T nDecimal2) {
		T ans = 0;

		for (int i = 0; nNumber >0; i++)
		{
			ans = ans + (nNumber % nDecimal1) * pow(nDecimal2, i);
			nNumber = nNumber / nDecimal1;
		}

		return ans;
	};

	template<typename T>
	static T GetReverseNumber(T nNumber)
	{
		int nReverse = 0;

		while (nNumber > 0) 
		{
			nReverse++;
			nNumber /= 10;
		}

		return nReverse;
	};

	static float Distance(VECTOR3 pos1, VECTOR3 pos2)
	{
		float distance = sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) +
			(pos1.y - pos2.y) * (pos1.y - pos2.y) +
			(pos1.z - pos2.z) * (pos1.z - pos2.z));

		return distance;
	}

protected:
	const char* m_pFileName;
};

#endif