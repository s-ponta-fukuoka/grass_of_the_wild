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

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemyManager
{
	std::list<Enemy *>		m_list;
public:

	EnemyManager::EnemyManager();

	HRESULT GenerateEnemy(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager*	pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera *pCamera,
		CollisionManager* pCollisionManager);

	void EnemyAdd(Enemy* enemy);

	void EnemyDelete(Enemy* enemy);

	void Update(void);

	void Release(void);
};

#endif
