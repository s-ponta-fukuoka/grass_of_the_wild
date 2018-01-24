//=============================================================================
//
// terrain.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

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
class Terrain : public Object
{
public:
	//�R���X�g���N�^
	Terrain();

	//�f�X�g���N�^
	virtual ~Terrain();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�A�j���[�V�����ύX
	void ChangeAnime(void);

	int* GetFrame(void) { return m_pFrame; }
	int* GetAnimeNumber(void) { return m_pAnimeNumber; }
	SkinMeshModel* GetSkinMeshModel(void) { return m_pModel; }

protected:

	ID3D11Buffer*			m_pIndexBuffer;

	ID3D11Buffer*			m_pVertexBuffer;

	SkinMeshModel*			m_pModel;

	int*					m_pFrame;

	int*					m_pAnimeNumber;
};

#endif