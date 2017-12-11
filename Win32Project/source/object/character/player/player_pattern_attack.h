//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_ATTACK_H_
#define _PLAYER_PATTERN_ATTACK_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class PlayerAttack;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerPatternAttack : public PlayerPattern
{
	PlayerAttack* m_pPlayerAttack;
public:

	PlayerPatternAttack();

	virtual ~PlayerPatternAttack();

	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;

	virtual void InputOperation(Player* pPlayer);

	void ChangeAttackAnime(Player* pPlayer, CollisionManager* pCollisionManager);
};

#endif