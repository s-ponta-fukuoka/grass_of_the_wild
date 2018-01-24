//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_run.h"
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"
#include "enemy_attack.h"
#include "../../../utility/utility.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MOVE (10)


///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternRun::EnemyPatternRun()
{
}

EnemyPatternRun::~EnemyPatternRun()
{
}

void EnemyPatternRun::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	Object::Transform* pTransform = pEnemy->GetTransform();
	Object::Transform* pPlayerTransform = pEnemy->GetPlayerTransform();

	VECTOR3 move = pEnemy->GetMove();

	XMVECTOR CompletionPosition = pEnemy->GetCompletionPosition();

	XMVECTOR CompletionRot = pEnemy->GetCompletionRot();

	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	VECTOR3 pos;
	pos.x = pTransform->position.x - pPlayerTransform->position.x;
	pos.z = pTransform->position.z - pPlayerTransform->position.z;

	pTransform->rot.y = atan2(pos.z, pos.x);
	move.x = -cosf(pTransform->rot.y) * ENEMY_MOVE;
	move.z = -sinf(pTransform->rot.y) * ENEMY_MOVE;

	pTransform->position.x += move.x;
	pTransform->position.z += move.z;

	pTransform->rot.y = atan2(pos.z, pos.x) + D3D_PI * 0.5;

	if (Utility::Distance(pTransform->position, pPlayerTransform->position) < 300)
	{
		pAnimeNumber[0] = 0;
		pEnemy->ChangeEnemyPattern(new EnemyPatternWait);
	}

	pEnemy->ChangeAnime();

}