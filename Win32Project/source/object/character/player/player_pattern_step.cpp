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
#include "player.h"
#include "../../../device/input.h"
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

	if (pInputKeyboard->GetKeyPress(DIK_W) ||
		pInputKeyboard->GetKeyPress(DIK_A) ||
		pInputKeyboard->GetKeyPress(DIK_S) ||
		pInputKeyboard->GetKeyPress(DIK_D))
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

	if (pInputKeyboard->GetKeyRelease(DIK_LSHIFT))
	{
		pAnimeNumber[0] = 1;
		pPlayer->ChangePlayerPattern(new PlayerPatternWalk);
	}

	pTransform->position += move;
	move *= 0.1f;

	StartRot = XMVectorLerp(StartRot, CompletionRot, 0.1f);

	pTransform->rot.y = XMVectorGetY(StartRot);

	StartPosition = XMVectorLerp(StartPosition, CompletionPosition, 0.1f);
	pTransform->position.x = XMVectorGetX(StartPosition);
	pTransform->position.z = XMVectorGetZ(StartPosition);

	pPlayer->SetCompletionPosition(CompletionPosition);
	pPlayer->SetCompletionRot(CompletionRot);
}