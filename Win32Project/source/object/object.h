//=============================================================================
//
// object.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "../lib/vector.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class Collider;
class SphereCollider;

//*****************************************************************************
// �N���X��`
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

	//�R���X�g���N�^
	Object();

	//�f�X�g���N�^
	virtual ~Object();

	//������
	virtual HRESULT Init(void);

	//�I��
	virtual void Release(void);

	//�X�V
	virtual void Update(void);

	//�g�����X�t�H�[���擾
	Transform* GetTransform(void);

	void SetTransform(Transform* pTransform) { m_pTransform = pTransform; }

	//�^�C�v�擾�E�ݒ�
	void SetObjectType(ObjectType objtype);
	ObjectType GetObjectType(void);

	virtual void OnCollision(Collider* col) {};
	virtual SphereCollider* GetSphereCollider(void) { return NULL; };

protected:
	ObjectType				m_eObjectType;

	Transform*				m_pTransform;
};

#endif