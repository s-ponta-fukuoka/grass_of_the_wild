//=============================================================================
//
// [render_manager.h]
// Author : shota fukuoka
//
//=============================================================================
#include "render_manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
RenderManager::RenderManager()
{
}

///////////////////////////////////////////////////////////////////////////////
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
RenderManager::~RenderManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//�ǉ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//�폜
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteRenderer(const Renderer* renderer)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�S�`��
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DrawAll(void)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) {
			m_listRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//�ǉ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddDeferredRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listDeferredRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//�폜
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteDeferredRenderer(const Renderer* renderer)
{
	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�S�`��
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeferredDrawAll(void)
{
	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL){
			m_listDeferredRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�p�ǉ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::AddShadowRenderer(Renderer* renderer)
{
	if (renderer == NULL) { return; }
	m_listShadowRenderer.push_back(renderer);
}

///////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�p�폜
///////////////////////////////////////////////////////////////////////////////
void RenderManager::DeleteShadowRenderer(const Renderer* renderer)
{
	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) != renderer) { continue; }
		(*ite) = NULL;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�p�I�u�W�F�N�g�`��
///////////////////////////////////////////////////////////////////////////////
void RenderManager::ShadowDrawAll(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(m_pShadowRenderTargetView, clearColor);//��ʃN���A 

	// ���O�̃����_�[�^�[�Q�b�g�r���[�ɐ؂�ւ�
	pDeviceContext->ClearDepthStencilView(m_pShadowDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_ShadowViewPort);

	// OM�ɕ`��^�[�Q�b�g �r���[�Ɛ[�x/�X�e���V���E�r���[��ݒ�
	ID3D11RenderTargetView* pRender[1] = { NULL };
	pDeviceContext->OMSetRenderTargets(1, &m_pShadowRenderTargetView, m_pShadowDepthStencilView);

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Draw();
	}

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) {
			m_listShadowRenderer.erase(ite);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//�S�폜
///////////////////////////////////////////////////////////////////////////////
void RenderManager::ReleaseAll(void)
{
	for (auto ite = m_listRenderer.begin(); ite != m_listRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}

	for (auto ite = m_listDeferredRenderer.begin(); ite != m_listDeferredRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}

	for (auto ite = m_listShadowRenderer.begin(); ite != m_listShadowRenderer.end(); ++ite)
	{
		if ((*ite) == NULL) { continue; }
		(*ite)->Release();
		delete (*ite);
		(*ite) = NULL;
	}
	m_listRenderer.clear();

	m_listShadowRenderer.clear();

	m_listDeferredRenderer.clear();
}

///////////////////////////////////////////////////////////////////////////////
//�e�r���[�|�[�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowViewPort(D3D11_VIEWPORT ShadowViewPort)
{
	m_ShadowViewPort = ShadowViewPort;
}

///////////////////////////////////////////////////////////////////////////////
//�e�X�e���V���r���[�ݒ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowDepthStencilView(ID3D11DepthStencilView* pShadowDepthStencilView)
{
	m_pShadowDepthStencilView = pShadowDepthStencilView;
}

///////////////////////////////////////////////////////////////////////////////
//�e�����_�[�^�[�Q�b�g�r���[�ݒ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowRenderTargetVie(ID3D11RenderTargetView* pShadowRenderTargetView)
{
	m_pShadowRenderTargetView = pShadowRenderTargetView;
}

///////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�ݒ�
///////////////////////////////////////////////////////////////////////////////
void RenderManager::SetShadowTexture(ID3D11ShaderResourceView* pShadowTexture)
{
	m_pShadowTexture = pShadowTexture;
}

///////////////////////////////////////////////////////////////////////////////
//�V���h�E�}�b�v�擾
///////////////////////////////////////////////////////////////////////////////
ID3D11ShaderResourceView* RenderManager::GetShadowTexture(void) const
{
	return m_pShadowTexture;
}