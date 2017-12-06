//=============================================================================
//
// [manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "../object/object.h"
#include "collision_manager.h"
#include "collider.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************


void CollisionManager::Update(void)
{
	for (auto iteSource = m_listCollider.begin(); iteSource != m_listCollider.end(); ++iteSource)
	{
		for (auto iteDest = iteSource; iteDest != m_listCollider.end(); ++iteDest)
		{
			if (*iteSource != *iteDest && *iteSource != NULL && *iteDest != NULL)
			{
				if (isCollision((*iteSource), (*iteDest)))
				{
					if ((*iteSource) != NULL && (*iteDest) != NULL)
					{
						(*iteSource)->GetGameObject()->OnCollision((*iteDest));
						(*iteDest)->GetGameObject()->OnCollision((*iteSource));
					}
				}
			}
		}
	}

	auto ite = m_listCollider.begin();
	while (ite != m_listCollider.end())
	{
		for (ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
		{
			if (*ite == NULL)
			{
				m_listCollider.erase(ite);
				break;
			}
		}
	}
}

void CollisionManager::AddCollider(Collider* col)
{
	m_listCollider.push_back(col);
}

void CollisionManager::DeleteCollider(Collider* col)
{
	for (auto ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
	{
		if (*ite == col)
		{
			m_listCollider.erase(ite);
			break;

		}
	}
}

bool CollisionManager::isCollision(Collider* Source, Collider* Dest)
{
	if (Source->GetType() == Collider::TYPE_BOX && Dest->GetType() == Collider::TYPE_BOX)
	{
	}

	if (Source->GetType() == Collider::TYPE_SPHERE && Dest->GetType() == Collider::TYPE_SPHERE)
	{
		return CollisionSphereSphere(Source->GetGameObject()->GetSphereCollider(), Dest->GetGameObject()->GetSphereCollider());
	}
	return true;
}

bool CollisionManager::CollisionSphereSphere(SphereCollider* Source, SphereCollider* Dest)
{
	if ((Source->m_mtx._41 - Dest->m_mtx._41)*(Source->m_mtx._41 - Dest->m_mtx._41) +
		(Source->m_mtx._42 - Dest->m_mtx._42)*(Source->m_mtx._42 - Dest->m_mtx._42) +
		(Source->m_mtx._43 - Dest->m_mtx._43)*(Source->m_mtx._43 - Dest->m_mtx._43) <= (Source->m_radius + Dest->m_radius)*(Source->m_radius + Dest->m_radius))
	{
		return true;
	}
	return false;
}