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
#include "../object.h"
#include "../../renderer/app_renderer.h"

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
class ShadowMap : public Object
{
public:
	//�R���X�g���N�^
	ShadowMap( );

	//�f�X�g���N�^
	virtual ~ShadowMap( );

	//������
	HRESULT Init( void );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�e�N�X�`��2D�ݒ�
	void ConfigTexture2D(void);

	//�����_�[�^�[�Q�b�g�ݒ�
	void ConfigRenderTargetView(void);

	//�X�e���V���^�[�Q�b�g�ݒ�
	void ConfigDepthStencilView(void);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	void ConfigShaderResourceView(void);

	//�r���[�|�[�g�ݒ�
	void ConfigViewPort(void);

protected:
	VECTOR3				m_pos;			// �|���S�����W
	VECTOR3				m_size;			// �T�C�Y
	VECTOR3				m_normal;
	VECTOR2				m_tex;			// �e�N�X�`�����W
	VECTOR2				m_ofsettex;		// �e�N�X�`���I�t�Z�b�g

private:
	ID3D11BlendState* m_pBlendState;
	D3D11_VIEWPORT m_pView;
	ID3D11DepthStencilView* m_pDepthStencilView; //�X�e���V���^�[�Q�b�g�r���[
	ID3D11Texture2D *m_pTexture2D;
	ID3D11Texture2D *m_pStencilTexture2D;
	ID3D11RenderTargetView *mpRTV;
	ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	static ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	ID3D11ShaderResourceView* m_pShadow;//�e�N�X�`���[
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;//�R���X�^���g�o�b�t�@
	bool m_bLoadTexture;
};

#endif