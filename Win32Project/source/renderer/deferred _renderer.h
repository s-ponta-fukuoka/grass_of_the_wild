//=============================================================================
//
// directxレンダリング処理 [rendere.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _DEFERRED_RENDERER_H_
#define _DEFERRED_RENDERER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "app_renderer.h"
#include "../object/object.h"
#include "../shader/shader.h"
#include "../texture/texture_manager.h"
#include "../model/model.h"
#include "../object/camera/main_camera.h"

#define G_BUFFER (3)
//*****************************************************************************
//構造体定義
//*****************************************************************************

class ShaderManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class DeferredRenderer
{
public:
	//コンストラクタ
	DeferredRenderer();

	//デストラクタ
	virtual ~DeferredRenderer( );

	//初期化
	HRESULT Init( HWND hWnd , ShaderManager* pShaderManager);

	//終了
	void Release( void );

	//描画
	void Draw(void);

	//定数バッファ設定
	void ConfigConstantBuffer(UINT ByteWidth);

	//サンプラーステート設定
	void ConfigSamplerState(void);

	//ブレンドステート設定
	void ConfigBlendState(BOOL bBlend);

	ID3D11RenderTargetView* GetRenderTargetView(const int nNumber) const { return m_pRenderTargetView[nNumber]; }
	ID3D11DepthStencilView* GetDepthStencilView(const int nNumber) const { return m_pDepthStencilView[nNumber]; }
	ID3D11ShaderResourceView* GetShaderResourceView(const int nNumber) const{ return m_pShaderResourceView[nNumber]; }

	ID3D11RenderTargetView* GetOnRenderTargetView(void) const { return m_pOnRenderTargetView; }
	ID3D11ShaderResourceView* GetShaderResourceView(void) const { return m_pOnRenderImage; }

private:
	//カラー
	void CreateDiffuseBuffer(void);

	//法線
	void CreateNormalBuffer(void);

	//深度
	void CreateDepthBuffer(void);

	//影
	void CreateShadowBuffer(void);

	//最終結果画像
	void CreateOnRenderImage(void);

	ID3D11RenderTargetView* m_pRenderTargetView[G_BUFFER];

	ID3D11DepthStencilView*	 m_pDepthStencilView[G_BUFFER];

	ID3D11ShaderResourceView*	 m_pShaderResourceView[G_BUFFER];

	ID3D11RenderTargetView* m_pOnRenderTargetView;

	ID3D11ShaderResourceView*	 m_pOnRenderImage;

	ID3D11BlendState*					m_pBlendState;

	ID3D11SamplerState*					m_pSampleLinear;

	ID3D11Buffer*						m_pVertexBuffer;

	ID3D11Buffer*						m_pConstantBuffer;

	VertexShader*						m_pVertexShader;

	PixelShader*						m_pPixelShader;
};

#endif