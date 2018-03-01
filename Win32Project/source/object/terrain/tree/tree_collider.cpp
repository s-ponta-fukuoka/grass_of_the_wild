//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "tree_collider.h"
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
TreeCollider::TreeCollider(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager,
	VECTOR3 pos)
{
	m_pTransform = new Transform();

	m_pCollider = new SphereCollider(pos, 100, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

	SetObjectType(Object::TYPE_TREE);

	m_pTransform->rot.y = 0;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
TreeCollider::~TreeCollider()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT TreeCollider::Init(void)
{
	Terrain::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void TreeCollider::Release(void)
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

	Terrain::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void TreeCollider::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* TreeCollider::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void TreeCollider::OnCollision(Collider* col)
{
}