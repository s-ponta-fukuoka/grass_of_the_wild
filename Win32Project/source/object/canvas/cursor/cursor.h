//=============================================================================
//
// cursor.h
// Author : shota fukuoka
//
//=============================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class PauseMenu;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Cursor : public Canvas
{
public:
	//コンストラクタ
	Cursor(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		PauseMenu*	pPauseMenu);

	//デストラクタ
	virtual ~Cursor( );

	//終了
	void Release( void );

	//更新
	void Update( void );

	//バッファ
	void MakeVertex(ID3D11Device* pDevice);

	//ゲームに戻るフラグ
	bool GetBackGame(void) { return m_bBackGame; }

private:
	VECTOR3 *						m_pos;

	PauseMenu*						m_pPauseMenu;

	VECTOR3							m_size;

	VECTOR3							m_sizeMin;

	RenderManager*				m_pRenderManager;

	Renderer*						m_pRenderer;

	bool							m_bUse;

	bool							m_bBackGame;

	int								m_nWaitTime;
};

#endif