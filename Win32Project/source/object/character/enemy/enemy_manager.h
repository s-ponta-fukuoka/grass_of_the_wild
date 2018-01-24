//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <list>
#include "enemy.h"

class Player;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemyManager
{
	std::list<Enemy *>		m_list;

	float					m_distance;
public:

	EnemyManager::EnemyManager();

	HRESULT GenerateEnemy(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager*	pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera *pCamera,
		CollisionManager* pCollisionManager, 
		Object::Transform* pPlayerTransform,
		MeshField* pMeshField);

	void EnemyAdd(Enemy* enemy);

	void EnemyDelete(Enemy* enemy);

	void Update(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager);

	void Release(void);

	Enemy* GetDistanceEnemy(Player* pPlayer);
};

#endif
