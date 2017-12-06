//=============================================================================
//
// grass.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _GRASS_H_
#define _GRASS_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "../mesh.h"

//*****************************************************************************
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class MainCamera;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Grass : public Mesh
{
public:
	//�萔
	struct Constant
	{
		XMMATRIX world[100];
		XMMATRIX view;
		XMMATRIX projection;
	};

	Grass( RenderManager* pRenderManager,
				ShaderManager* pShaderManager,
				TextureManager* pTextureManager,
				AppRenderer::Constant* pConstant,
				AppRenderer::Constant* pLightCameraConstant, MainCamera *pCamera, int cnt);			//�R���X�g���N�^

	~Grass( );			//�f�X�g���N�^

	HRESULT Init( void );	//����������

	void Release( void );	//�I������

	void Update( void );	//�X�V����

	void MakeVertex(void);

private:
	MainCamera* m_pCamera;
	VECTOR3 m_size;
};

#endif