//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <list>
#include "shader.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ShaderManager
{
	std::list<VertexShader *>		m_VSlist;
	std::list<PixelShader *>		m_PSlist;
public:

	ShaderManager::ShaderManager();

	HRESULT GenerateShader(void);

	void VSAdd(VertexShader* shader);

	void PSAdd(PixelShader *shader);

	VertexShader *GetVertexShader(VertexShader::VERTEX_TYPE etype);

	PixelShader *GetPixelShader(PixelShader::PIXEL_TYPE etype);

	void ReleaseVS(void);

	void ReleasePS(void);

	void ReleaseAll(void);
};

#endif
