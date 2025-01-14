//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "texture.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class TextureManager
{
	std::list<Texture*>  m_list;
public:
	void AddTexture(Texture* tex);
	void DeleteTexture(Texture* tex);
	Texture *GetTexture(Texture* tex);
	void ReleaseAll(void);
};

#endif
