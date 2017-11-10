//=============================================================================
//
// directx�����_�����O���� [rendere.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "app_renderer.h"
#include "../object/object.h"
#include "../shader/shader.h"
#include "../texture/texturemanager.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Renderer
{
public:
	//�R���X�g���N�^
	Renderer();

	//�f�X�g���N�^
	virtual ~Renderer();

	//�I��
	virtual void Release(void) = 0;

	//�`��
	virtual void Draw(void) = 0;

protected:
	//�萔�o�b�t�@�ݒ�
	void ConfigConstantBuffer(void);

	//�T���v���[�X�e�[�g�ݒ�
	void ConfigSamplerState(void);

	ID3D11BlendState*					m_pBlendState;

	ID3D11SamplerState*					m_pSampleLinear;

	ID3D11ShaderResourceView*			m_pTexture;

	ID3D11Buffer*						m_pVertexBuffer;

	ID3D11Buffer*						m_pConstantBuffer;

	ID3D11Buffer*						m_pIndexBuffer;

	VertexShader*						m_pVertexShader;

	PixelShader*						m_pPixelShader;

	Object::Transform*					m_pTransform;

};

//-----------------------------------------------------------------------------
// ���b�V�������_���[
//-----------------------------------------------------------------------------
class MeshRenderer : public Renderer
{
public:
	//�R���X�g���N�^
	MeshRenderer(ID3D11Buffer* pVertexBuffer,
		ID3D11Buffer* pIndexBuffer,
		ShaderManager* pShaderManager,
		ID3D11ShaderResourceView* pTexture,
		Object::Transform* pTransform,
		AppRenderer::Constant* pConstant,
		int	nNumVertexPolygon,
		D3D_PRIMITIVE_TOPOLOGY ePolygon,
		VertexShader::VERTEX_TYPE eVsType,
		PixelShader::PIXEL_TYPE ePsType);

	//�f�X�g���N�^
	virtual ~MeshRenderer();

	//�I��
	void Release(void) {};

	//�`��
	void Draw(void);

private:
	AppRenderer::Constant*				m_pConstant;

	int									m_nNumVertexPolygon;

	D3D_PRIMITIVE_TOPOLOGY				m_ePolygon;
};

//-----------------------------------------------------------------------------
// �X�L�j�[�h�����_���[
//-----------------------------------------------------------------------------
class SkinnedMeshRenderer : public Renderer
{
public:
	//�R���X�g���N�^
	SkinnedMeshRenderer();

	//�f�X�g���N�^
	virtual ~SkinnedMeshRenderer();

	//�I��
	void Release(void) {};

	//�`��
	void Draw(void);
};

//-----------------------------------------------------------------------------
// �L�����o�X�����_���[
//-----------------------------------------------------------------------------
class CanvasRenderer : public Renderer
{
public:
	//�R���X�g���N�^
	CanvasRenderer(ID3D11Buffer* pVertexBuffer,
		ID3D11Buffer* pIndexBuffer,
		ShaderManager* pShaderManager,
		ID3D11ShaderResourceView* pTexture,
		int	nNumVertexPolygon,
		D3D_PRIMITIVE_TOPOLOGY ePolygon,
		VertexShader::VERTEX_TYPE eVsType,
		PixelShader::PIXEL_TYPE ePsType);

	//�f�X�g���N�^
	virtual ~CanvasRenderer();

	//�I��
	void Release(void) {};

	//�`��
	void Draw(void);

private:
	int									m_nNumVertexPolygon;

	D3D_PRIMITIVE_TOPOLOGY				m_ePolygon;
};

#endif