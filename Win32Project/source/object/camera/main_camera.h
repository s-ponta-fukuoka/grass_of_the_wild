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
class CameraPattern;
class Player;
class Enemy;
class EnemyManager;

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
	HRESULT Init(Player* pPlayer, EnemyManager* pEnemyManager);

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

	//�J�����X�e�[�g�ύX
	void ChangeCameraPattern(CameraPattern* pCameraPattern);

	//�ړ��ʎ擾�E�ݒ�
	void SetMove(VECTOR3 move) { m_move = move; }
	VECTOR3 GetMove(void) { return m_move; }

	//�v���C���[�擾
	Player* GetPlayer(void) { return m_pPlayer; }

	//�G�l�~�[�擾
	Enemy* GetEnemy(void) { return m_pEnemy; }

private:
	Player*				m_pPlayer;

	Enemy*				m_pEnemy;

	EnemyManager*		m_pEnemyManager;

	CameraPattern*	m_pCameraPattern;

	VECTOR3		m_move;
};

#endif