//=============================================================================
//
// fade_scene.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "fade_scene.h"
#include "next_scene.h"
#include "../object/canvas/canvas.h"
#include "../renderer/app_renderer.h"
#include "../shader/shader_manager.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_TEXTURE_NAME	"resource/sprite/fade.png"	
#define COLOR_FADE_MIN	(0.0f)								
#define COLOR_FADE_MAX	(1.0f)								
#define FADE_RATE		(0.05f)								
#define FADE_VERTEX		(4)

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
Fade *Fade::m_pFade = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Fade::Fade()
{
	m_pNextScene = NULL;
	m_modeNext = NULL;
	m_pos = VECTOR3(-1.0f, 1.0f, 0.0f);			// �|���S���̈ʒu
	m_normal = VECTOR3(0.0f, 0.0f, 0.0f);			// �|���S���̈ʒu
	m_size = VECTOR3(2.0f, 2.0f, 0.0f);			// �|���S���̈ʒu
	m_tex = VECTOR2(0.0f, 0.0f);			// �|���S���̈ʒu
	m_ofsettex = VECTOR2(0.0f, 0.0f);			// �|���S���̈ʒu
	m_color = VECTOR4(1, 1, 1, 0);			// �|���S���̈ʒu
	m_bLoadTexture = false;
}

///////////////////////////////////////////////////////////////////////////////
//�C���X�^���X����
///////////////////////////////////////////////////////////////////////////////
void Fade::CreateInstance(void)
{
	if (m_pFade != NULL) { return; }
	m_pFade = new Fade();
}

///////////////////////////////////////////////////////////////////////////////
//�C���X�^���X�擾
///////////////////////////////////////////////////////////////////////////////
Fade* Fade::GetInstance(void)
{
	return m_pFade;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Fade::~Fade()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Fade::Init(NextScene* pNextScene,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pConstantBuffer = NULL;

	m_pNextScene = pNextScene;

	MakeVertex();

	ConfigSamplerState();

	ConfigBlendState(TRUE);

	m_pTexture = new Texture(FADE_TEXTURE_NAME, pTextureManager);

	m_pVertexShader = pShaderManager->GetVertexShader(VertexShader::VS_2D);

	m_pPixelShader = pShaderManager->GetPixelShader(PixelShader::PS_2D);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Fade::Release(void)
{
	ES_SAFE_RELEASE(m_pVertexBuffer);
	ES_SAFE_RELEASE(m_pBlendState);
	ES_SAFE_RELEASE(m_pConstantBuffer);
	ES_SAFE_RELEASE(m_pSampleLinear);
}

//=============================================================================
// �X�V����
//=============================================================================
void Fade::Update(void)
{
	if (m_fade == FADE_NONE)
	{
		return;
	}

	//�t�F�[�h�C��
	if (m_fade == FADE_IN)
	{
		// ���l��Min�ɂȂ����ꍇ�A�t�F�[�h�̏�Ԃ��t�F�[�h�Ȃ��֕ύX
		if ((m_color.w = GetMax((m_color.w - FADE_RATE), COLOR_FADE_MIN)) == COLOR_FADE_MIN)
		{
			m_fade = FADE_NONE;
		}
	}
	// �t�F�[�h�A�E�g�i���l�����Z���A�o�b�N��ʂ������Ă����j
	else if (m_fade == FADE_OUT)
	{
		// ���l��Max�ɂȂ����ꍇ�A�t�F�[�h�̏�Ԃ��t�F�[�h�C���֕ύX
		if ((m_color.w = GetMin((m_color.w + FADE_RATE), COLOR_FADE_MAX)) == COLOR_FADE_MAX)
		{
			m_fade = FADE_IN;

			// ���̃��[�h�̕ύX
			m_pNextScene->NextSceneUpdate(m_modeNext);
		}
	}

	SetVertex();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Fade::Draw()
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();

	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�u�����f�B���O���R���e�L�X�g�ɐݒ�
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pDeviceContext->OMSetBlendState(m_pBlendState, blendFactor, 0xffffffff);

	ID3D11DepthStencilView* pDSV = pAppRenderer->GetDepthStencilView();
	pDeviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_STENCIL, 1.0f, 0);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(AppRenderer::Vertex2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	pDeviceContext->IASetInputLayout(m_pVertexShader->GetVertexLayout());

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	ID3D11ShaderResourceView* pTexture = m_pTexture->GetTexture();
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	//�g�p����V�F�[�_�[�̓o�^
	pDeviceContext->VSSetShader(m_pVertexShader->GetVertexShader(), NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader->GetPixelShader(), NULL, 0);

	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	pDeviceContext->Draw(4, 0);
}

///////////////////////////////////////////////////////////////////////////////
//�萔�o�b�t�@�ݒ�
///////////////////////////////////////////////////////////////////////////////
void Fade::ConfigConstantBuffer(UINT ByteWidth)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = ByteWidth;
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
void Fade::ConfigSamplerState(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MaxAnisotropy = 2;
	SamDesc.MipLODBias = 0;
	SamDesc.MinLOD = -FLT_MAX;
	SamDesc.MaxLOD = FLT_MAX;

	pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);
}

///////////////////////////////////////////////////////////////////////////////
//�u�����h�X�e�[�g�ݒ�
///////////////////////////////////////////////////////////////////////////////
void Fade::ConfigBlendState(BOOL bBlend)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();

	D3D11_BLEND_DESC BlendStateDesc;
	BlendStateDesc.AlphaToCoverageEnable = FALSE;
	BlendStateDesc.IndependentBlendEnable = FALSE;
	for (int i = 0; i < 8; i++)
	{
		BlendStateDesc.RenderTarget[i].BlendEnable = bBlend;
		BlendStateDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendStateDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendStateDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendStateDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendStateDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	pDevice->CreateBlendState(&BlendStateDesc, &m_pBlendState);
}

//=============================================================================
// �t�F�[�h�̐ݒ�
//=============================================================================
void Fade::SetFade(FADE_TYPE fade, Scene* modenext)
{
	// �t�F�[�h�ݒ�G���[
	if (fade == FADE_NONE)
	{
		MessageBox(NULL, "�g�t�F�[�h�Ȃ��h���ݒ肳��܂���", "Error", MB_OK | MB_ICONHAND);
	}

	// �t�F�[�h�̏�Ԃ��Z�b�g
	m_fade = fade;

	// ���̃��[�h���Z�b�g
	m_modeNext = modenext;
}

//==============================================================================
// �t�F�[�h�̏�Ԃ̎擾
//==============================================================================
Fade::FADE_TYPE Fade::GetFade(void)
{
	return m_fade;
}

//==============================================================================
// ���_�J���[�̐ݒ�
//==============================================================================
void Fade::SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color)
{
}

