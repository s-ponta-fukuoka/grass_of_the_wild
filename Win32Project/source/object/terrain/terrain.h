//=============================================================================
//
// terrain.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../object.h"
#include "../../renderer/renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Terrain : public Object
{
public:
	//コンストラクタ
	Terrain();

	//デストラクタ
	virtual ~Terrain();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

	//アニメーション変更
	void ChangeAnime(void);

	int* GetFrame(void) { return m_pFrame; }
	int* GetAnimeNumber(void) { return m_pAnimeNumber; }
	SkinMeshModel* GetSkinMeshModel(void) { return m_pModel; }

protected:

	ID3D11Buffer*			m_pIndexBuffer;

	ID3D11Buffer*			m_pVertexBuffer;

	SkinMeshModel*			m_pModel;

	int*					m_pFrame;

	int*					m_pAnimeNumber;
};

#endif