//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "enemy_attack.h"
#include "../../../shader/shader_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

EnemyPatternAttack::EnemyPatternAttack()
{
	m_pEnemyAttack = NULL;
	m_nWait = 0;
}


EnemyPatternAttack::~EnemyPatternAttack()
{
}

void EnemyPatternAttack::Update(Enemy* pEnemy,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	if (m_pEnemyAttack == NULL)
	{
		m_pEnemyAttack = new EnemyAttack(pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant, pCollisionManager);
	}
	else
	{
		m_pEnemyAttack->Update();
	}
}