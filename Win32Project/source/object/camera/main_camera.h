//=============================================================================
//
// camera.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MAIN_CAMERA_H_
#define _MAIN_CAMERA_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

class Player;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class MainCamera : public Camera
{
public:
	//�R���X�g���N�^
	MainCamera(VECTOR3 position, VECTOR3 positionAt, VECTOR3 vectorUp);

	//�f�X�g���N�^
	virtual ~MainCamera();

	//������
	HRESULT Init(Player* pPlayer);

	//�I��
	void Release(void);

	//�X�V
	void Update(void);

	//�J�����Z�b�g
	void SetCamera(void);

	//�萔�擾
	AppRenderer::Constant* GetConstant(void);

	//�|�W�V�����擾
	const VECTOR3& GetPos(void) const;

	//�����_�擾
	const VECTOR3& GetPosAt(void) const;

	//�J�����x�N�g���擾
	const XMVECTOR& GetVec(void) const;
private:
	Player*		m_pPlayer;

	VECTOR3		m_move;
};

#endif