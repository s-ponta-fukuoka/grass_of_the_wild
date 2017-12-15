#ifndef _FADE_H_
#define _FADE_H_

/*==============================================================================
【 インクルード 】
==============================================================================*/
#include "scene.h"
#include "../renderer/app_renderer.h"
/*==============================================================================
【 マクロ定義 】
==============================================================================*/
#define LOAD_Fade_TEXTURE	(5)								// ロードするテクスチャの数
#define Fade_BUF			(64)							// バッファ
#define Fade_TEXTURENAME0	"data/TEXTURE/result000.png"	// タイトル時の背景
#define Fade_TEXTURENAME1	"data/TEXTURE/result000.png"	// 結果時の背景
#define Fade_TEXTURENAME2	"data/TEXTURE/result000.png"	// ゲーム時の背景
#define Fade_TEXTURENAME3	"data/TEXTURE/result000.png"	// 結果時の背景
#define Fade_TEXTURENAME4	"data/TEXTURE/result000.png"	// タイトル時の背景
#define FADE_POS_X		(0)									// ポリゴンの表示位置Ｘ
#define FADE_POS_Y		(0)									// ポリゴンの表示位置Ｙ
#define FADE_WIDTH		(SCREEN_WIDTH)								// ポリゴンの幅
#define FADE_HEIGHT		(SCREEN_HEIGHT)								// ポリゴンの高さ
#define COLOR_FADE_MIN	(0.0f)								// αの最小値
#define COLOR_FADE_MAX	(1.0f)								// αの最大値
#define FADE_RATE		(0.05f)								// フェードレート
#define FADE_VERTEX		(4)

class NextScene;
class ShaderManager;
class RenderManager;
class TextureManager;

/*==============================================================================
【 構造体の定義 】
==============================================================================*/
class Fade
{
public:
	// フェードの状態
	typedef enum
	{
		FADE_NONE,		// フェードなし
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX
	} FADE_TYPE;

	Fade();
	virtual ~Fade();

	HRESULT Init(NextScene* pNextScene,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);
	void Uninit(void);
	void Update(void);
	void SetFade(FADE_TYPE fade, Scene* SceneNext);
	FADE_TYPE GetFade(void);

	float GetMax(float a, float b);
	float GetMin(float a, float b);

	void SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color);

private:
	NextScene* m_pNextScene;

	ID3D11Buffer*			m_pVertexBuffer;

	VECTOR3				m_pos;			// ポリゴン座標
	VECTOR3				m_size;			// サイズ
	VECTOR3				m_normal;
	VECTOR2				m_tex;			// テクスチャ座標
	VECTOR2				m_ofsettex;		// テクスチャオフセット
	VECTOR4				m_color;			// ポリゴン座標
	bool m_bLoadTexture;

	FADE_TYPE m_fade;
	Scene *m_modeNext;

	char  m_setFade[LOAD_Fade_TEXTURE][Fade_BUF];
};


#endif