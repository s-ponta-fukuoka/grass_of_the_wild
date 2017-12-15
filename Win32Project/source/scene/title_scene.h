//=============================================================================
//
//  title.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "scene.h"

class MainCamera;
class LightCamera;
class TextureManager;
class RenderManager;
class ShaderManager;
class MeshManager;
class CanvasManager;
class ModelManager;
class Player;
class NextScene;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class Title : public Scene
{
public:
	Title();
	virtual ~Title() {};
	HRESULT Init(NextScene* pNextScene, Fade* pFade);
	void Release(void);
	void Update(void);
	void Draw(void);
private:
	Fade*							m_pFade;

	MainCamera*						m_pCamera;

	LightCamera*					m_pLightCamera;

	TextureManager*					m_pTextureManager;

	ShaderManager*					m_pShaderManager;

	RenderManager*					m_pRenderManager;

	MeshManager*					m_pMeshManager;

	CanvasManager*					m_pCanvasManager;

	ModelManager*					m_pModelManager;

	Player*							m_pPlayer;
};

#endif
