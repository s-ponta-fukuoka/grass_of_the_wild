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
#include "enemy_manager.h"
#include "../../../object/camera/main_camera.h"
#include "../../../model/model.h"
#include "../../../model/model_manager.h"
#include "../../../renderer/render_manager.h"
#include "../../../collision/collider.h"
#include "../../../collision/collision_manager.h"
#include "../../mesh/meshfiled/mesh_field.h"
#include "enemy_pattern.h"
#include "enemy_pattern_attack.h"
#include "enemy_pattern_wait.h"
#include "enemy_pattern_walk.h"
#include "enemy_pattern_run.h"
#include "enemy_pattern_deth.h"
#include "enemy_attack.h"
#include "enemy_pattern_hit.h"
#include "../../../device/input.h"
#include "enemy_life.h"
#include "../../../effect/effect_manager.h"
#include "../../../wwise/Wwise.h"
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
	CollisionManager* pCollisionManager,
	EnemyManager* pEnemyManager,
	Object::Transform* pPlayerTransform,
	MeshField* pMeshField,
	EffectManager* pEffectManager)
	: m_pCamera(NULL)
	, m_move(VECTOR3(0,0,0))
	, m_pCollider(NULL)
	, m_bUse(false)
	, m_pEnemyPattern(new EnemyPatternWait)
	, m_nTime(0)
	, m_pEnemyAttack(NULL)
	, m_pEnemyLife(NULL)
	, m_pEffectManager(NULL)
{
	m_nCounterCnt = 0;

	m_eMode = MODE_DEFENSE;

	m_pTransform = new Transform();

	m_pEffectManager = pEffectManager;

	m_pTransform->position = position;
	m_pTransform->rot = rot;
	m_pTransform->scale = scl;

	m_CompletionPosition = XMVectorSet(0,0,0,0);
	m_CompletionRot = XMVectorSet(0, 0, 0, 0);

	m_pPlayerTransform = pPlayerTransform;

	m_pMeshField = pMeshField;

	m_pEnemyLife = new EnemyLife(pRenderManager, pShaderManager, pTextureManager);

	m_pCamera = pCamera;

	m_pModelManager = pModelManager;

	m_pColor = new VECTOR4();
	m_pColor[0] = VECTOR4(1, 1, 1, 1);

	m_pModel = new SkinMeshModel("bin/model/enemy_001.taso");
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

	m_pEnemyManager = pEnemyManager;

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

		m_pRenderer[i]  = new SkinnedMeshRenderer(m_pVertexBuffer,
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
			m_pColor);

		m_pShadowRenderer[i] = new SkinnedMeshRenderer(m_pVertexBuffer,
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
			m_pColor);

		m_pRenderManager->AddDeferredRenderer(m_pRenderer[i]);

		m_pRenderManager->AddShadowRenderer(m_pShadowRenderer[i]);
	}

	m_pCollisionManager = pCollisionManager;

	m_pCollider = new SphereCollider(VECTOR3(0, 150, 0), 100, this, pCollisionManager, pRenderManager, pShaderManager, pTextureManager, pConstant, pLightCameraConstant);

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
	if (m_pCollider != NULL)
	{
		delete m_pCollider;
		m_pCollider = NULL;
	}

	if(m_pEnemyAttack != NULL)
	{
		m_pEnemyAttack->Release();
		delete m_pEnemyAttack;
		m_pEnemyAttack = NULL;
	}

	if (m_pRenderer != NULL)
	{
		for (int i = 0; i < m_pModel->GetNumMesh(); i++)
		{
			m_pRenderManager->DeleteDeferredRenderer(m_pRenderer[i]);

			m_pRenderManager->DeleteShadowRenderer(m_pShadowRenderer[i]);

			if (m_pRenderer[i] != NULL)
			{
				delete m_pRenderer[i];
				m_pRenderer[i] = NULL;
			}

			if (m_pShadowRenderer[i] != NULL)
			{
				delete m_pShadowRenderer[i];
				m_pShadowRenderer[i] = NULL;
			}

		}
	}

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
void Enemy::Update(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	m_pTransform->position.y -= 0.1f;
	m_pTransform->position.y = m_pMeshField->GetHeight(m_pTransform->position);
 
	m_pCollider->GetTransform()->position.x = m_pTransform->position.x;
	m_pCollider->GetTransform()->position.z = m_pTransform->position.z;
	m_pCollider->GetTransform()->position.y = m_pTransform->position.y + 100;

	m_oldPos = m_pTransform->position;

	m_pEnemyPattern->Update(this,
		pRenderManager,
		pShaderManager,
		pTextureManager,
		pConstant,
		pLightCameraConstant,
		pCollisionManager);

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
	{
		delete[] vertices;
		delete[] hIndexData;
		return;
	}

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
	{
		delete[] vertices;
		delete[] hIndexData;
		return;
	}

	delete[] vertices;

	delete[] hIndexData;
}

