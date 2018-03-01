//=============================================================================
//
//  tutorial.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _TUTORIAL_SCENE_H_
#define _TUTORIAL_SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �N���X��`
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
