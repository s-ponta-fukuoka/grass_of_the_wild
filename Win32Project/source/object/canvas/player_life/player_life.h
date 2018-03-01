//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _PLAYER_LIFE_H_
#define _PLAYER_LIFE_H_

//*****************************************************************************
// ヘッダファイル
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// ライブラリファイル
// [構成プロパティ]->[リンカー]->[入力]->[追加の依存ファイル]に記述しても可能
//*****************************************************************************

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class PlayerLife : public Canvas
{
public:
	//コンストラクタ
	PlayerLife(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//デストラクタ
	virtual ~PlayerLife( );

	//終了
	void Release( void );

	//更新
	void Update( void );

	//バッファ
	void MakeVertex(ID3D11Device* pDevice);

	//ライフ増減・減少
	void Add(int nLife) { m_nLife += nLife; }
	void Sub(int nLife) { 
		if (m_nCoolTime == 0)
		{
			m_nLife -= nLife;
		}
		CoolTimeCount(0);
	}

	void CoolTimeCount(int nCoolCnt) {
		if (nCoolCnt != m_nCoolTime)
		{
			m_nCoolTime++;
		}
		else if(nCoolCnt == m_nCoolTime)
		{
			m_nCoolTime = 0;
		}
	}

	void SetLifeZero(bool bUse) { m_bLifeZero = bUse; };
	bool GetLifeZero(void) { return m_bLifeZero; };

	//ライフ取得・設定
	void SetLife(int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

private:
	bool							m_bLifeZero;

	int								m_nLife;

	VECTOR3							m_size;

	int								m_nCoolTime;
};

#endif