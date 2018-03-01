//=============================================================================
//
// tree.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../terrain.h"
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
class TreeManager;
class MeshField;
class TreeCollider;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Tree : public Terrain
{
public:
	//コンストラクタ
	Tree(
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
		TreeManager* pTreeManager,
		MeshField* pMeshField);

	//デストラクタ
	virtual ~Tree();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager);

	//バッファ作成
	void MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh);

	//あたり判定
	void OnCollision(Collider* col);

	//コライダー取得
	SphereCollider* GetSphereCollider(void);

	//VECTOR3 GetMoveVector(void) { return m_moveVector; }
	//void SetMoveVector(VECTOR3 move) { m_moveVector = move; }

	VECTOR3 GetMove(void) { return m_move; }
	void SetMove(VECTOR3 move) { m_move = move; }

	MainCamera* GetCamera(void) { return m_pCamera; }

	void SetCompletionPosition(XMVECTOR CompletionPosition) { m_CompletionPosition = CompletionPosition; }

	void SetCompletionRot(XMVECTOR CompletionRot) { m_CompletionRot = CompletionRot; }

	XMVECTOR GetCompletionPosition(void) { return m_CompletionPosition; }

	XMVECTOR GetCompletionRot(void) { return m_CompletionRot; }

	//PlayerLife *GetPlayerLife(void) { return m_pPlayerLife; }

private:

	ID3D11Buffer * m_pInstanceBuffer;

	bool				m_bUse;

	MeshField*			m_pMeshField;

	Renderer**			m_pRenderer;

	Renderer**			m_pShadowRenderer;

	RenderManager*		m_pRenderManager;

	CollisionManager*	m_pCollisionManager;

	SphereCollider*		m_pCollider;

	TreeManager*		m_pTreeManager;

	ModelManager*		m_pModelManager;

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;

	TreeCollider*		m_pTreeCollider[20];
};

#endif