//=============================================================================
//
// canvas_manager.h
// Author : shoto fukuoka
//
//=============================================================================
#ifndef _CANVAS_MANAGER_H_
#define _CANVAS_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <list>
#include "canvas.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CanvasManager
{
	std::list<Canvas*>		m_list;
public:

	//�R���X�g���N�^
	CanvasManager();

	//�f�X�g���N�^
	virtual ~CanvasManager();

	//�ǉ�
	void AddCanvas(Canvas* canvas);

	//�폜
	void DeleteCanvas(const Canvas* canvas);

	//�S�X�V
	void UpdateAll(void);

	//�S����
	void ReleaseAll(void);
};

#endif
