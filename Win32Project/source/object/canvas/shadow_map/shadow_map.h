//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

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
class ShadowMap : public Canvas
{
public:
	//�R���X�g���N�^
	ShadowMap(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//�f�X�g���N�^
	virtual ~ShadowMap( );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�o�b�t�@
	void MakeVertex(ID3D11Device* pDevice);

	//�e�N�X�`��2D�ݒ�
	void ConfigTexture2D(ID3D11Device* pDevice);

	//�����_�[�^�[�Q�b�g�ݒ�
	void ConfigRenderTargetView(ID3D11Device* pDevice);

	//�X�e���V���^�[�Q�b�g�ݒ�
	void ConfigDepthStencilView(ID3D11Device* pDevice);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	void ConfigShaderResourceView(ID3D11Device* pDevice);

	//�r���[�|�[�g�ݒ�
	void ConfigViewPort(void);

private:
	VECTOR3							m_size;

	ID3D11BlendState*				m_pBlendState;

	D3D11_VIEWPORT					m_pView;

	ID3D11DepthStencilView*			m_pDepthStencilView;

	ID3D11RenderTargetView*			m_pRenderTargetView;

	ID3D11ShaderResourceView*		m_pTexture;

	ID3D11Texture2D*				m_pTexture2D;

	ID3D11Texture2D*				m_pStencilTexture2D;
};

#endif