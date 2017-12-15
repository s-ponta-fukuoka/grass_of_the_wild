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
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../scene/next_scene.h"
#include "../scene/scene.h"
#include "../scene/title_scene.h"
#include "../scene/tutorial_scene.h"
#include "../scene/game_scene.h"
#include "../scene/result_scene.h"
#include "../scene/fade_scene.h"

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
App::App()
	: m_pAppRenderer(NULL)
	, m_pScene(NULL)
	, m_pNextScene(NULL)
	, m_pInputKeybord(NULL)
	, m_pFade(NULL)
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

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

///////////////////////////////////////////////////////////////////////////////
//初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT App::Init(const HWND hWnd, HINSTANCE hInstance)
{
	InputKeyboard::CreateInstance();
	m_pInputKeybord = InputKeyboard::GetInstance();
	m_pInputKeybord->Init(hInstance, hWnd);

	AppRenderer::CreateInstance();
	m_pAppRenderer = AppRenderer::GetInstance();
	m_pAppRenderer->Init(hWnd);

	ImGui_ImplDX11_Init(hWnd, m_pAppRenderer->GetDevice(), m_pAppRenderer->GetDeviceContex());

	ID3D11Device* pDevice = m_pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pAppRenderer->GetDeviceContex();

	m_pNextScene = new NextScene();
	
	m_pFade = new Fade();

	m_pScene = new Game();
	m_pScene->Init(m_pNextScene, m_pFade);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void App::Release(void)
{
	ImGui_ImplDX11_Shutdown();

	m_pScene->Release();

	if (m_pFade == NULL) { return; }
	m_pFade->Uninit();
	delete m_pFade;
	m_pFade = NULL;

	if (m_pInputKeybord == NULL) { return; }
	m_pInputKeybord->Uninit();
	delete m_pInputKeybord;
	m_pInputKeybord = NULL;

	if (m_pAppRenderer == NULL) { return; }
	m_pAppRenderer->Release();
	delete[] m_pAppRenderer;
	m_pAppRenderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void App::Update(void)
{
	m_pInputKeybord->Update();

	m_pScene->Update();

	m_pFade->Update();
}

///////////////////////////////////////////////////////////////////////////////
//描画
///////////////////////////////////////////////////////////////////////////////
void App::Draw(void)
{
	ImGui_ImplDX11_NewFrame();

	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug".
	{
		static float f = 0.0f;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("float", &f, 0.0f, 100.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
	}

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
		m_pScene = pNextScene;
		m_pScene->Init(m_pNextScene, m_pFade);
	}
}
