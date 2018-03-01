//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "pause_menu.h"
#include "../../../app/app.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MENU_VERTEX (12)
#define PAUSE_MENU_SIZE_Y (0.1f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
PauseMenu::PauseMenu(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
	: m_pRenderer(NULL)
	, m_pRenderManager(NULL)
{
	m_pRenderManager = pRenderManager;

	for (int i = 0; i < PAUSE_MENU_MAX; i++)
	{
		m_pos[i] = new VECTOR3[4];
	}

	m_ePauseMenu = PAUSE_MENU::PAUSE_MENU_BACK_GAME;

	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( -0.85f, 0.65f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_size = VECTOR3( 0.4f, 0.2f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/pause_menu.png", pTextureManager);

	m_pRenderer = new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		PAUSE_MENU_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE);

	pRenderManager->AddRenderer(m_pRenderer);
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
PauseMenu::~PauseMenu()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void PauseMenu::Release(void)
{
	for (int i = 0; i < PAUSE_MENU_MAX; i++)
	{
		delete[] m_pos[i];
		m_pos[i] = NULL;
	}

	if (m_pRenderer != NULL)
	{
		m_pRenderManager->DeleteRenderer(m_pRenderer);
		m_pRenderer->Release();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	Canvas::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void PauseMenu::Update( void )
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void PauseMenu::MakeVertex(ID3D11Device* pDevice)
{
	//四角形
	AppRenderer::Vertex2D *vertices = new AppRenderer::Vertex2D[PAUSE_MENU_VERTEX];


	vertices[0].position = VECTOR3(m_vertex.position.x, m_vertex.position.y, m_vertex.position.z);
	vertices[1].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y, m_vertex.position.z);
	vertices[2].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y, m_vertex.position.z);
	vertices[3].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y, m_vertex.position.z);

	m_pos[0][0] = vertices[0].position;
	m_pos[0][1] = vertices[1].position;
	m_pos[0][2] = vertices[2].position;
	m_pos[0][3] = vertices[3].position;

	vertices[4].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y - PAUSE_MENU_SIZE_Y, m_vertex.position.z);
	vertices[5].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - PAUSE_MENU_SIZE_Y, m_vertex.position.z);
	vertices[6].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y - m_size.y - PAUSE_MENU_SIZE_Y, m_vertex.position.z);
	vertices[7].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y - PAUSE_MENU_SIZE_Y, m_vertex.position.z);

	m_pos[1][0] = vertices[4].position;
	m_pos[1][1] = vertices[5].position;
	m_pos[1][2] = vertices[6].position;
	m_pos[1][3] = vertices[7].position;

	vertices[8].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y - m_size.y - (PAUSE_MENU_SIZE_Y*2), m_vertex.position.z);
	vertices[9].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y - (PAUSE_MENU_SIZE_Y * 2), m_vertex.position.z);
	vertices[10].position = VECTOR3(m_vertex.position.x, m_vertex.position.y - m_size.y - m_size.y - m_size.y - (PAUSE_MENU_SIZE_Y * 2), m_vertex.position.z);
	vertices[11].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y - m_size.y - m_size.y - m_size.y - (PAUSE_MENU_SIZE_Y*2), m_vertex.position.z);

	m_pos[2][0] = vertices[8].position;
	m_pos[2][1] = vertices[9].position;
	m_pos[2][2] = vertices[10].position;
	m_pos[2][3] = vertices[11].position;

	vertices[0].tex = VECTOR2(0, 0);
	vertices[1].tex = VECTOR2(1, 0);
	vertices[2].tex = VECTOR2(0, 0.333);
	vertices[3].tex = VECTOR2(1, 0.333);
	
	vertices[4].tex = VECTOR2(0, 0.333);
	vertices[5].tex = VECTOR2(1, 0.333);
	vertices[6].tex = VECTOR2(0, 0.666);
	vertices[7].tex = VECTOR2(1, 0.666);
	
	vertices[8].tex = VECTOR2(0, 0.666);
	vertices[9].tex = VECTOR2(1, 0.666);
	vertices[10].tex = VECTOR2(0, 0.999);
	vertices[11].tex = VECTOR2(1, 0.999);
	

	for (int i = 0; i < PAUSE_MENU_VERTEX; i++)
	{
		vertices[i].color = VECTOR4(1, 1, 1, 1);
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * PAUSE_MENU_VERTEX;
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

///////////////////////////////////////////////////////////////////////////////
//ポジション取得
///////////////////////////////////////////////////////////////////////////////
VECTOR3 *PauseMenu::GetPosition(PauseMenu::PAUSE_MENU ePauseMenu)
{
	return m_pos[ePauseMenu];
}