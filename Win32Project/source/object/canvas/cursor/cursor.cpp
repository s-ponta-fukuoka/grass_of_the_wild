//=============================================================================
//
// cursor.cpp
// Author : shota fukuoka
//
//=============================================================================
#include "cursor.h"
#include "../pause_menu/pause_menu.h"
#include "../../../app/app.h"
#include "../../../device/input.h"
#include "../../../device/xbox_controller.h"
#include "../../../scene/fade_scene.h"
#include "../../../scene/game_scene.h"
#include "../../../scene/title_scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CURSOL_VERTEX (4)
#define SIZE_MOVE_X (0.003)
#define SIZE_MOVE_Y (0.003)
#define SIZE_MAX (0.03)
#define SIZE_MIN (0.0)
#define CURSOL_WAIT_TIME (10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Cursor::Cursor(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	PauseMenu* pPauseMenu)
	: m_pPauseMenu(NULL)
	, m_pos(NULL)
	, m_pRenderer(NULL)
	, m_pRenderManager(NULL)
	, m_nWaitTime(0)
{

	m_pRenderManager = pRenderManager;

	m_pPauseMenu = pPauseMenu;

	m_pos = m_pPauseMenu->GetPosition(PauseMenu::PAUSE_MENU_BACK_GAME);
	m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_BACK_GAME);

	m_bBackGame = false;
	m_bUse = false;

	m_pTransform = new Transform();

	m_pVertexBuffer = NULL;

	m_vertex.position = VECTOR3( 0.2f, -0.5f, 0.0f );

	m_vertex.color = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);			// ポリゴンの位置

	m_sizeMin = VECTOR3(0.0f, 0.0f, 0.0f);

	m_size = VECTOR3( 0.0f, 0.0f, 0.0f );

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	MakeVertex(pDevice);

	Texture* pTexture = new Texture("resource/sprite/cursor.png", pTextureManager);

	m_pRenderer = new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		CURSOL_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE);

	pRenderManager->AddRenderer(m_pRenderer);
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
Cursor::~Cursor()
{
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void Cursor::Release(void)
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
void Cursor::Update( void )
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XboxController* pXboxController = XboxController::GetInstance();

	XMVECTOR XMpos[4];
	XMVECTOR startPos[4];
	
	Fade* pFade = Fade::GetInstance();

	for (int i = 0; i < 4; i++)
	{
		startPos[i] = XMVectorSet(m_pos[i].x,
			m_pos[i].y,
			m_pos[i].z, 1.0f);
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_W) || 
		pXboxController->GetStick(0, XboxController::STICK_LY) >= XINPUT_STICK_RANGE_ENABLE && m_nWaitTime == 0)
	{
		m_nWaitTime = CURSOL_WAIT_TIME;
		switch (m_pPauseMenu->GetPauseMenu())
		{
		case PauseMenu::PAUSE_MENU_BACK_GAME:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_TITLE);
			break;
		}

		case PauseMenu::PAUSE_MENU_RETRY:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_BACK_GAME);
			break;
		}

		case PauseMenu::PAUSE_MENU_TITLE:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_RETRY);
			break;
		}

		default:
			break;
		}
	}
	else if (pInputKeyboard->GetKeyTrigger(DIK_S) ||
		pXboxController->GetStick(0, XboxController::STICK_LY) <= -XINPUT_STICK_RANGE_ENABLE && m_nWaitTime == 0)
	{
		m_nWaitTime = CURSOL_WAIT_TIME;
		switch (m_pPauseMenu->GetPauseMenu())
		{
		case PauseMenu::PAUSE_MENU_BACK_GAME:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_RETRY);
			break;
		}

		case PauseMenu::PAUSE_MENU_RETRY:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_TITLE);
			break;
		}

		case PauseMenu::PAUSE_MENU_TITLE:
		{
			m_pPauseMenu->SetPauseMenu(PauseMenu::PAUSE_MENU_BACK_GAME);
			break;
		}

		default:
			break;
		}
	}
	else if (pInputKeyboard->GetKeyTrigger(DIK_RETURN) || 
		pXboxController->GetButtonTrigger(0,XINPUT_GAMEPAD_A))
	{

		switch (m_pPauseMenu->GetPauseMenu())
		{
		case PauseMenu::PAUSE_MENU_BACK_GAME:
		{
			m_bBackGame = true;
			break;
		}

		case PauseMenu::PAUSE_MENU_RETRY:
		{
			pFade->SetFade(Fade::FADE_OUT, new Game);
			break;
		}

		case PauseMenu::PAUSE_MENU_TITLE:
		{
			pFade->SetFade(Fade::FADE_OUT, new Title);
			break;
		}

		default:
			break;
		}
	}

	m_pos = m_pPauseMenu->GetPosition(m_pPauseMenu->GetPauseMenu());

	D3D11_MAPPED_SUBRESOURCE msr;
	pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//四角形
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_pos[0].x - m_sizeMin.x,m_pos[0].y + m_sizeMin.y,m_vertex.position.z), VECTOR4(1,1,1,m_vertex.color.w), VECTOR2(0,0),
		VECTOR3(m_pos[1].x + m_size.x,m_pos[1].y +m_sizeMin.y,m_vertex.position.z), VECTOR4(1,1,1,m_vertex.color.w), VECTOR2(1,0),
		VECTOR3(m_pos[2].x - m_sizeMin.x,m_pos[2].y - m_size.y,m_vertex.position.z), VECTOR4(1,1,1,m_vertex.color.w), VECTOR2(0,1),
		VECTOR3(m_pos[3].x + m_size.x,m_pos[3].y - m_size.y,m_vertex.position.z),  VECTOR4(1,1,1,m_vertex.color.w), VECTOR2(1,1)
	};

	memcpy(msr.pData, vertices, sizeof(AppRenderer::Vertex2D) * (CURSOL_VERTEX)); // 3頂点分コピー

	pDeviceContext->Unmap(m_pVertexBuffer, 0);

	if (m_size.x >= SIZE_MAX)
	{
		m_bUse = true;
	}
	if (m_size.x <= SIZE_MIN)
	{
		m_bUse = false;
	}

	if (!m_bUse)
	{
		m_sizeMin.x += SIZE_MOVE_X;
		m_sizeMin.y += SIZE_MOVE_Y;
		m_size.x += SIZE_MOVE_X;
		m_size.y += SIZE_MOVE_Y;
	}
	else
	{
		m_sizeMin.x -= SIZE_MOVE_X;
		m_sizeMin.y -= SIZE_MOVE_Y;
		m_size.x -= SIZE_MOVE_X;
		m_size.y -= SIZE_MOVE_Y;
	}

	if (m_nWaitTime > 0)
	{
		m_nWaitTime--;
	}
	else
	{
		m_nWaitTime = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//頂点バッファ
///////////////////////////////////////////////////////////////////////////////
void Cursor::MakeVertex(ID3D11Device* pDevice)
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
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * CURSOL_VERTEX;
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