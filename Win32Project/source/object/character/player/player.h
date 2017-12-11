//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../character.h"
#include "../../../model/model_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (50)

class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;
class ModelManager;
class MainCamera;
class PlayerPattern;
class PlayerAttack;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Player : public Character
{
public:
	enum ANIME_STATE
	{
		ANIME_WAIT,
		ANIME_WALK,
		ANIME_ATTACK_001,
		ANIME_ATTACK_002,
	};

	//コンストラクタ
	Player(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager* pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera* pCamera,
		CollisionManager* pCollisionManager);

	//デストラクタ
	virtual ~Player();

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

	//プレイヤーステート変更
	void ChangePlayerPattern(PlayerPattern* pPlayerPattern);

	VECTOR3 GetMoveVector(void) { return m_moveVector; }
	void SetMoveVector(VECTOR3 move) { m_moveVector = move; }

	VECTOR3 GetMove(void) { return m_move; }
	void SetMove(VECTOR3 move) { m_move = move; }

	MainCamera* GetCamera(void) { return m_pCamera; }

	void SetCompletionPosition(XMVECTOR CompletionPosition) { m_CompletionPosition = CompletionPosition; }
	
	void SetCompletionRot(XMVECTOR CompletionRot) { m_CompletionRot = CompletionRot; }

	XMVECTOR GetCompletionPosition(void) { return m_CompletionPosition; }

	XMVECTOR GetCompletionRot(void) { return m_CompletionRot; }

private:

	VECTOR3				m_oldPos;

	SphereCollider*		m_pCollider;

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	VECTOR3				m_moveVector;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;

	PlayerPattern*		m_pPlayerPattern;

	PlayerAttack*		m_pPlayerAttack;
};

#endif