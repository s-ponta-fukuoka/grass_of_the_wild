//=============================================================================
//
// mesh_manager.h
// Author : shoto fukuoka
//
//=============================================================================
#ifndef _MESH_MANAGER_H_
#define _MESH_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <list>
#include "mesh.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class MeshManager
{
	std::list<Mesh*>		m_list;
public:

	//�R���X�g���N�^
	MeshManager();

	//�f�X�g���N�^
	virtual ~MeshManager();

	//�ǉ�
	void AddMesh(Mesh* mesh);

	//�폜
	void DeleteMesh(const Mesh* mesh);

	//�S�X�V
	void UpdateAll(void);

	//�S����
	void ReleaseAll(void);
};

#endif
