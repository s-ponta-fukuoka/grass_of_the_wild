//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_walk.h"
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"
#include "enemy_attack.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternWalk::EnemyPatternWalk()
{
}

EnemyPatternWalk::~EnemyPatternWalk()
{
}

void EnemyPatternWalk::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
}