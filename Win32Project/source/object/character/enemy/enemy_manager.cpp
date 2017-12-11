//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "enemy_manager.h"
#include "enemy.h"

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
{
}

HRESULT EnemyManager::GenerateEnemy(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	MainCamera *pCamera,
	CollisionManager* pCollisionManager)
{
	EnemyAdd(new Enemy(VECTOR3(100,0,700),VECTOR3(0,180,0), VECTOR3(1,1,1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));

	EnemyAdd(new Enemy(VECTOR3(0, 0, -900), VECTOR3(0, 90, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));
	
	EnemyAdd(new Enemy(VECTOR3(-1000, 0, 0), VECTOR3(0, -45, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));

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
		(*ite)->Release();
		m_list.erase(ite);
		return;
	}
}

void EnemyManager::Update(void)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Update();
	}
}

void EnemyManager::Release(void)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		(*ite)->Release();
	}
}