//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "enemy_life.h"
#include "../../../app/app.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_LIFE_VERTEX (4)
#define ENEMY_LIFE_MAX (1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
EnemyLife::EnemyLife(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pTransform = new Transform();

	m_nCoolTime = 0;

	m_nLife = ENEMY_LIFE_NUM;

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( 0.f, 0.f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_size = VECTOR3( 0.03f, 0.05f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/NULL.jpg", pTextureManager);

	//pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
	//	NULL,
	//	pShaderManager,
	//	pTexture->GetTexture(),
	//	ENEMY_LIFE_VERTEX * ENEMY_LIFE_MAX,
	//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	//	VertexShader::VS_2D,
	//	PixelShader::PS_2D,
	//	FALSE));
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
EnemyLife::~EnemyLife()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void EnemyLife::Release(void)
{
	Object::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void EnemyLife::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void EnemyLife::MakeVertex(ID3D11Device* pDevice)
{
	//四角形
	AppRenderer::Vertex2D *vertices = new AppRenderer::Vertex2D[ENEMY_LIFE_VERTEX * ENEMY_LIFE_MAX];

	vertices[0].position = VECTOR3(m_vertex.position.x, m_vertex.position.y, m_vertex.position.z);
	vertices[1].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
	vertices[2].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
	vertices[3].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);

	vertices[0].tex = VECTOR2(0, 0);
	vertices[1].tex = VECTOR2(0.5, 0);
	vertices[2].tex = VECTOR2(0, 0.5);
	vertices[3].tex = VECTOR2(0.5, 0.5);
	

	for (int i = 0; i < ENEMY_LIFE_VERTEX * ENEMY_LIFE_MAX; i++)
	{
		vertices[i].color = VECTOR4(1, 0.3, 0.3, 1);
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * ENEMY_LIFE_VERTEX * ENEMY_LIFE_MAX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return;
	}

	delete[] vertices;
}