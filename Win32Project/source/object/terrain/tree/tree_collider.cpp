//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "tree_collider.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../collision/collision_manager.h"
#include "../../../shader/shader_manager.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
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
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
TreeCollider::~TreeCollider()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//������
///////////////////////////////////////////////////////////////////////////////
HRESULT TreeCollider::Init(void)
{
	Terrain::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//�I��
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
//�X�V
///////////////////////////////////////////////////////////////////////////////
void TreeCollider::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//�R���C�_�[�擾
///////////////////////////////////////////////////////////////////////////////
SphereCollider* TreeCollider::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//�����蔻��
///////////////////////////////////////////////////////////////////////////////
void TreeCollider::OnCollision(Collider* col)
{
}