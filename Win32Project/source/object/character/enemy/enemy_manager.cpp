//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "enemy_manager.h"
#include "enemy.h"
#include "../player/player.h"
#include "../../mesh/meshfiled/mesh_field.h"
#include "../../../collision/collision_manager.h"
#include "../../../effect/effect_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

EnemyManager::EnemyManager()
	: m_distance(0)
	, m_pCollisionManager(NULL)
{
}

HRESULT EnemyManager::GenerateEnemy(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	MainCamera *pCamera,
	CollisionManager* pCollisionManager,
	Object::Transform* pPlayerTransform,
	MeshField* pMeshField, EffectManager* pEffectManager)
{
	m_pCollisionManager = pCollisionManager;

	EnemyAdd(new Enemy(VECTOR3(100,0,0),VECTOR3(0,180,0), VECTOR3(1,1,1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this,pPlayerTransform, pMeshField, pEffectManager));

	EnemyAdd(new Enemy(VECTOR3(-1000, 0, 0), VECTOR3(0, 90, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this, pPlayerTransform, pMeshField, pEffectManager));
	
	EnemyAdd(new Enemy(VECTOR3(0, 0, 2000), VECTOR3(0, 0, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this, pPlayerTransform, pMeshField, pEffectManager));

	//EnemyAdd(new Enemy(VECTOR3(0, 0, -900), VECTOR3(0, 90, 0), VECTOR3(1, 1, 1),
	//	pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));
	//
	//EnemyAdd(new Enemy(VECTOR3(-1000, 0, 0), VECTOR3(0, -45, 0), VECTOR3(1, 1, 1),
	//	pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));

	return S_OK;
}

void EnemyManager::EnemyAdd(Enemy *enemy)
{
	m_list.push_back(enemy);
}

void EnemyManager::EnemyDelete(Enemy *enemy)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) != enemy) { continue; }
		m_pCollisionManager->DeleteCollider((*ite)->GetSphereCollider());
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
		return;
	}
}

void EnemyManager::Update(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Update(pRenderManager,
			pShaderManager,
			pTextureManager,
			pConstant,
			pLightCameraConstant, 
			pCollisionManager);
	}

	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) == NULL)
		{
			m_list.erase(ite);
			return;
		}
	}
}

void EnemyManager::Release(void)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		(*ite)->Release();
	}

	m_list.clear();
}

Enemy* EnemyManager::GetDistanceEnemy(Player* pPlayer)
{
	VECTOR3 PlayerPos = pPlayer->GetTransform()->position;

	int count = 0;

	Enemy* pEnemy = NULL;

	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite, ++count)
	{
		if ((*ite) == NULL) { continue; }

		float distance = sqrt( ((*ite)->GetTransform()->position.x - PlayerPos.x) * ((*ite)->GetTransform()->position.x - PlayerPos.x) +
			((*ite)->GetTransform()->position.y - PlayerPos.y) * ((*ite)->GetTransform()->position.y - PlayerPos.y) + 
			((*ite)->GetTransform()->position.z - PlayerPos.z) * ((*ite)->GetTransform()->position.z - PlayerPos.z));

		if (count == 0)
		{
			m_distance = distance;
			pEnemy = (*ite);
		}

		if (m_distance > distance)
		{
			m_distance = distance;
			pEnemy = (*ite);
		}
	}

	return pEnemy;
}