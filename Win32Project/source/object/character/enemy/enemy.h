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
class ModelManager;
class MainCamera;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Enemy : public Character
{
public:
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
		MainCamera* pCamera);

	//�f�X�g���N�^
	virtual ~Enemy();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�o�b�t�@�쐬
	void MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh);

private:

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;
};

#endif