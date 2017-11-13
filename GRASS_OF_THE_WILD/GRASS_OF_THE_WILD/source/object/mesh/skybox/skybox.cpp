//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "skybox.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SKY_BOX_VERTEX (34)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
SkyBox::SkyBox(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant)
{
	m_pVertexBuffer = NULL;
	m_pos = VECTOR3(-10000.0f, 10000.0f, 10000.0f);
	m_vertex.normal = VECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_size = VECTOR3(20000.0f, 20000.0f, 100.0f);			// �|���S���̈ʒu

	MakeVertex();

	Texture* pTexture = new Texture("resource/sprite/skybox_000.png", pTextureManager);

	pRenderManager->AddRenderer(new MeshRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		NULL,
		m_pTransform,
		pConstant,
		NULL,
		SKY_BOX_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_3D,
		PixelShader::PS_3D));

	delete[] pTexture;
	pTexture = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
SkyBox::~SkyBox()
{
}

void SkyBox::MakeVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�l�p�`
	AppRenderer::Vertex3D vertices[] =
	{
		VECTOR3(m_pos.x + m_size.x, m_pos.y,-m_pos.z), VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.75,0.333),
		VECTOR3(m_pos.x, m_pos.y,-m_pos.z),VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(1,0.333),
		VECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1) , VECTOR2(0.75,0.666),
		VECTOR3(m_pos.x, m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(1,0.666),
		VECTOR3(m_pos.x, m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(1,0.666),

		VECTOR3(m_pos.x,m_pos.y,m_pos.z),VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.25,0.333),
		VECTOR3(m_pos.x,m_pos.y,m_pos.z),VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.25,0.333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.5,0.333333333),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.25,0.666666666),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z),  VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.5,0.666666666),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z),  VECTOR3(0,0,0), VECTOR4(0,0,0,1), VECTOR2(0.5,0.666666666),
		
		VECTOR3(m_pos.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0,0.333333333),
		VECTOR3(m_pos.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0,0.333333333),
		VECTOR3(m_pos.x,m_pos.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.333333333),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0,0.666666666),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z),  VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.666666666),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z),  VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.666666666),
		
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.333333333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.333333333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.75,0.333333333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.666666666),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(0.75,0.666666666),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(0.75,0.666666666),
		
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.666),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.666),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.666),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.999),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(0.5,0.999),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1), VECTOR2(0.5,0.999),
		
		VECTOR3(m_pos.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.0),
		VECTOR3(m_pos.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,-m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.0),
		VECTOR3(m_pos.x,m_pos.y,m_pos.z), VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.25,0.333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z),  VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.333),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z),  VECTOR3(0,0,0),VECTOR4(0,0,0,1),VECTOR2(0.5,0.333),
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex3D) * SKY_BOX_VERTEX;
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


//=============================================================================
// �|���S���̏I������
//=============================================================================
void SkyBox::Release(void)
{
	Mesh::Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void SkyBox::Update(void)
{
}