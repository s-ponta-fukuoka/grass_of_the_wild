//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "texture_manager.h"
#include "texture.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

void TextureManager::AddTexture(Texture* tex)
{
	m_list.push_back(tex);
}

void TextureManager::DeleteTexture(Texture* tex)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if (*ite == tex)
		{
			m_list.erase(ite);
			break;
		}
	}
}

Texture *TextureManager::GetTexture(Texture* tex)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if (!strcmp((*ite)->GetName(), tex->GetName()))
		{
			return (*ite);
		}
	}
	return NULL;
}