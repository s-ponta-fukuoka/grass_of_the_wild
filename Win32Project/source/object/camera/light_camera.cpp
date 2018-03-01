//=============================================================================
//
// light_camera.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "light_camera.h"
#include "../../../source/app/app.h"
#include "../../device/input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIGHT_CAMERA_NER (10000)
#define LIGHT_CAMERA_FOR (20000)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
LightCamera::LightCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp)
{
	m_pTransform->position = position;

	m_positionAt = positionAt;

	m_vectorUp = vectorUp;

	m_ner = LIGHT_CAMERA_NER;

	m_for = LIGHT_CAMERA_FOR;
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
LightCamera::~LightCamera()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// 初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT LightCamera::Init(void )
{
	Camera::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void LightCamera::Release(void)
{
	Camera::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void LightCamera::Update(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	Camera::Update();
}

///////////////////////////////////////////////////////////////////////////////
// カメラセット
///////////////////////////////////////////////////////////////////////////////
void LightCamera::SetCamera(void)
{
	XMMATRIX hView = XMMatrixIdentity();
	XMVECTOR hEye = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 0.0f);	//カメラの位置
	XMVECTOR hAt = XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 0.0f);		//焦点の位置
	XMVECTOR hUp = XMVectorSet(0, 1, 0, 0);
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);

	XMMATRIX hProjection;	//透視射影変換行列
	hProjection = XMMatrixOrthographicLH(7000* 1.414, 7000 * 1.414, m_ner, m_for);

	XMVECTOR LightPos = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 1);
	m_pConstant->light = LightPos;

	m_pConstant->view =  hView;
	m_pConstant->projection = hProjection;
}

///////////////////////////////////////////////////////////////////////////////
//定数取得
///////////////////////////////////////////////////////////////////////////////
AppRenderer::Constant* LightCamera::GetConstant(void)
{
	return m_pConstant;
}