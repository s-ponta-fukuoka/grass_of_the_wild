//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_wait.h"
#include "player_pattern_attack.h"
#include "player_pattern_walk.h"
#include "player_pattern_step.h"
#include "player.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"
#include "../../camera/main_camera.h"
#include "../../canvas/player_life/player_life.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternWait::PlayerPatternWait()
{
}


PlayerPatternWait::~PlayerPatternWait()
{
}

void PlayerPatternWait::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	InputOperation(pPlayer);

	pPlayer->ChangeAnime();
}

void PlayerPatternWait::InputOperation(Player* pPlayer)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	XMVECTOR inputVec = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - XMVectorSet(Xinput.Gamepad.sThumbLY, 0, -Xinput.Gamepad.sThumbLX, 0));


	if (pInputKeyboard->GetKeyPress(DIK_W) ||
		pInputKeyboard->GetKeyPress(DIK_A) ||
		pInputKeyboard->GetKeyPress(DIK_S) ||
		pInputKeyboard->GetKeyPress(DIK_D) ||
		Xinput.Gamepad.sThumbLX < -20000 ||
		Xinput.Gamepad.sThumbLX > 20000 ||
		Xinput.Gamepad.sThumbLY < -20000 ||
		Xinput.Gamepad.sThumbLY > 20000)
	{
		pAnimeNumber[0] = 1;
		pPlayer->ChangePlayerPattern(new PlayerPatternWalk);
	}

	//if (pInputKeyboard->GetKeyPress(DIK_LSHIFT) ||
	//	Xinput.Gamepad.bLeftTrigger > 250)
	//{
	//	pPlayer->ChangePlayerPattern(new PlayerPatternStep);
	//}

	if (pInputKeyboard->GetKeyTrigger(DIK_E))
	{
		pPlayer->GetPlayerLife()->Sub(1);
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE) ||
		Xinput.Gamepad.wButtons == XINPUT_GAMEPAD_A)
	{
		pAnimeNumber[0] = 2;
		pPlayer->ChangePlayerPattern(new PlayerPatternAttack);
	}

}
