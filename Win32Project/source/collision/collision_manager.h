//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "collider.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CollisionManager
{
	std::list<Collider*>  m_listCollider;
public:
	void AddCollider(Collider* col);
	void DeleteCollider(Collider* col);
	void Update(void);
	bool isCollision(Collider* Source, Collider* Dest);

	bool CollisionSphereSphere(SphereCollider* Source, SphereCollider* Dest);
};

#endif
