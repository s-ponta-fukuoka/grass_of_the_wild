//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_attack_jump.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../collision/collision_manager.h"
#include "../../../shader/shader_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
PlayerAttackJump::PlayerAttackJump(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	m_pTransform = new Transform();

	m_pCollider = new SphereCollider(VECTOR3(0, 70, 0), 50, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

	SetObjectType(Object::TYPE_PLAYER_ATTACK_JUMP);

	m_pTransform->rot.y = 0;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
PlayerAttackJump::~PlayerAttackJump()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT PlayerAttackJump::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void PlayerAttackJump::Release(void)
{
	if (m_pCollider != NULL)
	{
		delete m_pCollider;
		m_pCollider = NULL;
	}

	if (m_pTransform != NULL)
	{
		delete m_pTransform;
		m_pTransform = NULL;
	}

	if (m_pModel != NULL)
	{
		delete m_pModel;
		m_pModel = NULL;
	}

	if (m_pFrame != NULL)
	{
		delete m_pFrame;
		m_pFrame = NULL;
	}

	if (m_pAnimeNumber != NULL)
	{
		delete m_pAnimeNumber;
		m_pAnimeNumber = NULL;
	}

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void PlayerAttackJump::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* PlayerAttackJump::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void PlayerAttackJump::OnCollision(Collider* col)
{
	;
}