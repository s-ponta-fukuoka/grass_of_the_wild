//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

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
class PauseMenu : public Canvas
{
public:
	typedef enum
	{
		PAUSE_MENU_BACK_GAME,
		PAUSE_MENU_RETRY,
		PAUSE_MENU_TITLE,
		PAUSE_MENU_MAX
	}PAUSE_MENU;

	//�R���X�g���N�^
	PauseMenu(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//�f�X�g���N�^
	virtual ~PauseMenu( );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�o�b�t�@
	void MakeVertex(ID3D11Device* pDevice);

	VECTOR3 *GetPosition(PAUSE_MENU ePauseMenu);

	void SetPauseMenu(PAUSE_MENU ePauseMenu) { m_ePauseMenu = ePauseMenu; }
	PAUSE_MENU GetPauseMenu(void) { return m_ePauseMenu; }

private:
	VECTOR3 *						m_pos[PAUSE_MENU_MAX];

	VECTOR3							m_size;

	PAUSE_MENU						m_ePauseMenu;

	RenderManager*				m_pRenderManager;

	Renderer*						m_pRenderer;
};

#endif