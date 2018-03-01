//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_deth.h"
#include "enemy_pattern_hit.h"
#include "enemy_pattern_walk.h"
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"
#include "enemy_attack.h"
#include "enemy_manager.h"
#include "../../../effect/effect_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternDeth::EnemyPatternDeth()
{
}

EnemyPatternDeth::~EnemyPatternDeth()
{
}

void EnemyPatternDeth::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	VECTOR4* pColor = pEnemy->GetColor();

	XMVECTOR color;
	XMVECTOR startColor = XMVectorSet(pColor[0].x,
		pColor[0].y,
		pColor[0].z, 1.0f);

	color = XMVectorSet(0, 0, 0, 1);
	startColor = XMVectorLerp(startColor, color, 0.1f);
	pColor[0].x = XMVectorGetX(startColor);
	pColor[0].y = XMVectorGetY(startColor);
	pColor[0].z = XMVectorGetZ(startColor);

	pEnemy->SetColor(pColor);

	ChangeDethAnime(pEnemy, pCollisionManager);
}

void EnemyPatternDeth::ChangeDethAnime(Enemy* pEnemy, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pEnemy->GetSkinMeshModel()->GetAnime();

	int* pFrame = pEnemy->GetFrame();
	int* pAnimeNumber = pEnemy->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1)
	{

		if (pEnemy->GetEnemyManager()->GetNumEnemy() != 1)
		{
			// エフェクトの再生
			::Effekseer::Handle handle;
			handle = pEnemy->GetEffectManager()->GetEManager()->Play(pEnemy->GetEffectManager()->GetEffect(EffectManager::EFFECT_DETH),
				pEnemy->GetTransform()->position.x + cosf(-pEnemy->GetTransform()->rot.y - D3DToRadian(90)) * -200,
				pEnemy->GetTransform()->position.y - 50,
				pEnemy->GetTransform()->position.z - sinf(pEnemy->GetTransform()->rot.y - D3DToRadian(90)) * -200);
			pEnemy->GetEffectManager()->GetEManager()->SetScale(handle, 100, 100, 100);
			pEnemy->GetEffectManager()->GetEManager()->SetRotation(handle, 0, -pEnemy->GetTransform()->rot.y, 0);
		}

		pEnemy->GetEnemyManager()->EnemyDelete(pEnemy);
	}
	else
	{
		pFrame[0]++;
	}
}