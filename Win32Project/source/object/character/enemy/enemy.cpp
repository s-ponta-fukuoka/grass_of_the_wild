//=============================================================================
//
// enemy.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "enemy.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (200)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Enemy::Enemy(
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
	CollisionManager* pCollisionManager)
	: m_pCamera(NULL)
	, m_move(VECTOR3(0,0,0))
	, m_pCollider(NULL)
{
	m_pTransform = new Transform();

	m_pTransform->position = position;
	m_pTransform->rot = rot;
	m_pTransform->scale = scl;

	m_CompletionPosition = XMVectorSet(0,0,0,0);
	m_CompletionRot = XMVectorSet(0, 0, 0, 0);

	m_pCamera = pCamera;

	m_pModel = new SkinMeshModel("bin/model/enemy_000.taso");
	m_pModel = pModelManager->SeekSkinMeshModel(m_pModel);
	
	Texture* pToonTexture = new Texture("resource/sprite/toon.png", pTextureManager);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();

	SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();

	m_pAnimeNumber[0] = 1;
	m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;

	PixelShader::PIXEL_TYPE ePsType;

	for (int i = 0; i < m_pModel->GetNumMesh(); i++)
	{

		MakeVertex(i, pMesh);

		
		ID3D11ShaderResourceView* pTextureResource = NULL;

		if (pMesh[i].pFileName != NULL)
		{
			ePsType = PixelShader::PS_TOON;
			Texture* pTexture = new Texture(pMesh[i].pFileName, pTextureManager);
			pTextureResource = pTexture->GetTexture();
		}
		else
		{
			pMesh[i].pFileName = NULL;
			ePsType = PixelShader::PS_MAT;
		}

		pRenderManager->AddRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			pToonTexture->GetTexture(),
			pRenderManager->GetShadowTexture(),
			m_pTransform,
			pConstant,
			pLightCameraConstant,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_TOON,
			GeometryShader::GS_NONE,
			ePsType,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE));

		pRenderManager->AddShadowRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			pToonTexture->GetTexture(),
			NULL,
			m_pTransform,
			pLightCameraConstant,
			NULL,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_TOON,
			GeometryShader::GS_NONE,
			ePsType = PixelShader::PS_SHADOW,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE));
	}

	m_pCollider = new SphereCollider(VECTOR3(0, 70, 0), 50, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

	SetObjectType(Object::TYPE_ENEMY);
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
Enemy::~Enemy()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT Enemy::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void Enemy::Release(void)
{
	delete m_pModel;
	delete m_pFrame;
	delete m_pAnimeNumber;

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void Enemy::Update(void)
{
	m_pCollider->GetTransform()->position.x = m_pTransform->position.x;
	m_pCollider->GetTransform()->position.z = m_pTransform->position.z;

	ChangeAnime();

	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//バッファ作成
///////////////////////////////////////////////////////////////////////////////
void Enemy::MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//四角形
	SkinMeshModel::ModelVertex* vertices = new SkinMeshModel::ModelVertex[pMesh[nMeshCount].nNumVertex];
	for (int j = 0; j < pMesh[nMeshCount].nNumVertex; j++)
	{
		vertices[j].position = VECTOR3(pMesh[nMeshCount].pPosition[j].x, pMesh[nMeshCount].pPosition[j].y, pMesh[nMeshCount].pPosition[j].z);
		vertices[j].color = VECTOR4(pMesh[nMeshCount].pColor[j].x, pMesh[nMeshCount].pColor[j].y, pMesh[nMeshCount].pColor[j].z, pMesh[nMeshCount].pColor[j].w);
		vertices[j].boneIndex = VECTOR4(pMesh[nMeshCount].pBoneIndex[j].x, pMesh[nMeshCount].pBoneIndex[j].y, pMesh[nMeshCount].pBoneIndex[j].z, pMesh[nMeshCount].pBoneIndex[j].w);
		vertices[j].weight = VECTOR4(pMesh[nMeshCount].pWeight[j].x, pMesh[nMeshCount].pWeight[j].y, pMesh[nMeshCount].pWeight[j].z, pMesh[nMeshCount].pWeight[j].w);
	}

	WORD* hIndexData = new WORD[pMesh[nMeshCount].nNumPolygonVertex];

	for (int j = 0; j < pMesh[nMeshCount].nNumPolygonVertex; j++)
	{
		vertices[pMesh[nMeshCount].pIndexNumber[j]].normal = VECTOR3(pMesh[nMeshCount].pNormal[j].x, pMesh[nMeshCount].pNormal[j].y, pMesh[nMeshCount].pNormal[j].z);
		vertices[pMesh[nMeshCount].pIndexNumber[j]].tex = VECTOR2(pMesh[nMeshCount].pTex[j].x, 1 - pMesh[nMeshCount].pTex[j].y);
		hIndexData[j] = pMesh[nMeshCount].pIndexNumber[j];
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinMeshModel::ModelVertex) * pMesh[nMeshCount].nNumVertex;
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
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void Enemy::OnCollision(Collider* col)
{

}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* Enemy::GetSphereCollider(void)
{
	return m_pCollider;
}