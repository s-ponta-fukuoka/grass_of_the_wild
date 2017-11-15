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
#include "../texture/texture_manager.h"
#include "../model/model.h"

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
	void Release(void);

	//�`��
	virtual void Draw(void) = 0;

protected:
	//�萔�o�b�t�@�ݒ�
	void ConfigConstantBuffer(UINT ByteWidth);

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
		ID3D11ShaderResourceView* pShadowMap,
		Object::Transform* pTransform,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightConstant,
		int	nNumVertexPolygon,
		D3D_PRIMITIVE_TOPOLOGY ePolygon,
		VertexShader::VERTEX_TYPE eVsType,
		PixelShader::PIXEL_TYPE ePsType);

	//�f�X�g���N�^
	virtual ~MeshRenderer();

	//�I��
	void Release(void);

	//�`��
	void Draw(void);

private:
	AppRenderer::Constant*				m_pConstant;

	AppRenderer::Constant*				m_pLightConstant;

	int									m_nNumVertexPolygon;

	D3D_PRIMITIVE_TOPOLOGY				m_ePolygon;

	ID3D11ShaderResourceView*			m_pShadowMap;
};

//-----------------------------------------------------------------------------
// �X�L�j�[�h�����_���[
//-----------------------------------------------------------------------------
class SkinnedMeshRenderer : public Renderer
{
public:
	//�R���X�g���N�^
	SkinnedMeshRenderer(ID3D11Buffer* pVertexBuffer,
		ID3D11Buffer* pIndexBuffer,
		ShaderManager* pShaderManager,
		ID3D11ShaderResourceView* pTexture,
		ID3D11ShaderResourceView* pShadowMap,
		Object::Transform* pTransform,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightConstant,
		int	nNumVertexPolygon,
		int* pFrame,
		int* pAnimeNumber,
		D3D_PRIMITIVE_TOPOLOGY ePolygon,
		VertexShader::VERTEX_TYPE eVsType,
		PixelShader::PIXEL_TYPE ePsType,
		SkinMeshModel::Cluster*	pCluster,
		SkinMeshModel::Mesh mesh);

	//�f�X�g���N�^
	virtual ~SkinnedMeshRenderer();

	//�I��
	void Release(void);

	//�`��
	void Draw(void);

private:
	AppRenderer::Constant*				m_pConstant;

	AppRenderer::Constant*				m_pLightConstant;

	int									m_nNumVertexPolygon;

	int*								m_pFrame;

	int*								m_pAnimeNumber;

	D3D_PRIMITIVE_TOPOLOGY				m_ePolygon;

	ID3D11ShaderResourceView*			m_pShadowMap;

	SkinMeshModel::Cluster*				m_pCluster;

	SkinMeshModel::Mesh					m_mesh;
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
	void Release(void);

	//�`��
	void Draw(void);

private:
	int									m_nNumVertexPolygon;

	D3D_PRIMITIVE_TOPOLOGY				m_ePolygon;
};

#endif