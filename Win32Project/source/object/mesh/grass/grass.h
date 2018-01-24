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

#define GRASS_MAX (100000)

//*****************************************************************************
// マクロ定義
//*****************************************************************************
class ShaderManager;
class RenderManager;
class TextureManager;
class MainCamera;
class MeshField;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Grass : public Mesh
{
public:
	//定数
	struct Constant
	{
		XMMATRIX view;
		XMMATRIX projection;
		//XMFLOAT3 posEye;
		float time;
		XMFLOAT3 posPlayer;
	};

	Grass( RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, 
		MeshField* pMeshField,
		MainCamera *pCamera, 
		VECTOR3 pos,
		Object::Transform* pPlayerTransform);			//コンストラクタ

	~Grass( );			//デストラクタ

	HRESULT Init( void );	//初期化処理

	void Release( void );	//終了処理

	void Update( void );	//更新処理

	void MakeVertex(void);

private:
	MainCamera* m_pCamera;
	VECTOR3 m_size;
	MeshField* m_pMeshField;
	ID3D11Buffer* m_pInstanceBuffer;
};

#endif