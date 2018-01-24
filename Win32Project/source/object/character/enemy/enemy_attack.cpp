//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy_attack.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
EnemyAttack::EnemyAttack(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	m_pTransform = new Transform();

	m_pAnimeNumber[0] = 1;

	m_pCollider = new SphereCollider(VECTOR3(0, 70, 0), 50, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

	SetObjectType(Object::TYPE_PLAYER_ATTACK);

	m_pTransform->rot.y = 0;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
EnemyAttack::~EnemyAttack()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT EnemyAttack::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void EnemyAttack::Release(void)
{
	delete m_pModel;
	delete m_pFrame;
	delete m_pAnimeNumber;

	delete m_pTransform;

	delete	m_pCollider;

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void EnemyAttack::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* EnemyAttack::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void EnemyAttack::OnCollision(Collider* col)
{
	;
}