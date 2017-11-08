//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "shadow_map.h"
#include "../../app/app.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_MAP_VERTEX (4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
ShadowMap::ShadowMap()
{
	m_pVertexBuffer = NULL;
	m_pSampleLinear = NULL;
	m_pTexture = NULL;
	m_pConstantBuffer = NULL;
	m_pos = VECTOR3( 0.5f, 0.5f, 0.0f );			// ポリゴンの位置
	m_normal = VECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_size = VECTOR3( 0.3f, 0.3f, 0.0f );			// ポリゴンの位置
	m_tex = VECTOR2( 0.0f, 0.0f );			// ポリゴンの位置
	m_ofsettex = VECTOR2( 0.0f, 0.0f );			// ポリゴンの位置
	m_bLoadTexture = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
ShadowMap::~ShadowMap()
{
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT ShadowMap::Init( void )
{
	//m_pRender = new CRenderer;

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//四角形
	AppRenderer::Vertex2D vertices[
	] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(1,1,1,1), VECTOR2(1,1),
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * SHADOW_MAP_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return FALSE;


	///////////////////////////////////////////////////////////
	//シャドウマップ設定
	///////////////////////////////////////////////////////////
	// 2次元テクスチャの設定
 	D3D11_TEXTURE2D_DESC texDesc;
 	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = 2048;
	texDesc.Height = 2048;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
 	texDesc.Usage = D3D11_USAGE_DEFAULT;
  	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
 	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

  	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &m_pTexture2D);
	if (FAILED(hr))
	 {
	 		return false;
	 }

	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 2次元テクスチャの生成
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &m_pStencilTexture2D);
	if (FAILED(hr))
	{
		return false;
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(m_pTexture2D, &rtvDesc, &mpRTV);
	if (FAILED(hr))
	{
	  		return false;
	}
	

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hDepthStencilViewDesc.Flags = 0;
	hDepthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(m_pStencilTexture2D, &hDepthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(m_pTexture2D,  &srDesc, &m_pTexture);  // 受け取る変数
	if (FAILED(hr))
	{
		return false;
	}

	// ビューポートの設定
	m_pView.TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	m_pView.TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	m_pView.Width = 2048;			// ビューポート領域の幅
	m_pView.Height = 2048;			// ビューポート領域の高さ
	m_pView.MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	m_pView.MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void ShadowMap::Release(void)
{
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void ShadowMap::Update( void )
{
}

///////////////////////////////////////////////////////////////////////////////
//テクスチャ2D設定
///////////////////////////////////////////////////////////////////////////////
void ConfigTexture2D(void);

///////////////////////////////////////////////////////////////////////////////
//レンダーターゲット設定
///////////////////////////////////////////////////////////////////////////////
void ConfigRenderTargetView(void);

///////////////////////////////////////////////////////////////////////////////
//ステンシルターゲット設定
///////////////////////////////////////////////////////////////////////////////
void ConfigDepthStencilView(void);

///////////////////////////////////////////////////////////////////////////////
//シェーダーリソースビュー設定
///////////////////////////////////////////////////////////////////////////////
void ConfigShaderResourceView(void);

//ビューポート設定
void ConfigViewPort(void);