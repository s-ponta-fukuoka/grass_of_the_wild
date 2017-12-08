//=============================================================================
//
// object.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "../lib/vector.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class Collider;
class SphereCollider;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class Object
{
public:
	enum ObjectType
	{
		TYPE_NONE,
		TYPE_PLAYER,
		TYPE_PLAYER_ATTACK,
		TYPE_ENEMY,
	};

	//transform
	struct Transform
	{
		VECTOR3	position;
		VECTOR3 rot;
		VECTOR3 scale;
	};

	//コンストラクタ
	Object();

	//デストラクタ
	virtual ~Object();

	//初期化
	virtual HRESULT Init(void);

	//終了
	virtual void Release(void);

	//更新
	virtual void Update(void);

	//トランスフォーム取得
	Transform* GetTransform(void);

	void SetTransform(Transform* pTransform) { m_pTransform = pTransform; }

	//タイプ取得・設定
	void SetObjectType(ObjectType objtype);
	ObjectType GetObjectType(void);

	virtual void OnCollision(Collider* col) {};
	virtual SphereCollider* GetSphereCollider(void) { return NULL; };

protected:
	ObjectType				m_eObjectType;

	Transform*				m_pTransform;
};

#endif