//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include <stdio.h>
#include <tchar.h>
#include <bitset>
#include "grass.h"
#include "../../../renderer/render_manager.h"
#include "../../../utility/utility.h"
#include "../../camera/main_camera.h"
#include "../meshfiled/mesh_field.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GRASS_VERTEX	(3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
Grass::Grass(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	MeshField* pMeshField,
	MainCamera *pCamera,
	VECTOR3 pos, 
	Object::Transform* pPlayerTransform)
{
	m_pNumInstance = new int;

	m_pMeshField = pMeshField;

	m_pTransform = new Transform[GRASS_MAX];

	m_pVertexBuffer = NULL;

	m_pIndexBuffer = NULL;

	m_pCamera = pCamera;

	//m_pTransform->position = pos;

	m_vertex.position = VECTOR3(0, 0, 0);
	m_vertex.tex = VECTOR2(0.0f, 0.0f);
	m_size = VECTOR3(3, 80, 0);
	m_vertex.normal = VECTOR3(0, 0, -1);

	for (int nCnt = 0; nCnt < GRASS_MAX; nCnt++)
	{
		m_pTransform[nCnt].position = pos;

		m_pTransform[nCnt].rot = VECTOR3(0, 0, 0);
		m_pTransform[nCnt].scale = VECTOR3(1, 1, 1);

		int  n = Utility::DecimalConversion<int>(nCnt, 4, 10);

		float f = 0;

		int count = Utility::GetReverseNumber<int>(n);

		for (int i = 0; i < count; i++)
		{
			int num = n;
			num /= pow(10, i);
			num %= 10;

			float nf = num * pow(3, -(i + 1));

			f += nf;
		}

		//m_pTransform[nCnt].position.x += f * 5300;

		n = Utility::DecimalConversion<int>(nCnt, 5, 10);

		f = 0;

		count = Utility::GetReverseNumber<int>(n);

		for (int i = 0; i < count; i++)
		{
			int num = n;
			num /= pow(10, i);
			num %= 10;

			float nf = num * pow(5, -(i + 1));

			f += nf;
		}

		//m_pTransform[nCnt].position.z += f * 8000;

		m_pTransform[nCnt].position.x += Utility::Hulton(nCnt, 2)* 8000;
		m_pTransform[nCnt].position.z += Utility::Hulton(nCnt, 3)* 5300;

		m_pTransform[nCnt].rot.y += f * 8000;

		m_pTransform[nCnt].position.y = m_pMeshField->GetHeight(m_pTransform[nCnt].position);
	}

	MakeVertex();

	m_pNumInstance[0] = GRASS_MAX;

	Texture* pTexture = new Texture("resource/sprite/NULL.jpg", pTextureManager);
	Texture* pBayerTexture = new Texture("resource/sprite/bayer.png", pTextureManager);

	pRenderManager->AddDeferredRenderer(new GrowMeshRenderer(m_pVertexBuffer,
		m_pInstanceBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		pBayerTexture->GetTexture(),
		NULL,
		m_pTransform,
		pPlayerTransform,
		pConstant,
		pLightCameraConstant,
		pCamera,
		GRASS_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_GRASS,
		GeometryShader::GS_GRASS,
		PixelShader::PS_GRASS,
		FALSE,
		m_pNumInstance));
}

