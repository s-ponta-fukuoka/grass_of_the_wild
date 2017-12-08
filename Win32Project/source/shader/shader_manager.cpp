//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "shader_manager.h"
#include "shader.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

ShaderManager::ShaderManager()
{
}

HRESULT ShaderManager::GenerateShader(void)
{

	new VertexShader("bin/shader/VertexShader.cso", VertexShader::VS_NORMAL, this);
	new VertexShader("bin/shader/NormalVShader.cso", VertexShader::VS_2D, this);
	new VertexShader("bin/shader/Vertex3DShader.cso", VertexShader::VS_3D, this);
	new VertexShader("bin/shader/ToonShader.cso", VertexShader::VS_TOON, this);
	new VertexShader("bin/shader/VertexGrass.cso", VertexShader::VS_GRASS, this);

	new PixelShader("bin/shader/PixelShader.cso", PixelShader::PS_NORMAL, this);
	new PixelShader("bin/shader/NormalPShader.cso", PixelShader::PS_2D, this);
	new PixelShader("bin/shader/Pixel3DShader.cso", PixelShader::PS_3D, this);
	new PixelShader("bin/shader/ToonPixel.cso", PixelShader::PS_TOON, this);
	new PixelShader("bin/shader/Shadow.cso", PixelShader::PS_SHADOW, this);
	new PixelShader("bin/shader/matPixelShader.cso", PixelShader::PS_MAT, this);
	new PixelShader("bin/shader/PixelGrass.cso", PixelShader::PS_GRASS, this);

	new GeometryShader("bin/shader/GeometryGrass.cso", GeometryShader::GS_GRASS, this);
	new GeometryShader("bin/shader/outline.cso", GeometryShader::GS_OUTLINE, this);

	return S_OK;
}

void ShaderManager::VSAdd(VertexShader *shader)
{
	m_VSlist.push_back(shader);
}

VertexShader *ShaderManager::GetVertexShader(VertexShader::VERTEX_TYPE etype)
{
	for (auto ite = m_VSlist.begin(); ite != m_VSlist.end(); ++ite)
	{
		if (*ite == NULL) { return NULL; }
		if (dynamic_cast<VertexShader *>(*ite)->GetType() == etype)
		{
			return dynamic_cast<VertexShader *>(*ite);
		}
	}
	return NULL;
}

void ShaderManager::PSAdd(PixelShader *shader)
{
	m_PSlist.push_back(shader);
}

PixelShader *ShaderManager::GetPixelShader(PixelShader::PIXEL_TYPE etype)
{
	for (auto ite = m_PSlist.begin(); ite != m_PSlist.end(); ++ite)
	{
		if (*ite == NULL) { return NULL; }
		if (dynamic_cast<PixelShader *>(*ite)->GetType() == etype)
		{
			return dynamic_cast<PixelShader *>(*ite);
		}
	}
	return NULL;
}

void ShaderManager::GSAdd(GeometryShader *shader)
{
	m_GSlist.push_back(shader);
}

GeometryShader *ShaderManager::GetGeometryShader(GeometryShader::GEOMETRY_TYPE etype)
{
	for (auto ite = m_GSlist.begin(); ite != m_GSlist.end(); ++ite)
	{
		if (*ite == NULL) { return NULL; }
		if (dynamic_cast<GeometryShader *>(*ite)->GetType() == etype)
		{
			return dynamic_cast<GeometryShader *>(*ite);
		}
	}
	return NULL;
}

void ShaderManager::ReleaseVS(void)
{
	for (auto ite = m_VSlist.begin(); ite != m_VSlist.end(); ite = m_VSlist.begin())
	{
		if (*ite == NULL) { continue; }
		m_VSlist.erase(ite);
	}
}

void ShaderManager::ReleasePS(void)
{
	for (auto ite = m_PSlist.begin(); ite != m_PSlist.end(); ite = m_PSlist.begin())
	{
		if (*ite == NULL) { continue; }
		m_PSlist.erase(ite);
	}
}

void ShaderManager::ReleaseGS(void)
{
	for (auto ite = m_GSlist.begin(); ite != m_GSlist.end(); ite = m_GSlist.begin())
	{
		if (*ite == NULL) { continue; }
		m_GSlist.erase(ite);
	}
}

void ShaderManager::ReleaseAll(void)
{
	ReleaseVS();

	ReleaseGS();

	ReleasePS();
}