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
// クラス定義
//*****************************************************************************
class Camera : public Object
{
public:
	//コンストラクタ
	Camera();

	//デストラクタ
	virtual ~Camera();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//注視点設定・取得
	void SetPositionAt(VECTOR3 posAt) { m_positionAt = posAt; }
	VECTOR3 GetPositionAt(void) { return m_positionAt; }

	//上方向設定・取得
	void SetVecUp(VECTOR3 vecUp) { m_vectorUp = vecUp; }
	VECTOR3 GetVecUp(void) { return m_vectorUp; }

	//方向設定・取得
	void SetVec(XMVECTOR vec) { m_vector = vec; }
	XMVECTOR GetVec(void) { return m_vector; }

protected:
	AppRenderer::Constant*				m_pConstant;

	XMMATRIX							m_mtxView;

	XMMATRIX							m_mtxProjection;

	VECTOR3								m_positionAt;

	VECTOR3								m_vectorUp;

	XMVECTOR							m_vector;
};

#endif