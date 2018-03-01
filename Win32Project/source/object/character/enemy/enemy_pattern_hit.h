//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_HIT_H_
#define _ENEMY_PATTERN_HIT_H_

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
class EnemyPatternHit : public EnemyPattern
{
public:
	EnemyPatternHit();
	virtual ~EnemyPatternHit();
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;

	void ChangeHitAnime(Enemy* pEnemy, CollisionManager* pCollisionManager);
};

#endif