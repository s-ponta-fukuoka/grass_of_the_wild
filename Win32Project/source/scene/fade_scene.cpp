//=============================================================================
//
// [scene2D.cpp]
// Author : shota fukuoka
//
//=============================================================================
#include "fade_scene.h"
#include "next_scene.h"
#include "../object/canvas/canvas.h"
#include "../renderer/app_renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

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

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Fade::~Fade()
{
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT Fade::Init(NextScene* pNextScene,
	RenderManager* pRenderManager,
	ShaderManager* pShaderManager,
	TextureManager* pTextureManager)
{
	m_pNextScene = pNextScene;

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
		return FALSE;

	Texture* pTexture = new Texture("resource/sprite/fade.png", pTextureManager);

	pRenderManager->AddRenderer(new CanvasRenderer(m_pVertexBuffer,
		NULL,
		pShaderManager,
		pTexture->GetTexture(),
		FADE_VERTEX,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		VertexShader::VS_2D,
		PixelShader::PS_2D,
		TRUE));

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void Fade::Uninit(void)
{
	ES_SAFE_RELEASE(m_pVertexBuffer);
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void Fade::Update(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();
	pDeviceContext = pAppRenderer->GetDeviceContex();

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

/*==============================================================================
�y �t�F�[�h�̐ݒ� �z
==============================================================================*/
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

/*==============================================================================
�y �t�F�[�h�̏�Ԃ̎擾 �z
==============================================================================*/
Fade::FADE_TYPE Fade::GetFade(void)
{
	return m_fade;
}

/*==============================================================================
�y ���_�J���[�̐ݒ� �z
==============================================================================*/
void Fade::SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color)
{
}

/*==============================================================================
�y �傫���l��Ԃ��֐� �z
==============================================================================*/
float Fade::GetMax(float a, float b)
{
	// a���傫���ꍇ�Aa��Ԃ�
	if (a > b)
	{
		return a;
	}

	return b;
}
/*==============================================================================
�y �������l��Ԃ��֐� �z
==============================================================================*/
float Fade::GetMin(float a, float b)
{
	// a���傫���ꍇ�Ab��Ԃ�
	if (a > b)
	{
		return b;
	}

	return a;
}