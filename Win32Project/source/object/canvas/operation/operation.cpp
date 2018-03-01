//=============================================================================
//
// operation.cpp
// Author : shota fukuoka
//
//=============================================================================
#include "operation.h"
#include "../pause_menu/pause_menu.h"
#include "../../../app/app.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPERATION_VERTEX (4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Operation::Operation(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
	: m_pRenderer(NULL)
	, m_pRenderManager(NULL)
{

	m_pRenderManager = pRenderManager;

	m_bUse = false;

	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( -0.2f, 0.8f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_size = VECTOR3( 1.0f, 2.0f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/operation.png", pTextureManager);

	m_pRenderer = new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		OPERATION_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE);

	pRenderManager->AddRenderer(m_pRenderer);
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
Operation::~Operation()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void Operation::Release(void)
{
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
void Operation::Update( void )
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void Operation::MakeVertex(ID3D11Device* pDevice)
{
	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	if (Xinput.dwPacketNumber == 0)
	{
		//四角形
		AppRenderer::Vertex2D vertices[] =
		{
			VECTOR3(m_vertex.position.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,0.5f),
			VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(1,0.5f),
			VECTOR3(m_vertex.position.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,1),
			VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y - m_size.y,m_vertex.position.z),  VECTOR4(1,1,1,1), VECTOR2(1,1)
		};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * OPERATION_VERTEX;
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
	else
	{
		//四角形
		AppRenderer::Vertex2D vertices[] =
		{
			VECTOR3(m_vertex.position.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,0),
			VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(1,0),
			VECTOR3(m_vertex.position.x,m_vertex.position.y - m_size.y,m_vertex.position.z), VECTOR4(1,1,1,1), VECTOR2(0,0.5f),
			VECTOR3(m_vertex.position.x + m_size.x,m_vertex.position.y - m_size.y,m_vertex.position.z),  VECTOR4(1,1,1,1), VECTOR2(1,0.5f)
		};

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * OPERATION_VERTEX;
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
}