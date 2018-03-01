//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _PRESS_START_H_
#define _PRESS_START_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PressStart : public Canvas
{
public:
	//�R���X�g���N�^
	PressStart(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//�f�X�g���N�^
	virtual ~PressStart( );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�o�b�t�@
	void MakeVertex(ID3D11Device* pDevice);

private:
	VECTOR3							m_size;

	bool							m_bUse;
};

#endif