//=============================================================================
//
// directx�����_�����O���� [rendere.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _DEFERRED_RENDERER_H_
#define _DEFERRED_RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "app_renderer.h"
#include "../object/object.h"
#include "../shader/shader.h"
#include "../texture/texture_manager.h"
#include "../model/model.h"
#include "../object/camera/main_camera.h"

#define G_BUFFER (3)
//*****************************************************************************
//�\���̒�`
//*****************************************************************************

class ShaderManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class DeferredRenderer
{
public:
	//�R���X�g���N�^
	DeferredRenderer();

	//�f�X�g���N�^
	virtual ~DeferredRenderer( );

	//������
	HRESULT Init( HWND hWnd , ShaderManager* pShaderManager);

	//�I��
	void Release( void );

	//�`��
	void Draw(void);

	//�萔�o�b�t�@�ݒ�
	void ConfigConstantBuffer(UINT ByteWidth);

	//�T���v���[�X�e�[�g�ݒ�
	void ConfigSamplerState(void);

	//�u�����h�X�e�[�g�ݒ�
	void ConfigBlendState(BOOL bBlend);

	ID3D11RenderTargetView* GetRenderTargetView(const int nNumber) const { return m_pRenderTargetView[nNumber]; }
	ID3D11DepthStencilView* GetDepthStencilView(const int nNumber) const { return m_pDepthStencilView[nNumber]; }
	ID3D11ShaderResourceView* GetShaderResourceView(const int nNumber) const{ return m_pShaderResourceView[nNumber]; }

	ID3D11RenderTargetView* GetOnRenderTargetView(void) const { return m_pOnRenderTargetView; }
	ID3D11ShaderResourceView* GetShaderResourceView(void) const { return m_pOnRenderImage; }

private:
	//�J���[
	void CreateDiffuseBuffer(void);

	//�@��
	void CreateNormalBuffer(void);

	//�[�x
	void CreateDepthBuffer(void);

	//�e
	void CreateShadowBuffer(void);

	//�ŏI���ʉ摜
	void CreateOnRenderImage(void);

	ID3D11RenderTargetView* m_pRenderTargetView[G_BUFFER];

	ID3D11DepthStencilView*	 m_pDepthStencilView[G_BUFFER];

	ID3D11ShaderResourceView*	 m_pShaderResourceView[G_BUFFER];

	ID3D11RenderTargetView* m_pOnRenderTargetView;

	ID3D11ShaderResourceView*	 m_pOnRenderImage;

	ID3D11BlendState*					m_pBlendState;

	ID3D11SamplerState*					m_pSampleLinear;

	ID3D11Buffer*						m_pVertexBuffer;

	ID3D11Buffer*						m_pConstantBuffer;

	VertexShader*						m_pVertexShader;

	PixelShader*						m_pPixelShader;
};

#endif