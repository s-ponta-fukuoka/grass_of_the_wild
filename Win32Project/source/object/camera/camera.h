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

	//�����_�ݒ�E�擾
	void SetPositionAt(VECTOR3 posAt) { m_positionAt = posAt; }
	VECTOR3 GetPositionAt(void) { return m_positionAt; }

	//������ݒ�E�擾
	void SetVecUp(VECTOR3 vecUp) { m_vectorUp = vecUp; }
	VECTOR3 GetVecUp(void) { return m_vectorUp; }

	//�����ݒ�E�擾
	void SetVec(XMVECTOR vec) { m_vector = vec; }
	XMVECTOR GetVec(void) { return m_vector; }

protected:
	AppRenderer::Constant*				m_pConstant;

	XMMATRIX							m_mtxView;

	XMMATRIX							m_mtxProjection;

	VECTOR3								m_positionAt;

	VECTOR3								m_vectorUp;

	XMVECTOR							m_vector;
};

#endif