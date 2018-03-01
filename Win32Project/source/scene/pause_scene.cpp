//=============================================================================
//
// title.cpp
// Author : shota fukuoka
//
//=============================================================================
#include "pause_scene.h"
#include "../object/object.h"
#include "../renderer/renderer.h"
#include "../shader/shader_manager.h"
#include "../renderer/render_manager.h"
#include "../object/canvas/canvas_manager.h"
#include "../device/input.h"
#include "../gui/imgui.h"
#include "../gui/imgui_impl_dx11.h"
#include "../collision/collision_manager.h"
#include "../effect/effect_manager.h"
#include "../wwise/Wwise.h"
#include "../object/canvas/cursor/cursor.h"
#include "../object/canvas/pause_menu/pause_menu.h"
#include "../object/canvas/operation/operation.h"

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
Pause::Pause()
	:m_pCursor(NULL)
	,m_pPauseMenu(NULL)
	, m_pCanvasManager(NULL)
	, m_pRenderManager(NULL)
	, m_pOperation(NULL)
	, m_bBackGame(false)
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// 初期化処理
///////////////////////////////////////////////////////////////////////////////
HRESULT Pause::Init(ShaderManager* pShaderManager,
	CanvasManager* pCanvasManager,
	TextureManager* pTextureManager,
	RenderManager* pRenderManager)
{
	m_pRenderManager = pRenderManager;
	m_pCanvasManager = pCanvasManager;

	m_pPauseMenu = new PauseMenu(pRenderManager, pShaderManager, pTextureManager);
	pCanvasManager->AddCanvas(m_pPauseMenu);

	m_pCursor = new Cursor(pRenderManager, pShaderManager, pTextureManager, m_pPauseMenu);
	pCanvasManager->AddCanvas(m_pCursor);

	m_pOperation = new Operation(pRenderManager, pShaderManager, pTextureManager);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了処理
///////////////////////////////////////////////////////////////////////////////
void Pause::Release(void)
{
	if (m_pCursor != NULL)
	{
		m_pCanvasManager->DeleteCanvas(m_pCursor);
		m_pCursor->Release();
		delete m_pCursor;
		m_pCursor = NULL;
	}

	if (m_pPauseMenu != NULL)
	{
		m_pCanvasManager->DeleteCanvas(m_pPauseMenu);
		m_pPauseMenu->Release();
		delete m_pPauseMenu;
		m_pPauseMenu = NULL;
	}

	if (m_pOperation!= NULL)
	{
		m_pCanvasManager->DeleteCanvas(m_pOperation);
		m_pOperation->Release();
		delete m_pOperation;
		m_pOperation = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
// 更新処理
///////////////////////////////////////////////////////////////////////////////
void Pause::Update()
{
	m_bBackGame = m_pCursor->GetBackGame();
}