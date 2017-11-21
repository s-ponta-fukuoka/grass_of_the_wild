//=============================================================================
//
// camera.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "../../renderer/app_renderer.h"
#include "../object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Camera : public Object
{
public:
	//�R���X�g���N�^
	Camera();

	//�f�X�g���N�^
	virtual ~Camera();

	//������
	HRESULT Init(void);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

protected:
	AppRenderer::Constant*				m_pConstant;

	XMMATRIX							m_mtxView;

	XMMATRIX							m_mtxProjection;

	VECTOR3								m_positionAt;

	VECTOR3								m_vectorUp;

	XMVECTOR							m_vector;
};

#endif