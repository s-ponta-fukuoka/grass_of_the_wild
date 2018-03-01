//=============================================================================
//
// tree.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "tree.h"
#include "tree_manager.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "../../../utility/utility.h"
#include "../../mesh/meshfiled/mesh_field.h"
#include "tree_collider.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (200)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Tree::Tree(
	VECTOR3 position,
	VECTOR3 rot,
	VECTOR3 scl,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, 
	MainCamera *pCamera,
	CollisionManager* pCollisionManager,
	TreeManager* pTreeManager,
	MeshField* pMeshField)
	: m_pCamera(NULL)
	, m_pCollider(NULL)
	, m_move(VECTOR3(0,0,0))
	, m_bUse(false)
{
	m_pTransform = new Transform[TREE_MAX];

	m_CompletionPosition = XMVectorSet(0,0,0,0);
	m_CompletionRot = XMVectorSet(0, 0, 0, 0);

	m_pMeshField = pMeshField;

	m_pCamera = pCamera;

	m_pModelManager = pModelManager;

	m_pModel = new SkinMeshModel("bin/model/tree.TASO");
	m_pModel = pModelManager->SeekSkinMeshModel(m_pModel);
	
	Texture* pToonTexture = new Texture("resource/sprite/toon.png", pTextureManager);
	Texture* pBayerTexture = new Texture("resource/sprite/bayer.png", pTextureManager);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();

	SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();

	m_pAnimeNumber[0] = 0;
	m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;

	PixelShader::PIXEL_TYPE ePsType;
	
	m_pRenderer = new Renderer*[m_pModel->GetNumMesh()];
	m_pShadowRenderer = new Renderer*[m_pModel->GetNumMesh()];

	m_pTreeManager = pTreeManager;

	for (int i = 0; i < m_pModel->GetNumMesh(); i++)
	{

		MakeVertex(i, pMesh);

		
		ID3D11ShaderResourceView* pTextureResource = NULL;

		if (strcmp(pMesh[i].fileName.data(), "") != 0)
		{
			ePsType = PixelShader::PS_TOON;
			Texture* pTexture = new Texture(pMesh[i].fileName.data(), pTextureManager);
			pTextureResource = pTexture->GetTexture();
		}
		else
		{
			pMesh[i].pFileName = NULL;
			ePsType = PixelShader::PS_MAT;
		}

		m_pRenderManager = pRenderManager;

		m_pRenderer[i] = new ModelObjectRenderer(m_pVertexBuffer,
			m_pInstanceBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			pToonTexture->GetTexture(),
			pBayerTexture->GetTexture(),
			pRenderManager->GetShadowTexture(),
			m_pTransform,
			pConstant,
			pLightCameraConstant,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_MODEL,
			GeometryShader::GS_NONE,
			ePsType,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE);

		m_pShadowRenderer[i] = new ModelObjectRenderer(m_pVertexBuffer,
			m_pInstanceBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			pToonTexture->GetTexture(),
			pBayerTexture->GetTexture(),
			NULL,
			m_pTransform,
			pLightCameraConstant,
			NULL,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_MODEL,
			GeometryShader::GS_NONE,
			ePsType = PixelShader::PS_SHADOW,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE);

		m_pRenderManager->AddDeferredRenderer(m_pRenderer[i]);

		m_pRenderManager->AddShadowRenderer(m_pShadowRenderer[i]);
	}

	m_pCollisionManager = pCollisionManager;

	for (int nCnt = 0; nCnt < TREE_MAX; nCnt++)
	{
		m_pTreeCollider[nCnt] =  new TreeCollider(pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant, pCollisionManager, 
			VECTOR3(m_pTransform[nCnt].position.x, m_pTransform[nCnt].position.y + 70, m_pTransform[nCnt].position.z));
	}
	SetObjectType(Object::TYPE_TREE);
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
Tree::~Tree()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT Tree::Init(void)
{
	Terrain::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void Tree::Release(void)
{
	for (int nCnt = 0; nCnt < TREE_MAX; nCnt++)
	{
		if (m_pTreeCollider[nCnt] != NULL)
		{
			m_pTreeCollider[nCnt]->Release();
			delete m_pTreeCollider[nCnt];
			m_pTreeCollider[nCnt] = NULL;
		}
	}
	for (int i = 0; i < m_pModel->GetNumMesh(); i++)
	{
		m_pRenderManager->DeleteDeferredRenderer(m_pRenderer[i]);
		m_pRenderManager->DeleteShadowRenderer(m_pShadowRenderer[i]);
		delete[] m_pRenderer[i];
		delete[] m_pShadowRenderer[i];
	}

	Terrain::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void Tree::Update(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	for (int nCnt = 0; nCnt < TREE_MAX; nCnt++)
	{
		//m_pCollider[nCnt]->GetTransform()->position.x = m_pTransform->position.x;
		//m_pCollider[nCnt]->GetTransform()->position.z = m_pTransform->position.z;
	}

	Object::Update();

	if (m_bUse)
	{
		//Release();
		m_bUse = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//バッファ作成
///////////////////////////////////////////////////////////////////////////////
void Tree::MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//四角形
	AppRenderer::Vertex3D* vertices = new AppRenderer::Vertex3D[pMesh[nMeshCount].nNumVertex];
	for (int j = 0; j < pMesh[nMeshCount].nNumVertex; j++)
	{
		vertices[j].position = VECTOR3(pMesh[nMeshCount].pPosition[j].x, pMesh[nMeshCount].pPosition[j].y, pMesh[nMeshCount].pPosition[j].z);
		vertices[j].normal = VECTOR3(pMesh[nMeshCount].pNormal[j].x, pMesh[nMeshCount].pNormal[j].y, pMesh[nMeshCount].pNormal[j].z);
		vertices[j].tex = VECTOR2(pMesh[nMeshCount].pTex[j].x, pMesh[nMeshCount].pTex[j].y);
		vertices[j].color = VECTOR4(pMesh[nMeshCount].pColor[j].x, pMesh[nMeshCount].pColor[j].y, pMesh[nMeshCount].pColor[j].z, pMesh[nMeshCount].pColor[j].w);
	}

	WORD* hIndexData = new WORD[pMesh[nMeshCount].nNumPolygonVertex];

	for (int j = 0; j < pMesh[nMeshCount].nNumPolygonVertex; j++)
	{
		hIndexData[j] = pMesh[nMeshCount].pIndexNumber[j];
	}


	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AppRenderer::Vertex3D) * pMesh[nMeshCount].nNumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return;


	//インデックスバッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(WORD) *  pMesh[nMeshCount].nNumPolygonVertex;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(WORD);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = hIndexData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &m_pIndexBuffer)))
		return;
	
	delete[] vertices;

	delete[] hIndexData;

	XMMATRIX* hWorld = new XMMATRIX[TREE_MAX];

	for (int nCnt = 0; nCnt < TREE_MAX; nCnt++)
	{
		m_pTransform[nCnt].position = VECTOR3(-4000, 0, -4000);
		m_pTransform[nCnt].rot = VECTOR3(-90, 0, 0);
		m_pTransform[nCnt].scale = VECTOR3(100, 100, 100);

		m_pTransform[nCnt].position.x += Utility::Hulton(nCnt, 2) * 8000;
		m_pTransform[nCnt].position.z += Utility::Hulton(nCnt, 3) * 5300;
		m_pTransform[nCnt].position.y = m_pMeshField->GetHeight(m_pTransform[nCnt].position);

		XMMATRIX hPosition = XMMatrixIdentity();
		hPosition = XMMatrixTranslation(m_pTransform[nCnt].position.x, m_pTransform[nCnt].position.y, m_pTransform[nCnt].position.z);
		XMMATRIX hRotate = XMMatrixIdentity();
		hRotate = XMMatrixRotationRollPitchYaw(D3DToRadian(m_pTransform[nCnt].rot.x), D3DToRadian(m_pTransform[nCnt].rot.y), D3DToRadian(m_pTransform[nCnt].rot.z));
		XMMATRIX hScaling = XMMatrixIdentity();
		hScaling = XMMatrixScaling(m_pTransform[nCnt].scale.x, m_pTransform[nCnt].scale.y, m_pTransform[nCnt].scale.z);

		hWorld[nCnt] = XMMatrixIdentity();

		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hScaling);
		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hRotate);
		hWorld[nCnt] = XMMatrixMultiply(hWorld[nCnt], hPosition);

		hWorld[nCnt] = XMMatrixTranspose(hWorld[nCnt]);
	}

	D3D11_BUFFER_DESC Ibd;
	Ibd.Usage = D3D11_USAGE_DYNAMIC;
	Ibd.ByteWidth = sizeof(XMMATRIX) * TREE_MAX;
	Ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	Ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InstanceInitData;
	InstanceInitData.pSysMem = hWorld;
	pDevice->CreateBuffer(&Ibd, &InstanceInitData, &m_pInstanceBuffer);

	delete[] hWorld;
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void Tree::OnCollision(Collider* col)
{
	Object::ObjectType eObjectType = col->GetGameObject()->GetObjectType();
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* Tree::GetSphereCollider(void)
{
	return m_pCollider;
}