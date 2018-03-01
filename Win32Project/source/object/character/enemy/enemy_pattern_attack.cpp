//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "enemy_attack.h"
#include "../../../shader/shader_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternAttack::EnemyPatternAttack()
{
	m_pEnemyAttack = NULL;
	m_nWait = 0;
}


EnemyPatternAttack::~EnemyPatternAttack()
{
	if (m_pEnemyAttack != NULL)
	{
		m_pCollisionManager->DeleteCollider(m_pEnemyAttack->GetSphereCollider());
		m_pEnemyAttack->Release();
		delete m_pEnemyAttack;
		m_pEnemyAttack = NULL;
	}
}

void EnemyPatternAttack::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	SkinMeshModel::Anime* pAnime = pEnemy->GetSkinMeshModel()->GetAnime();

	m_pCollisionManager = pCollisionManager;

	int* pFrame = pEnemy->GetFrame();
	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	Object::Transform* pTransform = pEnemy->GetTransform();

	XMVECTOR CompletionPosition = pEnemy->GetCompletionPosition();

	XMVECTOR CompletionRot = pEnemy->GetCompletionRot();

	XMVECTOR StartPosition = XMVectorSet(pTransform->position.x, pTransform->position.y, pTransform->position.z, 1.0f);

	XMVECTOR vector = pEnemy->GetCamera()->GetVec();

	VECTOR3 move = pEnemy->GetMove();

	if (pFrame[0] == pAnime[pAnimeNumber[0]].nEndTime - 70)
	{
		if (m_pEnemyAttack == NULL)
		{
			m_pEnemyAttack = new EnemyAttack(pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant, pCollisionManager);
		}
	}

	if (pFrame[0] == pAnime[pAnimeNumber[0]].nEndTime - 60)
	{
		if (m_pEnemyAttack != NULL)
		{
			pCollisionManager->DeleteCollider(m_pEnemyAttack->GetSphereCollider());
			m_pEnemyAttack->Release();
			delete m_pEnemyAttack;
			m_pEnemyAttack = NULL;
		}
	}

	if (m_pEnemyAttack != NULL)
	{
		m_pEnemyAttack->Update();
		m_pEnemyAttack->GetSphereCollider()->GetTransform()->position.x = pEnemy->GetTransform()->position.x + cosf(-pEnemy->GetTransform()->rot.y - D3DToRadian(85)) * 200;
		m_pEnemyAttack->GetSphereCollider()->GetTransform()->position.y = pEnemy->GetTransform()->position.y + 70;
		m_pEnemyAttack->GetSphereCollider()->GetTransform()->position.z = pEnemy->GetTransform()->position.z - sinf(pEnemy->GetTransform()->rot.y - D3DToRadian(85)) * 200;
	}

	ChangeAttackAnime(pEnemy, pCollisionManager);
}

void EnemyPatternAttack::ChangeAttackAnime(Enemy* pEnemy, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pEnemy->GetSkinMeshModel()->GetAnime();

	int* pFrame = pEnemy->GetFrame();
	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1)
	{

		if (pAnimeNumber[0] == 2 || pAnimeNumber[0] == 3)
		{

			pEnemy->SetMode(Enemy::MODE_DEFENSE);

			pAnimeNumber[0] = 0;
			if (m_pEnemyAttack != NULL)
			{
				pCollisionManager->DeleteCollider(m_pEnemyAttack->GetSphereCollider());
				m_pEnemyAttack->Release();
				delete m_pEnemyAttack;
				m_pEnemyAttack = NULL;
			}

			pEnemy->ChangeEnemyPattern(new EnemyPatternWait);
		}
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
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