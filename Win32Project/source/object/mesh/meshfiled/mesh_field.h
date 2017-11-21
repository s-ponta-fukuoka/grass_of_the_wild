//=============================================================================
//
// mesh_filed.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

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

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class MeshField : public Mesh
{
public:
	MeshField( RenderManager* pRenderManager,
				ShaderManager* pShaderManager,
				TextureManager* pTextureManager,
				AppRenderer::Constant* pConstant,
				AppRenderer::Constant* pLightCameraConstant);			//�R���X�g���N�^

	~MeshField( );			//�f�X�g���N�^

	HRESULT Init( void );	//����������

	void Release( void );	//�I������

	void Update( void );	//�X�V����

	void MakeVertex(void);

	float GetHeight(VECTOR3 Position);

private:
	VECTOR3				m_size;
	int					m_nNumVertex;
	int					m_nNumAllPolygon;
	VECTOR3				m_NumPolygon;
	VECTOR3				m_Polygon[15000];
	int					m_NumVertexPolygon;
};

#endif