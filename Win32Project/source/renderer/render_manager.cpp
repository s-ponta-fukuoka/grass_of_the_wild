//=============================================================================
//
// [render_manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "render_manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
RenderManager::RenderManager()
{
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
RenderManager::~RenderManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//追加
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//削除
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteRenderer(const Renderer* renderer)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//全描画
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DrawAll(void)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) {
			m_listRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//追加
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddDeferredRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listDeferredRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//削除
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteDeferredRenderer(const Renderer* renderer)
{
	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//全描画
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeferredDrawAll(void)
{
	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL){
			m_listDeferredRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//シャドウマップ用追加
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddShadowRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listShadowRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//シャドウマップ用削除
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteShadowRenderer(const Renderer* renderer)
{
	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//シャドウマップ用オブジェクト描画
///////////////////////////////////////////////////////////////////////////////
void RenderManager::ShadowDrawAll(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(m_pShadowRenderTargetView, clearColor);//画面クリア 

	// 自前のレンダーターゲットビューに切り替え
	pDeviceContext->ClearDepthStencilView(m_pShadowDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_ShadowViewPort);

	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	ID3D11RenderTargetView* pRender[1] = { NULL };
	pDeviceContext->OMSetRenderTargets(1, &m_pShadowRenderTargetView, m_pShadowDepthStencilView);

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) {
			m_listShadowRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//全削除
///////////////////////////////////////////////////////////////////////////////
void RenderManager::ReleaseAll(void)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}

	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}
	m_listRenderer.clear();

	m_listShadowRenderer.clear();

	m_listDeferredRenderer.clear();
}

///////////////////////////////////////////////////////////////////////////////
//影ビューポート設定
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowViewPort(D3D11_VIEWPORT ShadowViewPort)
{
	m_ShadowViewPort = ShadowViewPort;
}

///////////////////////////////////////////////////////////////////////////////
//影ステンシルビュー設定
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowDepthStencilView(ID3D11DepthStencilView* pShadowDepthStencilView)
{
	m_pShadowDepthStencilView = pShadowDepthStencilView;
}

///////////////////////////////////////////////////////////////////////////////
//影レンダーターゲットビュー設定
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowRenderTargetVie(ID3D11RenderTargetView* pShadowRenderTargetView)
{
	m_pShadowRenderTargetView = pShadowRenderTargetView;
}

///////////////////////////////////////////////////////////////////////////////
//シャドウマップ設定
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowTexture(ID3D11ShaderResourceView* pShadowTexture)
{
	m_pShadowTexture = pShadowTexture;
}

///////////////////////////////////////////////////////////////////////////////
//シャドウマップ取得
///////////////////////////////////////////////////////////////////////////////
ID3D11ShaderResourceView* RenderManager::GetShadowTexture(void) const
{
	return m_pShadowTexture;
}