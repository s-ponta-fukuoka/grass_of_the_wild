//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_hit.h"
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

EnemyPatternHit::EnemyPatternHit()
{
}

EnemyPatternHit::~EnemyPatternHit()
{
}

void EnemyPatternHit::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	ChangeHitAnime(pEnemy, pCollisionManager);
}

void EnemyPatternHit::ChangeHitAnime(Enemy* pEnemy, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pEnemy->GetSkinMeshModel()->GetAnime();

	int* pFrame = pEnemy->GetFrame();
	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1)
	{

		if (pAnimeNumber[0] == 5)
		{
			pEnemy->SetMode(Enemy::MODE_DEFENSE);
			pAnimeNumber[0] = 0;
			pEnemy->ChangeEnemyPattern(new EnemyPatternWait);
		}
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}
	else
	{
		pFrame[0]++;
	}

	if (pFrame[0] < pAnime[pAnimeNumber[0]].nStartTime)
	{
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}
}