//==============================================================================
// �傫���l��Ԃ��֐�
//==============================================================================
float Fade::GetMax(float a, float b)
{
	// a���傫���ꍇ�Aa��Ԃ�
	if (a > b)
	{
		return a;
	}

	return b;
}

//==============================================================================
// �������l��Ԃ��֐�
//==============================================================================
float Fade::GetMin(float a, float b)
{
	// a���傫���ꍇ�Ab��Ԃ�
	if (a > b)
	{
		return b;
	}

	return a;
}

//==============================================================================
// ���_����
//==============================================================================
void Fade::MakeVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	//�l�p�`
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z),  VECTOR4(0,0,0,0), VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(0,0,0,0),VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(0,0,0,0),VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(0,0,0,0), VECTOR2(1,1),
	};


	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(AppRenderer::Vertex2D) * FADE_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return;
}

//==============================================================================
// ���_�ݒ�
//==============================================================================
void Fade::SetVertex(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	pDeviceContext = pAppRenderer->GetDeviceContex();


	D3D11_MAPPED_SUBRESOURCE msr;

	pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�J���[�̐ݒ�
	AppRenderer::Vertex2D vertices[] =
	{
		VECTOR3(m_pos.x,m_pos.y,m_pos.z),  VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w),VECTOR2(0,0),
		VECTOR3(m_pos.x + m_size.x,m_pos.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(1,0),
		VECTOR3(m_pos.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(0,1),
		VECTOR3(m_pos.x + m_size.x,m_pos.y - m_size.y,m_pos.z), VECTOR4(m_color.x,m_color.y,m_color.z,m_color.w), VECTOR2(1,1),
	};

	memcpy(msr.pData, vertices, sizeof(AppRenderer::Vertex2D) * FADE_VERTEX); // 3���_���R�s�[

	pDeviceContext->Unmap(m_pVertexBuffer, 0);
}