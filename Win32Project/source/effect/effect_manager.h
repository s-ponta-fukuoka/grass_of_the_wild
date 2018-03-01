//=============================================================================
//
// effect_manager.h(�G�t�F�N�V�A)
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#ifdef _DEBUG
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#else
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#endif

#define EFFECT_NUM (10)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class EffectManager
{
	::Effekseer::Manager*			m_pManager; //��ʓ��̃G�t�F�N�g�����ׂĊǗ�
	::EffekseerRenderer::Renderer*	m_pRenderer;	//�G�t�F�N�V�A��`�悷��C���X�^���X
	::Effekseer::Effect*			m_pEffect[EFFECT_NUM];	//�ǂݍ��񂾃G�t�F�N�g�������Ă���
	::Effekseer::Handle				m_handle;	//�G�t�F�N�gID
	::Effekseer::Vector3D			m_position;
public:

	typedef enum
	{
		EFFECT_HIT,
		EFFECT_ATTACK_001,
		EFFECT_ATTACK_002,
		EFFECT_ATTACK_003,
		EFFECT_ATTACK_JUMP,
		EFFECT_ENEMY_DEFFENCE,
		EFFECT_SMOKE,
		EFFECT_DETH,
		EFFECT_MAX
	}EFFECT;

	EffectManager();

	virtual ~EffectManager();

	void Init(void);

	void Release(void);

	void Update(void);

	void Draw(void);

	void GenerateEffect(void);

	::Effekseer::Effect *GetEffect(EFFECT effect);

	::Effekseer::Manager *GetEManager(void);

	::EffekseerRenderer::Renderer *GetERendererer(void);
};

#endif
