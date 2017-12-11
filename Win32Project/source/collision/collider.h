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
	virtual ~Collider();
	void SetType(TYPE type);
	TYPE GetType(void);

	virtual void MakeVertex(void) {};

	void SetColliderDelete(bool bDelete) { m_bDelete = bDelete; };
	bool isColliderDelete(void) { return m_bDelete; };

	void RendererDelete(void);

	Object* GetGameObject(void);

private:
	bool							m_bDelete;
	TYPE							eColType;
protected:
	Renderer*						m_pRenderer;
	RenderManager*					m_pRenderManager;
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
	SphereCollider( VECTOR3 pos,
		float fLength,
		Object *pObject, 
		CollisionManager* pCollisionManager,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager,
		AppRenderer::Constant* pConstant,
		AppRenderer::Constant* pLightCameraConstant);

	virtual ~SphereCollider();

	void MakeVertex(void);

	Object::Transform* GetTransform(void) { return m_pTransform; }

	float GetLength(void) { return m_fLength; }

private:
	Object::Transform*				m_pTransform;
	ID3D11Buffer*					m_pIndexBuffer;
	ID3D11Buffer*					m_pVertexBuffer;
	VECTOR3							m_rot;
	float							m_fLength;
	int								m_nNumVertex;
	int								m_nNumAllPolygon;
	VECTOR3							m_NumPolygon;
	VECTOR3							m_Polygon[15000];
	int								m_NumVertexPolygon;
};

#endif