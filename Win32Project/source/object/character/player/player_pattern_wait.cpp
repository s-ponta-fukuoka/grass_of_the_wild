//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_pattern_wait.h"
#include "player_pattern_attack.h"
#include "player_pattern_walk.h"
#include "player.h"
#include "../../../device/input.h"
#include "../../camera/main_camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////

PlayerPatternWait::PlayerPatternWait()
{
}


PlayerPatternWait::~PlayerPatternWait()
{
}

void PlayerPatternWait::Update(Player* pPlayer)
{
	InputOperation(pPlayer);

	pPlayer->ChangeAnime();
}

void PlayerPatternWait::InputOperation(Player* pPlayer)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	int* pAnimeNumber = pPlayer->GetAnimeNumber();


	if (pInputKeyboard->GetKeyPress(DIK_W) ||
		pInputKeyboard->GetKeyPress(DIK_A) ||
		pInputKeyboard->GetKeyPress(DIK_S) ||
		pInputKeyboard->GetKeyPress(DIK_D))
	{
		pAnimeNumber[0] = 1;
		pPlayer->ChangePlayerPattern(new PlayerPatternWalk);
	}

	if (pInputKeyboard->GetKeyTrigger(DIK_SPACE))
	{
		pAnimeNumber[0] = 2;
		pPlayer->ChangePlayerPattern(new PlayerPatternAttack);
	}

}
