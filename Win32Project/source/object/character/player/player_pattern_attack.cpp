//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "player_pattern_attack.h"
#include "player_pattern_wait.h"
#include "player.h"
#include "../../../model/model.h"
#include "../../../device/input.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////

PlayerPatternAttack::PlayerPatternAttack()
{
}


PlayerPatternAttack::~PlayerPatternAttack()
{
}

void PlayerPatternAttack::Update(Player* pPlayer)
{
	InputOperation(pPlayer);

	ChangeAttackAnime(pPlayer);
}

void PlayerPatternAttack::InputOperation(Player* pPlayer)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber = pPlayer->GetAnimeNumber();

	if (pInputKeyboard->GetKeyPress(DIK_SPACE) && pAnimeNumber[0] == 2)
	{
		if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 10)
		{
			pAnimeNumber[0] = 3;

			pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
		}
	}
}

void PlayerPatternAttack::ChangeAttackAnime(Player* pPlayer)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	SkinMeshModel::Anime* pAnime = pPlayer->GetSkinMeshModel()->GetAnime();

	int* pFrame = pPlayer->GetFrame();
	int* pAnimeNumber= pPlayer->GetAnimeNumber();

	if (pFrame[0] >= pAnime[pAnimeNumber[0]].nEndTime - 1 )
	{
		if (pAnimeNumber[0] == 2 || pAnimeNumber[0] == 3)
		{
			pAnimeNumber[0] = 0;
			pPlayer->ChangePlayerPattern(new PlayerPatternWait);
		}
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}

	if (pFrame[0] < pAnime[pAnimeNumber[0]].nStartTime)
	{
		pFrame[0] = pAnime[pAnimeNumber[0]].nStartTime;
	}

	pFrame[0]++;
}