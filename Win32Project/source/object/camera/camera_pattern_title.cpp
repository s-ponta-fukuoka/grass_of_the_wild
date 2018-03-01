//=============================================================================
//
// camera_pattern_title.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera_pattern_title.h"
#include "main_camera.h"
#include "../../device/input.h"
#include "../../device/xbox_controller.h"
#include "../character/player/player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_TITLE_MOVE (10)
#define CAMERA_TITLE_X (350)
#define CAMERA_TITLE_Y (350)
#define CAMERA_TITLE_Z (500)
#define CAMERA_TITLE_WIDTH (1500)
#define CAMERA_TITLE_HEIGT (100)

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CameraPatternTitle::CameraPatternTitle()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void CameraPatternTitle::Update(MainCamera* pMainCamera)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	VECTOR3 move = pMainCamera->GetMove();
	XMVECTOR vector = pMainCamera->GetVec();

	VECTOR3 posAt = pMainCamera->GetPosAt();

	Player*	pPlayer = pMainCamera->GetPlayer();

	{
		move.x += XMVectorGetZ(vector) * CAMERA_TITLE_MOVE;
		move.z -= XMVectorGetX(vector) * CAMERA_TITLE_MOVE;
	}

	pMainCamera->GetTransform()->position.x = move.x + (CAMERA_TITLE_X * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y));
	pMainCamera->GetTransform()->position.z = move.z + (CAMERA_TITLE_Z * cosf(D3D_PI + pMainCamera->GetTransform()->rot.y)) - CAMERA_TITLE_WIDTH;
	pMainCamera->GetTransform()->position.y = move.y + (CAMERA_TITLE_Y * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + CAMERA_TITLE_HEIGT;

	pMainCamera->SetPositionAt(posAt);
	pMainCamera->SetMove(move);
}
