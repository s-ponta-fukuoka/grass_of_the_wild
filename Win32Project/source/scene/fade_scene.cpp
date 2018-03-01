//=============================================================================
//
// fade_scene.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "fade_scene.h"
#include "next_scene.h"
#include "../object/canvas/canvas.h"
#include "../renderer/app_renderer.h"
#include "../shader/shader_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_TEXTURE_NAME	"resource/sprite/fade.png"	
#define COLOR_FADE_MIN	(0.0f)								
#define COLOR_FADE_MAX	(1.0f)								
#define FADE_RATE		(0.05f)								
#define FADE_VERTEX		(4)

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
Fade *Fade::m_pFade = NULL;

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

///////////////////////////////////////////////////////////////////////////////
//インスタンス生成
///////////////////////////////////////////////////////////////////////////////
void Fade::CreateInstance(void)
{
	if (m_pFade != NULL) { return; }
	m_pFade = new Fade();
}

///////////////////////////////////////////////////////////////////////////////
//インスタンス取得
///////////////////////////////////////////////////////////////////////////////
Fade* Fade::GetInstance(void)
{
	return m_pFade;
}


//=============================================================================
// デストラクタ
//=============================================================================
Fade::~Fade()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Fade::Init(NextScene* pNextScene,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pConstantBuffer = NULL;

	m_pNextScene = pNextScene;

	MakeVertex();

	ConfigSamplerState();

	ConfigBlendState(TRUE);

	m_pTexture = new Texture(FADE_TEXTURE_NAME, pTextureManager);

	m_pVertexShader = pShaderManager->GetVertexShader(VertexShader::VS_2D);

	m_pPixelShader = pShaderManager->GetPixelShader(PixelShader::PS_2D);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Fade::Release(void)
{
	ES_SAFE_RELEASE(m_pVertexBuffer);
	ES_SAFE_RELEASE(m_pBlendState);
	ES_SAFE_RELEASE(m_pConstantBuffer);
	ES_SAFE_RELEASE(m_pSampleLinear);
}

//=============================================================================
// 更新処理
//=============================================================================
void Fade::Update(void)
{
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

	SetVertex();
}

//=============================================================================
// 描画処理
//=============================================================================
void Fade::Draw()
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//ブレンディングをコンテキストに設定
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);

	ID3D11DepthStencilView* pDSV = pAppRenderer->GetDepthStencilView();
	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_STENCIL, 1.0f, 0);

	//バーテックスバッファーをセット
	UINT stride = sizeof(AppRenderer::Vertex2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	//テクスチャーをシェーダーに渡す
	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);

	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	pDeviceContext->Draw(4, 0);
}

///////////////////////////////////////////////////////////////////////////////
//定数バッファ設定
///////////////////////////////////////////////////////////////////////////////
void Fade::ConfigConstantBuffer(UINT ByteWidth)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = ByteWidth;
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = sizeof(float);
	pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//サンプラーステート設定
///////////////////////////////////////////////////////////////////////////////
void Fade::ConfigSamplerState(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MaxAnisotropy = 2;
	SamDesc.MipLODBias = 0;
	SamDesc.MinLOD = -FLT_MAX;
	SamDesc.MaxLOD = FLT_MAX;

	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
}

///////////////////////////////////////////////////////////////////////////////
//ブレンドステート設定
///////////////////////////////////////////////////////////////////////////////
void Fade::ConfigBlendState(BOOL bBlend)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++)
	{
		BlendStateDesc.RenderTarget[i].BlendEnable = bBlend;
		BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	pDevice->CreateBlendState(&BlendStateDesc, &m_pBlendState);
}

//=============================================================================
// フェードの設定
//=============================================================================
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

//==============================================================================
// フェードの状態の取得
//==============================================================================
Fade::FADE_TYPE Fade::GetFade(void)
{
	return m_fade;
}

//==============================================================================
// 頂点カラーの設定
//==============================================================================
void Fade::SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color)
{
}

//==============================================================================
// 大きい値を返す関数
//==============================================================================
float Fade::GetMax(float a, float b)
{
	// aが大きい場合、aを返す
	if (a > b)
	{
		return a;
	}

	return b;
}

//==============================================================================
// 小さい値を返す関数
//==============================================================================
float Fade::GetMin(float a, float b)
{
	// aが大きい場合、bを返す
	if (a > b)
	{
		return b;
	}

	return a;
}

//==============================================================================
// 頂点生成
//==============================================================================
void Fade::MakeVertex(void)
{
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
		return;
}

//==============================================================================
// 頂点設定
//==============================================================================
void Fade::SetVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	pDeviceContext = pAppRenderer->GetDeviceContex();


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