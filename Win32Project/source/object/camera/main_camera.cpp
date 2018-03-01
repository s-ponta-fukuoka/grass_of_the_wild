//=============================================================================
//
// main_camera.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main_camera.h"
#include "../../../source/app/app.h"
#include "../../device/input.h"
#include "../../device/xbox_controller.h"
#include "../character/player/player.h"
#include "camera_pattern.h"
#include "camera_pattern_compliance.h"
#include "camera_pattern_lock_on.h"
#include "../character/enemy/enemy.h"
#include "../character/enemy/enemy_manager.h"
#include "../../gui/imgui.h"
#include "../../gui/imgui_impl_dx11.h"
#include "../../effect/effect_manager.h"

///////////////////////////////////////////////////////////////////////////////
// コンストラクタ
///////////////////////////////////////////////////////////////////////////////
MainCamera::MainCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp, EffectManager* pEffectManager, CameraPattern* pCameraPattern)
	: m_pPlayer(NULL)
	, m_move(VECTOR3(0,0,0))
	, m_pCameraPattern(pCameraPattern)
	, m_pEnemy(NULL)
	, m_pEnemyManager(NULL)
	, m_pEffectManager(NULL)
{
	m_pTransform->position = position;

	m_pConstant->color = VECTOR4(1, 1, 1, 1);

	m_positionAt = positionAt;

	m_pEffectManager = pEffectManager;

	m_vectorUp = vectorUp;
}

///////////////////////////////////////////////////////////////////////////////
// デストラクタ
///////////////////////////////////////////////////////////////////////////////
MainCamera::~MainCamera()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
// 初期化
///////////////////////////////////////////////////////////////////////////////
HRESULT MainCamera::Init(Player *pPlayer, EnemyManager* pEnemyManager)
{
	m_pPlayer = pPlayer;
	
	m_pEnemyManager = pEnemyManager;

	Camera::Init();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 終了
///////////////////////////////////////////////////////////////////////////////
void MainCamera::Release(void)
{
	delete m_pCameraPattern;

	Camera::Release();
}

///////////////////////////////////////////////////////////////////////////////
// 更新
///////////////////////////////////////////////////////////////////////////////
void MainCamera::Update(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	XINPUT_STATE Xinput = { NULL };
	XInputGetState(0, &Xinput);

	Camera::Update();

	m_pCameraPattern->Update(this);

	ImGui::Begin("main_camera");
	{
		if (ImGui::TreeNode("Transform")) {

			ImGui::Text("Position : X %.2f  Y %.2f  Z %.2f", m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z);
			ImGui::Text("Rotation : X %.2f  Y %.2f  Z %.2f", m_pTransform->rot.x, m_pTransform->rot.y, m_pTransform->rot.z);
			ImGui::Text("Scale    : X %.2f  Y %.2f  Z %.2f", m_pTransform->scale.x, m_pTransform->scale.y, m_pTransform->scale.z);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////
// カメラセット
///////////////////////////////////////////////////////////////////////////////
void MainCamera::SetCamera(void)
{
	XMMATRIX hView = XMMatrixIdentity();
	XMVECTOR hEye = XMVectorSet(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z, 0.0f);	//カメラの位置
	XMVECTOR hAt = XMVectorSet(m_positionAt.x, m_positionAt.y, m_positionAt.z, 0.0f);		//焦点の位置
	XMVECTOR hUp = XMVectorSet(m_vectorUp.x, m_vectorUp.y, m_vectorUp.z, 0.0f);
	hView = XMMatrixLookAtLH(hEye, hAt, hUp);

	XMMATRIX hProjection;	//透視射影変換行列
	hProjection = XMMatrixPerspectiveFovLH(D3D_PI / 3.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 30000.0f);

	XMVECTOR LightPos = XMVectorSet(0, 1.f, -1.0f, 1);
	m_pConstant->light = LightPos;

	m_pConstant->view = hView;
	m_pConstant->projection = hProjection;

	m_pEffectManager->GetERendererer()->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(D3D_PI / 3.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 30000.0f));

	// カメラ行列を設定
	m_pEffectManager->GetERendererer()->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(::Effekseer::Vector3D(m_pTransform->position.x, m_pTransform->position.y, m_pTransform->position.z),
		::Effekseer::Vector3D(m_positionAt.x, m_positionAt.y, m_positionAt.z),
		::Effekseer::Vector3D(m_vectorUp.x, m_vectorUp.y, m_vectorUp.z)));
}

///////////////////////////////////////////////////////////////////////////////
//定数取得
///////////////////////////////////////////////////////////////////////////////
AppRenderer::Constant* MainCamera::GetConstant(void)
{
	return m_pConstant;
}

///////////////////////////////////////////////////////////////////////////////
//ポジション取得
///////////////////////////////////////////////////////////////////////////////
const VECTOR3& MainCamera::GetPos(void) const
{
	return m_pTransform->position;
}

///////////////////////////////////////////////////////////////////////////////
//注視点取得
///////////////////////////////////////////////////////////////////////////////
const VECTOR3& MainCamera::GetPosAt(void) const
{
	return m_positionAt;
}

///////////////////////////////////////////////////////////////////////////////
//カメラベクトル取得
///////////////////////////////////////////////////////////////////////////////
const XMVECTOR& MainCamera::GetVec(void) const
{
	return m_vector;
}

///////////////////////////////////////////////////////////////////////////////
//カメラステート変更
///////////////////////////////////////////////////////////////////////////////
void MainCamera::ChangeCameraPattern(CameraPattern* pCameraPattern)
{
	if (pCameraPattern == NULL) { return; }
	delete m_pCameraPattern;
	m_pCameraPattern = pCameraPattern;
}