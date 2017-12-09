//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_H_
#define _PLAYER_PATTERN_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../character.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
class Player;
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class PlayerPattern
{
public:
	virtual ~PlayerPattern() {}
	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) = 0;
	virtual void InputOperation(Player* pPlayer) = 0;
};

class PlayerPatternNone : public PlayerPattern
{
public:
	virtual ~PlayerPatternNone() {}
	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override {}
	virtual void InputOperation(Player* pPlayer) override {}
};

#endif