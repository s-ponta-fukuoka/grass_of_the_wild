//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_WAIT_H_
#define _PLAYER_PATTERN_WAIT_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerPatternWait : public PlayerPattern
{
public:
	PlayerPatternWait();
	virtual ~PlayerPatternWait();
	virtual void Update(Player* pPlayer,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;
	virtual void InputOperation(Player* pPlayer);
};

#endif