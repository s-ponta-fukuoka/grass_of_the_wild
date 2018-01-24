//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_WAIT_H_
#define _ENEMY_PATTERN_WAIT_H_

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
class EnemyPatternWait : public EnemyPattern
{
public:
	EnemyPatternWait();
	virtual ~EnemyPatternWait();
	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
};

#endif