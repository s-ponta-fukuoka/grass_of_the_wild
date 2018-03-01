//=============================================================================
//
// model.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*********************************************************
//�w�b�_�t�@�C��
//*********************************************************
#include "../renderer/app_renderer.h"
#include <vector>

//*********************************************************
//�O���錾
//*********************************************************
class ModelManager;

//*********************************************************
//�N���X
//*********************************************************
class Model
{
public:
	//�R���X�g���N�^
	Model();
	
	//�f�X�g���N�^
	virtual ~Model();
	
	//�I��
	void Release(void);

	//�t�@�C���l�[���擾
	const char *GetFileName();

protected:
	const char* m_pFileName;
};

//--------------------------------------------------------
//Fbx���f��
//--------------------------------------------------------
class SkinMeshModel : public Model
{
public:
	
	//���f�����_�o�b�t�@�p�\����
	struct ModelVertex
	{
		VECTOR3			position;
		VECTOR3			normal;
		VECTOR4			color;
		VECTOR2			tex;
		VECTOR4			boneIndex;
		VECTOR4			weight;
	};

	//���f���萔�o�b�t�@�p�\����
	struct ModelConstant
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX cluster[100];
		XMMATRIX lclCluster;
		XMVECTOR light;
		XMMATRIX wit;
		VECTOR4 color;
	};

	//�N���X�^�\����
	struct Cluster
	{
		XMMATRIX**	pMatrix;
		XMMATRIX	pBlendMatrix;
	};

	//���b�V���\����
	struct Mesh
	{
		int					nNumCluster;
		int					nNumVertex;
		int					nNumPolygon;
		int					nNumPolygonVertex;
		int*				pIndexNumber;
		std::string			fileName;
		const char*				pFileName;
		VECTOR3*			pPosition;
		VECTOR3*			pNormal;
		VECTOR4*			pColor;
		VECTOR2*			pTex;
		VECTOR4*			pBoneIndex;
		VECTOR4*			pWeight;
		Cluster*			pCluster;
		VECTOR3				LclPos;
		VECTOR3				LclRot;
		VECTOR3				LclScl;
	};

	//�A�j���\����
	struct Anime
	{
		int nStartTime;
		int nEndTime;
	};

	//�R���X�g���N�^
	SkinMeshModel(char* pFileName);

	//�f�X�g���N�^
	virtual ~SkinMeshModel();

	//�I��
	void Release(void);

	//�ǂݍ���
	void LoadFile(const  char* FilenName);

	//���b�V���̎擾
	Mesh *GetMesh(void);

	//�A�j���\�V�����̎擾
	Anime *GetAnime(void);

	//���b�V�����̎擾
	int GetNumMesh(void);

	//�A�j���[�V�������̎擾
	int GetNumAnime(void);

private:
	int					m_nNumMesh;

	int					m_nNumAnime;

	Anime*				m_pAnime;

	Mesh*				m_pMesh;
};

#endif