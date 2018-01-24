//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "enemy_search .h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
EnemySearch::EnemySearch(
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
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
EnemySearch::~EnemySearch()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//������
///////////////////////////////////////////////////////////////////////////////
HRESULT EnemySearch::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//�I��
///////////////////////////////////////////////////////////////////////////////
void EnemySearch::Release(void)
{
	delete m_pModel;
	delete m_pFrame;
	delete m_pAnimeNumber;

	delete m_pTransform;

	delete	m_pCollider;

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//�X�V
///////////////////////////////////////////////////////////////////////////////
void EnemySearch::Update(void)
{
	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//�R���C�_�[�擾
///////////////////////////////////////////////////////////////////////////////
SphereCollider* EnemySearch::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//�����蔻��
///////////////////////////////////////////////////////////////////////////////
void EnemySearch::OnCollision(Collider* col)
{
	;
}