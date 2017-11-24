//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <list>
#include "shader.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class ShaderManager
{
	std::list<VertexShader *>		m_VSlist;
	std::list<PixelShader *>		m_PSlist;
	std::list<GeometryShader *>		m_GSlist;
public:

	ShaderManager::ShaderManager();

	HRESULT GenerateShader(void);

	void VSAdd(VertexShader* shader);

	void PSAdd(PixelShader *shader);

	void GSAdd(GeometryShader *shader);

	VertexShader *GetVertexShader(VertexShader::VERTEX_TYPE etype);

	PixelShader *GetPixelShader(PixelShader::PIXEL_TYPE etype);

	GeometryShader *GetGeometryShader(GeometryShader::GEOMETRY_TYPE etype);

	void ReleaseVS(void);

	void ReleasePS(void);

	void ReleaseGS(void);

	void ReleaseAll(void);
};

#endif
