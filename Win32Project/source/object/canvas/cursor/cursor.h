//=============================================================================
//
// cursor.h
// Author : shota fukuoka
//
//=============================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

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

class PauseMenu;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Cursor : public Canvas
{
public:
	//�R���X�g���N�^
	Cursor(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		PauseMenu*	pPauseMenu);

	//�f�X�g���N�^
	virtual ~Cursor( );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�o�b�t�@
	void MakeVertex(ID3D11Device* pDevice);

	//�Q�[���ɖ߂�t���O
	bool GetBackGame(void) { return m_bBackGame; }

private:
	VECTOR3 *						m_pos;

	PauseMenu*						m_pPauseMenu;

	VECTOR3							m_size;

	VECTOR3							m_sizeMin;

	RenderManager*				m_pRenderManager;

	Renderer*						m_pRenderer;

	bool							m_bUse;

	bool							m_bBackGame;

	int								m_nWaitTime;
};

#endif