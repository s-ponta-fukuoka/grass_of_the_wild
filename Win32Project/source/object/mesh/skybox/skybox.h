//=============================================================================
//
// [scene2D.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

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
class CPlayer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class SkyBox : public Mesh
{
public:
	SkyBox(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant);			//�R���X�g���N�^

	virtual ~SkyBox();			//�f�X�g���N�^

	void Release(void);	//�I������

	void Update(void);	//�X�V����
	
	void MakeVertex(void);

private:
	VECTOR3				m_pos;
	ID3D11Buffer* m_pVertexBuffer;
	VECTOR3				m_size;
};

#endif