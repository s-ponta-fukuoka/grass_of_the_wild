//=============================================================================
//
// xinput���� [xbox_controller.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

//*********************************************************
//�@�C���N���[�h
//*********************************************************
#include "input.h"
#include <XInput.h>

//*********************************************************
// �}�N����`
//*********************************************************
#define MAX_XINPUT (1)
#define XINPUT_STICK_RANGE_ENABLE		( 20000 )
//*********************************************************
// ���C�u�����̃����N
//*********************************************************
#pragma comment(lib,"Xinput.lib")

//*********************************************************
// �N���X��`
//*********************************************************
class XboxController : public Input
{
public:
	typedef enum
	{
		STICK_LX,
		STICK_LY,
		STICK_RX,
		STICK_RY,
	} STICK;

	XboxController();
	virtual ~XboxController();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BOOL GetButtonPress(int nIndex, int nButton);
	BOOL GetButtonTrigger(int nIndex, int nButton);

	int GetStick(int nIndex, STICK eStick);

	//�C���X�^���X����
	static void CreateInstance(void);

	//�C���X�^���X�擾
	static XboxController* GetInstance(void);

private:
	static XboxController* m_pXboxController;

	XINPUT_STATE			m_Input[MAX_XINPUT];
	XINPUT_STATE			m_InputTrigger[MAX_XINPUT];
	XINPUT_VIBRATION		m_Vibration[MAX_XINPUT];	
};

#endif