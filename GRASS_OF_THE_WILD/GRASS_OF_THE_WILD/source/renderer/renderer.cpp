//=============================================================================
//
// directx�����_�����O���� [rendereDX.cpp]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#include "renderer.h"
#include "../shader/shader.h"
#include "../shader/shader_manager.h"
#include "../texture/texturemanager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
// �R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
Renderer::Renderer()
:m_pBlendState(NULL)
,m_pSampleLinear(NULL)
,m_pTexture(NULL)
,m_pVertexBuffer(NULL)
,m_pConstantBuffer(NULL)
,m_pIndexBuffer(NULL)
,m_pTransform(NULL)
{
	;
}

MeshRenderer::MeshRenderer(ID3D11Buffer* pVertexBuffer,
							ID3D11Buffer* pIndexBuffer,
							ShaderManager* pShaderManager,
							ID3D11ShaderResourceView* pTexture,
							Object::Transform* pTransform,
							AppRenderer::Constant* pConstant,
							int	nNumVertexPolygon,
							D3D_PRIMITIVE_TOPOLOGY ePolygon,
							VertexShader::VERTEX_TYPE eVsType,
							PixelShader::PIXEL_TYPE ePsType)
{
	m_ePolygon = ePolygon;

	m_pVertexBuffer = pVertexBuffer;

	m_pIndexBuffer = pIndexBuffer;

	m_pVertexShader = pShaderManager->GetVertexShader(eVsType);

	m_pPixelShader = pShaderManager->GetPixelShader(ePsType);

	m_pTransform = pTransform;

	m_pConstant = pConstant;

	m_nNumVertexPolygon = nNumVertexPolygon;

	m_pTexture = pTexture;

	ConfigConstantBuffer();

	ConfigSamplerState();
}

SkinnedMeshRenderer::SkinnedMeshRenderer()
{
	;
}

CanvasRenderer::CanvasRenderer()
{
	;
}

//�R���X�g���N�^
ShadowRenderer::ShadowRenderer(ID3D11Buffer* pVertexBuffer,
								ShaderManager* pShaderManager,
								VertexShader::VERTEX_TYPE eVsType,
								PixelShader::PIXEL_TYPE ePsType)
{
	m_pVertexBuffer = pVertexBuffer;

	m_pVertexShader = pShaderManager->GetVertexShader(eVsType);

	m_pPixelShader = pShaderManager->GetPixelShader(ePsType);
}

///////////////////////////////////////////////////////////////////////////////
// �f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
Renderer::~Renderer()
{
	;
}

MeshRenderer::~MeshRenderer()
{
	;
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	;
}

CanvasRenderer::~CanvasRenderer()
{
	;
}

ShadowRenderer::~ShadowRenderer()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//�萔�o�b�t�@�ݒ�
///////////////////////////////////////////////////////////////////////////////
void Renderer::ConfigConstantBuffer(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(AppRenderer::Constant);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = sizeof(float);
	pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//�T���v���[�X�e�[�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void Renderer::ConfigSamplerState(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
}

///////////////////////////////////////////////////////////////////////////////
// �`��
///////////////////////////////////////////////////////////////////////////////
void MeshRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(AppRenderer::Vertex3D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(m_ePolygon);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	AppRenderer::Constant hConstant;

	//���[���h�ϊ��p�s��𐶐�
	XMMATRIX hWorld;

	//������
	hWorld = XMMatrixIdentity();

	XMMATRIX hRotate = XMMatrixRotationY(m_pTransform->rot.y);
	hWorld = XMMatrixMultiply(hWorld, hRotate);

	hConstant.world = XMMatrixTranspose(hWorld);
	hConstant.view = m_pConstant->view;
	hConstant.projection = m_pConstant->projection;

	pDeviceContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &hConstant, 0, 0);

	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();

	//�R���e�L�X�g�ɐݒ�
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//�g�p����V�F�[�_�[�̓o�^
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	if (m_pIndexBuffer != NULL)
	{
		//���̃C���f�b�N�X�o�b�t�@���R���e�L�X�g�ɐݒ�
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pDeviceContext->DrawIndexed(m_nNumVertexPolygon, 0, 0);
	}
	else
	{
		pDeviceContext->Draw(m_nNumVertexPolygon, 0);
	}

}

