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
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Player::Player(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant)
	: m_pModel(NULL)
	, m_pFrame(NULL)
{
	m_pModel = new SkinMeshModel("bin/model/naka.txt");
	m_pModel = pModelManager->SeekSkinMeshModel(m_pModel);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();

	m_pFrame = new int();

	m_pAnimeNumber = new int();

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
			ePsType = PixelShader::PS_MAT;
		}

		pRenderManager->AddRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			pRenderManager->GetShadowTexture(),
			m_pTransform,
			pConstant,
			pLightCameraConstant,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_TOON,
			ePsType,
			pMesh[i].pCluster,
			pMesh[i]));

		pRenderManager->AddShadowRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
			m_pIndexBuffer,
			pShaderManager,
			pTextureResource,
			NULL,
			m_pTransform,
			pLightCameraConstant,
			NULL,
			pMesh[i].nNumPolygonVertex,
			m_pFrame,
			m_pAnimeNumber,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			VertexShader::VS_TOON,
			ePsType = PixelShader::PS_SHADOW,
			pMesh[i].pCluster,
			pMesh[i]));
	}

	m_pTransform->rot.y = 180;
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
	//移動処理
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		m_pTransform->position.z++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		m_pTransform->position.x--;
	}

	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		m_pTransform->position.z--;
	}

	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		m_pTransform->position.x++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{
		m_pTransform->position.y++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_N))
	{
		m_pTransform->position.y--;
	}

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

	delete[] pMesh[nMeshCount].pPosition;
	delete[] pMesh[nMeshCount].pColor;
	delete[] pMesh[nMeshCount].pNormal;
	delete[] pMesh[nMeshCount].pTex;
	delete[] pMesh[nMeshCount].pWeight;
	delete[] pMesh[nMeshCount].pBoneIndex;
}