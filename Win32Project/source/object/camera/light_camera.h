//=============================================================================
//
// light_camera.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _LIGHT_CAMERA_H_
#define _LIGHT_CAMERA_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "camera.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class LightCamera : public Camera
{
public:
	//コンストラクタ
	LightCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp);

	//デストラクタ
	virtual ~LightCamera();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//カメラセット
	void SetCamera(void);

	//定数取得
	AppRenderer::Constant* GetConstant(void);
private:
	int m_ner;
	int m_for;
};

#endif