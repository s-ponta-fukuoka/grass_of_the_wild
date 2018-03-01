//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_avoid.h"
#include "player_pattern_step.h"
#include "player_pattern_walk.h"
#include "player_pattern_attack.h"
#include "player_pattern_attack_jump.h"
#include "player_pattern_wait.h"
#include "player.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"
#include "../../camera/main_camera.h"
#include "player_attack.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_AVOID_MOVE (100)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternAvoid::PlayerPatternAvoid()
{
	m_fTime = 0.0f;
	m_bStep = false;
}

PlayerPatternAvoid::~PlayerPatternAvoid()
{
}

void PlayerPatternAvoid::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	InputOperation(pPlayer);

	ChangeAvoidAnime(pPlayer, pCollisionManager);
}

void PlayerPatternAvoid::InputOperation(Player* pPlayer)
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

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();

	if (!m_bStep)
	{
		if (pInputKeyboard->GetKeyPress(DIK_W) ||
			Xinput.Gamepad.sThumbLY > 20000)
		{
			pAnimeNumber[0] = 7;
			if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
			{
				move.x -= XMVectorGetX(vector) * PLAYER_AVOID_MOVE;
				move.z -= XMVectorGetZ(vector) * PLAYER_AVOID_MOVE;
			}
		}

		if (pInputKeyboard->GetKeyPress(DIK_A) ||
			Xinput.Gamepad.sThumbLX < -20000)
		{
			pAnimeNumber[0] = 5;
			if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
			{
				move.x += XMVectorGetZ(vector) * PLAYER_AVOID_MOVE;
				move.z -= XMVectorGetX(vector) * PLAYER_AVOID_MOVE;
			}
		}

		if (pInputKeyboard->GetKeyPress(DIK_S) ||
			Xinput.Gamepad.sThumbLY < -20000)
		{
			pAnimeNumber[0] = 8;
			if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
			{
				move.x += XMVectorGetX(vector) * PLAYER_AVOID_MOVE;
				move.z += XMVectorGetZ(vector) * PLAYER_AVOID_MOVE;
			}
		}

		if (pInputKeyboard->GetKeyPress(DIK_D) ||
			Xinput.Gamepad.sThumbLX > 20000)
		{
			pAnimeNumber[0] = 6;
			if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
			{
				move.x -= XMVectorGetZ(vector) * PLAYER_AVOID_MOVE;
				move.z += XMVectorGetX(vector) * PLAYER_AVOID_MOVE;
			}
		}
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
		if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
		{
			CompletionPosition = XMVectorSet(pTransform->position.x + move.x, 0, pTransform->position.z + move.z, 0);
			CompletionRot = XMVectorSet(0, atan2(XMVectorGetZ(vector), XMVectorGetX(vector)) + D3D_PI * 0.5, 0, 1);
		}
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

	StartPosition = XMVectorLerp(StartPosition, CompletionPosition, m_fTime);
	pTransform->position.x = XMVectorGetX(StartPosition);
	pTransform->position.z = XMVectorGetZ(StartPosition);

	pPlayer->SetCompletionPosition(CompletionPosition);
	pPlayer->SetCompletionRot(CompletionRot);

	if (m_fTime > 1)
	{
		m_fTime = 0;
	}
	else
	{
		m_fTime += 0.01f;
	}
}

void PlayerPatternAvoid::ChangeAvoidAnime(Player* pPlayer, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	if (pAnimeNumber[0] == 5 || pAnimeNumber[0] == 6 || pAnimeNumber[0] == 7 || pAnimeNumber[0] == 8)
	{
		if(Xinput.Gamepad.wButtons == XINPUT_GAMEPAD_A)
		{ 
			pAnimeNumber[0] = 9;
			pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
			pPlayer->ChangePlayerPattern(new PlayerPatternAttackJump);
		}
	}

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1)
	{

		pAnimeNumber[0] = 0;
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
		pPlayer->ChangePlayerPattern(new PlayerPatternWait);
	}
	else
	{
		pFrame[0]++;
	}

	if (pFrame[0] < pAnime[pAnimeNumber[0]].nStartTime)
	{
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}

}