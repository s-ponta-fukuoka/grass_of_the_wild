//=============================================================================
//
//  title.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

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
class EffectManager;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class GameOverScene : public Scene
{
public:
	GameOverScene();
	virtual ~GameOverScene() {};
	HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		ModelManager* pModelManager,
		TextureManager* pTextureManager,
		EffectManager* pEffectManager);
	void Release(void);
	void Update(void);
	void Draw(void);
private:
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
