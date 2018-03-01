//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_PATTERN_ATTACK_H_
#define _ENEMY_PATTERN_ATTACK_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class EnemyAttack;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class EnemyPatternAttack : public EnemyPattern
{
	EnemyAttack* m_pEnemyAttack;

	CollisionManager* m_pCollisionManager;

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

	void ChangeAttackAnime(Enemy* pEnemy, CollisionManager* pCollisionManager);
};

#endif