//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TREE_COLLIDER_H_
#define _TREE_COLLIDER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../terrain.h"

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
class TreeCollider : public Terrain
{
public:
	//コンストラクタ
	TreeCollider(
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	CollisionManager* pCollisionManager,
	VECTOR3 pos);

	//デストラクタ
	virtual ~TreeCollider();

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