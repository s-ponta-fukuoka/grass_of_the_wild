//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_DETH_H_
#define _PLAYER_PATTERN_DETH_H_

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
class PlayerPatternDeth : public PlayerPattern
{
public:
	PlayerPatternDeth();
	virtual ~PlayerPatternDeth();
	virtual void Update(Player* pEnemy,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager) override;

	virtual void InputOperation(Player* pPlayer) {};

	void ChangeDethAnime(Player* pEnemy, CollisionManager* pCollisionManager);
};

#endif