//=============================================================================
// デストラクタ
//=============================================================================
Grass::~Grass()
{
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT Grass::Init( void )
{
	m_vertex.position = VECTOR3(-4000.0f, 0.0f, 4000.0f);
	m_vertex.tex = VECTOR2(0.0f, 0.0f);
	m_size = VECTOR3(80, 0, 80);
	m_vertex.normal = VECTOR3(0, 0, -1);

	MakeVertex();


	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void Grass::Release(void)
{
	if (m_pTransform != NULL)
	{
		delete[] m_pTransform;
		m_pTransform = NULL;
	}

	Mesh::Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void Grass::Update( void )
{
	//AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	//ID3D11Device* pDevice = pAppRenderer->GetDevice();
	//ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	//
	//D3D11_MAPPED_SUBRESOURCE msr;
	//pDeviceContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	//
	//std::vector<Object::Transform> Transform;
	//
	//for (int nCnt = 0; nCnt < GRASS_MAX; nCnt++)
	//{
	//	float distance = sqrt((m_pTransform[nCnt].position.x - m_pCamera->GetTransform()->position.x) * (m_pTransform[nCnt].position.x - m_pCamera->GetTransform()->position.x) +
	//		(m_pTransform[nCnt].position.y - m_pCamera->GetTransform()->position.y) * (m_pTransform[nCnt].position.y - m_pCamera->GetTransform()->position.y) +
	//		(m_pTransform[nCnt].position.z - m_pCamera->GetTransform()->position.z) * (m_pTransform[nCnt].position.z - m_pCamera->GetTransform()->position.z));
	//
	//	if (distance <= 5000)
	//	{
	//		Transform.push_back(m_pTransform[nCnt]);
	//	}
	//}
	//
	//XMMATRIX* hWorld = new XMMATRIX[Transform.size()];
	//
	//for (int nCnt = 0; nCnt < Transform.size(); nCnt++)
	//{
	//	hWorld[nCnt] = XMMatrixIdentity();
	//
	//	XMMATRIX hPosition = XMMatrixTranslation(Transform[nCnt].position.x, Transform[nCnt].position.y, Transform[nCnt].position.z);
	//	XMMATRIX hRotate = XMMatrixRotationRollPitchYaw(D3DToRadian(Transform[nCnt].rot.x), D3DToRadian(Transform[nCnt].rot.y), D3DToRadian(Transform[nCnt].rot.z));
	//	XMMATRIX hScaling = XMMatrixScaling(1, 1, 1);
	//
	//	hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hScaling);
	//	hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hRotate);
	//	hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hPosition);
	//
	//	hWorld[nCnt] = XMMatrixTranspose(hWorld[nCnt]);
	//}
	//
	//memcpy(msr.pData, hWorld, sizeof(XMMATRIX) * Transform.size()); // 3頂点分コピー
	//
	//delete[] hWorld;
	//
	//pDeviceContext->Unmap(m_pInstanceBuffer, 0);
	//
	//m_pNumInstance[0] = Transform.size();

	//m_pTransform->position.x += 0.1f;

	//float angle = atan2( m_pTransform->position.x - m_pCamera->GetPos().x, m_pTransform->position.z - m_pCamera->GetPos().z) * (180.0 / D3D_PI);
	//
	//m_pTransform->rot.y = angle;
}

void Grass::MakeVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	AppRenderer::Vertex3D* vertices = new AppRenderer::Vertex3D[GRASS_VERTEX];

	vertices[0].position = VECTOR3(m_vertex.position.x + m_size.x, m_vertex.position.y + 35, m_vertex.position.z);
	vertices[1].position = VECTOR3(m_vertex.position.x - m_size.x, m_vertex.position.y + 35, m_vertex.position.z);
	vertices[2].position = VECTOR3(m_vertex.position.x, m_vertex.position.y + m_size.y, m_vertex.position.z);
	//vertices[3].position = VECTOR3(m_vertex.position.x, m_vertex.position.y, m_vertex.position.z);
	

	vertices[0].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	vertices[1].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	vertices[2].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);
	//vertices[3].normal = VECTOR3(m_vertex.normal.x,m_vertex.normal.y,m_vertex.normal.z);

	vertices[0].color = VECTOR4(1,0,0,1);
	vertices[1].color = VECTOR4(1,0,0,1);
	vertices[2].color = VECTOR4(1,0,0,1);
	//vertices[3].color = VECTOR4(1,0,0,1);

	vertices[0].tex = VECTOR2(0,0);
	vertices[1].tex = VECTOR2(1,0);
	vertices[2].tex = VECTOR2(0,1);
	//vertices[3].tex = VECTOR2(1,1);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex3D) * GRASS_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	delete[] vertices;


	XMMATRIX* hWorld = new XMMATRIX[GRASS_MAX];

	for (int nCnt = 0; nCnt < GRASS_MAX; nCnt++)
	{
		hWorld[nCnt] = XMMatrixIdentity();

		XMMATRIX hPosition = XMMatrixTranslation(m_pTransform[nCnt].position.x, m_pTransform[nCnt].position.y, m_pTransform[nCnt].position.z);
		XMMATRIX hRotate = XMMatrixRotationRollPitchYaw(D3DToRadian(m_pTransform[nCnt].rot.x), D3DToRadian(m_pTransform[nCnt].rot.y), D3DToRadian(m_pTransform[nCnt].rot.z));
		XMMATRIX hScaling = XMMatrixScaling(1, 1, 1);

		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hScaling);
		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hRotate);
		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hPosition);

		hWorld[nCnt] = XMMatrixTranspose(hWorld[nCnt]);
	}

	D3D11_BUFFER_DESC Ibd;
	Ibd.Usage = D3D11_USAGE_DYNAMIC;
	Ibd.ByteWidth = sizeof(XMMATRIX) * GRASS_MAX;
	Ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InstanceInitData;
	InstanceInitData.pSysMem = hWorld;
	pDevice->CreateBuffer(&Ibd, &InstanceInitData, &m_pInstanceBuffer);

	delete[] hWorld;

}