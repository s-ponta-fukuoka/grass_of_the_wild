//=============================================================================
//
// camera.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main_camera.h"
#include "../../../source/app/app.h"
#include "../../device/input.h"
#include "../character/player/player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
MainCamera::MainCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp)
	: m_pPlayer(NULL)
	, m_move(VECTOR3(0,0,0))
{
	m_pTransform->position = position;

	m_positionAt = positionAt;

	m_vectorUp = vectorUp;
}

///////////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
MainCamera::~MainCamera()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// ������
///////////////////////////////////////////////////////////////////////////////
HRESULT MainCamera::Init(Player *pPlayer)
{
	m_pPlayer = pPlayer;

	Camera::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// �I��
///////////////////////////////////////////////////////////////////////////////
void MainCamera::Release(void)
{
	Camera::Release();
}

///////////////////////////////////////////////////////////////////////////////
// �X�V
///////////////////////////////////////////////////////////////////////////////
void MainCamera::Update(void)
{
	Camera::Update();

	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	if (pInputKeyboard->GetKeyPress(DIK_C))// S�L�[�����ꂽ��
	{
		m_move.x += XMVectorGetZ(m_vector) * 10;
		m_move.z -= XMVectorGetX(m_vector) * 10;

		//m_pTransform->rot.y += 0.5f;
	}
	if (pInputKeyboard->GetKeyPress(DIK_Z))// S�L�[�����ꂽ��
	{
		m_move.x -= XMVectorGetZ(m_vector) * 10;
		m_move.z += XMVectorGetX(m_vector) * 10;

		//m_pTransform->rot.y -= 0.5f;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))// S�L�[�����ꂽ��
	{
		m_move.y += 5;

		m_pTransform->rot.y += 0.5f;
	}
	if (pInputKeyboard->GetKeyPress(DIK_N))// S�L�[�����ꂽ��
	{
		m_move.y -= 5;

		m_pTransform->rot.y -= 0.5f;
	}
	m_positionAt.x = m_pPlayer->GetTransform()->position.x;
	m_positionAt.z = m_pPlayer->GetTransform()->position.z;
	m_positionAt.y = 100;

	m_pTransform->position.x = m_move.x + (350 * sinf(D3D_PI + m_pTransform->rot.y)) + m_pPlayer->GetTransform()->position.x;
	m_pTransform->position.z = m_move.z + (500 * cosf(D3D_PI + m_pTransform->rot.y)) + m_pPlayer->GetTransform()->position.z;
	m_pTransform->position.y = m_move.y + (350 * sinf(D3D_PI + m_pTransform->rot.y)) + 100;
	
}

///////////////////////////////////////////////////////////////////////////////
// �J�����Z�b�g
///////////////////////////////////////////////////////////////////////////////
void MainCamera::SetCamera(void)
{
	XMMATRIX hView = XMMatrixIdentity();
	XMVECTOR hEye = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 0.0f);	//�J�����̈ʒu
	XMVECTOR hAt = XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 0.0f);		//�œ_�̈ʒu
	XMVECTOR hUp = XMVectorSet(m_vectorUp.x, m_vectorUp.y, m_vectorUp.z, 0.0f);
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);

	XMMATRIX hProjection;	//�����ˉe�ϊ��s��
	hProjection = XMMatrixPerspectiveFovLH(D3D_PI / 3.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 30000.0f);

	XMVECTOR LightPos = XMVectorSet(0, 1.f, -1.0f, 1);
	m_pConstant->light = LightPos;

	m_pConstant->view = hView;
	m_pConstant->projection = hProjection;
}

///////////////////////////////////////////////////////////////////////////////
//�萔�擾
///////////////////////////////////////////////////////////////////////////////
AppRenderer::Constant* MainCamera::GetConstant(void)
{
	return m_pConstant;
}

///////////////////////////////////////////////////////////////////////////////
//�|�W�V�����擾
///////////////////////////////////////////////////////////////////////////////
const VECTOR3& MainCamera::GetPos(void) const
{
	return m_pTransform->position;
}

///////////////////////////////////////////////////////////////////////////////
//�����_�擾
///////////////////////////////////////////////////////////////////////////////
const VECTOR3& MainCamera::GetPosAt(void) const
{
	return m_positionAt;
}

///////////////////////////////////////////////////////////////////////////////
//�J�����x�N�g���擾
///////////////////////////////////////////////////////////////////////////////
const XMVECTOR& MainCamera::GetVec(void) const
{
	return m_vector;
}