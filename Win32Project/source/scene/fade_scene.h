#ifndef _FADE_H_
#define _FADE_H_

/*==============================================================================
�y �C���N���[�h �z
==============================================================================*/
#include "scene.h"
#include "../renderer/app_renderer.h"
/*==============================================================================
�y �}�N����` �z
==============================================================================*/
#define LOAD_Fade_TEXTURE	(5)								// ���[�h����e�N�X�`���̐�
#define Fade_BUF			(64)							// �o�b�t�@
#define Fade_TEXTURENAME0	"data/TEXTURE/result000.png"	// �^�C�g�����̔w�i
#define Fade_TEXTURENAME1	"data/TEXTURE/result000.png"	// ���ʎ��̔w�i
#define Fade_TEXTURENAME2	"data/TEXTURE/result000.png"	// �Q�[�����̔w�i
#define Fade_TEXTURENAME3	"data/TEXTURE/result000.png"	// ���ʎ��̔w�i
#define Fade_TEXTURENAME4	"data/TEXTURE/result000.png"	// �^�C�g�����̔w�i
#define FADE_POS_X		(0)									// �|���S���̕\���ʒu�w
#define FADE_POS_Y		(0)									// �|���S���̕\���ʒu�x
#define FADE_WIDTH		(SCREEN_WIDTH)								// �|���S���̕�
#define FADE_HEIGHT		(SCREEN_HEIGHT)								// �|���S���̍���
#define COLOR_FADE_MIN	(0.0f)								// ���̍ŏ��l
#define COLOR_FADE_MAX	(1.0f)								// ���̍ő�l
#define FADE_RATE		(0.05f)								// �t�F�[�h���[�g
#define FADE_VERTEX		(4)

class NextScene;
class ShaderManager;
class RenderManager;
class TextureManager;

/*==============================================================================
�y �\���̂̒�` �z
==============================================================================*/
class Fade
{
public:
	// �t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE,		// �t�F�[�h�Ȃ�
		FADE_IN,		// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
		FADE_MAX
	} FADE_TYPE;

	Fade();
	virtual ~Fade();

	HRESULT Init(NextScene* pNextScene,
		RenderManager* pRenderManager,
		ShaderManager* pShaderManager,
		TextureManager* pTextureManager);
	void Uninit(void);
	void Update(void);
	void SetFade(FADE_TYPE fade, Scene* SceneNext);
	FADE_TYPE GetFade(void);

	float GetMax(float a, float b);
	float GetMin(float a, float b);

	void SetColorFade(AppRenderer::Vertex2D *pVtx, VECTOR4 color);

private:
	NextScene* m_pNextScene;

	ID3D11Buffer*			m_pVertexBuffer;

	VECTOR3				m_pos;			// �|���S�����W
	VECTOR3				m_size;			// �T�C�Y
	VECTOR3				m_normal;
	VECTOR2				m_tex;			// �e�N�X�`�����W
	VECTOR2				m_ofsettex;		// �e�N�X�`���I�t�Z�b�g
	VECTOR4				m_color;			// �|���S�����W
	bool m_bLoadTexture;

	FADE_TYPE m_fade;
	Scene *m_modeNext;

	char  m_setFade[LOAD_Fade_TEXTURE][Fade_BUF];
};


#endif