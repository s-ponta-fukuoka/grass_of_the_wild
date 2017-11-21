//=============================================================================
//
// directx�����_�����O���� [rendere.h]
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _APP_RENDERER_H_
#define _APP_RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include <d3d11.h>
#include <xnamath.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "../lib/vector.h"
#include "render_manager.h" 

//*****************************************************************************
// ���C�u�����t�@�C��
// [�\���v���p�e�B]->[�����J�[]->[����]->[�ǉ��̈ˑ��t�@�C��]�ɋL�q���Ă��\
//*****************************************************************************
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#ifdef _DEBUG
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Debug.lib" )
#else
#pragma comment(lib, "Effekseer.Release.lib" )
#pragma comment(lib, "EffekseerRendererDX11.Release.lib" )
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define D3D_PI ((FLOAT) 3.141592654f) 
#define D3DToRadian( degree ) ((degree) * (D3D_PI / 180.0f))
#define D3DToDegree( radian ) ((radian) * (180.0f / D3D_PI))

//*****************************************************************************
//�\���̒�`
//*****************************************************************************
class RenderManager;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class AppRenderer
{
public:
	//���_3D
	struct Vertex3D
	{
		VECTOR3 position;
		VECTOR3 normal;
		VECTOR4 color;
		VECTOR2 tex;
	};

	//���_2D
	struct Vertex2D
	{
		VECTOR3 position;
		VECTOR4 color;
		VECTOR2 tex;
	};

	//�萔
	struct Constant
	{
		XMMATRIX world;
		XMMATRIX view;			
		XMMATRIX projection;
		XMMATRIX lightView;		
		XMMATRIX lightProjection;
		XMVECTOR light;
	};

	//�f�X�g���N�^
	virtual ~AppRenderer( );

	//�C���X�^���X����
	static void CreateInstance(void);

	//�C���X�^���X�擾
	static AppRenderer* GetInstance(void);

	//������
	HRESULT Init( HWND hWnd );

	//�I��
	void Release( void );

	//�`��
	void Draw( RenderManager* pRenderManager );

	//�f�o�C�X�擾
	ID3D11Device* GetDevice(void) const;

	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContex(void) const;

	//�����_�[�^�[�Q�b�g�擾
	ID3D11RenderTargetView* GetRenderTargetView(void) const;

	//�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView(void) const;

private:
	//�R���X�g���N�^
	AppRenderer();

	// �f�o�C�X�ƃX���b�v�`�F�[���ݒ�
	void ConfigDeviceAndSwapChain(const HWND hWnd);

	//�X�e���V���^�[�Q�b�g�ݒ�
	void ConfigDepthStencilView(const DXGI_SWAP_CHAIN_DESC sd);

	//�����_�[�^�[�Q�b�g�r���[�ݒ�
	void ConfigRenderTargets(void);

	//�r���[�|�[�g�ݒ�
	void ConfigViewPort(void);

	static AppRenderer*				m_pAppRenderer;

	ID3D11Device*					m_pDevice;

	ID3D11DeviceContext*			m_pDeviceContext;

	IDXGISwapChain*					m_pSwapChain;

	ID3D11RenderTargetView*			m_pRenderTargetView;

	ID3D11DepthStencilView*			m_pDepthStencilView;

	D3D11_VIEWPORT					m_ViewPort;
};

#endif