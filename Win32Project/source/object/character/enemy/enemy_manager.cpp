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
	AppRenderer::Constant* pLightCameraConstant, MainCamera *pCamera)
{
	EnemyAdd(new Enemy(VECTOR3(100,0,100),VECTOR3(0,0,0), VECTOR3(1,1,1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera));

	EnemyAdd(new Enemy(VECTOR3(0, 0, -100), VECTOR3(0, 0, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera));

	EnemyAdd(new Enemy(VECTOR3(-300, 0, 0), VECTOR3(0, 0, 0), VECTOR3(1, 1, 1),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera));

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
		if ((*ite) == enemy)
		{
			m_list.erase(ite);
		}
	}
}

void EnemyManager::Update(void)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
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