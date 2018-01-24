//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "../object/object.h"
#include "collider.h"
#include "collision_manager.h"
#include "../renderer/renderer.h"
#include "../renderer/render_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

Collider::Collider()
{
	m_bDelete = false;
}

Collider::~Collider()
{

}

void Collider::SetType(TYPE type)
{
	eColType = type;
}

Collider::TYPE  Collider::GetType(void)
{
	return eColType;
}


void Collider::RendererDelete(void)
{
	m_pRenderManager->DeleteRenderer(m_pRenderer);
}

Object *Collider::GetGameObject(void)
{
	return m_pObject;
}

BoxCollider::BoxCollider(Object *pObject, CollisionManager* pCollisionManager)
{
	m_pObject = pObject;
	SetType(TYPE::TYPE_BOX);
	pCollisionManager->AddCollider(this);
}

SphereCollider::SphereCollider(VECTOR3 pos,
	float fLength,
	Object *pObject,
	CollisionManager* pCollisionManager,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant)
{
	m_pObject = pObject;
	SetType(TYPE::TYPE_SPHERE);
	pCollisionManager->AddCollider(this);

	m_pTransform = new Object::Transform();

	m_pTransform->position = pos;
	m_pTransform->rot = VECTOR3(0.0f, 0.0f, 0.0f);

	m_rot = VECTOR3(0, 0, 0);
	m_NumPolygon = VECTOR3(8, 0.0f, 8);
	m_fLength = fLength;

	m_NumVertexPolygon = (2 + 2 * m_NumPolygon.x) * m_NumPolygon.z + (m_NumPolygon.x - 1) * 2;
	m_nNumAllPolygon = m_NumVertexPolygon - 2;
	m_nNumVertex = (m_NumPolygon.x + 1) * (m_NumPolygon.z + 1);

	MakeVertex();

#ifdef _DEBUG

	Texture* pTexture = new Texture("resource/sprite/NULL.jpg", pTextureManager);

	m_pRenderer = new MeshRenderer(m_pVertexBuffer,
		m_pIndexBuffer,
		pShaderManager,
		pTexture->GetTexture(),
		pRenderManager->GetShadowTexture(),
		m_pTransform,
		pConstant,
		NULL,
		m_NumVertexPolygon,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_NORMAL,
		GeometryShader::GS_NONE,
		PixelShader::PS_NORMAL,
		FALSE);

	m_pRenderManager = pRenderManager;

	m_pRenderManager->AddRenderer(m_pRenderer);

#endif // DEBUG
}

SphereCollider::~SphereCollider()
{
	if (m_pRenderer != NULL)
	{
		m_pRenderManager->DeleteRenderer(m_pRenderer);
	}
}

void SphereCollider::MakeVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	int nCnt = 0;
	int PosCnt = 0;
	int NrmCnt = 0;
	int y = 0;

	AppRenderer::Vertex3D* vertices = new AppRenderer::Vertex3D[m_nNumVertex];
	for (int z = 0; z < (m_NumPolygon.z + 1); z++, m_rot.z -= 0.4f, m_rot.y = 0)
	{
		for (int x = 0; x < (m_NumPolygon.x + 1); x++, PosCnt++)
		{
			if (PosCnt != ((m_NumPolygon.x + 1) * (m_NumPolygon.z + 1)))
			{
				vertices[PosCnt].position = VECTOR3(m_fLength * sinf(m_rot.z) * cosf(m_rot.y),
					m_fLength * cosf(m_rot.z),
					m_fLength * sinf(m_rot.z) * sinf(m_rot.y));
			}
			m_rot.y += 0.786f;
		}
	}

	for (int nLoop = 0; nLoop < ((m_NumPolygon.x + 1) * (m_NumPolygon.z + 1)); nLoop++)
	{
		vertices[nLoop].normal = VECTOR3(0, 1, 0);
		vertices[nLoop].color = VECTOR4(0.5, 0.5, 1, 0.5);
	}

	for (int nLoop = 0; nLoop < (m_NumPolygon.z + 1); nLoop++)
	{
		for (int nLoop2 = 0; nLoop2 < (m_NumPolygon.x + 1); nLoop2++, nCnt++)
		{
			if (nCnt != ((m_NumPolygon.x + 1) * (m_NumPolygon.z + 1)))
			{
				vertices[nCnt].tex = VECTOR2(nLoop2, nLoop);
			}
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex3D) * m_nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

	WORD* hIndexData = new WORD[m_NumVertexPolygon];

	int nCntIdx = 0;
	int nNo = 0;

	for (int nLoop = 0; nLoop < m_NumPolygon.z; nLoop++)
	{
		if (nCntIdx != 0)
		{
			hIndexData[nCntIdx] = (nLoop + 1) * (m_NumPolygon.z + 1) + 0;
			nCntIdx++;
		}
		hIndexData[nCntIdx] = (nLoop + 1) * (m_NumPolygon.z + 1) + 0;
		nCntIdx++;

		hIndexData[nCntIdx] = nLoop * (m_NumPolygon.z + 1) + 0;
		nCntIdx++;
		for (int nLoop2 = 0; nLoop2 < m_NumPolygon.x; nLoop2++)
		{
			hIndexData[nCntIdx] = (nLoop + 1) * (m_NumPolygon.z + 1) + nLoop2 + 1;
			nCntIdx++;
			hIndexData[nCntIdx] = nLoop * (m_NumPolygon.z + 1) + nLoop2 + 1;
			if (nCntIdx != m_NumVertexPolygon - 1)
			{
				nCntIdx++;
			}
		}
		if (nCntIdx != m_NumVertexPolygon - 1)

		{
			hIndexData[nCntIdx] = nLoop * (m_NumPolygon.z + 1) + m_NumPolygon.z;
			nCntIdx++;
		}
	}

	//インデックスバッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(WORD) * m_NumVertexPolygon;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(WORD);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = hIndexData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &m_pIndexBuffer);

	delete[] vertices;
	delete[] hIndexData;
}