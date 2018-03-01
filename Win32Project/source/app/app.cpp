//=============================================================================
//
// app.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "app.h"
#include "../renderer/app_renderer.h"
#include "../device/input.h"
#include "../device/xbox_controller.h"
#include "../wwise/Wwise.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../scene/next_scene.h"
#include "../scene/scene.h"
#include "../scene/title_scene.h"
#include "../scene/tutorial_scene.h"
#include "../scene/game_scene.h"
#include "../scene/game_over_scene.h"
#include "../scene/fade_scene.h"
#include "../shader/shader_manager.h"
#include "../renderer/deferred _renderer.h"
#include "../model/model_manager.h"
#include "../texture/texture_manager.h"
#include "../effect/effect_manager.h"

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
App::App()
	: m_pAppRenderer(NULL)
	, m_pScene(NULL)
	, m_pNextScene(NULL)
	, m_pInputKeybord(NULL)
	, m_pFade(NULL)
	, m_pShaderManager(NULL)
	, m_pDeferredRenderer(NULL)
	, m_pModelManager(NULL)
	, m_pTextureManager(NULL)
	, m_pEffectManager(NULL)
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
App::~App()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT App::Init(const HWND hWnd, HINSTANCE hInstance)
{
	InputKeyboard::CreateInstance();
	m_pInputKeybord = InputKeyboard::GetInstance();
	m_pInputKeybord->Init(hInstance, hWnd);

	XboxController::CreateInstance();
	m_pXboxController = XboxController::GetInstance();
	m_pXboxController->Init();

	Wwise::CreateInstance();
	m_pWwise = Wwise::GetInstance();
	m_pWwise->Init();

	AppRenderer::CreateInstance();

	m_pAppRenderer = AppRenderer::GetInstance();
	m_pAppRenderer->Init(hWnd);

	ImGui_ImplDX11_Init(hWnd, m_pAppRenderer->GetDevice(), m_pAppRenderer->GetDeviceContex());

	ID3D11Device* pDevice = m_pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pAppRenderer->GetDeviceContex();

	m_pEffectManager = new EffectManager();
	m_pEffectManager->Init();

	m_pNextScene = new NextScene();
	
	m_pShaderManager = new ShaderManager();
	m_pShaderManager->GenerateShader();

	m_pTextureManager = new TextureManager();

	m_pModelManager = new ModelManager();

	m_pDeferredRenderer = new DeferredRenderer();
	m_pDeferredRenderer->Init(hWnd, m_pShaderManager);
	m_pAppRenderer->SetDeferredRenderer(m_pDeferredRenderer);
	m_pAppRenderer->SetEffectManager(m_pEffectManager);

	Fade::CreateInstance();
	m_pFade = Fade::GetInstance();
	m_pFade->Init(m_pNextScene, m_pShaderManager, m_pTextureManager);

	m_pScene = new Game();
	m_pScene->Init(m_pNextScene, m_pShaderManager, m_pModelManager, m_pTextureManager, m_pEffectManager);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void App::Release(void)
{
	ImGui_ImplDX11_Shutdown();

	m_pScene->Release();

	if (m_pWwise != NULL)
	{
		m_pWwise->Uninit();
		delete m_pWwise;
		m_pWwise = NULL;
	}

	if (m_pFade != NULL)
	{
		m_pFade->Release();
		delete m_pFade;
		m_pFade = NULL;
	}

	if (m_pDeferredRenderer != NULL)
	{
		m_pDeferredRenderer->Release();
		delete m_pDeferredRenderer;
		m_pDeferredRenderer = NULL;
	}

	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Release();
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}

	if (m_pShaderManager != NULL)
	{
		m_pShaderManager->ReleaseAll();
		delete m_pShaderManager;
		m_pShaderManager = NULL;
	}

	if (m_pModelManager != NULL)
	{
		m_pModelManager->ReleaseAll();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->ReleaseAll();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	if (m_pInputKeybord != NULL)
	{
		m_pInputKeybord->Uninit();
		delete m_pInputKeybord;
		m_pInputKeybord = NULL;
	}

	if (m_pXboxController != NULL)
	{
		m_pXboxController->Uninit();
		delete m_pXboxController;
		m_pXboxController = NULL;
	}

	if (m_pAppRenderer == NULL)
	{
		m_pAppRenderer->Release();
		delete m_pAppRenderer;
		m_pAppRenderer = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void App::Update(void)
{
	ImGui_ImplDX11_NewFrame();

	m_pWwise->Update();

	m_pEffectManager->Update();

	m_pInputKeybord->Update();

	m_pXboxController->Update();

	m_pScene->Update();

	m_pFade->Update();
}

///////////////////////////////////////////////////////////////////////////////
//描画
///////////////////////////////////////////////////////////////////////////////
void App::Draw(int nCountFPS)
{
	ImGui::Begin("debug");
;	{
		static float f = 0.0f;
		ImGui::Text("fps:%d", nCountFPS);
	}
	ImGui::End();

	m_pScene->Draw();
}

///////////////////////////////////////////////////////////////////////////////
//シーン入れ替え
///////////////////////////////////////////////////////////////////////////////
void App::SceneChange(void)
{
	Scene* pNextScene = m_pNextScene->GetNextScene();

	if (pNextScene == NULL) { return; }

	if (m_pScene != pNextScene)
	{
		m_pScene->Release();
		delete m_pScene;
		m_pScene = NULL;
		m_pScene = pNextScene;
		m_pScene->Init(m_pNextScene, m_pShaderManager, m_pModelManager, m_pTextureManager, m_pEffectManager);
	}
}
