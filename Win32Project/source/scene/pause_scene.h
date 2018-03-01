//=============================================================================
//
//  title.h
// Author : Shota Fukuoka
//
//=============================================================================
#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "scene.h"

class TextureManager;
class RenderManager;
class ShaderManager;
class CanvasManager;
class Cursor;
class PauseMenu;
class Operation;
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Pause
{
public:
	Pause();
	virtual ~Pause() {};
	HRESULT Init(ShaderManager* pShaderManager,
		CanvasManager* pCanvasManager,
		TextureManager* pTextureManager,
		RenderManager* pRenderManager);
	void Release(void);
	void Update(void);

	//�Q�[���ɖ߂�t���O
	void SetBackGame(bool bBackGame) {m_bBackGame = bBackGame; }
	bool GetBackGame(void) { return m_bBackGame; }

private:
	Cursor*			m_pCursor;
	PauseMenu*		m_pPauseMenu;
	RenderManager*	m_pRenderManager;
	CanvasManager*	m_pCanvasManager;
	Operation*		m_pOperation;
	bool			m_bBackGame;
};

#endif
