//=============================================================================
//
// enemy.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../character.h"
#include "../../../model/model_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
class ModelManager;
class MainCamera;
class EnemyManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Enemy : public Character
{
public:
	//コンストラクタ
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
		EnemyManager* pEnemyManager);

	//デストラクタ
	virtual ~Enemy();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//バッファ作成
	void MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh);

	//あたり判定
	void OnCollision(Collider* col);

	//コライダー取得
	SphereCollider* GetSphereCollider(void);

private:

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
};

#endif