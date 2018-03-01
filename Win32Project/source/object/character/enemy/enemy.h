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
class EnemyAttack;
class EnemyLife;
class MeshField;
class EffectManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Enemy : public Character
{
public:
	typedef enum
	{
		MODE_ATTACK,
		MODE_DEFENSE
	}MODE;

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
		MeshField* pMeshField,
		EffectManager* pEffectManager);

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

	VECTOR3 GetMove(void) { return m_move; }
	void SetMove(VECTOR3 move) { m_move = move; }

	MainCamera* GetCamera(void) { return m_pCamera; }

	void SetCompletionPosition(XMVECTOR CompletionPosition) { m_CompletionPosition = CompletionPosition; }

	void SetCompletionRot(XMVECTOR CompletionRot) { m_CompletionRot = CompletionRot; }

	XMVECTOR GetCompletionPosition(void) { return m_CompletionPosition; }

	XMVECTOR GetCompletionRot(void) { return m_CompletionRot; }

	Object::Transform* GetPlayerTransform(void) { return m_pPlayerTransform; }

	EnemyAttack* GetEnemyAttack(void) { return m_pEnemyAttack; }
	void SetEnemyAttack(EnemyAttack* pEnemyAttack) { m_pEnemyAttack = pEnemyAttack; }

	int GetCounterCnt(void) { return m_nCounterCnt; }
	void SetCounterCnt(int nCounterCnt) { m_nCounterCnt = nCounterCnt; }

	MODE GetMode(void) { return m_eMode; }
	void SetMode(MODE eMode) { m_eMode = eMode; }

	VECTOR4* GetColor(void) { return m_pColor; }
	void SetColor(VECTOR4* pColor) { m_pColor = pColor; }

	EnemyLife* GetEnemyLife(void) { return m_pEnemyLife; }

	EffectManager* GetEffectManager(void) { return m_pEffectManager; }

	EnemyManager* GetEnemyManager(void) { return m_pEnemyManager; }

private:

	EnemyPattern*		m_pEnemyPattern;

	EffectManager*		m_pEffectManager;

	bool				m_bUse;

	Renderer**			m_pRenderer;

	Renderer**			m_pShadowRenderer;

	RenderManager*		m_pRenderManager;

	CollisionManager*	m_pCollisionManager;

	SphereCollider*		m_pCollider;

	EnemyManager*		m_pEnemyManager;

	EnemyAttack*		m_pEnemyAttack;

	EnemyLife*			m_pEnemyLife;

	ModelManager*		m_pModelManager;

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;

	Object::Transform*	m_pPlayerTransform;

	MeshField*			m_pMeshField;

	VECTOR3				m_oldPos;

	int				    m_nTime;

	MODE				m_eMode;

	int					m_nCounterCnt;

	VECTOR4*			m_pColor;
};

#endif