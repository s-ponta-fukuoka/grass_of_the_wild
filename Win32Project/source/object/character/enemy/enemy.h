//=============================================================================
//
// enemy.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "../character.h"
#include "../../../model/model_manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
class ModelManager;
class MainCamera;
class EnemyManager;
class EnemyPattern;
class MeshField;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Enemy : public Character
{
public:
	//�R���X�g���N�^
	Enemy(
		VECTOR3 position,
		VECTOR3 rot,
		VECTOR3 scl,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager* pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera* pCamera,
		CollisionManager* pCollisionManager,
		EnemyManager* pEnemyManager,
		Object::Transform* pPlayerTransform,
		MeshField* pMeshField);

	//�f�X�g���N�^
	virtual ~Enemy();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager);

	//�o�b�t�@�쐬
	void MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh);

	//�����蔻��
	void OnCollision(Collider* col);

	//�R���C�_�[�擾
	SphereCollider* GetSphereCollider(void);

	//�G�l�~�[�X�e�[�g�ύX
	void ChangeEnemyPattern(EnemyPattern* pEnemyPattern);

	//VECTOR3 GetMoveVector(void) { return m_moveVector; }
	//void SetMoveVector(VECTOR3 move) { m_moveVector = move; }

	VECTOR3 GetMove(void) { return m_move; }
	void SetMove(VECTOR3 move) { m_move = move; }

	MainCamera* GetCamera(void) { return m_pCamera; }

	void SetCompletionPosition(XMVECTOR CompletionPosition) { m_CompletionPosition = CompletionPosition; }

	void SetCompletionRot(XMVECTOR CompletionRot) { m_CompletionRot = CompletionRot; }

	XMVECTOR GetCompletionPosition(void) { return m_CompletionPosition; }

	XMVECTOR GetCompletionRot(void) { return m_CompletionRot; }

	Object::Transform* GetPlayerTransform(void) { return m_pPlayerTransform; }

	//PlayerLife *GetPlayerLife(void) { return m_pPlayerLife; }

private:

	EnemyPattern*		m_pEnemyPattern;

	bool				m_bUse;

	Renderer**			m_pRenderer;

	Renderer**			m_pShadowRenderer;

	RenderManager*		m_pRenderManager;

	CollisionManager*	m_pCollisionManager;

	SphereCollider*		m_pCollider;

	EnemyManager*		m_pEnemyManager;

	ModelManager*		m_pModelManager;

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;

	Object::Transform*	m_pPlayerTransform;

	MeshField*			m_pMeshField;

	VECTOR3				m_oldPos;

	int				    m_nTime;
};

#endif