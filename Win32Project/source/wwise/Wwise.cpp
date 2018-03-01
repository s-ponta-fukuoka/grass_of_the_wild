
// -------- -------- -------- -------- -------- -------- -------- --------
// �C���N���[�h
// -------- -------- -------- -------- -------- -------- -------- --------

#include "../app/app.h"
#include "../renderer/app_renderer.h"
#include "Wwise.h"

// �������}�l�[�W���ɕK�v
#include <AK/SoundEngine/Common/AkMemoryMgr.h>				// �������}�l�[�W��
#include <AK/SoundEngine/Common/AkModule.h>					// �f�t�H���g������, �X�g���[���}�l�[�W��

// �X�g���[�~���O�Đ��ɕK�v
#include <AK/SoundEngine/Common/IAkStreamMgr.h>				// �X�g���[���}�l�[�W��
#include <AK/Tools/Common/AkPlatformFuncs.h>				// �X���b�h
#include "../Wwise_Win32/AkFilePackageLowLevelIOBlocking.h"	// �჌�x��I/O

// �~���[�W�b�N�G���W���ɕK�v
#include <AK/MusicEngine/Common/AkMusicEngine.h>			// �~���[�W�b�N�G���W��

// Wwise�ƃQ�[���̊Ԃ̒ʐM�ɕK�v - �����[�X�o�[�W�����ł͕s�v
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

// ======== ======== ======== ======== ======== ======== ======== ========
//	�������t�b�N
// -------- -------- -------- -------- -------- -------- -------- --------
// �J�X�^��alloc/free�֐��B
// ������AkMemoryMgr.h�� "extern"�Ƃ��Đ錾����Ă���
// -------- -------- -------- -------- -------- -------- -------- --------
namespace AK
{
#ifdef WIN32
	void *AllocHook(size_t in_size) { return malloc(in_size); }

	void FreeHook(void *in_ptr) { free(in_ptr); }

	void *VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(void *in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

#ifdef AK_MOTION
#include <AK/MotionEngine/Common/AkMotionEngine.h>	// Motion Engine (required only for playback of Motion objects)
#endif

static const AkGameObjectID LISTENER_ID = 10001;

// #include <AK/Plugin/AllPluginsRegistrationHelpers.h>

// ======== ======== ======== ======== ======== ======== ======== ========
// ���O�ȗ�
using namespace std;

// ======== ======== ======== ======== ======== ======== ======== ========
// �}�N��
#define CHECK_SOUND_ENGINE if( !SoundEngine::IsInitialized() ) return;
#define CHECK_SOUND_ENGINE0 if( !SoundEngine::IsInitialized() ) return 0;
#define CHECK_NULLPTR(p) { if(p == nullptr) return; }

// ======== ======== ======== ======== ======== ======== ======== ========
// �ÓI�����o
CAkFilePackageLowLevelIOBlocking Wwise::m_lowLevelIO;
std::list<int> Wwise::dummies;
CAkLock Wwise::dummiesLock;

Wwise *Wwise::m_pWwise = NULL;

// ======== ======== ======== ======== ======== ======== ======== ========
// ������
void Wwise::Init()
{
	// -------- -------- -------- --------
	// �������}�l�[�W��
	//
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return;
	}

	// -------- -------- -------- --------
	// �X�g���[�~���O�}�l�[�W��
	//
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Stream Manager");
		return;
	}

	// -------- -------- -------- --------
	// �X�g���[�~���O�f�o�C�X
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (m_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
		return;
	}

	// -------- -------- -------- --------
	// �T�E���h�G���W��
	//
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// Setting pool sizes for this game. Here, allow for user content; every game should determine its own optimal values.
	initSettings.uDefaultPoolSize = 2 * 1024 * 1024;
	platformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return;
	}

	// -------- -------- -------- --------
	// �~���[�W�b�N�G���W��
	//
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return;
	}

	/*
	// -------- -------- -------- --------
	// �v���O�C��
	//
	/// Note: This a convenience method for rapid prototyping. 
	/// To reduce executable code size register/link only the plug-ins required by your game 
	if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
	{
		AKASSERT(!"Error while registering plug-ins");
		return;
	}
	*/

	// -------- -------- -------- --------
	// �ʐM�̏�����
	//
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return;
	}
