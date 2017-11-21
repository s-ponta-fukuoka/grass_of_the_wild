//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "texture_manager.h"
#include "texture.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
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

unsigned char *TextureManager::GetTexture(Texture* tex)
{
	for (auto ite = m_list.begin(); ite != m_list.end(); ++ite)
	{
		if (dynamic_cast<Texture *>(*ite)->GetName() == tex->GetName())
		{
			return dynamic_cast<Texture *>(*ite)->GetPixels();
		}
	}
	return NULL;
}