//=============================================================================
//
// app.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _APP_H_
#define _APP_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include <assert.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// ウインドウの幅
#define SCREEN_HEIGHT	(720)		// ウインドウの高さ

//安全に解放する
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

#define D3D_PI ((FLOAT) 3.141592654f) 
#define D3DToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))
#define D3DToDegree( radian ) ((radian) * (180.0f / D3DX_PI))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class AppRenderer;
class Camera;
class ShaderManager;
class TextureManager;
class RenderManager;
class MeshManager;
class InputKeyboard;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class App
{
public:
	//コンストラクタ
	App();

	//デストラクタ
	virtual ~App();

	//初期化
	HRESULT Init(const HWND hWnd, HINSTANCE hInstance);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//描画
	void Draw(void);

private:
	AppRenderer*					m_pAppRenderer;

	Camera*							m_pCamera;

	TextureManager*					m_pTextureManager;

	ShaderManager*					m_pShaderManager;

	RenderManager*					m_pRenderManager;

	MeshManager*					m_pMeshManager;

	InputKeyboard*					m_pInputKeybord;
};

#endif