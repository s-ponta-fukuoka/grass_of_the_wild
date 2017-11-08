//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "shadow_map.h"
#include "../../app/app.h"

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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ShadowMap::ShadowMap()
{
	m_pVertexBuffer = NULL;
	m_pSampleLinear = NULL;
	m_pTexture = NULL;
	m_pConstantBuffer = NULL;
	m_pos = VECTOR3( 0.5f, 0.5f, 0.0f );			// �|���S���̈ʒu
	m_normal = VECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_size = VECTOR3( 0.3f, 0.3f, 0.0f );			// �|���S���̈ʒu
	m_tex = VECTOR2( 0.0f, 0.0f );			// �|���S���̈ʒu
	m_ofsettex = VECTOR2( 0.0f, 0.0f );			// �|���S���̈ʒu
	m_bLoadTexture = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ShadowMap::~ShadowMap()
{
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT ShadowMap::Init( void )
{
	//m_pRender = new CRenderer;

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�l�p�`
	AppRenderer::Vertex2D vertices[
	] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(1,1),
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
		return FALSE;


	///////////////////////////////////////////////////////////
	//�V���h�E�}�b�v�ݒ�
	///////////////////////////////////////////////////////////
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
	 		return false;
	 }

	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 2�����e�N�X�`���̐���
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &m_pStencilTexture2D);
	if (FAILED(hr))
	{
		return false;
	}

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(m_pTexture2D, &rtvDesc, &mpRTV);
	if (FAILED(hr))
	{
	  		return false;
	}
	

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hDepthStencilViewDesc.Flags = 0;
	hDepthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(m_pStencilTexture2D, &hDepthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = -1;  // ���ׂẴ~�b�v�}�b�v ���x��
	hr = pDevice->CreateShaderResourceView(m_pTexture2D,  &srDesc, &m_pTexture);  // �󂯎��ϐ�
	if (FAILED(hr))
	{
		return false;
	}

	// �r���[�|�[�g�̐ݒ�
	m_pView.TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	m_pView.TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	m_pView.Width = 2048;			// �r���[�|�[�g�̈�̕�
	m_pView.Height = 2048;			// �r���[�|�[�g�̈�̍���
	m_pView.MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_pView.MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void ShadowMap::Release(void)
{
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void ShadowMap::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//�e�N�X�`��2D�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ConfigTexture2D(void);

///////////////////////////////////////////////////////////////////////////////
//�����_�[�^�[�Q�b�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ConfigRenderTargetView(void);

///////////////////////////////////////////////////////////////////////////////
//�X�e���V���^�[�Q�b�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ConfigDepthStencilView(void);

///////////////////////////////////////////////////////////////////////////////
//�V�F�[�_�[���\�[�X�r���[�ݒ�
///////////////////////////////////////////////////////////////////////////////
void ConfigShaderResourceView(void);

//�r���[�|�[�g�ݒ�
void ConfigViewPort(void);