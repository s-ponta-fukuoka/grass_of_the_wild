//=============================================================================
//
// render_manager.h
// Author : shoto fukuoka
//
//=============================================================================
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <list>

class Renderer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class RenderManager
{
	std::list<Renderer*>		m_listRenderer;

	std::list<Renderer*>		m_listShadowRenderer;
public:

	//コンストラクタ
	RenderManager();

	//デストラクタ
	virtual ~RenderManager();

	//追加
	void AddRenderer(Renderer* renderer);

	//削除
	void DeleteRenderer(const Renderer* renderer);

	//全描画
	void DrawAll(void);

	//追加
	void AddShadowRenderer(Renderer* renderer);

	//削除
	void DeleteShadowRenderer(const Renderer* renderer);

	//シャドウマップ用描画
	void ShadowDrawAll(void);

	//全消去
	void ReleaseAll(void);
};

#endif