void SkinnedMeshRenderer::Draw()
{
	//for (int i = 0; i < modelCnt; i++)
	//{
	//	//���̃u�����f�B���O���R���e�L�X�g�ɐݒ�
	//	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//	pDeviceContext->OMSetBlendState(m_fbxModel[i].pBlendState, blendFactor, 0xffffffff);
	//
	//	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	//	UINT stride = sizeof(ModelVertex);
	//	UINT offset = 0;
	//	pDeviceContext->IASetVertexBuffers(0, 1, &m_fbxModel[i].pVertexBuffer, &stride, &offset);
	//
	//	//���̃C���f�b�N�X�o�b�t�@���R���e�L�X�g�ɐݒ�
	//	pDeviceContext->IASetIndexBuffer(m_fbxModel[i].pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//
	//	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	//	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//	//���_�C���v�b�g���C�A�E�g���Z�b�g
	//	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());
	//
	//
	//	ConstantModel hConstantBuffer;
	//
	//	//������
	//	XMMATRIX hLcl = XMMatrixIdentity();
	//	XMMATRIX hLclPosition = XMMatrixTranslation(m_Bone[i]->pos.x, m_Bone[i]->pos.y, m_Bone[i]->pos.z);
	//	XMMATRIX hLclRotate = XMMatrixRotationRollPitchYaw(D3DXToRadian(m_Bone[i]->rot.x), D3DXToRadian(m_Bone[i]->rot.y), D3DXToRadian(m_Bone[i]->rot.z));
	//	XMMATRIX hLclScaling = XMMatrixScaling(m_Bone[i]->scl.x, m_Bone[i]->scl.y, m_Bone[i]->scl.z);
	//
	//	hLcl = XMMatrixMultiply(hLcl, hLclScaling);
	//	hLcl = XMMatrixMultiply(hLcl, hLclRotate);
	//	hLcl = XMMatrixMultiply(hLcl, hLclPosition);
	//
	//	//������
	//	XMMATRIX hWorld = XMMatrixIdentity();
	//	XMMATRIX hPosition = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//	XMMATRIX hRotate = XMMatrixRotationRollPitchYaw(D3DXToRadian(m_rot.x), -m_rot.y, D3DXToRadian(m_rot.z));
	//	XMMATRIX hScaling = XMMatrixScaling(1, 1, 1);
	//
	//	hWorld = XMMatrixMultiply(hWorld, hScaling);
	//	hWorld = XMMatrixMultiply(hWorld, hRotate);
	//	hWorld = XMMatrixMultiply(hWorld, hPosition);
	//
	//	hConstantBuffer.mWorld = XMMatrixTranspose(hWorld);
	//	hConstantBuffer.mLclBone = XMMatrixTranspose(hLcl);
	//	hConstantBuffer.mView = XMMatrixTranspose(CSubCamera::GetConstant().mView);
	//	hConstantBuffer.mProjection = XMMatrixTranspose(CSubCamera::GetConstant().mProjection);
	//	hConstantBuffer.mLight = CSubCamera::GetConstant().mLight;
	//
	//	for (int j = 0; j < m_fbxModel[i].clusterCount; j++)
	//	{
	//		hConstantBuffer.mBone[j] = m_Bone[i][j].mat[m_Number][timeCnt];
	//	}
	//
	//	pDeviceContext->UpdateSubresource(m_fbxModel[i].pConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	//
	//	//�g�p����V�F�[�_�[�̓o�^
	//	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	//	pDeviceContext->PSSetShader(m_pShadowPixelShader->GetPixelShader(), NULL, 0);
	//
	//	//�R���e�L�X�g�ɐݒ�
	//	pDeviceContext->VSSetConstantBuffers(0, 1, &m_fbxModel[i].pConstantBuffer);
	//
	//	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	//	pDeviceContext->PSSetSamplers(0, 1, &m_fbxModel[i].pSampleLinear);
	//
	//	if (i < 10 && m_fbxModel[i].filename != NULL)
	//	{
	//		pDeviceContext->PSSetShaderResources(0, 1, &m_fbxModel[i].pTexture);
	//	}
	//	pDeviceContext->PSSetShaderResources(1, 1, &m_ToonTexture);
	//	pDeviceContext->PSSetShaderResources(2, 1, &m_ShadowTexture);
	//
	//	//�v���~�e�B�u�������_�����O
	//	//pDeviceContext->Draw(m_fbxModel[i].PolygonVertexNum, 0);
	//	pDeviceContext->DrawIndexed(m_fbxModel[i].PolygonVertexNum, 0, 0);
	//}
}

void CanvasRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(AppRenderer::Vertex2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	//�R���e�L�X�g�ɐݒ�
	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//�g�p����V�F�[�_�[�̓o�^
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);

	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	//�v���~�e�B�u�������_�����O
	pDeviceContext->Draw(m_nNumVertexPolygon, 0);
}

void ShadowRenderer::Draw(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	ID3D11DepthStencilView* pDepthStencilView = pAppRenderer->GetDepthStencilView();
	ID3D11RenderTargetView* pRenderTargetView = pAppRenderer->GetRenderTargetView();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);//��ʃN���A 

															 // ���O�̃����_�[�^�[�Q�b�g�r���[�ɐ؂�ւ�
	pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_pView[1]);

	// OM�ɕ`��^�[�Q�b�g �r���[�Ɛ[�x/�X�e���V���E�r���[��ݒ�
	ID3D11RenderTargetView* pRender[1] = { NULL };
	pDeviceContext->OMSetRenderTargets(1, &mpRTV, m_pDepthStencilView);

	//�`��

	float ClearColor[4] = { 0,0,1,1 }; //�����F
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);//��ʃN���A 

	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	pDeviceContext->RSSetViewports(1, &m_pView[0]);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}