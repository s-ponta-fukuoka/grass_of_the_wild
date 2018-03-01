//=============================================================================
//
// effect_manager.h(エフェクシア)
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _EFFECT_MANAGER_H_
#define _EFFECT_MANAGER_H_

//*****************************************************************************
// ヘッダファイル
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
// クラス定義
//*****************************************************************************
class EffectManager
{
	::Effekseer::Manager*			m_pManager; //画面内のエフェクトをすべて管理
	::EffekseerRenderer::Renderer*	m_pRenderer;	//エフェクシアを描画するインスタンス
	::Effekseer::Effect*			m_pEffect[EFFECT_NUM];	//読み込んだエフェクトを持っている
	::Effekseer::Handle				m_handle;	//エフェクトID
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
