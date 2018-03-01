//=============================================================================
//
// effect_manager.cpp(�G�t�F�N�V�A)
// Author : shota fukuoka
//
//=============================================================================
#include "effect_manager.h"
#include "../app/app.h"
#include "../renderer/app_renderer.h"
#include "../device/input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
///////////////////////////////////////////////////////////////////////////////
EffectManager::EffectManager()
	: m_pManager(NULL)
	, m_pRenderer(NULL)
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		m_pEffect[i] = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//�f�X�g���N�^
///////////////////////////////////////////////////////////////////////////////
EffectManager::~EffectManager()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//������
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Init(void)
{
	AppRenderer* pAppRenderer = AppRenderer::GetInstance();
	ID3D11Device* pDevice = pAppRenderer->GetDevice();
	ID3D11DeviceContext* pDeviceContext = pAppRenderer->GetDeviceContex();

	// �`��p�C���X�^���X�̐���
	m_pRenderer = ::EffekseerRendererDX11::Renderer::Create(pDevice, pDeviceContext, 10000);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_pManager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
	m_pManager->SetTrackRenderer(m_pRenderer->CreateTrackRenderer());
	m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());

	GenerateEffect();
}

///////////////////////////////////////////////////////////////////////////////
//�G�t�F�N�g����
///////////////////////////////////////////////////////////////////////////////
void EffectManager::GenerateEffect(void)
{
	// �G�t�F�N�g�̓Ǎ�
	m_pEffect[EFFECT_HIT] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/hit.efk");
	m_pEffect[EFFECT_DETH] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/deth.efk");
	m_pEffect[EFFECT_ATTACK_001] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_001.efk");
	m_pEffect[EFFECT_ATTACK_002] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_002.efk");
	m_pEffect[EFFECT_ATTACK_003] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_003.efk");
	m_pEffect[EFFECT_SMOKE] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/smoke.efk");
	m_pEffect[EFFECT_ENEMY_DEFFENCE] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/guard.efk");
	m_pEffect[EFFECT_ATTACK_JUMP] = Effekseer::Effect::Create(m_pManager, (const EFK_CHAR*)L"resource/EFK/attack_jump.efk");
}

///////////////////////////////////////////////////////////////////////////////
//�I��
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Release(void)
{
	// �G�t�F�N�g�̒�~
	m_pManager->StopEffect(m_handle);

	// �G�t�F�N�g�̔j��
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		SAFE_RELEASE(m_pEffect[i]);
	}

	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	m_pManager->Destroy();
}

///////////////////////////////////////////////////////////////////////////////
//�X�V
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Update(void)
{
	m_pManager->Update();
}

///////////////////////////////////////////////////////////////////////////////
//�`��
///////////////////////////////////////////////////////////////////////////////
void EffectManager::Draw(void)
{
	InputKeyboard* pInputKeyboard = InputKeyboard::GetInstance();

	// �G�t�F�N�g�̕`��J�n�������s���B
	m_pRenderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	m_pManager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	m_pRenderer->EndRendering();
}

///////////////////////////////////////////////////////////////////////////////
//�G�t�F�N�g�擾
///////////////////////////////////////////////////////////////////////////////
::Effekseer::Effect* EffectManager::GetEffect(EFFECT effect)
{
	return m_pEffect[effect];
}

///////////////////////////////////////////////////////////////////////////////
//�G�t�F�N�g�}�l�[�W���[�擾
///////////////////////////////////////////////////////////////////////////////
::Effekseer::Manager* EffectManager::GetEManager(void)
{
	return m_pManager;
}

///////////////////////////////////////////////////////////////////////////////
//�G�t�F�N�g�����_���[�擾
///////////////////////////////////////////////////////////////////////////////
::EffekseerRenderer::Renderer* EffectManager::GetERendererer(void)
{
	return m_pRenderer;
}