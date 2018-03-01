//=============================================================================
//
// collision_manager.h
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../object/object.h"
#include "collision_manager.h"
#include "collider.h"

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CollisionManager::CollisionManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
CollisionManager::~CollisionManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//更新
///////////////////////////////////////////////////////////////////////////////
void CollisionManager::Update(void)
{
	for (auto iteSource = m_listCollider.begin(); iteSource != m_listCollider.end(); ++iteSource)
	{
		for (auto iteDest = iteSource; iteDest != m_listCollider.end(); ++iteDest)
		{
			SearchCollision(*iteSource, *iteDest);
		}
	}

	for (auto ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
	{
		if (*ite == NULL)
		{
			m_listCollider.erase(ite);
			break;
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
//追加
///////////////////////////////////////////////////////////////////////////////
void CollisionManager::SearchCollision(Collider* Source, Collider* Dest)
{
	if (Source == Dest)
	{
		return;
	}

	if (Source == NULL)
	{
		return;
	}

	if (Dest == NULL)
	{
		return;
	}

	if (!isCollision(Source, Dest))
	{
		return;
	}

	if (Source == NULL)
	{
		return;
	}

	if (Dest == NULL)
	{
		return;
	}

	Source->GetGameObject()->OnCollision(Dest);
	if (!Source->isColliderDelete())
	{
		Dest->GetGameObject()->OnCollision(Source);
	}
	else
	{
		Source = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//追加
///////////////////////////////////////////////////////////////////////////////
void CollisionManager::AddCollider(Collider* col)
{
	m_listCollider.push_back(col);
}

///////////////////////////////////////////////////////////////////////////////
//削除
///////////////////////////////////////////////////////////////////////////////
void CollisionManager::DeleteCollider(Collider* col)
{
	for (auto ite = m_listCollider.begin(); ite != m_listCollider.end(); ++ite)
	{
		if (*ite == col)
		{
			(*ite) = NULL;
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//当たっているか
///////////////////////////////////////////////////////////////////////////////
bool CollisionManager::isCollision(Collider* Source, Collider* Dest)
{
	if (Source->GetType() == Collider::TYPE_BOX && Dest->GetType() == Collider::TYPE_BOX)
	{
		;
	}

	if (Source->GetType() == Collider::TYPE_SPHERE && Dest->GetType() == Collider::TYPE_SPHERE)
	{
		return CollisionSphereSphere(Source->GetGameObject()->GetSphereCollider(), Dest->GetGameObject()->GetSphereCollider());
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////
//球と球
///////////////////////////////////////////////////////////////////////////////
bool CollisionManager::CollisionSphereSphere(SphereCollider* Source, SphereCollider* Dest)
{
	VECTOR3 SourcePos = Source->GetTransform()->position;
	VECTOR3 DestPos = Dest->GetTransform()->position;

	float SourceLength = Source->GetLength();
	float DestLength = Dest->GetLength();

	if ((SourcePos.x - DestPos.x)*(SourcePos.x - DestPos.x) +
		(SourcePos.y - DestPos.y)*(SourcePos.y - DestPos.y) +
		(SourcePos.z - DestPos.z)*(SourcePos.z - DestPos.z) <= (SourceLength + DestLength)*(SourceLength + DestLength))
	{
		return true;
	}
	return false;
}