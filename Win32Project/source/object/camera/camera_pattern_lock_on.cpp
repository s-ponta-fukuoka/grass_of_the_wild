//=============================================================================
//
// camera_pattern_lock_on.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera_pattern_lock_on.h"
#include "camera_pattern_compliance.h"
#include "main_camera.h"
#include "../../device/input.h"
#include "../../device/xbox_controller.h"
#include "../character/player/player.h"
#include "../character/enemy/enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_LOCK_X (300)
#define CAMERA_LOCK_Z (350)
#define CAMERA_LOCK_HEIGT (150)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CameraPatternLockOn::CameraPatternLockOn()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void CameraPatternLockOn::Update(MainCamera* pMainCamera)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XMVECTOR pos;
	XMVECTOR startPos = XMVectorSet(pMainCamera->GetTransform()->position.x,
		pMainCamera->GetTransform()->position.y,
		pMainCamera->GetTransform()->position.z, 1.0f);

	VECTOR3 move = pMainCamera->GetMove();
	XMVECTOR vector = pMainCamera->GetVec();

	VECTOR3 posAt = pMainCamera->GetPosAt();

	Player*	pPlayer = pMainCamera->GetPlayer();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	if (pInputKeyboard->GetKeyPress(DIK_LSHIFT) ||
		Xinput.Gamepad.bLeftTrigger > XINPUT_TRIGGER)
	{
		if (pMainCamera->GetEnemy() != NULL) { 
			if (pMainCamera->GetEnemy()->GetTransform() != NULL) {
				posAt.x = pMainCamera->GetEnemy()->GetTransform()->position.x;
				posAt.y = pMainCamera->GetEnemy()->GetTransform()->position.y + CAMERA_LOCK_HEIGT;
				posAt.z = pMainCamera->GetEnemy()->GetTransform()->position.z;
			}
		}

		pMainCamera->GetTransform()->position = pPlayer->GetTransform()->position;
		pMainCamera->GetTransform()->position.z += XMVectorGetZ(vector) * CAMERA_LOCK_Z;
		pMainCamera->GetTransform()->position.x += XMVectorGetX(vector) * CAMERA_LOCK_X;
		pMainCamera->GetTransform()->position.y += CAMERA_LOCK_HEIGT;

		pos = XMVectorSet(pMainCamera->GetTransform()->position.x,
			pMainCamera->GetTransform()->position.y,
			pMainCamera->GetTransform()->position.z, 1.0f);
		startPos = XMVectorLerp(startPos, pos, 0.1f);
		pMainCamera->GetTransform()->position.x = XMVectorGetX(startPos);
		pMainCamera->GetTransform()->position.y = XMVectorGetY(startPos);
		pMainCamera->GetTransform()->position.z = XMVectorGetZ(startPos);

	}
	else
	{
		pMainCamera->ChangeCameraPattern(new CameraPatternCompliance);
	}

	pMainCamera->SetMove(move);
	pMainCamera->SetPositionAt(posAt);
}
