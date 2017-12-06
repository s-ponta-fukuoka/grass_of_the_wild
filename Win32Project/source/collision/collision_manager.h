//=============================================================================
//
// [collisionmanager.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include "collider.h"

//*****************************************************************************
// クラス定義
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
