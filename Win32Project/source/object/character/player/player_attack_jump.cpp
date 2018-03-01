//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_attack_jump.h"
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
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
PlayerAttackJump::~PlayerAttackJump()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//������
///////////////////////////////////////////////////////////////////////////////
HRESULT PlayerAttackJump::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//�I��
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
//�X�V
///////////////////////////////////////////////////////////////////////////////
void PlayerAttackJump::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//�R���C�_�[�擾
///////////////////////////////////////////////////////////////////////////////
SphereCollider* PlayerAttackJump::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//�����蔻��
///////////////////////////////////////////////////////////////////////////////
void PlayerAttackJump::OnCollision(Collider* col)
{
	;
}