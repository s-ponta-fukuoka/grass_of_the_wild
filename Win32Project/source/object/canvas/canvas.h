//=============================================================================
//
// canvas.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CANVAS_H_
#define _CANVAS_H_

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
class Canvas : public Object
{
public:
	//コンストラクタ
	Canvas();

	//デストラクタ
	virtual ~Canvas();

	//初期化
	HRESULT Init(void);

	//終了
	void Release(void);

	//更新
	void Update(void);

protected:

	ID3D11Buffer*			m_pIndexBuffer;

	ID3D11Buffer*			m_pVertexBuffer;

	AppRenderer::Vertex2D	m_vertex;
};

#endif