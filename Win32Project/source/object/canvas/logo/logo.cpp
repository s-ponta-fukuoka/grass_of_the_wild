//=============================================================================
//
// [scene2D.cpp]

// Author : shota fukuoka
//
//=============================================================================
#include "logo.h"
#include "../../../app/app.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LOGO_VERTEX (4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
Logo::Logo(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( -0.9f, 0.9f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// �|���S���̈ʒu

	m_size = VECTOR3( 1.2f, 0.5f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/title.png", pTextureManager);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		LOGO_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE));
}

///////////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
Logo::~Logo()
{
}

///////////////////////////////////////////////////////////////////////////////
// �I��
///////////////////////////////////////////////////////////////////////////////
void Logo::Release(void)
{
	Canvas::Release();
}

///////////////////////////////////////////////////////////////////////////////
// �X�V
///////////////////////////////////////////////////////////////////////////////
void Logo::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//���_�o�b�t�@
///////////////////////////////////////////////////////////////////////////////
void Logo::MakeVertex(ID3D11Device* pDevice)
{
	//�l�p�`

	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_vertex.position.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,0),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(1,0),
		VECTOR3(m_vertex.position.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,1),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y - m_size.y,m_vertex.position.z),  VECTOR4(1,1,1,1), VECTOR2(1,1)
	};
	
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * LOGO_VERTEX;
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