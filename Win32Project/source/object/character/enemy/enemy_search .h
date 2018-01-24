//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _ENEMY_SEARCH_H_
#define _ENEMY_SEARCH_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class CollisionManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class EnemySearch : public Character
{
public:
	//コンストラクタ
	EnemySearch(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager);

	//デストラクタ
	virtual ~EnemySearch();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//あたり判定
	void OnCollision(Collider* col);

	//コライダー取得
	SphereCollider* GetSphereCollider(void);

private:
	SphereCollider*		m_pCollider;
};

#endif