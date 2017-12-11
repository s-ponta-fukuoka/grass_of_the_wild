//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_attack.h"
#include "player_pattern_wait.h"
#include "player.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "player_attack.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternAttack::PlayerPatternAttack()
{
	m_pPlayerAttack = NULL;
}


PlayerPatternAttack::~PlayerPatternAttack()
{
}

void PlayerPatternAttack::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	if (m_pPlayerAttack == NULL)
	{
		m_pPlayerAttack = new PlayerAttack(pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant, pCollisionManager);
	}
	else
	{
		m_pPlayerAttack->Update();
	}

	InputOperation(pPlayer);

	ChangeAttackAnime(pPlayer, pCollisionManager);
}

void PlayerPatternAttack::InputOperation(Player* pPlayer)
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

	XMVECTOR vector = pPlayer->GetCamera()->GetVec();

	XMMATRIX hWorld = XMMatrixIdentity();
	XMMATRIX hPosition = XMMatrixTranslation(pTransform->position.x, pTransform->position.y, pTransform->position.z);
	XMMATRIX hRotate = XMMatrixRotationRollPitchYaw(D3DToRadian(pTransform->rot.x), -pTransform->rot.y, D3DToRadian(pTransform->rot.z));
	XMMATRIX hScaling = XMMatrixScaling(1, 1, 1);

	hWorld = XMMatrixMultiply(hWorld, hScaling);
	hWorld = XMMatrixMultiply(hWorld, hRotate);
	hWorld = XMMatrixMultiply(hWorld, hPosition);

	XMMATRIX hAttackWorld = XMMatrixIdentity();
	hPosition = XMMatrixTranslation(0, 0, 50);
	hRotate = XMMatrixRotationRollPitchYaw(0,0,0);
	hScaling = XMMatrixScaling(1, 1, 1);

	hAttackWorld = XMMatrixMultiply(hAttackWorld, hScaling);
	hAttackWorld = XMMatrixMultiply(hAttackWorld, hRotate);
	hAttackWorld = XMMatrixMultiply(hAttackWorld, hPosition);
	hAttackWorld = XMMatrixMultiply(hAttackWorld, hWorld);



	m_pPlayerAttack->GetSphereCollider()->GetTransform()->position.x = m_pPlayerAttack->GetTransform()->position.x =  hAttackWorld._41;
	m_pPlayerAttack->GetSphereCollider()->GetTransform()->position.z = m_pPlayerAttack->GetTransform()->position.z =  hAttackWorld._43;

	if (pInputKeyboard->GetKeyPress(DIK_SPACE) && pAnimeNumber[0] == 2)
	{
		if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 10)
		{
			pAnimeNumber[0] = 3;

			pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
		}
	}
}

void PlayerPatternAttack::ChangeAttackAnime(Player* pPlayer, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber= pPlayer->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1 )
	{
		if (pAnimeNumber[0] == 2 || pAnimeNumber[0] == 3)
		{
			pAnimeNumber[0] = 0;
			pCollisionManager->DeleteCollider(m_pPlayerAttack->GetSphereCollider());
			m_pPlayerAttack->Release();
			m_pPlayerAttack = NULL;
			pPlayer->ChangePlayerPattern(new PlayerPatternWait);
		}
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}

	if (pFrame[0] < pAnime[pAnimeNumber[0]].nStartTime)
	{
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}

	pFrame[0]++;
}