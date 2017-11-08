//=============================================================================
//
// camera.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../../renderer/app_renderer.h"
#include "../object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Camera : public Object
{
public:
	//コンストラクタ
	Camera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp);

	//デストラクタ
	virtual ~Camera();

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
	AppRenderer::Constant*		m_pConstant;

	XMMATRIX							m_mtxView;

	XMMATRIX							m_mtxProjection;

	VECTOR3								m_positionAt;

	VECTOR3								m_vectorUp;

	XMVECTOR							m_vector;
};

#endif