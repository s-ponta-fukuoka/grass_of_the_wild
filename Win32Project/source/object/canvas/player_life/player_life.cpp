//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "player_life.h"
#include "../../../app/app.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_LIFE_VERTEX (16)
#define PLAYER_LIFE_MAX (5)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
PlayerLife::PlayerLife(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( -0.95f, 0.95f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_size = VECTOR3( 0.03f, 0.05f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/player_life.png", pTextureManager);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		PLAYER_LIFE_VERTEX * PLAYER_LIFE_MAX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE));
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
PlayerLife::~PlayerLife()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void PlayerLife::Release(void)
{
	Canvas::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void PlayerLife::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void PlayerLife::MakeVertex(ID3D11Device* pDevice)
{
	//四角形
	AppRenderer::Vertex2D *vertices = new AppRenderer::Vertex2D[PLAYER_LIFE_VERTEX * PLAYER_LIFE_MAX];

	for (int i = 0; i < 5; i++)
	{
		vertices[16 * i + 0].position = VECTOR3(m_vertex.position.x, m_vertex.position.y, m_vertex.position.z);
		vertices[16 * i + 1].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
		vertices[16 * i + 2].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 3].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);


		vertices[16 * i + 4].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 5].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 6].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 7].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y, m_vertex.position.z);

		vertices[16 * i + 8].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
		vertices[16 * i + 9].position = VECTOR3(m_vertex.position.x + m_size.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
		vertices[16 * i + 10].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 11].position = VECTOR3(m_vertex.position.x + m_size.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);

		vertices[16 * i + 12].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 13].position = VECTOR3(m_vertex.position.x + m_size.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 14].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y, m_vertex.position.z);
		vertices[16 * i + 15].position = VECTOR3(m_vertex.position.x + m_size.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y, m_vertex.position.z);

		m_vertex.position.x += 0.07;
	}
	m_vertex.position = VECTOR3(-0.95f, 0.95f, 0.0f);			// ポリゴンの位置
	for (int i = 0; i < 5; i++)
	{
		vertices[16 * i + 0].tex = VECTOR2(0, 0);
		vertices[16 * i + 1].tex = VECTOR2(0.5, 0);
		vertices[16 * i + 2].tex = VECTOR2(0, 0.5);
		vertices[16 * i + 3].tex = VECTOR2(0.5, 0.5);

		vertices[16 * i + 4].tex = VECTOR2(0, 0.5);
		vertices[16 * i + 5].tex = VECTOR2(0.5, 0.5);
		vertices[16 * i + 6].tex = VECTOR2(0, 1);
		vertices[16 * i + 7].tex = VECTOR2(0.5, 1);

		vertices[16 * i + 8].tex = VECTOR2(0.5, 0);
		vertices[16 * i + 9].tex = VECTOR2(1, 0);
		vertices[16 * i + 10].tex = VECTOR2(0.5, 0.5);
		vertices[16 * i + 11].tex = VECTOR2(1, 0.5);

		vertices[16 * i + 12].tex = VECTOR2(0.5, 0.5);
		vertices[16 * i + 13].tex = VECTOR2(1, 0.5);
		vertices[16 * i + 14].tex = VECTOR2(0.5, 1);
		vertices[16 * i + 15].tex = VECTOR2(1, 1);
	}

	for (int i = 0; i < 16 * 5; i++)
	{
		vertices[i].color = VECTOR4(1, 0, 0, 1);
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * PLAYER_LIFE_VERTEX * PLAYER_LIFE_MAX;
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