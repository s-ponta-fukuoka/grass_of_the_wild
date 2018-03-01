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
#include "player_attack.h"
#include "player_pattern.h"
#include "player_pattern_attack.h"
#include "player_pattern_wait.h"
#include "player_pattern_walk.h"
#include "player_pattern_deth.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../device/input.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "../../canvas/player_life/player_life.h"
#include "../../../utility/utility.h"
#include "../../mesh/meshfiled/mesh_field.h"
#include "../../../gui/imgui.h"
#include "../../../gui/imgui_impl_dx11.h"
#include "../../../effect/effect_manager.h"
#include "../../../wwise/Wwise.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define VECTOR_UP (10)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Player::Player(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	MainCamera *pCamera,
	CollisionManager* pCollisionManager,
	PlayerLife* pPlayerLife,
	MeshField* pMeshField,
	EffectManager* pEffectManager)
	: m_pCamera(NULL)
	, m_move(VECTOR3(0, 0, 0))
	, m_pCollider(NULL)
	, m_pPlayerPattern(new PlayerPatternWait)
	, m_pPlayerLife(NULL)
	, m_nTime(0)
	, m_pMeshField(NULL)
	, m_pEffectManager(NULL)
	, m_bLife(false)
{
	m_pTransform = new Transform();

	m_pColor = new VECTOR4();
	m_pColor[0] = VECTOR4(1, 1, 1, 1);

	m_pMeshField = pMeshField;

	m_pEffectManager = pEffectManager;

	m_pPlayerLife = pPlayerLife;

	m_CompletionPosition = XMVectorSet(0,0,0,0);
	m_CompletionRot = XMVectorSet(0, 0, 0, 0);

	m_pCamera = pCamera;

	m_pConstant = pConstant;

	m_pModel = new SkinMeshModel("bin/model/naka.taso");
	m_pModel = pModelManager->SeekSkinMeshModel(m_pModel);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();

	SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();

	m_pAnimeNumber[0] = 0;
	m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;

	PixelShader::PIXEL_TYPE ePsType;

	Texture* pToonTexture = new Texture("resource/sprite/toon.png", pTextureManager);
	Texture* pBayerTexture = new Texture("resource/sprite/bayer.png", pTextureManager);

	for (int i = 0; i < m_pModel->GetNumMesh(); i++)
	{

		MakeVertex(i, pMesh);

		
		ID3D11ShaderResourceView* pTextureResource = NULL;

		if(strcmp(pMesh[i].fileName.data(), "") != 0)
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

		pRenderManager->AddDeferredRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
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
			VertexShader::VS_TOON,
			GeometryShader::GS_NONE,
			ePsType,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE,
			m_pColor));
		
		pRenderManager->AddShadowRenderer(new SkinnedMeshRenderer(m_pVertexBuffer,
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
			VertexShader::VS_TOON,
			GeometryShader::GS_NONE,
			ePsType = PixelShader::PS_SHADOW,
			pMesh[i].pCluster,
			pMesh[i],
			FALSE,
			m_pColor));
	}

	if (pCollisionManager != NULL)
	{
		m_pCollider = new SphereCollider(VECTOR3(0, 70, 0), 20, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);
	}

	SetObjectType(Object::TYPE_PLAYER);

	m_pTransform->scale = VECTOR3(1,1,1);

	m_pTransform->position.y = 30;
	m_pTransform->position.z = -2000;

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
	if (m_pTransform != NULL)
	{
		delete m_pTransform;
		m_pTransform = NULL;
	}

	if (m_pFrame != NULL)
	{
		delete m_pFrame;
		m_pFrame = NULL;
	}

	if (m_pAnimeNumber != NULL)
	{
		delete m_pAnimeNumber;
		m_pAnimeNumber = NULL;
	}

	Character::Release();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void Player::Update(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	m_pTransform->position.y = m_pMeshField->GetHeight(m_pTransform->position);
	
	m_oldPos = m_pTransform->position;

	m_pPlayerPattern->Update(this, 
		pRenderManager,
		pShaderManager, 
		pTextureManager,
		pConstant,
		pLightCameraConstant, 
		pCollisionManager);

	SkinMeshModel::Mesh* pMesh = m_pModel->GetMesh();
	
	if (m_pCollider != NULL)
	{
		m_pCollider->GetTransform()->position.x = m_pTransform->position.x;
		m_pCollider->GetTransform()->position.y = m_pTransform->position.y + 70;
		m_pCollider->GetTransform()->position.z = m_pTransform->position.z;
	}

	Object::Update();

	WallCollision();

	Damage();

	ImGui::Begin("player");
	{
		if (ImGui::TreeNode("Transform")) {

			ImGui::Text("Position : X %.2f  Y %.2f  Z %.2f", m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z);
			ImGui::Text("Rotation : X %.2f  Y %.2f  Z %.2f", m_pTransform->rot.x, m_pTransform->rot.y, m_pTransform->rot.z);
			ImGui::Text("Scale    : X %.2f  Y %.2f  Z %.2f", m_pTransform->scale.x, m_pTransform->scale.y, m_pTransform->scale.z);
			ImGui::TreePop();
		}
	}
	ImGui::End();

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
		vertices[j].normal = VECTOR3(pMesh[nMeshCount].pNormal[j].x, pMesh[nMeshCount].pNormal[j].y, pMesh[nMeshCount].pNormal[j].z);
		vertices[j].tex = VECTOR2(pMesh[nMeshCount].pTex[j].x, pMesh[nMeshCount].pTex[j].y);
		vertices[j].color = VECTOR4(pMesh[nMeshCount].pColor[j].x, pMesh[nMeshCount].pColor[j].y, pMesh[nMeshCount].pColor[j].z, pMesh[nMeshCount].pColor[j].w);
		vertices[j].boneIndex = VECTOR4(pMesh[nMeshCount].pBoneIndex[j].x, pMesh[nMeshCount].pBoneIndex[j].y, pMesh[nMeshCount].pBoneIndex[j].z, pMesh[nMeshCount].pBoneIndex[j].w);
		vertices[j].weight = VECTOR4(pMesh[nMeshCount].pWeight[j].x, pMesh[nMeshCount].pWeight[j].y, pMesh[nMeshCount].pWeight[j].z, pMesh[nMeshCount].pWeight[j].w);
	}

	WORD* hIndexData = new WORD[pMesh[nMeshCount].nNumPolygonVertex];

	for (int j = 0; j < pMesh[nMeshCount].nNumPolygonVertex; j++)
	{
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
void Player::OnCollision(Collider* col)
{
	Object::ObjectType eType = col->GetGameObject()->GetObjectType();
	if (eType == Object::TYPE_ENEMY)
	{
		VECTOR3 pos = m_pTransform->position - col->GetGameObject()->GetTransform()->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z,1.0);

		XMVector3Dot(Dot,Dot);

		m_pTransform->position.x += XMVectorGetX(Dot);
		m_pTransform->position.z += XMVectorGetZ(Dot);
	}

	if (eType == Object::TYPE_TREE)
	{
		VECTOR3 pos = m_pTransform->position - col->GetGameObject()->GetSphereCollider()->GetTransform()->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * 4;
		m_pTransform->position.z += XMVectorGetZ(Dot) * 4;
	}

	if (eType == Object::TYPE_ENEMY_ATTACK)
	{
		if (m_pPlayerLife->GetLife() > 0)
		{
			if (!m_bLife)
			{
				Wwise* pWwise = Wwise::GetInstance();
				pWwise->MainListenerGameObjEvent(EVENTS::ATTACK);
				m_pPlayerLife->Sub(1);
			}
			m_bLife = true;
		}
		if (m_pPlayerLife->GetLife() <= 0)
		{
			m_pAnimeNumber[0] = 10;
			SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();
			m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;
			ChangePlayerPattern(new PlayerPatternDeth());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//ダメージ受けた時の処理
///////////////////////////////////////////////////////////////////////////////
void Player::Damage(void)
{
	XMVECTOR color;
	XMVECTOR startColor = XMVectorSet(m_pColor[0].x,
		m_pColor[0].y,
		m_pColor[0].z, 1.0f);

	if (m_bLife)
	{
		color = XMVectorSet(1, 0.6f, 0.6f, 1);
		startColor = XMVectorLerp(startColor, color, 0.1f);
		m_pColor[0].x = XMVectorGetX(startColor);
		m_pColor[0].y = XMVectorGetY(startColor);
		m_pColor[0].z = XMVectorGetZ(startColor);
		if (m_pColor[0].y <= 0.65)
		{
			m_bLife = !m_bLife;
		}
	}
	else
	{
		color = XMVectorSet(1, 1, 1, 1);
		startColor = XMVectorLerp(startColor, color, 0.1f);
		m_pColor[0].x = XMVectorGetX(startColor);
		m_pColor[0].y = XMVectorGetY(startColor);
		m_pColor[0].z = XMVectorGetZ(startColor);
	}
}

///////////////////////////////////////////////////////////////////////////////
//壁との当たり判定
///////////////////////////////////////////////////////////////////////////////
void Player::WallCollision(void)
{
	if (m_pTransform->position.x <= -4000)
	{
		VECTOR3 pos = VECTOR3(1, 0, 0) - m_pTransform->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * VECTOR_UP;
		m_pTransform->position.z += XMVectorGetZ(Dot) * VECTOR_UP;
	}
	if (m_pTransform->position.x >= 4000)
	{
		VECTOR3 pos = VECTOR3(-1, 0, 0) - m_pTransform->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * VECTOR_UP;
		m_pTransform->position.z += XMVectorGetZ(Dot) * VECTOR_UP;
	}

	if (m_pTransform->position.z <= -3900)
	{
		VECTOR3 pos = VECTOR3(0, 0, 1) - m_pTransform->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * VECTOR_UP;
		m_pTransform->position.z += XMVectorGetZ(Dot) * VECTOR_UP;
	}
	if (m_pTransform->position.z >= 3900)
	{
		VECTOR3 pos = VECTOR3(0, 0, -1) - m_pTransform->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * VECTOR_UP;
		m_pTransform->position.z += XMVectorGetZ(Dot) * VECTOR_UP;
	}
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* Player::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//プレイヤーステート変更
///////////////////////////////////////////////////////////////////////////////
void Player::ChangePlayerPattern(PlayerPattern* pPlayerPattern)
{
	if (pPlayerPattern == NULL) { return; }
	delete m_pPlayerPattern;
	m_pPlayerPattern = pPlayerPattern;
}