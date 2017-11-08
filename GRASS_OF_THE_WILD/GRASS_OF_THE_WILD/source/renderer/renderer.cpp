//=============================================================================
//
// directxレンダリング処理 [rendereDX.cpp]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#include "renderer.h"
#include "../shader/shader.h"
#include "../shader/shader_manager.h"
#include "../texture/texturemanager.h"

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
Renderer::Renderer()
:m_pBlendState(NULL)
,m_pSampleLinear(NULL)
,m_pTexture(NULL)
,m_pVertexBuffer(NULL)
,m_pConstantBuffer(NULL)
,m_pIndexBuffer(NULL)
,m_pTransform(NULL)
{
	;
}

MeshRenderer::MeshRenderer(ID3D11Buffer* pVertexBuffer,
							ID3D11Buffer* pIndexBuffer,
							ShaderManager* pShaderManager,
							ID3D11ShaderResourceView* pTexture,
							Object::Transform* pTransform,
							AppRenderer::Constant* pConstant,
							int	nNumVertexPolygon,
							D3D_PRIMITIVE_TOPOLOGY ePolygon,
							VertexShader::VERTEX_TYPE eVsType,
							PixelShader::PIXEL_TYPE ePsType)
{
	m_ePolygon = ePolygon;

	m_pVertexBuffer = pVertexBuffer;

	m_pIndexBuffer = pIndexBuffer;

	m_pVertexShader = pShaderManager->GetVertexShader(eVsType);

	m_pPixelShader = pShaderManager->GetPixelShader(ePsType);

	m_pTransform = pTransform;

	m_pConstant = pConstant;

	m_nNumVertexPolygon = nNumVertexPolygon;

	m_pTexture = pTexture;

	ConfigConstantBuffer();

	ConfigSamplerState();
}

SkinnedMeshRenderer::SkinnedMeshRenderer()
{
	;
}

CanvasRenderer::CanvasRenderer()
{
	;
}

//コンストラクタ
ShadowRenderer::ShadowRenderer(ID3D11Buffer* pVertexBuffer,
								ShaderManager* pShaderManager,
								VertexShader::VERTEX_TYPE eVsType,
								PixelShader::PIXEL_TYPE ePsType)
{
	m_pVertexBuffer = pVertexBuffer;

	m_pVertexShader = pShaderManager->GetVertexShader(eVsType);

	m_pPixelShader = pShaderManager->GetPixelShader(ePsType);
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
Renderer::~Renderer()
{
	;
}

MeshRenderer::~MeshRenderer()
{
	;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	;
}

CanvasRenderer::~CanvasRenderer()
{
	;
}

ShadowRenderer::~ShadowRenderer()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//定数バッファ設定
///////////////////////////////////////////////////////////////////////////////
void Renderer::ConfigConstantBuffer(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(AppRenderer::Constant);
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
void Renderer::ConfigSamplerState(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
}

///////////////////////////////////////////////////////////////////////////////
// 描画
///////////////////////////////////////////////////////////////////////////////
void MeshRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//バーテックスバッファーをセット
	UINT stride = sizeof(AppRenderer::Vertex3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(m_ePolygon);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	AppRenderer::Constant hConstant;

	//ワールド変換用行列を生成
	XMMATRIX hWorld;

	//初期化
	hWorld = XMMatrixIdentity();

	XMMATRIX hRotate = XMMatrixRotationY(m_pTransform->rot.y);
	hWorld = XMMatrixMultiply(hWorld, hRotate);

	hConstant.world = XMMatrixTranspose(hWorld);
	hConstant.view = m_pConstant->view;
	hConstant.projection = m_pConstant->projection;

	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &hConstant, 0, 0);

	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();

	//コンテキストに設定
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);
	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	if (m_pIndexBuffer != NULL)
	{
		//そのインデックスバッファをコンテキストに設定
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pDeviceContext->DrawIndexed(m_nNumVertexPolygon, 0, 0);
	}
	else
	{
		pDeviceContext->Draw(m_nNumVertexPolygon, 0);
	}

}

