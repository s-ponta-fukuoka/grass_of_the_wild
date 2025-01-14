//=============================================================================
//
// シェーダー [shader.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _SHADER_H_
#define _SHADER_H_

#include <tchar.h>
#include "../renderer/app_renderer.h"

class ShaderManager;

//*********************************************************
//クラス
//*********************************************************
class Shader
{
public:
	Shader();

protected:
	TCHAR* m_csoName;
};

class VertexShader : public Shader
{
public:
	typedef enum
	{
		VS_NORMAL,
		VS_2D,
		VS_3D,
		VS_TOON,
		VS_GRASS,
		VS_MODEL,
		VS_DEFERRED,
		VS_MAX
	}VERTEX_TYPE;

	VertexShader(TCHAR* csoName, VERTEX_TYPE vsType, ShaderManager* pShaderManager);

	void CreateVertexShader(TCHAR* csoName);

	VERTEX_TYPE GetType(void);
	ID3D11VertexShader *GetVertexShader(void);
	ID3D11InputLayout *GetVertexLayout(void);

private:
	VERTEX_TYPE m_eVsType;
	ID3D11VertexShader *m_pVertexShader;
	ID3D11InputLayout* m_pVertexLayout;
};

class PixelShader : public Shader
{
public:
	typedef enum
	{
		PS_NORMAL,
		PS_2D,
		PS_3D,
		PS_MAT,
		PS_SHADOW,
		PS_TOON,
		PS_GRASS,
		PS_DEFERRED,
		PS_MAX
	}PIXEL_TYPE;

	PixelShader(TCHAR* csoName, PIXEL_TYPE psType, ShaderManager* pShaderManager);

	void CreatePixelShader(TCHAR* csoName);

	PIXEL_TYPE GetType(void);
	ID3D11PixelShader *GetPixelShader(void);
private:
	PIXEL_TYPE m_ePsType;
	ID3D11PixelShader *m_pPixelShader;
};

class GeometryShader : public Shader
{
public:
	typedef enum
	{
		GS_NONE,
		GS_GRASS,
		GS_OUTLINE,
		GS_MAX
	}GEOMETRY_TYPE;

	GeometryShader(TCHAR* csoName, GEOMETRY_TYPE gsType, ShaderManager* pShaderManager);

	void CreateGeometryShader(TCHAR* csoName);

	GEOMETRY_TYPE GetType(void);
	ID3D11GeometryShader *GetGeometryShaderr(void);
private:
	GEOMETRY_TYPE m_eGsType;
	ID3D11GeometryShader *m_pGeometryShader;
};

#endif