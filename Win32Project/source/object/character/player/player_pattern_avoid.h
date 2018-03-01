//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_AVOID_H_
#define _PLAYER_PATTERN_AVOID_H_

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
class PlayerPatternAvoid : public PlayerPattern
{
	bool m_bStep;
	float m_fTime;
public:
	PlayerPatternAvoid();
	virtual ~PlayerPatternAvoid();
	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
	virtual void InputOperation(Player* pPlayer);

	void ChangeAvoidAnime(Player* pPlayer, CollisionManager* pCollisionManager);
};

#endif