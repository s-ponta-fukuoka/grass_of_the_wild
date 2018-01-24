//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_H_
#define _ENEMY_PATTERN_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../character.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Enemy;
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemyPattern
{
public:
	virtual ~EnemyPattern() {}
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) = 0;
};

class EnemyPatternNone : public EnemyPattern
{
public:
	virtual ~EnemyPatternNone() {}
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override {}
};

#endif