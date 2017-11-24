//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include <stdio.h>
#include <tchar.h>
#include "grass.h"
#include "../../../renderer/render_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GRASS_VERTEX	(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
Grass::Grass(RenderManager* pRenderManager, ShaderManager* pShaderManager, TextureManager* pTextureManager, AppRenderer::Constant* pConstant, AppRenderer::Constant* pLightCameraConstant)
{
	m_pVertexBuffer = NULL;

	m_pIndexBuffer = NULL;

	m_vertex.position = VECTOR3(0.0f, 0.0f, 0.0f);
	m_vertex.tex = VECTOR2(0.0f, 0.0f);
	m_size = VECTOR3(20, 80, 0);
	m_vertex.normal = VECTOR3(0, 0, -1);

	MakeVertex();

	Texture* pTexture = new Texture("resource/sprite/NULL.jpg", pTextureManager);


	pRenderManager->AddRenderer(new MeshRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		pRenderManager->GetShadowTexture(),
		m_pTransform,
		pConstant,
		pLightCameraConstant,
		GRASS_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_GRASS,
		GeometryShader::GS_GRASS,
		PixelShader::PS_NORMAL));

	pRenderManager->AddShadowRenderer(new MeshRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		NULL,
		m_pTransform,
		pLightCameraConstant,
		NULL,
		GRASS_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_NORMAL,
		GeometryShader::GS_GRASS,
		PixelShader::PS_SHADOW));

	delete[] pTexture;
	pTexture = NULL;

	m_pTransform->position.y = 100;
}

//=============================================================================
// デストラクタ
//=============================================================================
Grass::~Grass()
{
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT Grass::Init( void )
{
	m_vertex.position = VECTOR3(-4000.0f, 0.0f, 4000.0f);
	m_vertex.tex = VECTOR2(0.0f, 0.0f);
	m_size = VECTOR3(80, 0, 80);
	m_vertex.normal = VECTOR3(0, 1, 0);

	MakeVertex();


	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void Grass::Release(void)
{
	Mesh::Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void Grass::Update( void )
{

}

void Grass::MakeVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	AppRenderer::Vertex3D* vertices = new AppRenderer::Vertex3D[GRASS_VERTEX];

	vertices[0].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
	vertices[1].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
	vertices[2].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
	vertices[3].position = VECTOR3(m_vertex.position.x, m_vertex.position.y, m_vertex.position.z);
	

	vertices[0].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	vertices[1].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	vertices[2].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	vertices[3].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);

	vertices[0].color = VECTOR4(1,0,0,1);
	vertices[1].color = VECTOR4(1,0,0,1);
	vertices[2].color = VECTOR4(1,0,0,1);
	vertices[3].color = VECTOR4(1,0,0,1);

	vertices[0].tex = VECTOR2(0,0);
	vertices[1].tex = VECTOR2(1,0);
	vertices[2].tex = VECTOR2(0,1);
	vertices[3].tex = VECTOR2(1,1);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex3D) * GRASS_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	delete[] vertices;
}