//=============================================================================
//
// camera_pattern.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CAMERA_PATTERN_H_
#define _CAMERA_PATTERN_H_

//*****************************************************************************
// �O���錾
//*****************************************************************************
class MainCamera;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CameraPattern
{
public:
	virtual ~CameraPattern() {}

	virtual void Update(MainCamera* pMainCamera) = 0;
};

class CameraPatternNone : public CameraPattern
{
public:
	virtual ~CameraPatternNone() {}

	virtual void Update(MainCamera* pMainCamera) override {}
};

#endif