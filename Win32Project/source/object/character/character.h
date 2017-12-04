//=============================================================================
//
// character.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "../object.h"
#include "../../renderer/renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Character : public Object
{
public:
	//�R���X�g���N�^
	Character();

	//�f�X�g���N�^
	virtual ~Character();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�A�j���[�V�����ύX
	void ChangeAnime(void);

protected:

	ID3D11Buffer*			m_pIndexBuffer;

	ID3D11Buffer*			m_pVertexBuffer;

	SkinMeshModel*			m_pModel;

	int*					m_pFrame;

	int*					m_pAnimeNumber;
};

#endif