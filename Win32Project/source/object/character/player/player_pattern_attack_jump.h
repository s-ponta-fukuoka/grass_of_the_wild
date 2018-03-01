//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_ATTACK_JUMP_H_
#define _PLAYER_PATTERN_ATTACK_JUMP_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class PlayerAttackJump;

//*****************************************************************************
// クラス定義
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