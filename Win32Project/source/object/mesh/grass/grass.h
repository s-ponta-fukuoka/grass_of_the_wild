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

#define GRASS_MAX (100000)

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class MainCamera;
class MeshField;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Grass : public Mesh
{
public:
	//�萔
	struct Constant
	{
		XMMATRIX view;
		XMMATRIX projection;
		//XMFLOAT3 posEye;
		float time;
		XMFLOAT3 posPlayer;
	};

	Grass( RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, 
		MeshField* pMeshField,
		MainCamera *pCamera, 
		VECTOR3 pos,
		Object::Transform* pPlayerTransform);			//�R���X�g���N�^

	~Grass( );			//�f�X�g���N�^

	HRESULT Init( void );	//����������

	void Release( void );	//�I������

	void Update( void );	//�X�V����

	void MakeVertex(void);

private:
	MainCamera* m_pCamera;
	VECTOR3 m_size;
	MeshField* m_pMeshField;
	ID3D11Buffer* m_pInstanceBuffer;
};

#endif