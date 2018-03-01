//=============================================================================
//
// game_clear.h
// Author : shota fukuoka
//
//=============================================================================
#ifndef _GAME_CLEAR_H_
#define _GAME_CLEAR_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class GameClear : public Canvas
{
public:
	//コンストラクタ
	GameClear(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//デストラクタ
	virtual ~GameClear( );

	//終了
	void Release( void );

	//更新
	void Update( void );

	//バッファ
	void MakeVertex(ID3D11Device* pDevice);

private:
	VECTOR3							m_size;

	bool							m_bUse;
};

#endif