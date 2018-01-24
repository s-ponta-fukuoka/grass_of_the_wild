//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TREE_MANAGER_H_
#define _TREE_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <list>
#include "tree.h"

class Player;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class TreeManager
{
	std::list<Tree *>		m_list;

	float					m_distance;
public:

	TreeManager::TreeManager();

	HRESULT GenerateTree(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		ModelManager*	pModelManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant,
		MainCamera *pCamera,
		CollisionManager* pCollisionManager);

	void TreeAdd(Tree* tree);

	void TreeDelete(Tree* tree);

	void Update(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager);

	void Release(void);
};

#endif
