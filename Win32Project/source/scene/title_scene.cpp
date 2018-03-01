//=============================================================================
//
// title.cpp
// Author : shota fukuoka
//
//=============================================================================
#include "title_scene.h"
#include "game_scene.h"
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
#include "../object/canvas/player_life/player_life.h"
#include "../object/canvas/logo/logo.h"
#include "../object/canvas/press_start/press_start.h"
#include "../model/model_manager.h"
#include "../object/character/player/player.h"
#include "../device/input.h"
#include "../device/xbox_controller.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../object/mesh/grass/grass.h"
#include "../object/character/enemy/enemy_manager.h"
#include "../collision/collision_manager.h"
#include "../scene/next_scene.h"
#include "../scene/scene.h"
#include "../scene/title_scene.h"
#include "../scene/tutorial_scene.h"
#include "../scene/game_scene.h"
#include "../scene/game_over_scene.h"
#include "../scene/fade_scene.h"
#include "../effect/effect_manager.h"
#include "../object/camera/camera_pattern_title.h"
#include "../wwise/Wwise.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Title::Title()
	: m_pModelManager(NULL)
	, m_pPlayer(NULL)
	, m_pCamera(NULL)
	, m_pLightCamera(NULL)
	, m_pShaderManager(NULL)
	, m_pTextureManager(NULL)
	, m_pRenderManager(NULL)
	, m_pMeshManager(NULL)
	, m_pCanvasManager(NULL)
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// 初期化処理
///////////////////////////////////////////////////////////////////////////////
HRESULT Title::Init(NextScene* pNextScene,
	ShaderManager* pShaderManager,
	ModelManager* pModelManager,
	TextureManager* pTextureManager,
	EffectManager* pEffectManager)
{
	Wwise* pWwise = Wwise::GetInstance();
	pWwise->MainListenerGameObjEvent(EVENTS::TITLE);
	
	
	m_pLightCamera = new LightCamera(VECTOR3(-3000, 8000, -12000), VECTOR3(0, 0, 0), VECTOR3(0, 1, 0));
	m_pLightCamera->Init();
	
	m_pTextureManager = pTextureManager;
	
	new Texture("resource/sprite/toon.png", m_pTextureManager);
	
	m_pShaderManager = pShaderManager;
	
	m_pModelManager = pModelManager;
	
	m_pRenderManager = new RenderManager();
	
	m_pCanvasManager = new CanvasManager();
	m_pCanvasManager->AddCanvas(new ShadowMap(m_pRenderManager, m_pShaderManager, m_pTextureManager));
	
	m_pMeshManager = new MeshManager();
	
	m_pCamera = new MainCamera(VECTOR3(0.0f, 100.0f, -1500.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), pEffectManager, new CameraPatternTitle());
	
	MeshField* pMeshField = new MeshField(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant());
	
	m_pMeshManager->AddMesh(pMeshField);
	m_pMeshManager->AddMesh(new SkyBox(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant()));
	
	m_pMeshManager->AddMesh(new Grass(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), pMeshField, m_pCamera, VECTOR3(-4000, 0, -4000), NULL));
	
	m_pCanvasManager->AddCanvas(new Logo(m_pRenderManager, m_pShaderManager, m_pTextureManager));
	
	m_pCanvasManager->AddCanvas(new PressStart(m_pRenderManager, m_pShaderManager, m_pTextureManager));
	
	m_pCamera->Init(m_pPlayer, NULL);
	

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了処理
///////////////////////////////////////////////////////////////////////////////
void Title::Release(void)
{
	if (m_pCamera != NULL)
	{
		m_pCamera->Release();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	if (m_pLightCamera != NULL)
	{
		m_pLightCamera->Release();
		delete m_pLightCamera;
		m_pLightCamera = NULL;
	}

	if (m_pMeshManager != NULL)
	{
		m_pMeshManager->ReleaseAll();
		delete m_pMeshManager;
		m_pMeshManager = NULL;
	}

	if (m_pCanvasManager != NULL)
	{
		m_pCanvasManager->ReleaseAll();
		delete m_pCanvasManager;
		m_pCanvasManager = NULL;
	}

	if (m_pRenderManager != NULL)
	{
		m_pRenderManager->ReleaseAll();
		delete m_pRenderManager;
		m_pRenderManager = NULL;
	}

	Wwise* pWwise = Wwise::GetInstance();
	pWwise->StopMainListener();
}

///////////////////////////////////////////////////////////////////////////////
// 更新処理
///////////////////////////////////////////////////////////////////////////////
void Title::Update()
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();
	
	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);
	
	if (pInputKeyboard->GetKeyTrigger(DIK_RETURN) || Xinput.Gamepad.wButtons == XINPUT_GAMEPAD_START)
	{
		Fade* pFade = Fade::GetInstance();
		pFade->SetFade(Fade::FADE_OUT, new Game);
	}
	
	m_pLightCamera->Update();
	
	m_pCamera->Update();

	if (m_pMeshManager != NULL)
	{
		m_pMeshManager->UpdateAll();
	}

	if (m_pCanvasManager != NULL)
	{
		m_pCanvasManager->UpdateAll();
	}
}

///////////////////////////////////////////////////////////////////////////////
// 描画処理
///////////////////////////////////////////////////////////////////////////////
void Title::Draw(void)
{
	m_pLightCamera->SetCamera();
	
	m_pCamera->SetCamera();
	
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	pAppRenderer->Draw(m_pRenderManager);
}