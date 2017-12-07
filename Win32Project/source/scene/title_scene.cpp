//=============================================================================
//
// title.cpp
// Author : shota fukuoka
//
//=============================================================================
#include "title_scene.h"
#include "../device/input.h"
#include "game_scene.h"

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
// 初期化処理
///////////////////////////////////////////////////////////////////////////////
HRESULT Title::Init(NextScene* pScene)
{
	m_pNextScene = pScene;

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了処理
///////////////////////////////////////////////////////////////////////////////
void Title::Release(void)
{

}

///////////////////////////////////////////////////////////////////////////////
// 更新処理
///////////////////////////////////////////////////////////////////////////////
void Title::Update(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	if (pInputKeyboard->GetKeyTrigger(DIK_RETURN))
	{
		m_pNextScene->NextSceneUpdate(new Game);
	}
}

///////////////////////////////////////////////////////////////////////////////
// 描画処理
///////////////////////////////////////////////////////////////////////////////
void Title::Draw(void)
{
}