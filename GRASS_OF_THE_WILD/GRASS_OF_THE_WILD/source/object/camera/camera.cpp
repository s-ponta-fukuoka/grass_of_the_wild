//=============================================================================
//
// camera.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera.h"
#include "../../../source/app/app.h"
#include "../../device/input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Camera::Camera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp)
	: m_positionAt(positionAt)
	, m_vectorUp(vectorUp)
	, m_vector(XMVectorSet(0,0,0,0))
	, m_pConstant(NULL)
{
	m_pTransform->position = position;
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// 初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT Camera::Init(void )
{
	m_pConstant = new AppRenderer::Constant();

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void Camera::Release(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void Camera::Update(void)
{
	m_vector = XMVector3Normalize(XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 1) - XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 1));

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
// カメラセット
///////////////////////////////////////////////////////////////////////////////
void Camera::SetCamera(void)
{
	XMMATRIX hView = XMMatrixIdentity();
	XMVECTOR hEye = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 1.0f);	//カメラの位置
	XMVECTOR hAt = XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 1.0f);		//焦点の位置
	XMVECTOR hUp = XMVectorSet(m_vectorUp.x, m_vectorUp.y, m_vectorUp.z, 1.0f);
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);

	XMVECTOR LightPos = XMVectorSet(0, 1.f, -1.0f, 1);
	m_pConstant->light = LightPos;

	XMMATRIX hProjection;	//透視射影変換行列
	hProjection = XMMatrixPerspectiveFovLH(D3D_PI / 3.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 30000.0f);

	m_pConstant->view = XMMatrixTranspose(hView);
	m_pConstant->projection = XMMatrixTranspose(hProjection);
}

///////////////////////////////////////////////////////////////////////////////
//定数取得
///////////////////////////////////////////////////////////////////////////////
AppRenderer::Constant* Camera::GetConstant(void)
{
	return m_pConstant;
}