//=============================================================================
//
// camera_pattern_compliance.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main_camera.h"
#include "../../device/xbox_controller.h"
#include "../../device/input.h"
#include "../character/player/player.h"
#include "camera_pattern.h"
#include "camera_pattern_compliance.h"
#include "camera_pattern_lock_on.h"
#include "../character/enemy/enemy.h"
#include "../character/enemy/enemy_manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_COMP_MOVE (10)
#define CAMERA_COMP_MAX_UP (1000)
#define CAMERA_COMP_MAX_DOWN (0)
#define CAMERA_COMP_MOVE_Y (5)
#define CAMERA_COMP_X (350)
#define CAMERA_COMP_Y (350)
#define CAMERA_COMP_Z (500)
#define CAMERA_COMP_HEIGT (185)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CameraPatternCompliance::CameraPatternCompliance()
{
	m_bUse = false;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void CameraPatternCompliance::Update(MainCamera* pMainCamera)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	VECTOR3 move = pMainCamera->GetMove();
	XMVECTOR vector = pMainCamera->GetVec();

	VECTOR3 posAt;
	VECTOR3 posPlayer;

	Player*	pPlayer = pMainCamera->GetPlayer();

	XMVECTOR pos;
	XMVECTOR startPos = XMVectorSet(pMainCamera->GetTransform()->position.x,
		pMainCamera->GetTransform()->position.y,
		pMainCamera->GetTransform()->position.z, 1.0f);

	if (pInputKeyboard->GetKeyPress(DIK_UP))
	{
		move.x -= XMVectorGetX(vector) * CAMERA_COMP_MOVE;
		move.z -= XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_LEFT))
	{
		move.x += XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
		move.z -= XMVectorGetX(vector) * CAMERA_COMP_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_DOWN))
	{
		move.x += XMVectorGetX(vector) * CAMERA_COMP_MOVE;
		move.z += XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
	{
		move.x -= XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
		move.z += XMVectorGetX(vector) * CAMERA_COMP_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_C) || 
		Xinput.Gamepad.sThumbRX > XINPUT_VECTOR)
	{
		move.x += XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
		move.z -= XMVectorGetX(vector) * CAMERA_COMP_MOVE;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Z) ||
		Xinput.Gamepad.sThumbRX < -XINPUT_VECTOR)
	{
		move.x -= XMVectorGetZ(vector) * CAMERA_COMP_MOVE;
		move.z += XMVectorGetX(vector) * CAMERA_COMP_MOVE;
	}

	if (pMainCamera->GetTransform()->position.y > CAMERA_COMP_MAX_DOWN)
	{
		if (pInputKeyboard->GetKeyPress(DIK_N) ||
			Xinput.Gamepad.sThumbRY < -XINPUT_VECTOR)
		{
			move.y -= CAMERA_COMP_MOVE_Y;
		}
	}

	if (pMainCamera->GetTransform()->position.y < CAMERA_COMP_MAX_UP)
	{
		if (pInputKeyboard->GetKeyPress(DIK_Y) ||
			Xinput.Gamepad.sThumbRY > XINPUT_VECTOR)
		{
			move.y += CAMERA_COMP_MOVE_Y;
		}
	}
	
	posPlayer.x = pPlayer->GetTransform()->position.x;
	posPlayer.y = pPlayer->GetTransform()->position.y;
	posPlayer.z = pPlayer->GetTransform()->position.z;

	pMainCamera->GetTransform()->position.x = move.x + (CAMERA_COMP_X * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + pPlayer->GetTransform()->position.x;
	pMainCamera->GetTransform()->position.z = move.z + (CAMERA_COMP_Z * cosf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + pPlayer->GetTransform()->position.z;
	pMainCamera->GetTransform()->position.y = move.y + (CAMERA_COMP_Y * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + CAMERA_COMP_HEIGT + pPlayer->GetTransform()->position.y;

	pMainCamera->SetPositionAt(posPlayer);
	pMainCamera->SetMove(move);

	if (pInputKeyboard->GetKeyPress(DIK_LSHIFT) ||
		Xinput.Gamepad.bLeftTrigger > XINPUT_TRIGGER)
	{
		Enemy* pEnemy = pMainCamera->GetEnemy();
		pEnemy = pMainCamera->GetEnemyManager()->GetDistanceEnemy(pMainCamera->GetPlayer());
		pMainCamera->SetEnemy(pEnemy);
		pMainCamera->ChangeCameraPattern(new CameraPatternLockOn);
	}
}