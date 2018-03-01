//=============================================================================
//
// fade_scene.h
// Author : shota fukuoka
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "scene.h"
#include "../renderer/app_renderer.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class NextScene;
class ShaderManager;
class RenderManager;
class TextureManager;
class Texture;
class VertexShader;
class PixelShader;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Fade
{
public:
	typedef enum
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	} FADE_TYPE;

	virtual ~Fade();

	//インスタンス生成
	static void CreateInstance(void);

	//インスタンス取得
	static Fade* GetInstance(void);

	HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	void Release(void);

	void Update(void);

	void Draw(void);

	void SetFade(FADE_TYPE fade, Scene* SceneNext);

	FADE_TYPE GetFade(void);

	float GetMax(float a, float b);

	float GetMin(float a, float b);

	void SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color);

private:
	Fade();

	void MakeVertex(void);

	void SetVertex(void);

	//定数バッファ設定
	void ConfigConstantBuffer(UINT ByteWidth);

	//サンプラーステート設定
	void ConfigSamplerState(void);

	//ブレンドステート設定
	void ConfigBlendState(BOOL bBlend);

	static Fade*		m_pFade;

	NextScene*			m_pNextScene;

	Texture*			m_pTexture;

	ID3D11Buffer*		m_pVertexBuffer;

	ID3D11BlendState*					m_pBlendState;

	ID3D11SamplerState*					m_pSampleLinear;

	ID3D11Buffer*						m_pConstantBuffer;

	VertexShader*						m_pVertexShader;

	PixelShader*						m_pPixelShader;

	VECTOR3				m_pos;		
	VECTOR3				m_size;		
	VECTOR3				m_normal;
	VECTOR2				m_tex;		
	VECTOR2				m_ofsettex;	
	VECTOR4				m_color;	
	bool m_bLoadTexture;

	FADE_TYPE m_fade;
	Scene *m_modeNext;
};


#endif