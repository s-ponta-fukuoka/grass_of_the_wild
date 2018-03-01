//=============================================================================
//
// [ShadowMap.h]
// Author : shota fukuoka
//
//=============================================================================
#ifndef _PLAYER_LIFE_H_
#define _PLAYER_LIFE_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <Windows.h>
#include "../../canvas/canvas.h"
#include "../../../renderer/app_renderer.h"

//*****************************************************************************
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class PlayerLife : public Canvas
{
public:
	//�R���X�g���N�^
	PlayerLife(RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);

	//�f�X�g���N�^
	virtual ~PlayerLife( );

	//�I��
	void Release( void );

	//�X�V
	void Update( void );

	//�o�b�t�@
	void MakeVertex(ID3D11Device* pDevice);

	//���C�t�����E����
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

	//���C�t�擾�E�ݒ�
	void SetLife(int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

private:
	bool							m_bLifeZero;

	int								m_nLife;

	VECTOR3							m_size;

	int								m_nCoolTime;
};

#endif