//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_ATTACK_H_
#define _PLAYER_PATTERN_ATTACK_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "player_pattern.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerPatternAttack : public PlayerPattern
{
public:
	PlayerPatternAttack();
	virtual ~PlayerPatternAttack();
	virtual void Update(Player* pPlayer) override;
	virtual void InputOperation(Player* pPlayer);
	void ChangeAttackAnime(Player* pPlayer);
};

#endif