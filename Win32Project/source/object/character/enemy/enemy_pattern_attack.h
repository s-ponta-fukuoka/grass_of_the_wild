//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_ATTACK_H_
#define _ENEMY_PATTERN_ATTACK_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class EnemyAttack;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemyPatternAttack : public EnemyPattern
{
	EnemyAttack* m_pEnemyAttack;

	int			m_nWait;
public:

	EnemyPatternAttack();

	virtual ~EnemyPatternAttack();

	virtual void Update(Enemy* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
};

#endif