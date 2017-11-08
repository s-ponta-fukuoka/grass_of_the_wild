//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <Windows.h>
#include "../object.h"
#include "../../renderer/app_renderer.h"

//*****************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class ShadowMap : public Object
{
public:
	//コンストラクタ
	ShadowMap( );

	//デストラクタ
	virtual ~ShadowMap( );

	//初期化
	HRESULT Init( void );

	//終了
	void Release( void );

	//更新
	void Update( void );

	//テクスチャ2D設定
	void ConfigTexture2D(void);

	//レンダーターゲット設定
	void ConfigRenderTargetView(void);

	//ステンシルターゲット設定
	void ConfigDepthStencilView(void);

	//シェーダーリソースビュー設定
	void ConfigShaderResourceView(void);

	//ビューポート設定
	void ConfigViewPort(void);

protected:
	VECTOR3				m_pos;			// ポリゴン座標
	VECTOR3				m_size;			// サイズ
	VECTOR3				m_normal;
	VECTOR2				m_tex;			// テクスチャ座標
	VECTOR2				m_ofsettex;		// テクスチャオフセット

private:
	ID3D11BlendState* m_pBlendState;
	D3D11_VIEWPORT m_pView;
	ID3D11DepthStencilView* m_pDepthStencilView; //ステンシルターゲットビュー
	ID3D11Texture2D *m_pTexture2D;
	ID3D11Texture2D *m_pStencilTexture2D;
	ID3D11RenderTargetView *mpRTV;
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	static ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	ID3D11ShaderResourceView* m_pShadow;//テクスチャー
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;//コンスタントバッファ
	bool m_bLoadTexture;
};

#endif