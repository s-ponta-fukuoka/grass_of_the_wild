//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "fade_scene.h"
#include "next_scene.h"
#include "../object/canvas/canvas.h"
#include "../renderer/app_renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
Fade::Fade()
{
	m_pNextScene = NULL;
	m_modeNext = NULL;
	m_pos = VECTOR3(-1.0f, 1.0f, 0.0f);			// ポリゴンの位置
	m_normal = VECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_size = VECTOR3(2.0f, 2.0f, 0.0f);			// ポリゴンの位置
	m_tex = VECTOR2(0.0f, 0.0f);			// ポリゴンの位置
	m_ofsettex = VECTOR2(0.0f, 0.0f);			// ポリゴンの位置
	m_color = VECTOR4(1, 1, 1, 0);			// ポリゴンの位置
	m_bLoadTexture = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
Fade::~Fade()
{
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT Fade::Init(NextScene* pNextScene,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pNextScene = pNextScene;

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//四角形
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z),  VECTOR4(0,0,0,0), VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(0,0,0,0),VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(0,0,0,0),VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(0,0,0,0), VECTOR2(1,1),
	};


	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * FADE_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return FALSE;

	Texture* pTexture = new Texture("resource/sprite/fade.png", pTextureManager);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		FADE_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE));

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void Fade::Uninit(void)
{
	ES_SAFE_RELEASE(m_pVertexBuffer);
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void Fade::Update(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	pDeviceContext = pAppRenderer->GetDeviceContex();

	if (m_fade == FADE_NONE)
	{
		return;
	}

	//フェードイン
	if (m_fade == FADE_IN)
	{
		// α値がMinになった場合、フェードの状態をフェードなしへ変更
		if ((m_color.w = GetMax((m_color.w - FADE_RATE), COLOR_FADE_MIN)) == COLOR_FADE_MIN)
		{
			m_fade = FADE_NONE;
		}
	}
	// フェードアウト（α値を加算し、バック画面を消していく）
	else if (m_fade == FADE_OUT)
	{
		// α値がMaxになった場合、フェードの状態をフェードインへ変更
		if ((m_color.w = GetMin((m_color.w + FADE_RATE), COLOR_FADE_MAX)) == COLOR_FADE_MAX)
		{
			m_fade = FADE_IN;

			// 次のモードの変更
			m_pNextScene->NextSceneUpdate(m_modeNext);
		}
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点カラーの設定
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z),  VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w),VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(1,1),
	};

	memcpy(msr.pData, vertices, sizeof(AppRenderer::Vertex2D) * FADE_VERTEX); // 3頂点分コピー

	pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

/*==============================================================================
【 フェードの設定 】
==============================================================================*/
void Fade::SetFade(FADE_TYPE fade, Scene* modenext)
{
	// フェード設定エラー
	if (fade == FADE_NONE)
	{
		MessageBox(NULL, "“フェードなし”が設定されました", "Error", MB_OK | MB_ICONHAND);
	}

	// フェードの状態をセット
	m_fade = fade;

	// 次のモードをセット
	m_modeNext = modenext;
}

/*==============================================================================
【 フェードの状態の取得 】
==============================================================================*/
Fade::FADE_TYPE Fade::GetFade(void)
{
	return m_fade;
}

/*==============================================================================
【 頂点カラーの設定 】
==============================================================================*/
void Fade::SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color)
{
}

/*==============================================================================
【 大きい値を返す関数 】
==============================================================================*/
float Fade::GetMax(float a, float b)
{
	// aが大きい場合、aを返す
	if (a > b)
	{
		return a;
	}

	return b;
}
/*==============================================================================
【 小さい値を返す関数 】
==============================================================================*/
float Fade::GetMin(float a, float b)
{
	// aが大きい場合、bを返す
	if (a > b)
	{
		return b;
	}

	return a;
}