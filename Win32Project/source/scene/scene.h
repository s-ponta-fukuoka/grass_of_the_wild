//=============================================================================
//
// scene.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#include "next_scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class Fade;
class ShaderManager;
class TextureManager;
class ModelManager;
class EffectManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Scene
{
public:
	//コンストラクタ
	Scene() {};

	//デストラクタ
	virtual ~Scene() {};

	//初期化
	virtual HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		ModelManager* pModelManager,
		TextureManager* pTextureManager,
		EffectManager* pEffectManager) { return S_OK; };

	//終了
	virtual void Release(void) = 0;

	//更新
	virtual void Update(void) = 0;

	//描画
	virtual void Draw(void) = 0;
};

#endif