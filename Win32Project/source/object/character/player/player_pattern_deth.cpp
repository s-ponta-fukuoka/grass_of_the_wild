//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_deth.h"
#include "player_pattern_walk.h"
#include "player_pattern_attack.h"
#include "player_pattern_wait.h"
#include "player.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"
#include "player_attack.h"
#include "../../../effect/effect_manager.h"
#include "../../canvas/player_life/player_life.h"
#include "../../../wwise/Wwise.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternDeth::PlayerPatternDeth()
{
	Wwise* pWwise = Wwise::GetInstance();
	pWwise->StopMainListener();
}

PlayerPatternDeth::~PlayerPatternDeth()
{
}

void PlayerPatternDeth::Update(Player* pPlayer,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	ChangeDethAnime(pPlayer, pCollisionManager);
}

void PlayerPatternDeth::ChangeDethAnime(Player* pPlayer, CollisionManager* pCollisionManager)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1)
	{
		pFrame[0] = pAnime[pAnimeNumber[0]].nEndTime - 1;
		pPlayer->GetPlayerLife()->SetLifeZero(true);
	}
	else
	{
		pFrame[0]++;
	}
}