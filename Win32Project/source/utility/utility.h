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
		T nReverse = 0;

		while (nNumber > 0) {
			nReverse = nReverse * 10 + nNumber % 10;
			nNumber /= 10;
		}

		return nNumber;
	};

protected:
	const char* m_pFileName;
};

#endif