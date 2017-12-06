//=============================================================================
//
// grass.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _GRASS_H_
#define _GRASS_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "../mesh.h"

//*****************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class MainCamera;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Grass : public Mesh
{
public:
	//定数
	struct Constant
	{
		XMMATRIX world[100];
		XMMATRIX view;
		XMMATRIX projection;
	};

	Grass( RenderManager* pRenderManager,
				ShaderManager* pShaderManager,
				TextureManager* pTextureManager,
				AppRenderer::Constant* pConstant,
				AppRenderer::Constant* pLightCameraConstant, MainCamera *pCamera, int cnt);			//コンストラクタ

	~Grass( );			//デストラクタ

	HRESULT Init( void );	//初期化処理

	void Release( void );	//終了処理

	void Update( void );	//更新処理

	void MakeVertex(void);

private:
	MainCamera* m_pCamera;
	VECTOR3 m_size;
};

#endif