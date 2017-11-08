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
#include "../object/object.h"
#include "../renderer/renderer.h"
#include "../object/camera/camera.h"
#include "../shader/shader_manager.h"
#include "../renderer/render_manager.h"
#include "../object/mesh/mesh_manager.h"
#include "../object/mesh/meshfiled/mesh_field.h"
#include "../object/mesh/skybox/skybox.h"
#include "../device/input.h"

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
App::App()
	: m_pAppRenderer(NULL)
	, m_pCamera(NULL)
	, m_pShaderManager(NULL)
	, m_pTextureManager(NULL)
	, m_pRenderManager(NULL)
	, m_pMeshManager(NULL)
	, m_pInputKeybord(NULL)
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

	AppRenderer::CreateInstance();
	m_pAppRenderer = AppRenderer::GetInstance();
	m_pAppRenderer->Init(hWnd);

	ID3D11Device* pDevice = m_pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = m_pAppRenderer->GetDeviceContex();

	m_pCamera = new Camera(VECTOR3(0,100,-500), VECTOR3(0,0,0), VECTOR3(0,1,0));
	m_pCamera->Init();

	m_pTextureManager = new TextureManager();

	m_pShaderManager = new ShaderManager();
	m_pShaderManager->GenerateShader();

	m_pRenderManager = new RenderManager();

	m_pMeshManager = new MeshManager();
	m_pMeshManager->AddMesh(new MeshField(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant()));
	m_pMeshManager->AddMesh(new SkyBox(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant()));

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void App::Release(void)
{
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

	if (m_pShaderManager == NULL) { return; }
	m_pShaderManager->ReleaseAll();
	delete m_pShaderManager;
	m_pShaderManager = NULL;

	if (m_pMeshManager == NULL) { return; }
	m_pMeshManager->ReleaseAll();
	delete m_pMeshManager;
	m_pMeshManager = NULL;

	if (m_pRenderManager == NULL) { return; }
	m_pRenderManager->ReleaseAll();
	delete m_pRenderManager;
	m_pRenderManager = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void App::Update(void)
{
	m_pInputKeybord->Update();

	m_pCamera->Update();

	m_pMeshManager->UpdateAll();

}

///////////////////////////////////////////////////////////////////////////////
//描画
///////////////////////////////////////////////////////////////////////////////
void App::Draw(void)
{
	m_pCamera->SetCamera();

	m_pAppRenderer->Draw(m_pRenderManager);
}