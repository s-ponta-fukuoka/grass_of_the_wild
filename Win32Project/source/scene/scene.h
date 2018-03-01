//=============================================================================
//
// scene.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#include "next_scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class Fade;
class ShaderManager;
class TextureManager;
class ModelManager;
class EffectManager;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Scene
{
public:
	//�R���X�g���N�^
	Scene() {};

	//�f�X�g���N�^
	virtual ~Scene() {};

	//������
	virtual HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		ModelManager* pModelManager,
		TextureManager* pTextureManager,
		EffectManager* pEffectManager) { return S_OK; };

	//�I��
	virtual void Release(void) = 0;

	//�X�V
	virtual void Update(void) = 0;

	//�`��
	virtual void Draw(void) = 0;
};

#endif