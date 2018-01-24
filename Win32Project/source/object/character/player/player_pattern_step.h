//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_STEP_H_
#define _PLAYER_PATTERN_STEP_H_

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
class PlayerPatternStep : public PlayerPattern
{
public:
	PlayerPatternStep();
	virtual ~PlayerPatternStep();
	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
	virtual void InputOperation(Player* pPlayer);
};

#endif