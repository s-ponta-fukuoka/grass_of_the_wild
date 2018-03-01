//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_ATTACK_JUMP_H_
#define _PLAYER_PATTERN_ATTACK_JUMP_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class PlayerAttackJump;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerPatternAttackJump : public PlayerPattern
{
	PlayerAttackJump* m_pPlayerAttack;
	float m_fTime;
	int			m_nWait;
public:

	PlayerPatternAttackJump();

	virtual ~PlayerPatternAttackJump();

	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;

	virtual void InputOperation(Player* pPlayer) {};

	void InputAttackOperation(Player* pPlayer, CollisionManager* pCollisionManager);

	void ChangeAttackAnime(Player* pPlayer, CollisionManager* pCollisionManager);
};

#endif