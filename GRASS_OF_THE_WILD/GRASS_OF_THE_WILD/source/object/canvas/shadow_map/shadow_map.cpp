//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "shadow_map.h"
#include "../../../app/app.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SHADOW_MAP_VERTEX (4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
ShadowMap::ShadowMap(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pVertexBuffer = NULL;

	m_pTexture = NULL;

	m_vertex.position = VECTOR3( 0.5f, 0.5f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// �|���S���̈ʒu

	m_size = VECTOR3( 0.3f, 0.3f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	ConfigTexture2D(pDevice);

	ConfigRenderTargetView(pDevice);

	ConfigDepthStencilView(pDevice);

	ConfigShaderResourceView(pDevice);

	ConfigViewPort();

	pRenderManager->SetShadowViewPort(m_pView);

	pRenderManager->SetShadowDepthStencilView(m_pDepthStencilView);

	pRenderManager->SetShadowRenderTargetVie(m_pRenderTargetView);

	pRenderManager->SetShadowTexture(m_pTexture);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		m_pTexture,
		SHADOW_MAP_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D));
}

///////////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
ShadowMap::~ShadowMap()
{
}

///////////////////////////////////////////////////////////////////////////////
// �I��
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::Release(void)
{
	Canvas::Release();
}

///////////////////////////////////////////////////////////////////////////////
// �X�V
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//���_�o�b�t�@
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::MakeVertex(ID3D11Device* pDevice)
{
	//�l�p�`
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_vertex.position.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(m_vertex.color.x,m_vertex.color.y,m_vertex.color.z,m_vertex.color.w), VECTOR2(0,0),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(m_vertex.color.x,m_vertex.color.y,m_vertex.color.z,m_vertex.color.w), VECTOR2(1,0),
		VECTOR3(m_vertex.position.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(m_vertex.color.x,m_vertex.color.y,m_vertex.color.z,m_vertex.color.w), VECTOR2(0,1),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(m_vertex.color.x,m_vertex.color.y,m_vertex.color.z,m_vertex.color.w), VECTOR2(1,1),
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * SHADOW_MAP_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�e�N�X�`��2D�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::ConfigTexture2D(ID3D11Device* pDevice)
{

	// 2�����e�N�X�`���̐ݒ�
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = 2048;
	texDesc.Height = 2048;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2�����e�N�X�`���̐���
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &m_pTexture2D);
	if (FAILED(hr))
	{
		return;
	}

	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 2�����e�N�X�`���̐���
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &m_pStencilTexture2D);
	if (FAILED(hr))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�����_�[�^�[�Q�b�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::ConfigRenderTargetView(ID3D11Device* pDevice)
{
	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	HRESULT hr = pDevice->CreateRenderTargetView(m_pTexture2D, &rtvDesc, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�X�e���V���^�[�Q�b�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::ConfigDepthStencilView(ID3D11Device* pDevice)
{
	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hDepthStencilViewDesc.Flags = 0;
	hDepthStencilViewDesc.Texture2D.MipSlice = 0;
	HRESULT hr = pDevice->CreateDepthStencilView(m_pStencilTexture2D, &hDepthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�V�F�[�_�[���\�[�X�r���[�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::ConfigShaderResourceView(ID3D11Device* pDevice)
{
	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��
	HRESULT hr = pDevice->CreateShaderResourceView(m_pTexture2D, &srDesc, &m_pTexture);  // �󂯎��ϐ�
	if (FAILED(hr))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�r���[�|�[�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ShadowMap::ConfigViewPort(void)
{
	// �r���[�|�[�g�̐ݒ�
	m_pView.TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	m_pView.TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	m_pView.Width = 2048;			// �r���[�|�[�g�̈�̕�
	m_pView.Height = 2048;			// �r���[�|�[�g�̈�̍���
	m_pView.MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_pView.MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l
}