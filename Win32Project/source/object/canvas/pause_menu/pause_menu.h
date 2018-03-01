//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

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

//*****************************************************************************
// クラス定義
//*****************************************************************************
class PauseMenu : public Canvas
{
public:
	typedef enum
	{
		PAUSE_MENU_BACK_GAME,
		PAUSE_MENU_RETRY,
		PAUSE_MENU_TITLE,
		PAUSE_MENU_MAX
	}PAUSE_MENU;

	//コンストラクタ
	PauseMenu(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//デストラクタ
	virtual ~PauseMenu( );

	//終了
	void Release( void );

	//更新
	void Update( void );

	//バッファ
	void MakeVertex(ID3D11Device* pDevice);

	VECTOR3 *GetPosition(PAUSE_MENU ePauseMenu);

	void SetPauseMenu(PAUSE_MENU ePauseMenu) { m_ePauseMenu = ePauseMenu; }
	PAUSE_MENU GetPauseMenu(void) { return m_ePauseMenu; }

private:
	VECTOR3 *						m_pos[PAUSE_MENU_MAX];

	VECTOR3							m_size;

	PAUSE_MENU						m_ePauseMenu;

	RenderManager*				m_pRenderManager;

	Renderer*						m_pRenderer;
};

#endif