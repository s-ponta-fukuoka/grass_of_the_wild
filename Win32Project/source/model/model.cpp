//=============================================================================
//
// model.cpp
// Author : shota fukuoka
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "model.h"

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
	if (m_pAnime != NULL)
	{
		delete[] m_pAnime;
		m_pAnime = NULL;
	}

	if (m_pMesh != NULL)
	{
		for (int i = 0; i < m_nNumMesh; i++)
		{
			if (m_pMesh[i].pCluster != NULL)
			{
				for (int j = 0; j < m_pMesh[i].nNumCluster; j++)
				{
					for (int k = 0; k < m_nNumAnime; k++)
					{
						delete[] m_pMesh[i].pCluster[j].pMatrix[k];
						m_pMesh[i].pCluster[j].pMatrix[k] = NULL;
					}
					delete[] m_pMesh[i].pCluster[j].pMatrix;
					m_pMesh[i].pCluster[j].pMatrix = NULL;
				}
				delete[] m_pMesh[i].pCluster;
				m_pMesh[i].pCluster = NULL;
			}
		}
		delete[] m_pMesh;
		m_pMesh = NULL;
	}
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
	FILE *pFile;

	pFile = fopen(FileName, "r");

	char buf[2];

	fscanf(pFile, "%d", &m_nNumAnime);
	m_pAnime = new Anime[m_nNumAnime];
	for (int i = 0; i < m_nNumAnime; i++)
	{
		fscanf(pFile, "%c", &buf);

		//スタート
		fscanf(pFile, "%d", &m_pAnime[i].nStartTime);

		fscanf(pFile, "%c", &buf);

		//エンド
		fscanf(pFile, "%d", &m_pAnime[i].nEndTime);
	}

	fscanf(pFile, "%2c", &buf);

	//メッシュの数
	fscanf(pFile, "%d", &m_nNumMesh);
	m_pMesh = new Mesh[m_nNumMesh];

	fscanf(pFile, "%2c", &buf);
	for (int i = 0; i < m_nNumMesh; i++)
	{
		fscanf(pFile, "%d", &m_pMesh[i].nNumVertex);//ポリゴン数
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pPosition = new VECTOR3[m_pMesh[i].nNumVertex];
		m_pMesh[i].pColor = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pBoneIndex = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pWeight = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pNormal = new VECTOR3[m_pMesh[i].nNumVertex];
		m_pMesh[i].pTex = new VECTOR2[m_pMesh[i].nNumVertex];
		for (int j = 0; j < m_pMesh[i].nNumVertex; j++)
		{
			//座標
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].z);
			fscanf(pFile, "%c", &buf);

			//法線
			fscanf(pFile, "%f", &m_pMesh[i].pNormal[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pNormal[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pNormal[j].z);
			fscanf(pFile, "%c", &buf);

			//UV
			fscanf(pFile, "%f", &m_pMesh[i].pTex[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pTex[j].y);
			fscanf(pFile, "%c", &buf);

			//色
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].w);
			fscanf(pFile, "%c", &buf);

			//クラスターインデックス
			fscanf(pFile, "%f", &m_pMesh[i].pBoneIndex[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pBoneIndex[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pBoneIndex[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pBoneIndex[j].w);
			fscanf(pFile, "%c", &buf);

			//ウェイト
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].w);
			fscanf(pFile, "%c", &buf);
		}

		fscanf(pFile, "%d", &m_pMesh[i].nNumPolygonVertex);//頂点数
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pIndexNumber = new int[m_pMesh[i].nNumPolygonVertex];
		for (int j = 0; j < m_pMesh[i].nNumPolygonVertex; j++)
		{
			//インデックスナンバー
			fscanf(pFile, "%d", &m_pMesh[i].pIndexNumber[j]);
			fscanf(pFile, "%c", &buf);
		}

		if (m_nNumAnime != 0)
		{
			fscanf(pFile, "%d", &m_pMesh[i].nNumCluster);//クラスタ数
			fscanf(pFile, "%c", &buf);
			m_pMesh[i].pCluster = new Cluster[m_pMesh[i].nNumCluster];

			for (int j = 0; j < m_pMesh[i].nNumCluster; j++)
			{
				m_pMesh[i].pCluster[j].pMatrix = new  XMMATRIX*[m_nNumAnime];
				for (int k = 0; k < m_nNumAnime; k++)
				{
					m_pMesh[i].pCluster[j].pMatrix[k] = new XMMATRIX[m_pAnime[k].nEndTime];
					for (int l = m_pAnime[k].nStartTime; l < m_pAnime[k].nEndTime; l++)
					{
						//クラスター行列
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._11);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._12);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._13);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._14);
						fscanf(pFile, "%c", &buf);

						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._21);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._22);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._23);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._24);
						fscanf(pFile, "%c", &buf);

						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._31);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._32);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._33);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._34);
						fscanf(pFile, "%c", &buf);

						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._41);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._42);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._43);
						fscanf(pFile, "%c", &buf);
						fscanf(pFile, "%f", &m_pMesh[i].pCluster[j].pMatrix[k][l]._44);
						fscanf(pFile, "%c", &buf);
					}
				}
			}
		}
		else
		{
			m_pMesh[i].pCluster = NULL;
		}

		//変換行列
		fscanf(pFile, "%f", &m_pMesh[i].LclPos.x);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclPos.y);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclPos.z);
		fscanf(pFile, "%c", &buf);

		fscanf(pFile, "%f", &m_pMesh[i].LclRot.x);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclRot.y);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclRot.z);
		fscanf(pFile, "%c", &buf);

		fscanf(pFile, "%f", &m_pMesh[i].LclScl.x);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclScl.y);
		fscanf(pFile, "%c", &buf);
		fscanf(pFile, "%f", &m_pMesh[i].LclScl.z);
		fscanf(pFile, "%c", &buf);

		fscanf(pFile, "%c", &buf);
	}

	char Cname[100][100];
	for (int i = 0; i < m_nNumMesh; i++)
	{
		//ファイル名
		fscanf_s(pFile, "%s", Cname[i], _countof(Cname[i]));
		//fscanf(pFile, "%s", );
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pFileName = Cname[i];
		m_pMesh[i].fileName = Cname[i];
		if (strcmp(m_pMesh[i].fileName.data(), "(null)") == 0)
		{
			m_pMesh[i].fileName = { 0 };
			m_pMesh[i].pFileName = NULL;
		}
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