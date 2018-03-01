//=============================================================================
//
// effect_manager.cpp(エフェクシア)
// Author : shota fukuoka
//
//=============================================================================
#include "effect_manager.h"
#include "../app/app.h"
#include "../renderer/app_renderer.h"
#include "../device/input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
EffectManager::EffectManager()
	: m_pManager(NULL)
	, m_pRenderer(NULL)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		m_pEffect[i] = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
EffectManager::~EffectManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Init(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	// 描画用インスタンスの生成
	m_pRenderer = ::EffekseerRendererDX11::Renderer::Create(pDevice, pDeviceContext, 10000);

	// エフェクト管理用インスタンスの生成
	m_pManager = ::Effekseer::Manager::Create(2000);

	// 描画用インスタンスから描画機能を設定
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());

	GenerateEffect();
}

///////////////////////////////////////////////////////////////////////////////
//エフェクト生成
///////////////////////////////////////////////////////////////////////////////
void EffectManager::GenerateEffect(void)
{
	// エフェクトの読込
	m_pEffect[EFFECT_HIT] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/hit.efk");
	m_pEffect[EFFECT_DETH] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/deth.efk");
	m_pEffect[EFFECT_ATTACK_001] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_001.efk");
	m_pEffect[EFFECT_ATTACK_002] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_002.efk");
	m_pEffect[EFFECT_ATTACK_003] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_003.efk");
	m_pEffect[EFFECT_SMOKE] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/smoke.efk");
	m_pEffect[EFFECT_ENEMY_DEFFENCE] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/guard.efk");
	m_pEffect[EFFECT_ATTACK_JUMP] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_jump.efk");
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Release(void)
{
	// エフェクトの停止
	m_pManager->StopEffect(m_handle);

	// エフェクトの破棄
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		SAFE_RELEASE(m_pEffect[i]);
	}

	// 先にエフェクト管理用インスタンスを破棄
	m_pManager->Destroy();
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Update(void)
{
	m_pManager->Update();
}

///////////////////////////////////////////////////////////////////////////////
//描画
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Draw(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	// エフェクトの描画開始処理を行う。
	m_pRenderer->BeginRendering();

	// エフェクトの描画を行う。
	m_pManager->Draw();

	// エフェクトの描画終了処理を行う。
	m_pRenderer->EndRendering();
}

///////////////////////////////////////////////////////////////////////////////
//エフェクト取得
///////////////////////////////////////////////////////////////////////////////
::Effekseer::Effect* EffectManager::GetEffect(EFFECT effect)
{
	return m_pEffect[effect];
}

///////////////////////////////////////////////////////////////////////////////
//エフェクトマネージャー取得
///////////////////////////////////////////////////////////////////////////////
::Effekseer::Manager* EffectManager::GetEManager(void)
{
	return m_pManager;
}

///////////////////////////////////////////////////////////////////////////////
//エフェクトレンダラー取得
///////////////////////////////////////////////////////////////////////////////
::EffekseerRenderer::Renderer* EffectManager::GetERendererer(void)
{
	return m_pRenderer;
}