//=============================================================================
//
//  game.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

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
class InputKeyboard;
class ModelManager;
class Player;
class EnemyManager;
class TreeManager;
class CollisionManager;
class NextScene;
class PlayerLife;
class EffectManager;
class Pause;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Game : public Scene
{
public:
	Game();
	virtual ~Game();
	HRESULT Init(NextScene* pNextScene,
		ShaderManager* pShaderManager,
		ModelManager* pModelManager,
		TextureManager* pTextureManager,
		EffectManager* pEffectManager);
	void Release(void);
	void Update(void);
	void Draw(void);

	bool isPause(void);

private:
	bool							m_bPause;

	MainCamera*						m_pCamera;

	LightCamera*					m_pLightCamera;

	TextureManager*					m_pTextureManager;

	ShaderManager*					m_pShaderManager;

	RenderManager*					m_pRenderManager;

	MeshManager*					m_pMeshManager;

	CanvasManager*					m_pCanvasManager;

	InputKeyboard*					m_pInputKeybord;

	ModelManager*					m_pModelManager;

	Player*							m_pPlayer;

	EnemyManager*					m_pEnemyManager;

	TreeManager*					m_pTreeManager;

	CollisionManager*				m_pCollisionManager;

	PlayerLife*						m_pPlayerLife;

	Pause*							m_pPause;
};

#endif
