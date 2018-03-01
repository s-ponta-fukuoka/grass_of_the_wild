//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "tree_manager.h"
#include "tree.h"
#include "../../mesh/meshfiled/mesh_field.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TREE_SIZE (3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

TreeManager::TreeManager()
	: m_distance(0)
{
}

HRESULT TreeManager::GenerateTree(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	ModelManager*	pModelManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant,
	MainCamera *pCamera,
	CollisionManager* pCollisionManager,
	MeshField* pMeshField)
{
	TreeAdd(new Tree(VECTOR3(100,50,700),VECTOR3(0,0,0), VECTOR3(TREE_SIZE, TREE_SIZE, TREE_SIZE),
		pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this,pMeshField));
	
	//TreeAdd(new Tree(VECTOR3(0, 20, -900), VECTOR3(90, 90, 0), VECTOR3(TREE_SIZE, TREE_SIZE, TREE_SIZE),
	//	pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));
	//
	//TreeAdd(new Tree(VECTOR3(-1000, 20, 0), VECTOR3(90, -45, 0), VECTOR3(TREE_SIZE, TREE_SIZE, TREE_SIZE),
	//	pRenderManager, pShaderManager, pTextureManager, pModelManager, pConstant, pLightCameraConstant, pCamera, pCollisionManager, this));

	return S_OK;
}

void TreeManager::TreeAdd(Tree *tree)
{
	m_list.push_back(tree);
}

void TreeManager::TreeDelete(Tree *tree)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) != tree) { continue; }
		(*ite)->Release();
		m_list.erase(ite);
		return;
	}
}

void TreeManager::Update(RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager,
	AppRenderer::Constant* pConstant,
	AppRenderer::Constant* pLightCameraConstant, CollisionManager* pCollisionManager)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Update(pRenderManager,
			pShaderManager,
			pTextureManager,
			pConstant,
			pLightCameraConstant, 
			pCollisionManager);
	}
}

void TreeManager::Release(void)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		(*ite)->Release();
	}
}