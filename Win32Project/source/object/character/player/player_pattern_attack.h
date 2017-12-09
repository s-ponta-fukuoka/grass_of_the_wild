//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_ATTACK_H_
#define _PLAYER_PATTERN_ATTACK_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// クラス定義
//*****************************************************************************
class PlayerPatternAttack : public PlayerPattern
{
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
	void ChangeAttackAnime(Player* pPlayer);
};

#endif