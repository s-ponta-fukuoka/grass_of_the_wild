//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_DETH_H_
#define _ENEMY_PATTERN_DETH_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemyPatternDeth : public EnemyPattern
{
public:
	EnemyPatternDeth();
	virtual ~EnemyPatternDeth();
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;

	void ChangeDethAnime(Enemy* pEnemy, CollisionManager* pCollisionManager);
};

#endif