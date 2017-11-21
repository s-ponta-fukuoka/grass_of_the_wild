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

#include "app_renderer.h"

class Renderer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class RenderManager
{
	std::list<Renderer*>		m_listRenderer;

	std::list<Renderer*>		m_listShadowRenderer;

	D3D11_VIEWPORT					m_ShadowViewPort;

	ID3D11DepthStencilView*			m_pShadowDepthStencilView;

	ID3D11RenderTargetView*			m_pShadowRenderTargetView;

	ID3D11ShaderResourceView*		m_pShadowTexture;
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

	//�e�r���[�|�[�g�ݒ�
	void SetShadowViewPort(D3D11_VIEWPORT ShadowViewPort);

	//�e�X�e���V���r���[�ݒ�
	void SetShadowDepthStencilView(ID3D11DepthStencilView* pShadowDepthStencilView);

	//�e�����_�[�^�[�Q�b�g�r���[�ݒ�
	void SetShadowRenderTargetVie(ID3D11RenderTargetView* pShadowRenderTargetView);

	//�V���h�E�}�b�v�ݒ�
	void SetShadowTexture(ID3D11ShaderResourceView* pShadowTexture);

	//�V���h�E�}�b�v�擾
	ID3D11ShaderResourceView* GetShadowTexture(void) const;
};

#endif
