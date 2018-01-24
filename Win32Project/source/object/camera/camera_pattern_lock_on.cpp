//=============================================================================
//
// player.cpp
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
#include "../character/player/player.h"
#include "../character/enemy/enemy.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CameraPatternLockOn::CameraPatternLockOn()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////

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

	if (pInputKeyboard->GetKeyPress(DIK_LSHIFT))
	{
		posAt.x = pMainCamera->GetEnemy()->GetTransform()->position.x;
		posAt.z = pMainCamera->GetEnemy()->GetTransform()->position.z;

		pMainCamera->GetTransform()->position = pPlayer->GetTransform()->position;
		pMainCamera->GetTransform()->position.z += XMVectorGetZ(vector) * 350;
		pMainCamera->GetTransform()->position.x += XMVectorGetX(vector) * 300;
		pMainCamera->GetTransform()->position.y += 150;

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

	pMainCamera->SetPositionAt(posAt);
}
