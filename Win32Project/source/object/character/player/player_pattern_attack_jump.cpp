//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_attack_jump.h"
#include "player_pattern_wait.h"
#include "player.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"
#include "../../../collision/collision_manager.h"
#include "player_attack_jump.h"
#include "../../../shader/shader_manager.h"
#include "../../../effect/effect_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternAttackJump::PlayerPatternAttackJump()
{
	m_pPlayerAttack = NULL;
	m_nWait = 0;
	m_fTime = 0.0f;
}


PlayerPatternAttackJump::~PlayerPatternAttackJump()
{
}

void PlayerPatternAttackJump::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	if (m_pPlayerAttack == NULL)
	{
		if (pAnimeNumber[0] == 9)
		{
			if (pFrame[0] == pAnime[pAnimeNumber[0]].nEndTime - 10)
			{
				m_pPlayerAttack = new PlayerAttackJump(pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant, pCollisionManager);
			}
		}
	}
	if (m_pPlayerAttack != NULL)
	{
		m_pPlayerAttack->Update();
	}

	InputAttackOperation(pPlayer, pCollisionManager);

	ChangeAttackAnime(pPlayer, pCollisionManager);
}

void PlayerPatternAttackJump::InputAttackOperation(Player* pPlayer, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	Object::Transform* pTransform = pPlayer->GetTransform();

	VECTOR3 moveVector = pPlayer->GetMoveVector();

	XMVECTOR CompletionPosition = pPlayer->GetCompletionPosition();

	XMVECTOR CompletionRot = pPlayer->GetCompletionRot();

	XMVECTOR StartPosition = XMVectorSet(pTransform->position.x, pTransform->position.y, pTransform->position.z, 1.0f);
	
	XMVECTOR StartRot = XMVectorSet(0, pTransform->rot.y, 0, 1.0f);

	XMVECTOR vector = pPlayer->GetCamera()->GetVec();

	VECTOR3 move = pPlayer->GetMove();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	XMVECTOR inputVec = XMVector3Normalize(XMVectorSet(0, 0, 0, 0) - XMVectorSet(-Xinput.Gamepad.sThumbLX, 0, -Xinput.Gamepad.sThumbLY, 0));

	if (m_pPlayerAttack != NULL)
	{
		m_pPlayerAttack->GetSphereCollider()->GetTransform()->position.x = pPlayer->GetTransform()->position.x + cosf(-pPlayer->GetTransform()->rot.y - D3DToRadian(85)) * 100;
		m_pPlayerAttack->GetSphereCollider()->GetTransform()->position.y = pPlayer->GetTransform()->position.y + 70;
		m_pPlayerAttack->GetSphereCollider()->GetTransform()->position.z = pPlayer->GetTransform()->position.z - sinf(pPlayer->GetTransform()->rot.y - D3DToRadian(85)) * 100;
	}

	if (pAnimeNumber[0] == 9)
	{
		if (pFrame[0] == pAnime[pAnimeNumber[0]].nEndTime - 5)
		{
			pCollisionManager->DeleteCollider(m_pPlayerAttack->GetSphereCollider());
			m_pPlayerAttack->Release();
			delete m_pPlayerAttack;
			m_pPlayerAttack = NULL;
		}
	}

	if (Xinput.Gamepad.sThumbLX != 0)
	{
		VECTOR3 vecForward = VECTOR3(XMVectorGetX(vector),
			XMVectorGetY(vector),
			XMVectorGetZ(vector));

		VECTOR3 vecRight;

		VECTOR3::Cross(&vecRight, &vecForward, &VECTOR3(0, 1, 0));
	}


	EffectManager* pEffect = pPlayer->GetEffectManager();

	if (pFrame[0] == pAnime[pAnimeNumber[0]].nEndTime - 20 && pAnimeNumber[0] == 9)
	{
		// エフェクトの再生
		::Effekseer::Handle handle;
		handle = pEffect->GetEManager()->Play(pEffect->GetEffect(EffectManager::EFFECT_ATTACK_JUMP),
			pPlayer->GetTransform()->position.x + cosf(-pPlayer->GetTransform()->rot.y - D3DToRadian(85)) * 50,
			pPlayer->GetTransform()->position.y + 70,
			pPlayer->GetTransform()->position.z - sinf(pPlayer->GetTransform()->rot.y - D3DToRadian(85)) * 50);
		pEffect->GetEManager()->SetScale(handle, 50, 50, 50);
		pEffect->GetEManager()->SetRotation(handle, 0, -pPlayer->GetTransform()->rot.y, 0);
	}

	if (pFrame[0] == pAnime[pAnimeNumber[0]].nStartTime)
	{
		move.x -= XMVectorGetX(vector) * 200;
		move.z -= XMVectorGetZ(vector) * 200;

		CompletionPosition = XMVectorSet(pTransform->position.x + move.x, 0, pTransform->position.z + move.z, 0);
		CompletionRot = XMVectorSet(0, atan2(XMVectorGetZ(vector), XMVectorGetX(vector)) + D3D_PI * 0.5, 0, 1);
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

void PlayerPatternAttackJump::ChangeAttackAnime(Player* pPlayer, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber= pPlayer->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1 )
	{
		if (m_nWait > 20)
		{
			m_nWait = 0;
			if (pAnimeNumber[0] == 9)
			{
				pAnimeNumber[0] = 0;
				if (m_pPlayerAttack != NULL)
				{
					pCollisionManager->DeleteCollider(m_pPlayerAttack->GetSphereCollider());
					m_pPlayerAttack->Release(); 
					delete m_pPlayerAttack;
					m_pPlayerAttack = NULL;
				}
				pPlayer->ChangePlayerPattern(new PlayerPatternWait);
			}
			pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
		}
		else
		{
			m_nWait++;
			pFrame[0] = pAnime[pAnimeNumber[0]].nEndTime - 1;
		}
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