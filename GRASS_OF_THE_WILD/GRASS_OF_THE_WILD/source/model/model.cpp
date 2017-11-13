//=============================================================================
//
// model.h
// Author : shota fukuoka
//
//=============================================================================
#include "model.h"
#//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************

///////////////////////////////////////////////////////////////////////////////
//�R���X�g���N�^
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
//�f�X�g���N�^
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
//�t�@�C���l�[���擾
///////////////////////////////////////////////////////////////////////////////
const char* Model::GetFileName()
{
	return m_pFileName;
}

///////////////////////////////////////////////////////////////////////////////
//�ǂݍ���
///////////////////////////////////////////////////////////////////////////////
void SkinMeshModel::LoadFile(const  char* FilenName)
{
	FILE *pFile;

	pFile = fopen(FilenName, "r");

	char buf[2];

	fscanf(pFile, "%d", &m_nNumAnime);
	m_pAnime = new Anime[m_nNumAnime];
	for (int i = 0; i < m_nNumAnime; i++)
	{
		fscanf(pFile, "%c", &buf);

		//�X�^�[�g
		fscanf(pFile, "%d", &m_pAnime[i].nStartTime);

		fscanf(pFile, "%c", &buf);

		//�G���h
		fscanf(pFile, "%d", &m_pAnime[i].nEndTime);
	}

	fscanf(pFile, "%2c", &buf);

	//���b�V���̐�
	fscanf(pFile, "%d", &m_nNumMesh);
	m_pMesh = new Mesh[m_nNumMesh];

	fscanf(pFile, "%2c", &buf);
	for (int i = 0; i < m_nNumMesh; i++)
	{
		fscanf(pFile, "%d", &m_pMesh[i].nNumVertex);//�|���S����
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pPosition = new VECTOR3[m_pMesh[i].nNumVertex];
		m_pMesh[i].pColor = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pBoneIndex = new VECTOR4[m_pMesh[i].nNumVertex];
		m_pMesh[i].pWeight = new VECTOR4[m_pMesh[i].nNumVertex];
		for (int j = 0; j < m_pMesh[i].nNumVertex; j++)
		{
			//���W
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pPosition[j].z);
			fscanf(pFile, "%c", &buf);

			//�F
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pColor[j].w);
			fscanf(pFile, "%c", &buf);

			//�N���X�^�[�C���f�b�N�X
			fscanf(pFile, "%d", &m_pMesh[i].pBoneIndex[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%d", &m_pMesh[i].pBoneIndex[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%d", &m_pMesh[i].pBoneIndex[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%d", &m_pMesh[i].pBoneIndex[j].w);
			fscanf(pFile, "%c", &buf);

			//�E�F�C�g
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].x);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].y);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].z);
			fscanf(pFile, "%c", &buf);
			fscanf(pFile, "%f", &m_pMesh[i].pWeight[j].z);
			fscanf(pFile, "%c", &buf);
		}

		fscanf(pFile, "%d", &m_pMesh[i].nNumPolygonVertex);//���_��
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pNormal = new VECTOR3[m_pMesh[i].nNumPolygonVertex];
		m_pMesh[i].pTex = new VECTOR2[m_pMesh[i].nNumPolygonVertex];
		m_pMesh[i].pIndexNumber = new int[m_pMesh[i].nNumPolygonVertex];
		for (int j = 0; j < m_pMesh[i].nNumPolygonVertex; j++)
		{
			//�@��
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

			//�C���f�b�N�X�i���o�[
			fscanf(pFile, "%d", &m_pMesh[i].pIndexNumber[j]);
			fscanf(pFile, "%c", &buf);
		}

		fscanf(pFile, "%d", &m_pMesh[i].nNumCluster);//�N���X�^��
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pCluster = new Cluster[m_pMesh[i].nNumCluster];

		for (int j = 0; j < m_pMesh[i].nNumCluster; j++)
		{
			m_pMesh[i].pCluster[j].pMatrix = new  XMMATRIX*[m_nNumAnime];
			for (int k = 0; k < m_nNumAnime; k++)
			{
				m_pMesh[i].pCluster[j].pMatrix[k] = new XMMATRIX[m_pAnime[k].nEndTime];
				for (int l = 0; l < m_pAnime[k].nEndTime; l++)
				{
					//�N���X�^�[�s��
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

		//�ϊ��s��
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
		//�t�@�C����
		fscanf(pFile, "%s", &Cname[i]);
		fscanf(pFile, "%c", &buf);
		m_pMesh[i].pFileName = (char*)Cname[i];
		if (strcmp(m_pMesh[i].pFileName, "(null)") == 0)
		{
			m_pMesh[i].pFileName = NULL;
		}
	}


	fclose(pFile);
}

///////////////////////////////////////////////////////////////////////////////
//���b�V���̎擾
///////////////////////////////////////////////////////////////////////////////
SkinMeshModel::Mesh *SkinMeshModel::GetMesh(void)
{
	return m_pMesh;
}

///////////////////////////////////////////////////////////////////////////////
//�A�j���\�V�����̎擾
///////////////////////////////////////////////////////////////////////////////
SkinMeshModel::Anime *SkinMeshModel::GetAnime(void)
{
	return m_pAnime;
}

///////////////////////////////////////////////////////////////////////////////
//���b�V�����̎擾
///////////////////////////////////////////////////////////////////////////////
int SkinMeshModel::GetNumMesh(void)
{
	return m_nNumMesh;
}

///////////////////////////////////////////////////////////////////////////////
//�A�j���[�V�������̎擾
///////////////////////////////////////////////////////////////////////////////
int SkinMeshModel::GetNumAnime(void)
{
	return m_nNumAnime;
}