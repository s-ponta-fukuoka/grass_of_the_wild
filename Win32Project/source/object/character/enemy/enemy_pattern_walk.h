//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_WALK_H_
#define _ENEMY_PATTERN_WALK_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
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