//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_wait.h"
#include "enemy_pattern_attack.h"
#include "enemy_pattern_walk.h"
#include "enemy_pattern_run.h"
#include "enemy.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"
#include "../../canvas/player_life/player_life.h"
#include "../../../utility/utility.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternWait::EnemyPatternWait()
{
}


EnemyPatternWait::~EnemyPatternWait()
{
}

void EnemyPatternWait::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	Object::Transform* pTransform = pEnemy->GetTransform();
	Object::Transform* pPlayerTransform = pEnemy->GetPlayerTransform();

	if (Utility::Distance(pTransform->position, pPlayerTransform->position) < 1000 &&
		Utility::Distance(pTransform->position, pPlayerTransform->position) >= 100)
	{
		pAnimeNumber[0] = 6;
		pEnemy->ChangeEnemyPattern(new EnemyPatternRun);
	}

	pEnemy->ChangeAnime();
}
