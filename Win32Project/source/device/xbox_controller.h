//=============================================================================
//
// xinput処理 [xbox_controller.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

//*********************************************************
//　インクルード
//*********************************************************
#include "input.h"
#include <XInput.h>

//*********************************************************
// マクロ定義
//*********************************************************
#define MAX_XINPUT (1)
#define XINPUT_STICK_RANGE_ENABLE		( 20000 )
//*********************************************************
// ライブラリのリンク
//*********************************************************
#pragma comment(lib,"Xinput.lib")

//*********************************************************
// クラス定義
//*********************************************************
class XboxController : public Input
{
public:
	typedef enum
	{
		STICK_LX,
		STICK_LY,
		STICK_RX,
		STICK_RY,
	} STICK;

	XboxController();
	virtual ~XboxController();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BOOL GetButtonPress(int nIndex, int nButton);
	BOOL GetButtonTrigger(int nIndex, int nButton);

	int GetStick(int nIndex, STICK eStick);

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static XboxController* GetInstance(void);

private:
	static XboxController* m_pXboxController;

	XINPUT_STATE			m_Input[MAX_XINPUT];
	XINPUT_STATE			m_InputTrigger[MAX_XINPUT];
	XINPUT_VIBRATION		m_Vibration[MAX_XINPUT];	
};

#endif