//=============================================================================
//
// [scene2D.cpp]

// Author : shota fukuoka
//
//=============================================================================
#include "game_over.h"
#include "../../../app/app.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PRESS_START_VERTEX (4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
GameOver::GameOver(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( -0.8f, 0.3f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_size = VECTOR3( 1.5f, 0.5f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/gameover.png", pTextureManager);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		PRESS_START_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE));
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
GameOver::~GameOver()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void GameOver::Release(void)
{
	Canvas::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void GameOver::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void GameOver::MakeVertex(ID3D11Device* pDevice)
{
	//四角形

	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_vertex.position.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,0),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(1,0),
		VECTOR3(m_vertex.position.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,1),
		VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y - m_size.y,m_vertex.position.z),  VECTOR4(1,1,1,1), VECTOR2(1,1)
	};
	
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * PRESS_START_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return;
	}
}