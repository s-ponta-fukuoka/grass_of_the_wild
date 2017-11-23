//=============================================================================
//
// model.h
// Author : shota fukuoka
//
//=============================================================================
#include "model.h"
#//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//コンストラクタ
///////////////////////////////////////////////////////////////////////////////
Model::Model()
{
	;
}

SkinMeshModel::SkinMeshModel(char* pFileName)
	:m_nNumAnime(0)
	,m_nNumMesh(0)
	,m_pAnime(NULL)
	,m_pMesh(NULL)
{
	m_pFileName = pFileName;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
///////////////////////////////////////////////////////////////////////////////
Model::~Model()
{
	;
}

SkinMeshModel::~SkinMeshModel()
{
	;
}

///////////////////////////////////////////////////////////////////////////////
//終了
///////////////////////////////////////////////////////////////////////////////
void Model::Release(void)
{
	;
}

void SkinMeshModel::Release(void)
{
	for (int i = 0; i < m_nNumMesh; i++)
	{
		for (int j = 0; j < m_pMesh[i].nNumCluster; j++)
		{
			for (int k = 0; k < m_nNumAnime; k++)
			{
				delete[] m_pMesh[i].pCluster[j].pMatrix[k];
			}
			delete[] m_pMesh[i].pCluster[j].pMatrix;
		}
		delete[]  m_pMesh[i].pCluster;
	}
	delete[] m_pMesh;
}


///////////////////////////////////////////////////////////////////////////////
//ファイルネーム取得
///////////////////////////////////////////////////////////////////////////////
const char* Model::GetFileName()
{
	return m_pFileName;
}

///////////////////////////////////////////////////////////////////////////////
//読み込み
///////////////////////////////////////////////////////////////////////////////
void SkinMeshModel::LoadFile(const  char* FileName)
{
	FILE* pFile;

	pFile = fopen(FileName, "rb");

	fread(&m_nNumAnime, sizeof(int), 1, pFile);

	m_pAnime = new Anime[m_nNumAnime];
	for (int i = 0; i < m_nNumAnime; i++)
	{
		fread(&m_pAnime[i].nStartTime, sizeof(int), 1, pFile);
		fread(&m_pAnime[i].nEndTime, sizeof(int), 1, pFile);
	}

	fread(&m_nNumMesh, sizeof(int), 1, pFile);

	m_pMesh = new Mesh[m_nNumMesh];

	for (int i = 0; i < m_nNumMesh; i++)
	{
		fread(&m_pMesh[i].nNumVertex, sizeof(int), 1, pFile);

		m_pMesh[i].pPosition = new VECTOR3[m_pMesh[i].nNumVertex];
		m_pMesh[i].pColor = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pBoneIndex = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pWeight = new VECTOR4[m_pMesh[i].nNumVertex];

		for (int j = 0; j < m_pMesh[i].nNumVertex; j++)
		{
			VECTOR3 pos;
			fread(&pos, sizeof(VECTOR3), 1, pFile);
			m_pMesh[i].pPosition[j] = pos;

			fread(&m_pMesh[i].pColor[j], sizeof(VECTOR4), 1, pFile);

			fread(&m_pMesh[i].pBoneIndex[j], sizeof(VECTOR4), 1, pFile);

			fread(&m_pMesh[i].pWeight[j], sizeof(VECTOR4), 1, pFile);
		}

		fread(&m_pMesh[i].nNumPolygonVertex, sizeof(int), 1, pFile);

		m_pMesh[i].pNormal = new VECTOR3[m_pMesh[i].nNumPolygonVertex];
		m_pMesh[i].pTex = new VECTOR2[m_pMesh[i].nNumPolygonVertex];
		m_pMesh[i].pIndexNumber = new int[m_pMesh[i].nNumPolygonVertex];

		for (int j = 0; j < m_pMesh[i].nNumPolygonVertex; j++)
		{
			fread(&m_pMesh[i].pNormal[j], sizeof(VECTOR3), 1, pFile);

			fread(&m_pMesh[i].pTex[j], sizeof(VECTOR2), 1, pFile);

			fread(&m_pMesh[i].pIndexNumber[j], sizeof(int), 1, pFile);
		}

		fread(&m_pMesh[i].nNumCluster, sizeof(int), 1, pFile);

		m_pMesh[i].pCluster = new SkinMeshModel::Cluster[m_pMesh[i].nNumCluster];

		for (int j = 0; j < m_pMesh[i].nNumCluster; j++)
		{
			m_pMesh[i].pCluster[j].pMatrix = new XMMATRIX*[m_nNumAnime];
			for (int k = 0; k < m_nNumAnime; k++)
			{
				m_pMesh[i].pCluster[j].pMatrix[k] = new XMMATRIX[m_pAnime[k].nEndTime];

				for (int l = m_pAnime[k].nStartTime; l < m_pAnime[k].nEndTime; l++)
				{
					fread(&m_pMesh[i].pCluster[j].pMatrix[k][l], sizeof(XMMATRIX), 1, pFile);
				}
			}
		}

		fread(&m_pMesh[i].LclPos, sizeof(VECTOR3), 1, pFile);
		fread(&m_pMesh[i].LclRot, sizeof(VECTOR3), 1, pFile);
		fread(&m_pMesh[i].LclScl, sizeof(VECTOR3), 1, pFile);

		int size;

		fread(&size, sizeof(int), 1, pFile);

		char FileName[256];

		fread(&FileName, sizeof(char), size, pFile);
		
		//std::string name = FileName;
		//
		//name = name.substr(0, size);
		//
		m_pMesh[i].pFileName = FileName;
	}

	fclose(pFile);
}

///////////////////////////////////////////////////////////////////////////////
//メッシュの取得
///////////////////////////////////////////////////////////////////////////////
SkinMeshModel::Mesh *SkinMeshModel::GetMesh(void)
{
	return m_pMesh;
}

///////////////////////////////////////////////////////////////////////////////
//アニメ―ションの取得
///////////////////////////////////////////////////////////////////////////////
SkinMeshModel::Anime *SkinMeshModel::GetAnime(void)
{
	return m_pAnime;
}

///////////////////////////////////////////////////////////////////////////////
//メッシュ数の取得
///////////////////////////////////////////////////////////////////////////////
int SkinMeshModel::GetNumMesh(void)
{
	return m_nNumMesh;
}

///////////////////////////////////////////////////////////////////////////////
//アニメーション数の取得
///////////////////////////////////////////////////////////////////////////////
int SkinMeshModel::GetNumAnime(void)
{
	return m_nNumAnime;
}