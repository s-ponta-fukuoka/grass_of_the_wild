//=============================================================================
//
// îªíËèàóù [collider.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "../renderer/app_renderer.h"

class Object;
class CollisionManager;
class ShaderManager;
class RenderManager;
class TextureManager;

//*********************************************************
//ÉNÉâÉX
//*********************************************************
class Collider
{
public:
	typedef enum
	{
		TYPE_BOX,
		TYPE_SPHERE,
		TYPE_MAX
	}TYPE;

	Collider();
	void SetType(TYPE type);
	TYPE GetType(void);

	virtual void MakeVertex(void) {};

	Object* GetGameObject(void);

private:
	TYPE eColType;
protected:
	Object* m_pObject;
};

class BoxCollider : public Collider
{
public:
	BoxCollider(Object *pObject, CollisionManager* pCollisionManager);

	VECTOR3 m_center;
	VECTOR3 m_size;
	float m_radius;
};

class SphereCollider : public Collider
{
public:
	SphereCollider(Object *pObject, 
		CollisionManager* pCollisionManager,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant);

	XMFLOAT4X4 m_mtx;
	float m_radius;

	void MakeVertex(void);

private:
	ID3D11Buffer*					m_pIndexBuffer;
	ID3D11Buffer*					m_pVertexBuffer;
	AppRenderer::Vertex3D			m_vertex;
	VECTOR3							m_rot;
	int								m_nLength;
	int								m_nNumVertex;
	int								m_nNumAllPolygon;
	VECTOR3							m_NumPolygon;
	VECTOR3							m_Polygon[15000];
	int								m_NumVertexPolygon;
};

#endif