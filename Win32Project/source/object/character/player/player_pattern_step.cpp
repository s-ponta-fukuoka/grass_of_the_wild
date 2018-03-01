//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_step.h"
#include "player_pattern_walk.h"
#include "player_pattern_attack.h"
#include "player_pattern_wait.h"
#include "player_pattern_avoid.h"
#include "player.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"
#include "../../camera/main_camera.h"
#include "player_attack.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternStep::PlayerPatternStep()
{
}

PlayerPatternStep::~PlayerPatternStep()
{
}

void PlayerPatternStep::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	InputOperation(pPlayer);

	pPlayer->ChangeAnime();
}

void PlayerPatternStep::InputOperation(Player* pPlayer)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	Object::Transform* pTransform = pPlayer->GetTransform();

	VECTOR3 move = pPlayer->GetMove();

	XMVECTOR CompletionPosition = pPlayer->GetCompletionPosition();

	XMVECTOR CompletionRot = pPlayer->GetCompletionRot();

	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	XMVECTOR StartPosition = XMVectorSet(pTransform->position.x, pTransform->position.y, pTransform->position.z, 1.0f);

	XMVECTOR vector = pPlayer->GetCamera()->GetVec();

	XMVECTOR StartRot = XMVectorSet(0, pTransform->rot.y, 0, 1.0f);

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	XMVECTOR inputVec = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - XMVectorSet(-Xinput.Gamepad.sThumbLX, 0, -Xinput.Gamepad.sThumbLY, 0));

	if (Xinput.Gamepad.wButtons == XINPUT_GAMEPAD_X ||
		pInputKeyboard->GetKeyPress(DIK_RSHIFT))
	{
		pPlayer->ChangePlayerPattern(new PlayerPatternAvoid);
		return;
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE) ||
		Xinput.Gamepad.wButtons == XINPUT_GAMEPAD_A)
	{
		pAnimeNumber[0] = 2;
		pPlayer->ChangePlayerPattern(new PlayerPatternAttack);
		return;
	}

	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		pAnimeNumber[0] = 1;
		move.x -= XMVectorGetX(vector) * PLAYER_MOVE;
		move.z -= XMVectorGetZ(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		pAnimeNumber[0] = 11;
		move.x += XMVectorGetZ(vector) * PLAYER_MOVE;
		move.z -= XMVectorGetX(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		pAnimeNumber[0] = 12;
		move.x += XMVectorGetX(vector) * PLAYER_MOVE;
		move.z += XMVectorGetZ(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		pAnimeNumber[0] = 11;
		move.x -= XMVectorGetZ(vector) * PLAYER_MOVE;
		move.z += XMVectorGetX(vector) * PLAYER_MOVE;
	}

	if (Xinput.Gamepad.sThumbLY > 20000)
	{
		pAnimeNumber[0] = 1;
	}

	if (Xinput.Gamepad.sThumbLX < -20000)
	{
		pAnimeNumber[0] = 11;
	}

	if (Xinput.Gamepad.sThumbLX > 20000)
	{
		pAnimeNumber[0] = 11;
	}

	if (Xinput.Gamepad.sThumbLY < -20000)
	{
		pAnimeNumber[0] = 12;
	}


	if (Xinput.Gamepad.sThumbLX != 0)
	{
		VECTOR3 vecForward = VECTOR3(XMVectorGetX(vector),
			XMVectorGetY(vector),
			XMVectorGetZ(vector));

		VECTOR3 vecRight;

		VECTOR3::Cross(&vecRight, &vecForward, &VECTOR3(0, 1, 0));

		move = vecForward * -XMVectorGetZ(inputVec) * PLAYER_MOVE +
			vecRight * XMVectorGetX(inputVec) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_W) ||
		pInputKeyboard->GetKeyPress(DIK_A) ||
		pInputKeyboard->GetKeyPress(DIK_S) ||
		pInputKeyboard->GetKeyPress(DIK_D) ||
		Xinput.Gamepad.sThumbLX < -20000 ||
		Xinput.Gamepad.sThumbLX > 20000 ||
		Xinput.Gamepad.sThumbLY < -20000 ||
		Xinput.Gamepad.sThumbLY > 20000)
	{
		CompletionPosition = XMVectorSet(pTransform->position.x + move.x, 0, pTransform->position.z + move.z, 0);
		CompletionRot = XMVectorSet(0, atan2(XMVectorGetZ(vector), XMVectorGetX(vector)) + D3D_PI * 0.5, 0, 1);
	}
	else
	{
		pAnimeNumber[0] = 0;
		pPlayer->ChangePlayerPattern(new PlayerPatternWait);
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		pAnimeNumber[0] = 2;
		pPlayer->ChangePlayerPattern(new PlayerPatternAttack);
	}

	if (pInputKeyboard->GetKeyRelease(DIK_LSHIFT) ||
		Xinput.Gamepad.bLeftTrigger < 250)
	{
		pAnimeNumber[0] = 1;
		pPlayer->ChangePlayerPattern(new PlayerPatternWalk);
	}

	pTransform->position += move;
	move *= 0.1f;

	StartRot = XMVectorLerp(StartRot, CompletionRot, 0.1f);

	if (Xinput.Gamepad.sThumbLX != 0)
	{
		pTransform->rot.y = XMVectorGetY(CompletionRot);
	}
	else
	{
		pTransform->rot.y = XMVectorGetY(StartRot);
	}

	StartPosition = XMVectorLerp(StartPosition, CompletionPosition, 0.1f);
	pTransform->position.x = XMVectorGetX(StartPosition);
	pTransform->position.z = XMVectorGetZ(StartPosition);

	pPlayer->SetCompletionPosition(CompletionPosition);
	pPlayer->SetCompletionRot(CompletionRot);
}