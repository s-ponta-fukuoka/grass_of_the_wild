//=============================================================================
//
// app.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "app.h"
#include "../renderer/app_renderer.h"
#include "../object/object.h"
#include "../renderer/renderer.h"
#include "../object/camera/main_camera.h"
#include "../object/camera/light_camera.h"
#include "../shader/shader_manager.h"
#include "../renderer/render_manager.h"
#include "../object/mesh/mesh_manager.h"
#include "../object/mesh/meshfiled/mesh_field.h"
#include "../object/mesh/skybox/skybox.h"
#include "../object/canvas/canvas_manager.h"
#include "../object/canvas/shadow_map/shadow_map.h"
#include "../model/model_manager.h"
#include "../object/character/player/player.h"
#include "../device/input.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../object/mesh/grass/grass.h"
#include "../object/character/enemy/enemy_manager.h"

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
App::App()
	: m_pAppRenderer(NULL)
	, m_pCamera(NULL)
	, m_pLightCamera(NULL)
	, m_pShaderManager(NULL)
	, m_pTextureManager(NULL)
	, m_pRenderManager(NULL)
	, m_pMeshManager(NULL)
	, m_pInputKeybord(NULL)
	, m_pCanvasManager(NULL)
	, m_pEnemyManager(NULL)
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
App::~App()
{
	Release();
}

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

///////////////////////////////////////////////////////////////////////////////
//������
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

	m_pLightCamera = new LightCamera(VECTOR3(-3000, 8000, -12000), VECTOR3(0, 0, 0), VECTOR3(0, 1, 0));
	m_pLightCamera->Init();

	m_pTextureManager = new TextureManager();

	m_pShaderManager = new ShaderManager();
	m_pShaderManager->GenerateShader();

	m_pModelManager = new ModelManager();

	m_pRenderManager = new RenderManager();

	m_pCanvasManager = new CanvasManager();
	m_pCanvasManager->AddCanvas(new ShadowMap(m_pRenderManager, m_pShaderManager, m_pTextureManager));

	m_pMeshManager = new MeshManager();

	//for (int i = 0; i < 1000; i++)
	//{
	//	m_pMeshManager->AddMesh(new Grass(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(),i));
	//}

	m_pCamera = new MainCamera(VECTOR3(0.0f, 100.0f, -500.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f));

	m_pMeshManager->AddMesh(new MeshField(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant()));
	m_pMeshManager->AddMesh(new SkyBox(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant()));

	m_pPlayer = new Player(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pModelManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCamera);

	m_pEnemyManager = new EnemyManager();
	m_pEnemyManager->GenerateEnemy(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pModelManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCamera);

	m_pCamera->Init(m_pPlayer);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//�I��
///////////////////////////////////////////////////////////////////////////////
void App::Release(void)
{
	ImGui_ImplDX11_Shutdown();

	if (m_pInputKeybord == NULL) { return; }
	m_pInputKeybord->Uninit();
	delete m_pInputKeybord;
	m_pInputKeybord = NULL;

	if (m_pAppRenderer == NULL) { return; }
	m_pAppRenderer->Release();
	delete[] m_pAppRenderer;
	m_pAppRenderer = NULL;

	if (m_pCamera == NULL) { return; }
	m_pCamera->Release();
	delete[] m_pCamera;
	m_pCamera = NULL;

	if (m_pLightCamera == NULL) { return; }
	m_pLightCamera->Release();
	delete[] m_pCamera;
	m_pLightCamera = NULL;

	if (m_pShaderManager == NULL) { return; }
	m_pShaderManager->ReleaseAll();
	delete m_pShaderManager;
	m_pShaderManager = NULL;

	if (m_pModelManager == NULL) { return; }
	m_pModelManager->ReleaseAll();
	delete m_pModelManager;
	m_pModelManager = NULL;

	if (m_pMeshManager == NULL) { return; }
	m_pMeshManager->ReleaseAll();
	delete m_pMeshManager;
	m_pMeshManager = NULL;

	if (m_pCanvasManager == NULL) { return; }
	m_pCanvasManager->ReleaseAll();
	delete m_pCanvasManager;
	m_pCanvasManager = NULL;

	if (m_pRenderManager == NULL) { return; }
	m_pRenderManager->ReleaseAll();
	delete m_pRenderManager;
	m_pRenderManager = NULL;

	if (m_pPlayer == NULL) { return; }
	delete m_pPlayer;
	m_pPlayer = NULL;

	if (m_pEnemyManager == NULL) { return; }
	delete m_pEnemyManager;
	m_pEnemyManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//�X�V
///////////////////////////////////////////////////////////////////////////////
void App::Update(void)
{
	m_pInputKeybord->Update();

	m_pLightCamera->Update();

	m_pCamera->Update();

	m_pMeshManager->UpdateAll();

	m_pPlayer->Update();

	m_pEnemyManager->Update();

	m_pCanvasManager->UpdateAll();

}

///////////////////////////////////////////////////////////////////////////////
//�`��
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

	m_pLightCamera->SetCamera();

	m_pCamera->SetCamera();

	m_pAppRenderer->Draw(m_pRenderManager);

}
