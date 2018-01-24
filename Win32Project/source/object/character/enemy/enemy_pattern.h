//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_H_
#define _ENEMY_PATTERN_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../character.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
class Enemy;
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
//*****************************************************************************
// �N���X��`
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