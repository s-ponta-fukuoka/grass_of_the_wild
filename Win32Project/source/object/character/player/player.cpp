//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (50)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Player::Player(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, MainCamera *pCamera, CollisionManager* pCollisionManager)
	: m_pCamera(NULL)
	, m_move(VECTOR3(0,0,0))
	, m_pCollider(NULL)
{
	m_pTransform = new Transform();

	m_pCollider = new SphereCollider(this,pCollisionManager,  pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

	m_CompletionPosition = XMVectorSet(0,0,0,0);
	m_CompletionRot = XMVectorSet(0, 0, 0, 0);

	m_pCamera = pCamera;

	m_pModel = new SkinMeshModel("bin/model/naka.taso");
	m_pModel = pModelManager->SeekSkinMeshModel(m_pModel);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();

	SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();

	m_pAnimeNumber[0] = 0;
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

		//pRenderManager->AddRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
		//	m_pIndexBuffer,
		//	pShaderManager,
		//	pTextureResource,
		//	pRenderManager->GetShadowTexture(),
		//	m_pTransform,
		//	pConstant,
		//	pLightCameraConstant,
		//	pMesh[i].nNumPolygonVertex,
		//	m_pFrame,
		//	m_pAnimeNumber,
		//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		//	VertexShader::VS_TOON,
		//	ePsType,
		//	pMesh[i].pCluster,
		//	pMesh[i]));
		//
		//pRenderManager->AddShadowRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
		//	m_pIndexBuffer,
		//	pShaderManager,
		//	pTextureResource,
		//	NULL,
		//	m_pTransform,
		//	pLightCameraConstant,
		//	NULL,
		//	pMesh[i].nNumPolygonVertex,
		//	m_pFrame,
		//	m_pAnimeNumber,
		//	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		//	VertexShader::VS_TOON,
		//	ePsType = PixelShader::PS_SHADOW,
		//	pMesh[i].pCluster,
		//	pMesh[i]));
	}

	SetObjectType(Object::TYPE_PLAYER);

	m_pTransform->rot.y = 0;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
Player::~Player()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT Player::Init(void)
{
	Character::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void Player::Release(void)
{
	delete m_pModel;
	delete m_pFrame;
	delete m_pAnimeNumber;

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void Player::Update(void)
{
	InputOperation();

	ChangeAnime();

	Object::Update();
}

///////////////////////////////////////////////////////////////////////////////
//バッファ作成
///////////////////////////////////////////////////////////////////////////////
void Player::MakeVertex(int nMeshCount, SkinMeshModel::Mesh* pMesh)
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
//キーボード操作
///////////////////////////////////////////////////////////////////////////////
void Player::InputOperation(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XMVECTOR StartPosition = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 1.0f);

	XMVECTOR vector = m_pCamera->GetVec();

	XMVECTOR StartRot = XMVectorSet(0, m_pTransform->rot.y, 0, 1.0f);

	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		m_move.x -= XMVectorGetX(vector) * PLAYER_MOVE;
		m_move.z -= XMVectorGetZ(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		m_move.x += XMVectorGetZ(vector) * PLAYER_MOVE;
		m_move.z -= XMVectorGetX(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		m_move.x += XMVectorGetX(vector) * PLAYER_MOVE;
		m_move.z += XMVectorGetZ(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		m_move.x -= XMVectorGetZ(vector) * PLAYER_MOVE;
		m_move.z += XMVectorGetX(vector) * PLAYER_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_W) ||
		pInputKeyboard->GetKeyPress(DIK_A) ||
		pInputKeyboard->GetKeyPress(DIK_S) ||
		pInputKeyboard->GetKeyPress(DIK_D))
	{
		m_CompletionPosition = XMVectorSet(m_pTransform->position.x + m_move.x, 0, m_pTransform->position.z + m_move.z, 0);
		m_CompletionRot = XMVectorSet(0,atan2(-m_move.z, -m_move.x) + D3D_PI * 0.5, 0,1);
		m_pAnimeNumber[0] = 1;
	}
	else
	{
		m_pAnimeNumber[0] = 0;
	}

	m_pTransform->position += m_move;
	m_move *= 0.1f;

	StartRot = XMVectorLerp(StartRot, m_CompletionRot, 0.1f);
	m_pTransform->rot.y = XMVectorGetY(StartRot);

	StartPosition = XMVectorLerp(StartPosition, m_CompletionPosition, 0.1f);
	m_pTransform->position.x = XMVectorGetX(StartPosition);
	m_pTransform->position.z = XMVectorGetZ(StartPosition);
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void Player::OnCollision(Collider* col)
{
	
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* Player::GetSphereCollider(void)
{
	return m_pCollider;
}