//=============================================================================
//
// game_clear.h
// Author : shota fukuoka
//
//=============================================================================
#ifndef _GAME_CLEAR_H_
#define _GAME_CLEAR_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class GameClear : public Canvas
{
public:
	//�R���X�g���N�^
	GameClear(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//�f�X�g���N�^
	virtual ~GameClear( );

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