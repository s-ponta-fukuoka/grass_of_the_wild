//=============================================================================
//
// camera_pattern.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _CAMERA_PATTERN_H_
#define _CAMERA_PATTERN_H_

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class MainCamera;

//*****************************************************************************
// クラス定義
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