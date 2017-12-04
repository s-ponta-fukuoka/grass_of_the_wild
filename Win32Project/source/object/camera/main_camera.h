//=============================================================================
//
// camera.h
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
// マクロ定義
//*****************************************************************************

class Player;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class MainCamera : public Camera
{
public:
	//コンストラクタ
	MainCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp);

	//デストラクタ
	virtual ~MainCamera();

	//初期化
	HRESULT Init(Player* pPlayer);

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
private:
	Player*		m_pPlayer;

	VECTOR3		m_move;
};

#endif