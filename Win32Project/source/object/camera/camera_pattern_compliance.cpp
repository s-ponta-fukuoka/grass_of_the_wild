//=============================================================================
//
// player.cpp
// Author : SHOTA FUKUOKA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "camera_pattern_compliance.h"
#include "main_camera.h"
#include "../../device/input.h"
#include "../character/player/player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
CameraPatternCompliance::CameraPatternCompliance()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//�X�V
///////////////////////////////////////////////////////////////////////////////
void CameraPatternCompliance::Update(MainCamera* pMainCamera)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	VECTOR3 move = pMainCamera->GetMove();
	XMVECTOR vector = pMainCamera->GetVec();

	VECTOR3 posAt = pMainCamera->GetPosAt();

	Player*	pPlayer = pMainCamera->GetPlayer();

	if (pInputKeyboard->GetKeyPress(DIK_UP))
	{
		move.x -= XMVectorGetX(vector) * 10;
		move.z -= XMVectorGetZ(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_LEFT))
	{
		move.x += XMVectorGetZ(vector) * 10;
		move.z -= XMVectorGetX(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_DOWN))
	{
		move.x += XMVectorGetX(vector) * 10;
		move.z += XMVectorGetZ(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_RIGHT))
	{
		move.x -= XMVectorGetZ(vector) * 10;
		move.z += XMVectorGetX(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_C))// S�L�[�����ꂽ��
	{
		move.x += XMVectorGetZ(vector) * 10;
		move.z -= XMVectorGetX(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Z))// S�L�[�����ꂽ��
	{
		move.x -= XMVectorGetZ(vector) * 10;
		move.z += XMVectorGetX(vector) * 10;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))// S�L�[�����ꂽ��
	{
		move.y += 5;

		pMainCamera->GetTransform()->rot.y += 0.5f;
	}
	if (pInputKeyboard->GetKeyPress(DIK_N))// S�L�[�����ꂽ��
	{
		move.y -= 5;

		pMainCamera->GetTransform()->rot.y -= 0.5f;
	}
	
	posAt.x = pPlayer->GetTransform()->position.x;
	posAt.z = pPlayer->GetTransform()->position.z;
	posAt.y = 100 + pPlayer->GetTransform()->position.y;

	pMainCamera->GetTransform()->position.x = move.x + (350 * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + pPlayer->GetTransform()->position.x;
	pMainCamera->GetTransform()->position.z = move.z + (500 * cosf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + pPlayer->GetTransform()->position.z;
	pMainCamera->GetTransform()->position.y = move.y + (350 * sinf(D3D_PI + pMainCamera->GetTransform()->rot.y)) + 100 + pPlayer->GetTransform()->position.y;

	pMainCamera->SetPositionAt(posAt);
	pMainCamera->SetMove(move);
}
