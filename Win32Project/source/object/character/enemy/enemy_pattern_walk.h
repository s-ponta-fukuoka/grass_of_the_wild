//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_WALK_H_
#define _ENEMY_PATTERN_WALK_H_

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
class EnemyPatternWalk : public EnemyPattern
{
public:
	EnemyPatternWalk();
	virtual ~EnemyPatternWalk();
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
};

#endif