//=============================================================================
//
// main_camera.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MAIN_CAMERA_H_
#define _MAIN_CAMERA_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CameraPattern;
class Player;
class Enemy;
class EnemyManager;
class EffectManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class MainCamera : public Camera
{
public:
	//コンストラクタ
	MainCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp, EffectManager* pEffectManager, CameraPattern* pCameraPattern);

	//デストラクタ
	virtual ~MainCamera();

	//初期化
	HRESULT Init(Player* pPlayer, EnemyManager* pEnemyManager);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//カメラセット
	void SetCamera(void);

	//定数取得
	AppRenderer::Constant* GetConstant(void);

	//ポジション取得
	const VECTOR3& GetPos(void) const;

	//注視点取得
	const VECTOR3& GetPosAt(void) const;

	//カメラベクトル取得
	const XMVECTOR& GetVec(void) const;

	//カメラステート変更
	void ChangeCameraPattern(CameraPattern* pCameraPattern);

	//移動量取得・設定
	void SetMove(VECTOR3 move) { m_move = move; }
	VECTOR3 GetMove(void) { return m_move; }

	//プレイヤー取得
	Player* GetPlayer(void) { return m_pPlayer; }

	//エネミー取得
	Enemy* GetEnemy(void) { return m_pEnemy; }
	void SetEnemy(Enemy* pEnemy) { m_pEnemy = pEnemy; }

	//エネミー取得
	EnemyManager* GetEnemyManager(void) { return m_pEnemyManager; }

private:
	Player*				m_pPlayer;

	Enemy*				m_pEnemy;

	EnemyManager*		m_pEnemyManager;

	CameraPattern*	m_pCameraPattern;

	EffectManager*	m_pEffectManager;

	VECTOR3		m_move;
};

#endif