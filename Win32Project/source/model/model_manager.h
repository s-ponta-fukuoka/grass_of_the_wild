//=============================================================================
//
// model_manager.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "model.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class ModelManager
{
	std::list<SkinMeshModel*>  m_SkinMeshList;
public:

	//�ǉ�
	void AddSkinMeshModel(SkinMeshModel* model);

	//���f���f�[�^�T��
	SkinMeshModel* SeekSkinMeshModel(SkinMeshModel* model);
	
	//�X�L�����b�V���폜
	void ReleasekSkinMeshModel(void);

	//�S�폜
	void ReleaseAll(void);
};

#endif