///////////////////////////////////////////////////////////////////////////////
//あたり判定
///////////////////////////////////////////////////////////////////////////////
void Enemy::OnCollision(Collider* col)
{
	Object::ObjectType eObjectType = col->GetGameObject()->GetObjectType();

	if (eObjectType == Object::TYPE_PLAYER_ATTACK || eObjectType == Object::TYPE_PLAYER_ATTACK_JUMP)
	{
		if (m_eMode == MODE_ATTACK)
		{
			int nlife = m_pEnemyLife->GetLife();

			Wwise* pWwise = Wwise::GetInstance();
			pWwise->MainListenerGameObjEvent(EVENTS::ATTACK);
			pWwise->MainListenerGameObjEvent(EVENTS::HIT);

			if (eObjectType == Object::TYPE_PLAYER_ATTACK)
			{
				m_pEnemyLife->Sub(1);
			}
			else if (eObjectType == Object::TYPE_PLAYER_ATTACK_JUMP)
			{
				m_pEnemyLife->Sub(2);
			}

			if (nlife != m_pEnemyLife->GetLife() && m_pEnemyLife->GetLife() > 0)
			{
					// エフェクトの再生
					::Effekseer::Handle handle;
					handle = m_pEffectManager->GetEManager()->Play(m_pEffectManager->GetEffect(EffectManager::EFFECT_HIT),
						m_pTransform->position.x + cosf(-m_pTransform->rot.y - D3DToRadian(85)),
						m_pTransform->position.y + 150,
						m_pTransform->position.z - sinf(m_pTransform->rot.y - D3DToRadian(85)));
					m_pEffectManager->GetEManager()->SetScale(handle, 50, 50, 50);

				if (m_pAnimeNumber[0] != 3)
				{
					m_pAnimeNumber[0] = 5;
					SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();
					m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime + 100;
					ChangeEnemyPattern(new EnemyPatternHit);
				}
			}
		}
		else
		{

			// エフェクトの再生
			::Effekseer::Handle handle;
			handle = m_pEffectManager->GetEManager()->Play(m_pEffectManager->GetEffect(EffectManager::EFFECT_ENEMY_DEFFENCE),
				m_pTransform->position.x + cosf(-m_pTransform->rot.y - D3DToRadian(90)) * 100,
				m_pTransform->position.y + 150,
				m_pTransform->position.z - sinf(m_pTransform->rot.y - D3DToRadian(90))* 100);
			m_pEffectManager->GetEManager()->SetScale(handle, 50, 50, 50);
			m_pEffectManager->GetEManager()->SetRotation(handle, 0, -m_pTransform->rot.y, 0);
			m_pAnimeNumber[0] = 0;
			SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();
			m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;
			m_nCounterCnt++;
			ChangeEnemyPattern(new EnemyPatternWait);
		}

		if (m_pEnemyLife->GetLife() <= 0 && !m_bUse)
		{
			m_pAnimeNumber[0] = 7;
			SkinMeshModel::Anime* pAnime = m_pModel->GetAnime();
			m_pFrame[0] = pAnime[m_pAnimeNumber[0]].nStartTime;
			ChangeEnemyPattern(new EnemyPatternDeth);
			m_bUse = true;
		}
	}

	if (eObjectType == Object::TYPE_PLAYER)
	{
		VECTOR3 pos = m_pTransform->position - col->GetGameObject()->GetTransform()->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot);
		m_pTransform->position.z += XMVectorGetZ(Dot);
	}

	if (eObjectType == Object::TYPE_ENEMY)
	{
		VECTOR3 pos = m_pTransform->position - col->GetGameObject()->GetTransform()->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot);
		m_pTransform->position.z += XMVectorGetZ(Dot);
	}

	if (eObjectType == Object::TYPE_TREE)
	{
		VECTOR3 pos = m_pTransform->position - col->GetGameObject()->GetSphereCollider()->GetTransform()->position;

		VECTOR3::Normalize(&pos, &pos);

		XMVECTOR Dot = XMVectorSet(pos.x, pos.y, pos.z, 1.0);

		XMVector3Dot(Dot, Dot);

		m_pTransform->position.x += XMVectorGetX(Dot) * 10;
		m_pTransform->position.z += XMVectorGetZ(Dot) * 10;
	}
}

///////////////////////////////////////////////////////////////////////////////
//コライダー取得
///////////////////////////////////////////////////////////////////////////////
SphereCollider* Enemy::GetSphereCollider(void)
{
	return m_pCollider;
}

///////////////////////////////////////////////////////////////////////////////
//エネミーステート変更
///////////////////////////////////////////////////////////////////////////////
void Enemy::ChangeEnemyPattern(EnemyPattern* pEnemyPattern)
{
	if (pEnemyPattern == NULL) { return; }
	delete m_pEnemyPattern;
	m_pEnemyPattern = pEnemyPattern;
}