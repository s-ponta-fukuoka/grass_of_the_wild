//=============================================================================
//
// model.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "../renderer/app_renderer.h"

#include <vector>

class ModelManager;

//*********************************************************
//クラス
//*********************************************************
class Model
{
public:
	//コンストラクタ
	Model();
	
	//デストラクタ
	virtual ~Model();
	
	//ファイルネーム取得
	const char *GetFileName();

protected:
	const char* m_pFileName;
};

//--------------------------------------------------------
//Fbxモデル
//--------------------------------------------------------
class SkinMeshModel : public Model
{
public:
	
	struct ModelVertex
	{
		VECTOR3			position;
		VECTOR3			normal;
		VECTOR4			color;
		VECTOR2			tex;
		VECTOR4			boneIndex;
		VECTOR4			weight;
	};

	struct ModelConstant
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX cluster[256];
		XMMATRIX lclCluster;
		XMVECTOR light;
	};

	struct Cluster
	{
		XMMATRIX**	pMatrix;
	};

	struct Mesh
	{
		int					nNumCluster;
		int					nNumVertex;
		int					nNumPolygon;
		int					nNumPolygonVertex;
		int*				pIndexNumber;
		char*				pFileName;
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

	struct Anime
	{
		int nStartTime;
		int nEndTime;
	};

	//コンストラクタ
	SkinMeshModel(char* pFileName);

	//デストラクタ
	virtual ~SkinMeshModel();

	//読み込み
	void LoadFile(const  char* FilenName);

	//メッシュの取得
	Mesh *GetMesh(void);

	//アニメ―ションの取得
	Anime *GetAnime(void);

	//メッシュ数の取得
	int GetNumMesh(void);

	//アニメーション数の取得
	int GetNumAnime(void);

private:
	int					m_nNumMesh;

	int					m_nNumAnime;

	Anime*				m_pAnime;

	Mesh*				m_pMesh;
};

#endif