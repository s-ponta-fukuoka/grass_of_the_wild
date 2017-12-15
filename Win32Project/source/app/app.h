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

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class AppRenderer;
class MainCamera;
class LightCamera;
class TextureManager;
class RenderManager;
class ShaderManager;
class MeshManager;
class CanvasManager;
class InputKeyboard;
class ModelManager;
class Player;
class EnemyManager;
class CollisionManager;
class Scene;
class NextScene;
class Fade;
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

	//シーン入れ替え
	void SceneChange(void);

private:
	Scene*							m_pScene;

	NextScene*						m_pNextScene;

	AppRenderer*					m_pAppRenderer;

	InputKeyboard*					m_pInputKeybord;

	Fade*							m_pFade;
};

#endif