#endif // AK_OPTIMIZED

	// -------- -------- -------- --------
	// �჌�x��I/O
	//
	// �o���N�X�N���v�g�̃p�X
	m_lowLevelIO.SetBasePath(AKTEXT("resource/sound/grass_of_the_wild/GeneratedSoundBanks/Windows/"));

	// �Q�[���̌��݂̌���̎w��
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	// -------- -------- -------- --------
	// �o���N�̓ǂݍ���
	//
	AkBankID bankID;
	AKRESULT retValue;

	// �������o���N
	retValue = SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	// ���C���o���N
	retValue = SoundEngine::LoadBank("main.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	/*
	// �{�����[���̏�����
	musicvol(255);
	soundvol(255);
	voicevol(255);
	*/

	/*
	// Wwise���[�V����
#ifdef AK_MOTION
	MotionEngine::AddPlayerMotionDevice(0, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE, nullptr);
	MotionEngine::SetPlayerListener(0, 0);
	SoundEngine::SetListenerPipeline(0, true, true);
#endif
	*/
	AkVector pos, rot;
	pos.X = 0.0f;
	pos.Y = 0.0f;
	pos.Z = 0.0f;
	rot.X = 0.0f;
	rot.Y = 0.0f;
	rot.Z = 0.0f;

	// ���C�����X�i�[�̐ݒ�
	m_pMainListener = Wwise::CreateGameObj(pos, rot, "MainListener");	// �Q�[���I�u�W�F�N�g�Ƃ��ēo�^
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �I��
void Wwise::Uninit()
{
	// �Q�[���I�u�W�F�N�g�̏���
	for (auto gameObject : m_umapGameObj)
	{
		if (gameObject.second != nullptr)
		{
			Wwise::unregistPtr(gameObject.second);	// �Q�[���I�u�W�F�N�g�̓o�^������
			delete gameObject.second;
			gameObject.second = nullptr;
		}
	}
	m_umapGameObj.clear();

	// �ʐM
#ifndef AK_OPTIMIZED
	Comm::Term();
#endif // AK_OPTIMIZED

	// �~���[�W�b�N�G���W��
	MusicEngine::Term();

	// �T�E���h�G���W��
	SoundEngine::Term();

	// �჌�x�����o��
	m_lowLevelIO.Term();
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	// �������}�l�[�W��
	MemoryMgr::Term();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�V
void Wwise::Update()
{
	CHECK_SOUND_ENGINE;

	// ======== ======== ======== ========
	// ���X�i�[�̐ݒ�
	AkSoundPosition MainListener;

	float fRadPitch = m_pMainListener->rot.Z;
	float fCosPitch = cos(fRadPitch);
	float fSinPitch = sin(fRadPitch);

	float fRadYaw = m_pMainListener->rot.Y - D3D_PI * 0.5f;
	float fCosYaw = cos(fRadYaw);
	float fSinYaw = sin(fRadYaw);

	// �O�����x�N�g��
	AkVector front;
	front.X = fCosYaw * fCosPitch;
	front.Y = fSinYaw * fCosPitch;
	front.Z = fSinPitch;

	// ������x�N�g��
	AkVector top;
	top.X = -fCosYaw * fSinPitch;
	top.Y = -fSinYaw * fSinPitch;
	top.Z = fCosPitch;

	// �O�����A������x�N�g���̐ݒ�
	MainListener.SetOrientation(front, top);

	// ���W�ݒ�
	AkVector position;
	VecToAkVector(m_pMainListener->pos, position);
	MainListener.SetPosition(position);

	// ���X�i�[�̐ݒ�
	SoundEngine::SetPosition(LISTENER_ID, MainListener);

	// ======== ======== ======== ========
	// �e�Q�[���I�u�W�F�N�g�̐ݒ�
	AkSoundPosition soundPos;
	for (auto gameObject : m_umapGameObj)
	{
		fRadPitch = gameObject.second->rot.Z;
		fCosPitch = cos(fRadPitch);
		fRadYaw = gameObject.second->rot.Y - D3D_PI * 0.5f;

		// �O�����x�N�g��
		front.X = (float)(cos(fRadYaw) * fCosPitch);
		front.Y = (float)(sin(fRadYaw) * fCosPitch);
		front.Z = (float)(sin(fRadPitch));

		// ������x�N�g��
		top = { 0, 1.f, 0 };
		soundPos.SetOrientation(front, top);

		// ���W�ݒ�
		VecToAkVector(gameObject.second->pos, position);
		soundPos.SetPosition(position);

		// �Q�[���I�u�W�F�N�g�̐ݒ�
		SoundEngine::SetPosition((AkGameObjectID)gameObject.second, soundPos);
	}

	// �ݒ肳�ꂽ�C�x���g�̎��s
	SoundEngine::RenderAudio();
};

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g��o�^
WWISE_GAMEOBJ *Wwise::CreateGameObj(AkVector pos, AkVector rot, char *pName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// ���W
	pGameObject->rot = rot;							// �p�x
	pGameObject->vel.X = 0.0f;	// ���x(�h�b�v���[�p)
	pGameObject->vel.Y = 0.0f;	// ���x(�h�b�v���[�p)
	pGameObject->vel.Z = 0.0f;	// ���x(�h�b�v���[�p)
	
	// �G���W���֓o�^
	Wwise::registPtr(pGameObject, pName);

	// ���X�g�֒ǉ�
	if (m_umapGameObj.count(pName) >= 1)
	{
		delete m_umapGameObj[pName];
		m_umapGameObj[pName] = nullptr;
	}
	m_umapGameObj[pName] = pGameObject;
	AK::SoundEngine::RegisterGameObj(LISTENER_ID, "Listener (Default)");
	AK::SoundEngine::SetDefaultListeners(&LISTENER_ID, 1);

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���X�i�[�����Q�[���I�u�W�F�N�g��o�^
WWISE_GAMEOBJ *Wwise::CreateNoListenerGameObj(AkVector pos, AkVector rot, char *pName)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// ���W
	pGameObject->rot = rot;							// �p�x
	pGameObject->vel.X = 0.0f;	// ���x(�h�b�v���[�p)
	pGameObject->vel.Y = 0.0f;	// ���x(�h�b�v���[�p)
	pGameObject->vel.Z = 0.0f;	// ���x(�h�b�v���[�p)

	// �G���W���֓o�^
	Wwise::registPtr(pGameObject, pName);

	// ���X�g�֒ǉ�
	if (m_umapGameObj.count(pName) >= 1)
	{
		delete m_umapGameObj[pName];
		m_umapGameObj[pName] = nullptr;
	}
	m_umapGameObj[pName] = pGameObject;

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̏���
void Wwise::DeleteGameObj(std::string gameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(gameObjectName) == 0) { return; }

	// �w��Q�[���I�u�W�F�N�g�̏���
	Wwise::unregistPtr(m_umapGameObj[gameObjectName]);
	delete m_umapGameObj[gameObjectName];
	m_umapGameObj[gameObjectName] = nullptr;
	m_umapGameObj.erase(gameObjectName);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̓o�^(�G���W���ɓo�^)
void Wwise::registPtr(void *gameObject, char *pName)
{
	CHECK_SOUND_ENGINE;

	if (pName == nullptr)
	{
		static int cEntity = 0;
		char name[256];
		sprintf(name, "Entity #%i", (int) ++cEntity);

		SoundEngine::RegisterGameObj((AkGameObjectID)gameObject, name);
	}
	else
	{
		SoundEngine::RegisterGameObj((AkGameObjectID)gameObject, pName);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�̏���(�G���W������폜)
void Wwise::unregistPtr(void *gameObject)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::UnregisterGameObj((AkGameObjectID)gameObject);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 
// �C�x���g�̐ݒ�
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// ���C�����X�i�[�ɃC�x���g�𑗐M
void Wwise::MainListenerGameObjEvent(int eventID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::PostEvent(eventID, (AkGameObjectID)m_pMainListener);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ID, ���[���h���W (���[���h���W���ݒ�̏ꍇ�͎����I�Ƀv���C���[�ւ̃C�x���g�ݒ�ƂȂ�)
void Wwise::SetEvent(int eventID, AkVector *loc)
{
	CHECK_SOUND_ENGINE;

	if (loc == nullptr)
		SoundEngine::PostEvent(eventID, (AkGameObjectID)m_pMainListener);
	else
	{
		// �_�~�[���g�p
		dummiesLock.Lock();			// ���b�N
		dummies.push_back(0);			// �_�~�[��o�^
		int &dummy = dummies.back();	// �擾
		dummiesLock.Unlock();			// �A�����b�N

		// �_�~�[���C�x���g�֓o�^
		SoundEngine::RegisterGameObj((AkGameObjectID)&dummy, "PlayAtLocation");

		// �_�~�[�̎p���ƍ��W��ݒ�
		AkVector front = { 0, 0, 1.f };
		AkVector top = { 0, 1.f, 0 };
		AkVector position;
		VecToAkVector(*loc, position);

		// ���W���_�~�[�ɓo�^
		AkSoundPosition soundPos;
		soundPos.Set(position, front, top);
		SoundEngine::SetPosition((AkGameObjectID)&dummy, soundPos);

		// �C�x���g�̑��M
		AkPlayingID id = SoundEngine::PostEvent(eventID, (AkGameObjectID)&dummy, AK_EndOfEvent, dummyCallback);
		if (id != AK_INVALID_PLAYING_ID)
		{
			// ID�������łȂ���Γo�^
			dummy = id;
		}
		else
		{
			// ID�������ł���΁A�R���e�i����_�~�[�����o��
			SoundEngine::UnregisterGameObj((AkGameObjectID)&dummy);
			dummiesLock.Lock();		// ���b�N
			dummies.pop_back();			// ���o��
			dummiesLock.Unlock();		// �A�����b�N
		}
	}
};

// ======== ======== ======== ======== ======== ======== ======== ========
// �C�x���g��, ���[���h���W
void Wwise::SetEvent(char *pName, AkVector *loc)
{
	CHECK_SOUND_ENGINE;

	AkUniqueID eventID = SoundEngine::GetIDFromString(pName);
	if (eventID != AK_INVALID_UNIQUE_ID)
	{
		Wwise::SetEvent((int)eventID, loc);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ID, �Q�[���I�u�W�F�N�g��
void Wwise::SetEvent(int eventID, std::string GameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(GameObjectName) == 0) { return; }

	SoundEngine::PostEvent(eventID, (AkGameObjectID)m_umapGameObj[GameObjectName]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �C�x���g��, �Q�[���I�u�W�F�N�g��
void Wwise::SetEvent(char *pName, std::string GameObjectName)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(GameObjectName) == 0) { return; }

	SoundEngine::PostEvent(pName, (AkGameObjectID)m_umapGameObj[GameObjectName]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �Đ����C�x���g�̒�~
void Wwise::Stop(std::string name)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(name) == 0) { assert(!"�����̖��O���s��"); }

	SoundEngine::StopAll((AkGameObjectID)m_umapGameObj[name]);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���C�����X�i�[�̍Đ����C�x���g�̒�~
void Wwise::StopMainListener()
{
	CHECK_SOUND_ENGINE;
	if (m_pMainListener == nullptr) { return; }

	SoundEngine::StopAll((AkGameObjectID)m_pMainListener);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �X�e�[�g�̐ݒ�
void Wwise::SetState(int stateGroup, int stateID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetState(stateGroup, stateID);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// RTPC�̐ݒ�
void Wwise::SetGameParam(int rtpcID, float value)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(rtpcID, value);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// wwise�L���v�`���[���X�g�փG���[���b�Z�[�W�𑗂�
void Wwise::PostMessage(char *szMessage)
{
	Monitor::PostString(szMessage, Monitor::ErrorLevel_Message);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �_�~�[�R�[���o�b�N
void Wwise::dummyCallback(AkCallbackType type, AkCallbackInfo *pCallbackInfo)
{
	UNREFERENCED_PARAMETER(type);
	AkEventCallbackInfo *pInfo = (AkEventCallbackInfo *)pCallbackInfo;
	AkGameObjectID id = pInfo->gameObjID;

	dummiesLock.Lock();		// ���b�N

	// ID�̗L�������m���߂�
	auto it = std::find(dummies.begin(), dummies.end(), static_cast<int>(pInfo->playingID));
	if (it != dummies.end())
	{
		dummies.erase(it);
		AK::SoundEngine::UnregisterGameObj(id);
	}
	dummiesLock.Unlock();	// �A�����b�N
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ���O����}�e���A��ID�̎擾
int Wwise::GetMaterialId(char *pName)
{
	CHECK_SOUND_ENGINE0;

	return SoundEngine::GetIDFromString(pName);
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// �Q�[���I�u�W�F�N�g�Ƀ}�e���A��ID��ݒ�
void Wwise::SetMaterial(void *gameObject, int materialid)
{
CHECK_SOUND_ENGINE;

SoundEngine::SetSwitch(SWITCHES::MATERIAL::GROUP, materialid, (AkGameObjectID)gameObject);
}
*/

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectX��Vec3��Wwise��Vec3�ɕϊ�
void Wwise::VecToAkVector(const AkVector & vec, AkVector & vector)
{
	vector.X = vec.X;
	vector.Y = vec.Y;
	vector.Z = vec.Z;
}

///////////////////////////////////////////////////////////////////////////////
//�C���X�^���X����
///////////////////////////////////////////////////////////////////////////////
void Wwise::CreateInstance(void)
{
	if (m_pWwise != NULL) { return; }
	m_pWwise = new Wwise();
}

///////////////////////////////////////////////////////////////////////////////
//�C���X�^���X�擾
///////////////////////////////////////////////////////////////////////////////
Wwise* Wwise::GetInstance(void)
{
	return m_pWwise;
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// 
// �X�N���v�g�R�}���h
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// ======== ======== ======== ======== ======== ======== ======== ========
// �{�����[���ݒ� (0-255)
void Wwise::musicvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::MUSICVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// SFX(�G�t�F�N�g)�{�����[���ݒ� (0-255)
void Wwise::soundvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::SFXVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// �{�C�X�{�����[���ݒ� (0-255)
void Wwise::voicevol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::VOICEVOLUME, (AkRtpcValue)vol);
}
*/
