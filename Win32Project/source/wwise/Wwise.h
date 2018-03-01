
#ifndef _WWISE_H_
#define _WWISE_H_

#include <windows.h>
#include <assert.h>
#include <unordered_map>

// Release���ɒ�`������
#ifndef _DEBUG
#define AK_OPTIMIZED
#endif

// Wwise(�T�E���h�G���W��)���g�p���邽�߂ɕK�v
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// �T�E���h�G���W��
#include <AK/IBytes.h>


// #include <AK/Plugin/AkCompressorFXFactory.h>			// �v���O�C��

#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>	// �^�C�v

#include "Wwise_IDs.h"									// �C�x���g��ID

using namespace AK;
using namespace AK::SoundEngine;

// ======== ======== ======== ======== ======== ======== ======== ========
// �\����
// -------- -------- -------- -------- -------- -------- -------- --------
// �Q�[���I�u�W�F�N�g
struct WWISE_GAMEOBJ
{
	AkVector pos, rot;		// ���W, ��]�p
	AkVector vel;			// ���x(�h�b�v���[���ʂŕK�v�H)
};

// �O���錾
class CAkFilePackageLowLevelIOBlocking;

// ======== ======== ======== ======== ======== ======== ======== ========
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
// 
// �g�����I
// �@ CreateNoListenerGameObj()�ŃQ�[���I�u�W�F�N�g���쐬�I
// �A SetEvent()�ŃC�x���g���Đ��I
// �B Stop()�ŃC�x���g�̒�~�I
//
// �E Init(), Uninit(), Update() ��Manager.cpp�ɏ����ĉ�����
// �E ���X�i�[(m_pMainListener)�́A�v���C���[���̈ʒu������ɍ��킹�Ė��t���[���ݒ肵�����ĉ�����
// 
class Wwise
{
public:
	Wwise(){}
	~Wwise(){}

	//�C���X�^���X����
	static void CreateInstance(void);

	//�C���X�^���X�擾
	static Wwise* GetInstance(void);

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V

	// �Q�[���I�u�W�F�N�g�̐���
	WWISE_GAMEOBJ *CreateGameObj(AkVector pos, AkVector rot, char *name);				// Listener�t��
	WWISE_GAMEOBJ *CreateNoListenerGameObj(AkVector pos, AkVector rot, char *name);	// Listener����
	void DeleteGameObj(std::string gameObjectName);

	// �C�x���g���M
	void MainListenerGameObjEvent(int eventID);				// ���C�����X�i�[(�v���C���[)�ɃC�x���g�𑗐M
	void SetEvent(int eventID, AkVector *loc = 0);		// ���[���h���W�̎w��ʒu��ID�ŃC�x���g�𑗐M
	void SetEvent(char *name, AkVector *loc = 0);		// ���[���h���W�̎w��ʒu�ɖ��O�ŃC�x���g�𑗐M
	void SetEvent(int eventID, std::string GameObjectName);	// �w��Q�[���I�u�W�F�N�g��ID�ŃC�x���g�𑗐M
	void SetEvent(char *name, std::string GameObjectName);	// �w��Q�[���I�u�W�F�N�g�ɖ��O�ŃC�x���g�𑗐M

	// �Q�[���I�u�W�F�N�g�̍Đ����~
	void Stop(std::string name);
	void StopMainListener();

	// �C�x���g�̏�Ԃ�ύX
	void SetState(int stateGroup, int stateID);			// �X�e�[�g�̐ݒ�
	void SetGameParam(int rtpcID, float value);			// �Q�[���p�����[�^�[(RTPC)�̐ݒ�

	// �}�e���A��
	int GetMaterialId(char *name);						// ���O����}�e���A��ID�̎擾
//	void SetMaterial(void *gameObject, int materialid);	// �Q�[���I�u�W�F�N�g�Ƀ}�e���A��ID��ݒ�

	// ���C�����X�i�[�n
	void SetPos(const AkVector &pos) { m_pMainListener->pos = pos; }		// ���W
	void SetRot(const AkVector &rot) { m_pMainListener->rot = rot; }		// ��]
	void SetVel(const AkVector &vel) { m_pMainListener->vel = vel; }		// ���x

	// �e�Q�[���I�u�W�F�N�g
	void SetPos(const std::string &name, const AkVector &pos) {			// ���W
		if (m_umapGameObj.count(name) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[name]->pos = pos;
	}
	void SetRot(const std::string &name, const AkVector &rot) {			// ��]
		if (m_umapGameObj.count(name) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[name]->rot = rot;
	}
	void SetVel(const std::string &name, const AkVector &vel) {			// ���x
		if (m_umapGameObj.count(name) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[name]->vel = vel;
	}

	// �L���v�`�����X�g�փG���[���b�Z�[�W�𑗂�
	void PostMessage(char *szMessage);

	// D3DXVECTOR3��AkVector�̕ϊ�
	static void VecToAkVector(const AkVector &vec, AkVector &vector);

private:
	static Wwise* m_pWwise;

	// �Q�[���I�u�W�F�N�g�̓o�^(���ڂ͌Ăяo���Ȃ�)
	void registPtr(void *gameObject, char *name = NULL);
	void unregistPtr(void *gameObject);

	// �჌�x��I/O
	static CAkFilePackageLowLevelIOBlocking m_lowLevelIO;			

	// �Q�[���I�u�W�F�N�g
	std::unordered_map<std::string, WWISE_GAMEOBJ*> m_umapGameObj;	// �Q�[���I�u�W�F�N�g
	WWISE_GAMEOBJ *m_pMainListener;									// ���C�����X�i�[�ւ̃|�C���^

	// �_�~�[�R�[���o�b�N
	static void dummyCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<int> dummies;
	static CAkLock dummiesLock;

	/*	void soundvol(int vol);
	void musicvol(int vol);
	void voicevol(int vol);*/				
};

#endif
