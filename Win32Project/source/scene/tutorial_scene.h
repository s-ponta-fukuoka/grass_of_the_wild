//=============================================================================
//
//  tutorial.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _TUTORIAL_SCENE_H_
#define _TUTORIAL_SCENE_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Tutorial : public Scene
{
public:
	Tutorial(){};
	virtual ~Tutorial(){};
	HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		ModelManager* pModelManager,
		TextureManager* pTextureManager,
		EffectManager* pEffectManager);
	void Release(void);
	void Update(void);
	void Draw(void);
};

#endif
