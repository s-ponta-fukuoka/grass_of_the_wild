//=============================================================================
//
// camera.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "light_camera.h"
#include "../../../source/app/app.h"
#include "../../device/input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
LightCamera::LightCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp)
{
	m_pTransform->position = position;

	m_positionAt = positionAt;

	m_vectorUp = vectorUp;
}

///////////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
LightCamera::~LightCamera()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// ������
///////////////////////////////////////////////////////////////////////////////
HRESULT LightCamera::Init(void )
{
	Camera::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// �I��
///////////////////////////////////////////////////////////////////////////////
void LightCamera::Release(void)
{
	Camera::Release();
}

///////////////////////////////////////////////////////////////////////////////
// �X�V
///////////////////////////////////////////////////////////////////////////////
void LightCamera::Update(void)
{
	Camera::Update();

	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	if (pInputKeyboard->GetKeyPress(DIK_W))
	{
		m_pTransform->position.z++;
		m_positionAt.z++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_A))
	{
		m_pTransform->position.x--;
		m_positionAt.x--;
	}

	if (pInputKeyboard->GetKeyPress(DIK_S))
	{
		m_pTransform->position.z--;
		m_positionAt.z--;
	}

	if (pInputKeyboard->GetKeyPress(DIK_D))
	{
		m_pTransform->position.x++;
		m_positionAt.x++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_Y))
	{
		m_pTransform->position.y++;
		m_positionAt.y++;
	}

	if (pInputKeyboard->GetKeyPress(DIK_N))
	{
		m_pTransform->position.y--;
		m_positionAt.y--;
	}
}

///////////////////////////////////////////////////////////////////////////////
// �J�����Z�b�g
///////////////////////////////////////////////////////////////////////////////
void LightCamera::SetCamera(void)
{
	XMMATRIX hView = XMMatrixIdentity();
	XMVECTOR hEye = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 1.0f);	//�J�����̈ʒu
	XMVECTOR hAt = XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 1.0f);		//�œ_�̈ʒu
	XMVECTOR hUp = XMVectorSet(m_vectorUp.x, m_vectorUp.y, m_vectorUp.z, 1.0f);
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);

	XMMATRIX hProjection;	//�����ˉe�ϊ��s��
	hProjection = XMMatrixPerspectiveFovLH(D3D_PI / 3.0f, (float)1024 / 1024, 5000.0f, 30000.0f);

	m_pConstant->view = XMMatrixTranspose(hView);
	m_pConstant->projection = XMMatrixTranspose(hProjection);
}

///////////////////////////////////////////////////////////////////////////////
//�萔�擾
///////////////////////////////////////////////////////////////////////////////
AppRenderer::Constant* LightCamera::GetConstant(void)
{
	return m_pConstant;
}