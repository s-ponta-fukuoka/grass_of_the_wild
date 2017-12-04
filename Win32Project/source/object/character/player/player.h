//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

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

	//�R���X�g���N�^
	Player(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager* pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera* pCamera);

	//�f�X�g���N�^
	virtual ~Player();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�o�b�t�@�쐬
	void MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh);

private:
	//�L�[�{�[�h����
	void InputOperation(void);

	MainCamera*			m_pCamera;

	VECTOR3				m_move;

	XMVECTOR			m_CompletionPosition;

	XMVECTOR			m_CompletionRot;
};

#endif