void SkinnedMeshRenderer::Draw()
{
	//for (int i = 0; i < modelCnt; i++)
	//{
	//	//そのブレンディングをコンテキストに設定
	//	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//	pDeviceContext->OMSetBlendState(m_fbxModel[i].pBlendState, blendFactor, 0xffffffff);
	//
	//	//バーテックスバッファーをセット
	//	UINT stride = sizeof(ModelVertex);
	//	UINT offset = 0;
	//	pDeviceContext->IASetVertexBuffers(0, 1, &m_fbxModel[i].pVertexBuffer, &stride, &offset);
	//
	//	//そのインデックスバッファをコンテキストに設定
	//	pDeviceContext->IASetIndexBuffer(m_fbxModel[i].pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//
	//	//プリミティブ・トポロジーをセット
	//	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//	//頂点インプットレイアウトをセット
	//	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());
	//
	//
	//	ConstantModel hConstantBuffer;
	//
	//	//初期化
	//	XMMATRIX hLcl = XMMatrixIdentity();
	//	XMMATRIX hLclPosition = XMMatrixTranslation(m_Bone[i]->pos.x, m_Bone[i]->pos.y, m_Bone[i]->pos.z);
	//	XMMATRIX hLclRotate = XMMatrixRotationRollPitchYaw(D3DXToRadian(m_Bone[i]->rot.x), D3DXToRadian(m_Bone[i]->rot.y), D3DXToRadian(m_Bone[i]->rot.z));
	//	XMMATRIX hLclScaling = XMMatrixScaling(m_Bone[i]->scl.x, m_Bone[i]->scl.y, m_Bone[i]->scl.z);
	//
	//	hLcl = XMMatrixMultiply(hLcl, hLclScaling);
	//	hLcl = XMMatrixMultiply(hLcl, hLclRotate);
	//	hLcl = XMMatrixMultiply(hLcl, hLclPosition);
	//
	//	//初期化
	//	XMMATRIX hWorld = XMMatrixIdentity();
	//	XMMATRIX hPosition = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//	XMMATRIX hRotate = XMMatrixRotationRollPitchYaw(D3DXToRadian(m_rot.x), -m_rot.y, D3DXToRadian(m_rot.z));
	//	XMMATRIX hScaling = XMMatrixScaling(1, 1, 1);
	//
	//	hWorld = XMMatrixMultiply(hWorld, hScaling);
	//	hWorld = XMMatrixMultiply(hWorld, hRotate);
	//	hWorld = XMMatrixMultiply(hWorld, hPosition);
	//
	//	hConstantBuffer.mWorld = XMMatrixTranspose(hWorld);
	//	hConstantBuffer.mLclBone = XMMatrixTranspose(hLcl);
	//	hConstantBuffer.mView = XMMatrixTranspose(CSubCamera::GetConstant().mView);
	//	hConstantBuffer.mProjection = XMMatrixTranspose(CSubCamera::GetConstant().mProjection);
	//	hConstantBuffer.mLight = CSubCamera::GetConstant().mLight;
	//
	//	for (int j = 0; j < m_fbxModel[i].clusterCount; j++)
	//	{
	//		hConstantBuffer.mBone[j] = m_Bone[i][j].mat[m_Number][timeCnt];
	//	}
	//
	//	pDeviceContext->UpdateSubresource(m_fbxModel[i].pConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	//
	//	//使用するシェーダーの登録
	//	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	//	pDeviceContext->PSSetShader(m_pShadowPixelShader->GetPixelShader(), NULL, 0);
	//
	//	//コンテキストに設定
	//	pDeviceContext->VSSetConstantBuffers(0, 1, &m_fbxModel[i].pConstantBuffer);
	//
	//	//テクスチャーをシェーダーに渡す
	//	pDeviceContext->PSSetSamplers(0, 1, &m_fbxModel[i].pSampleLinear);
	//
	//	if (i < 10 && m_fbxModel[i].filename != NULL)
	//	{
	//		pDeviceContext->PSSetShaderResources(0, 1, &m_fbxModel[i].pTexture);
	//	}
	//	pDeviceContext->PSSetShaderResources(1, 1, &m_ToonTexture);
	//	pDeviceContext->PSSetShaderResources(2, 1, &m_ShadowTexture);
	//
	//	//プリミティブをレンダリング
	//	//pDeviceContext->Draw(m_fbxModel[i].PolygonVertexNum, 0);
	//	pDeviceContext->DrawIndexed(m_fbxModel[i].PolygonVertexNum, 0, 0);
	//}
}

void CanvasRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//バーテックスバッファーをセット
	UINT stride = sizeof(AppRenderer::Vertex2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//プリミティブ・トポロジーをセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点インプットレイアウトをセット
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	//コンテキストに設定
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//使用するシェーダーの登録
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);

	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();

	//テクスチャーをシェーダーに渡す
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	//プリミティブをレンダリング
	pDeviceContext->Draw(m_nNumVertexPolygon, 0);
}

void ShadowRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	ID3D11DepthStencilView* pDepthStencilView = pAppRenderer->GetDepthStencilView();
	ID3D11RenderTargetView* pRenderTargetView = pAppRenderer->GetRenderTargetView();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);//画面クリア 

															 // 自前のレンダーターゲットビューに切り替え
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_pView[1]);

	// OMに描画ターゲット ビューと深度/ステンシル・ビューを設定
	ID3D11RenderTargetView* pRender[1] = { NULL };
	pDeviceContext->OMSetRenderTargets(1, &mpRTV, m_pDepthStencilView);

	//描画

	float ClearColor[4] = { 0,0,1,1 }; //消去色
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);//画面クリア 

	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_pView[0]);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}