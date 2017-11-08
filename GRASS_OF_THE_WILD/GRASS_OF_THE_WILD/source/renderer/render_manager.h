//=============================================================================
//
// render_manager.h
// Author : shoto fukuoka
//
//=============================================================================
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <list>

class Renderer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class RenderManager
{
	std::list<Renderer*>		m_listRenderer;

	std::list<Renderer*>		m_listShadowRenderer;
public:

	//�R���X�g���N�^
	RenderManager();

	//�f�X�g���N�^
	virtual ~RenderManager();

	//�ǉ�
	void AddRenderer(Renderer* renderer);

	//�폜
	void DeleteRenderer(const Renderer* renderer);

	//�S�`��
	void DrawAll(void);

	//�ǉ�
	void AddShadowRenderer(Renderer* renderer);

	//�폜
	void DeleteShadowRenderer(const Renderer* renderer);

	//�V���h�E�}�b�v�p�`��
	void ShadowDrawAll(void);

	//�S����
	void ReleaseAll(void);
};

#endif
