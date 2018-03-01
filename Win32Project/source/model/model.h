//=============================================================================
//
// model.h
// Author : SHOTA FUKUOKA
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*********************************************************
//ヘッダファイル
//*********************************************************
#include "../renderer/app_renderer.h"
#include <vector>

//*********************************************************
//前方宣言
//*********************************************************
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
	
	//終了
	void Release(void);

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
	
	//モデル頂点バッファ用構造体
	struct ModelVertex
	{
		VECTOR3			position;
		VECTOR3			normal;
		VECTOR4			color;
		VECTOR2			tex;
		VECTOR4			boneIndex;
		VECTOR4			weight;
	};

	//モデル定数バッファ用構造体
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

	//クラスタ構造体
	struct Cluster
	{
		XMMATRIX**	pMatrix;
		XMMATRIX	pBlendMatrix;
	};

	//メッシュ構造体
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

	//アニメ構造体
	struct Anime
	{
		int nStartTime;
		int nEndTime;
	};

	//コンストラクタ
	SkinMeshModel(char* pFileName);

	//デストラクタ
	virtual ~SkinMeshModel();

	//終了
	void Release(void);

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