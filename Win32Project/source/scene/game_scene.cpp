//=============================================================================
//
// game.cpp
// Author : shota fukuoka
//
//=============================================================================
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
#include "../model/model_manager.h"
#include "../object/character/player/player.h"
#include "../device/input.h"
#include "../device/xbox_controller.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../object/mesh/grass/grass.h"
#include "../object/character/enemy/enemy_manager.h"
#include "../object/terrain/tree/tree_manager.h"
#include "../collision/collision_manager.h"
#include "../scene/next_scene.h"
#include "../scene/scene.h"
#include "../scene/title_scene.h"
#include "../scene/tutorial_scene.h"
#include "../scene/game_scene.h"
#include "../scene/game_over_scene.h"
#include "../scene/game_clear_scene.h"
#include "../scene/fade_scene.h"
#include "../effect/effect_manager.h"
#include "../object/camera/camera_pattern_compliance.h"
#include "../wwise/Wwise.h"
#include "pause_scene.h"

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
Game::Game()
	: m_pCamera(NULL)
	, m_pLightCamera(NULL)
	, m_pShaderManager(NULL)
	, m_pTextureManager(NULL)
	, m_pRenderManager(NULL)
	, m_pMeshManager(NULL)
	, m_pInputKeybord(NULL)
	, m_pCanvasManager(NULL)
	, m_pEnemyManager(NULL)
	, m_pCollisionManager(NULL)
	, m_pPlayerLife(NULL)
	, m_pPlayer(NULL)
	, m_pTreeManager(NULL)
	, m_pModelManager(NULL)
	, m_bPause(NULL)
	, m_pPause(NULL)
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
Game::~Game()
{
	Release();
}

///////////////////////////////////////////////////////////////////////////////
// 初期化処理
///////////////////////////////////////////////////////////////////////////////
HRESULT Game::Init(NextScene* pNextScene,
	ShaderManager* pShaderManager,
	ModelManager* pModelManager,
	TextureManager* pTextureManager,
	EffectManager* pEffectManager)
{
	Wwise* pWwise = Wwise::GetInstance();
	pWwise->MainListenerGameObjEvent(EVENTS::GAME);

	m_pLightCamera = new LightCamera(VECTOR3(-3000, 8000, -12000), VECTOR3(0, 0, 0), VECTOR3(0, 1, 0));
	m_pLightCamera->Init();

	m_pTextureManager = pTextureManager;

	new Texture("resource/sprite/toon.png", m_pTextureManager);
	new Texture("resource/sprite/bayer.png", m_pTextureManager);

	m_pCollisionManager = new CollisionManager();

	m_pShaderManager = pShaderManager;

	m_pModelManager = pModelManager;

	m_pRenderManager = new RenderManager();

	m_pCanvasManager = new CanvasManager();
	m_pCanvasManager->AddCanvas(new ShadowMap(m_pRenderManager, m_pShaderManager, m_pTextureManager));

	m_pMeshManager = new MeshManager();

	m_pCamera = new MainCamera(VECTOR3(0.0f, 100.0f, -500.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 1.0f, 0.0f), pEffectManager, new CameraPatternCompliance());

	MeshField* pMeshField = new MeshField(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant());

	m_pMeshManager->AddMesh(pMeshField);
	m_pMeshManager->AddMesh(new SkyBox(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant()));

	m_pTreeManager = new TreeManager();
	m_pTreeManager->GenerateTree(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pModelManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCamera, m_pCollisionManager, pMeshField);

	m_pPlayerLife = new PlayerLife(m_pRenderManager, m_pShaderManager, m_pTextureManager);

	m_pPlayer = new Player(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pModelManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCamera, m_pCollisionManager, m_pPlayerLife, pMeshField, pEffectManager);

	m_pMeshManager->AddMesh(new Grass(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), pMeshField, m_pCamera, VECTOR3(-4000, 0, -4000), m_pPlayer->GetTransform()));
	
	m_pEnemyManager = new EnemyManager();
	m_pEnemyManager->GenerateEnemy(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pModelManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCamera, m_pCollisionManager, m_pPlayer->GetTransform(),pMeshField, pEffectManager);

	m_pCanvasManager->AddCanvas(m_pPlayerLife);

	m_pCamera->Init(m_pPlayer, m_pEnemyManager);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了処理
///////////////////////////////////////////////////////////////////////////////
void Game::Release(void)
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
	
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	if (m_pEnemyManager != NULL)
	{
		m_pEnemyManager->Release();
		delete m_pEnemyManager;
		m_pEnemyManager = NULL;
	}

	if (m_pTreeManager != NULL)
	{
		m_pTreeManager->Release();
		delete m_pTreeManager;
		m_pTreeManager = NULL;
	}

	if (m_pCollisionManager != NULL)
	{
		delete m_pCollisionManager;
		m_pCollisionManager = NULL;
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
void Game::Update(void)
{	
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	if (!isPause())
	{
		m_pLightCamera->Update();

		m_pCamera->Update();

		m_pMeshManager->UpdateAll();

		m_pTreeManager->Update(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCollisionManager);

		m_pEnemyManager->Update(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCollisionManager);

		m_pPlayer->Update(m_pRenderManager, m_pShaderManager, m_pTextureManager, m_pCamera->GetConstant(), m_pLightCamera->GetConstant(), m_pCollisionManager);

		m_pCollisionManager->Update();

		Fade* pFade = Fade::GetInstance();

		if (m_pPlayerLife->GetLifeZero())
		{
			pFade->SetFade(Fade::FADE_OUT, new GameOverScene);
		}

		if (m_pEnemyManager->GetNumEnemy() <= 0)
		{
			pFade->SetFade(Fade::FADE_OUT, new GameClearScene);
		}

	}
	m_pCanvasManager->UpdateAll();

	if (m_pPause != NULL)
	{
		m_pPause->Update();
	}
}

///////////////////////////////////////////////////////////////////////////////
// 描画処理
///////////////////////////////////////////////////////////////////////////////
void Game::Draw(void)
{
	m_pLightCamera->SetCamera();

	m_pCamera->SetCamera();

	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	pAppRenderer->Draw(m_pRenderManager);
}

///////////////////////////////////////////////////////////////////////////////
// ポーズ画面にするか
///////////////////////////////////////////////////////////////////////////////
bool Game::isPause(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XboxController* pXboxController = XboxController::GetInstance();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	if (pInputKeyboard->GetKeyTrigger(DIK_P) || pXboxController->GetButtonTrigger(0, XINPUT_GAMEPAD_START))
	{
		m_bPause = !m_bPause;

		if (m_bPause)
		{
			m_pPause = new Pause();
			m_pPause->Init(m_pShaderManager, m_pCanvasManager, m_pTextureManager, m_pRenderManager);
		}
		else
		{
			if (m_pPause != NULL)
			{
				m_pPause->Release();
				delete m_pPause;
				m_pPause = NULL;
			}
		}
	}

	if (m_pPause != NULL)
	{
		if (m_pPause->GetBackGame())
		{
			if (m_pPause != NULL)
			{
				m_pPause->Release();
				delete m_pPause;
				m_pPause = NULL;
			}
			m_bPause = false;
		}
	}

	return m_bPause;
}