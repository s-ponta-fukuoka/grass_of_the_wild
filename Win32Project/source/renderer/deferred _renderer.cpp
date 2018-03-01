//=============================================================================
//
// directxレンダリング処理 [rendereDX.cpp]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#include "deferred _renderer.h"
#include "../app/app.h"
#include "render_manager.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../shader/shader_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
DeferredRenderer::DeferredRenderer( )
{
	for (int i = 0; i < G_BUFFER; i++)
	{
		m_pRenderTargetView[i] = NULL;
		m_pDepthStencilView[i] = NULL;
		m_pShaderResourceView[i] = NULL;
	}

	m_pOnRenderTargetView = NULL;
	m_pOnRenderImage = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
DeferredRenderer::~DeferredRenderer( )
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
// 初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT DeferredRenderer::Init(HWND hWnd, ShaderManager* pShaderManager)
{
	CreateOnRenderImage();

	CreateDiffuseBuffer();

	CreateNormalBuffer();

	CreateDepthBuffer();

	CreateShadowBuffer();
	
	ConfigSamplerState();

	ConfigBlendState(FALSE);

	m_pVertexShader = pShaderManager->GetVertexShader(VertexShader::VS_DEFERRED);

	m_pPixelShader = pShaderManager->GetPixelShader(PixelShader::PS_DEFERRED);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了処理
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::Release(void)
{
	for (int i = 0; i < G_BUFFER; i++)
	{
		SAFE_RELEASE(m_pRenderTargetView[i]);
		SAFE_RELEASE(m_pDepthStencilView[i]);
		SAFE_RELEASE(m_pShaderResourceView[i]);
	}

	SAFE_RELEASE(m_pOnRenderImage);
}

///////////////////////////////////////////////////////////////////////////////
// 描画処理
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::Draw(void)
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

	//コンテキストに設定
	//pDeviceContext->VSSetConstantBuffers(0, 1, NULL);
	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);

	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &m_pShaderResourceView[0]);
	pDeviceContext->PSSetShaderResources(1, 1, &m_pShaderResourceView[1]);
	pDeviceContext->PSSetShaderResources(2, 1, &m_pShaderResourceView[2]);
	pDeviceContext->PSSetShaderResources(3, 1, &m_pShaderResourceView[3]);

	pDeviceContext->Draw(4, 0);

	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.0f, 0);
}

///////////////////////////////////////////////////////////////////////////////
//定数バッファ設定
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::ConfigConstantBuffer(UINT ByteWidth)
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
void DeferredRenderer::ConfigSamplerState(void)
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
void DeferredRenderer::ConfigBlendState(BOOL bBlend)
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

///////////////////////////////////////////////////////////////////////////////
//カラー
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::CreateDiffuseBuffer(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	// 2次元テクスチャの設定
	ID3D11Texture2D* pTexture2D;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &pTexture2D);
	if (FAILED(hr))
	{
		assert(pTexture2D);
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture2D, &rtvDesc, &m_pRenderTargetView[0]);
	if (FAILED(hr))
	{
		assert(m_pRenderTargetView[0]);
	}

		// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(pTexture2D, &srDesc, &m_pShaderResourceView[0]);  // 受け取る変数
	if (FAILED(hr))
	{
		assert(m_pShaderResourceView[0]);
	}
}

///////////////////////////////////////////////////////////////////////////////
//法線
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::CreateNormalBuffer(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	// 2次元テクスチャの設定
	ID3D11Texture2D* pTexture2D;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &pTexture2D);
	if (FAILED(hr))
	{
		assert(pTexture2D);
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture2D, &rtvDesc, &m_pRenderTargetView[1]);
	if (FAILED(hr))
	{
		assert(m_pRenderTargetView[1]);
	}

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(pTexture2D, &srDesc, &m_pShaderResourceView[1]);  // 受け取る変数
	if (FAILED(hr))
	{
		assert(m_pShaderResourceView[1]);
	}
}

///////////////////////////////////////////////////////////////////////////////
//深度
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::CreateDepthBuffer(void)
{

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	// 2次元テクスチャの設定
	ID3D11Texture2D* pTexture2D;
	ID3D11Texture2D* pDepthStencilTexture2D;


	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET  | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &pTexture2D);
	if (FAILED(hr))
	{
		assert(pTexture2D);
	}

	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 2次元テクスチャの生成
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &pDepthStencilTexture2D);
	if (FAILED(hr))
	{
		return;
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture2D, &rtvDesc, &m_pRenderTargetView[2]);
	if (FAILED(hr))
	{
		assert(m_pRenderTargetView[2]);
	}

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(pTexture2D, &srDesc, &m_pShaderResourceView[2]);  // 受け取る変数
	if (FAILED(hr))
	{
		assert(m_pShaderResourceView[2]);
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hDepthStencilViewDesc.Flags = 0;
	hDepthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(pDepthStencilTexture2D, &hDepthStencilViewDesc, &m_pDepthStencilView[0]);
	if (FAILED(hr))
	{
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//影
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::CreateShadowBuffer(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	// 2次元テクスチャの設定
	ID3D11Texture2D* pTexture2D;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &pTexture2D);
	if (FAILED(hr))
	{
		assert(pTexture2D);
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture2D, &rtvDesc, &m_pRenderTargetView[3]);
	if (FAILED(hr))
	{
		assert(m_pRenderTargetView[3]);
	}

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(pTexture2D, &srDesc, &m_pShaderResourceView[3]);  // 受け取る変数
	if (FAILED(hr))
	{
		assert(m_pShaderResourceView[3]);
	}
}

///////////////////////////////////////////////////////////////////////////////
//最終結果画像
///////////////////////////////////////////////////////////////////////////////
void DeferredRenderer::CreateOnRenderImage(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	// 2次元テクスチャの設定
	ID3D11Texture2D* pTexture2D;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Width = SCREEN_WIDTH;
	texDesc.Height = SCREEN_HEIGHT;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	// 2次元テクスチャの生成
	HRESULT  hr = pDevice->CreateTexture2D(&texDesc, NULL, &pTexture2D);
	if (FAILED(hr))
	{
		assert(pTexture2D);
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateRenderTargetView(pTexture2D, &rtvDesc, &m_pOnRenderTargetView);
	if (FAILED(hr))
	{
		assert(m_pRenderTargetView[3]);
	}

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	hr = pDevice->CreateShaderResourceView(pTexture2D, &srDesc, &m_pOnRenderImage);  // 受け取る変数
	if (FAILED(hr))
	{
		assert(m_pOnRenderImage);
	}

	//四角形
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(-1,1,0), VECTOR4(1,1,1,1), VECTOR2(0,0),
		VECTOR3(1, 1, 0), VECTOR4(1,1,1,1), VECTOR2(1,0),
		VECTOR3(-1, -1, 0), VECTOR4(1,1,1,1), VECTOR2(0,1),
		VECTOR3(1, -1, 0),  VECTOR4(1,1,1,1), VECTOR2(1,1)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * 4;
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