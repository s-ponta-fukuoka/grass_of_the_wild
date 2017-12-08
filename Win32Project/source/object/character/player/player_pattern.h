//=============================================================================
//
// player.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _PLAYER_PATTERN_H_
#define _PLAYER_PATTERN_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************
class Player;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerPattern
{
public:
	virtual ~PlayerPattern() {}
	virtual void Update(Player* pPlayer) = 0;
	virtual void InputOperation(Player* pPlayer) = 0;
};

class PlayerPatternNone : public PlayerPattern
{
public:
	virtual ~PlayerPatternNone() {}
	virtual void Update(Player* pPlayer) override {}
	virtual void InputOperation(Player* pPlayer) override {}
};

#endif