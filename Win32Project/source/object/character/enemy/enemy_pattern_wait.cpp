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
		Utility::Distance(pTransform->position, pPlayerTransform->position) >= 500)
	{
		pAnimeNumber[0] = 6;
		pEnemy->ChangeEnemyPattern(new EnemyPatternRun);
	}

	int nCounterCnt = pEnemy->GetCounterCnt();

	if (nCounterCnt >= 10)
	{
		pAnimeNumber[0] = 3;
		pEnemy->SetMode(Enemy::MODE_ATTACK);
		pEnemy->ChangeEnemyPattern(new EnemyPatternAttack);
		nCounterCnt = 0;
	}

	pEnemy->SetCounterCnt(nCounterCnt);

	VECTOR3 pos;
	pos.x = pTransform->position.x - pPlayerTransform->position.x;
	pos.z = pTransform->position.z - pPlayerTransform->position.z;

	pTransform->rot.y = atan2(pos.z, pos.x);

	pTransform->rot.y = atan2(pos.z, pos.x) + D3D_PI * 0.5;

	pEnemy->ChangeAnime();
}
