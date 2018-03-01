//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TREE_COLLIDER_H_
#define _TREE_COLLIDER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "../terrain.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class TreeCollider : public Terrain
{
public:
	//�R���X�g���N�^
	TreeCollider(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	CollisionManager* pCollisionManager,
	VECTOR3 pos);

	//�f�X�g���N�^
	virtual ~TreeCollider();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�����蔻��
	void OnCollision(Collider* col);

	//�R���C�_�[�擾
	SphereCollider* GetSphereCollider(void);

private:
	SphereCollider*		m_pCollider;
};

#endif