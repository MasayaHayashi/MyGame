//����������������������������������������������������������������
// C_ResourceManager.cpp
// ���\�[�X�Ǘ�(���b�V���A�e�N�X�`���Ǘ�)
// Author : Masaya Hayashi
//����������������������������������������������������������������

#define _CRT_SECURE_NO_WARNINGS

// ===== �C���N���[�h�� =====
#include "C_ResourceManager.h"
#include <string>
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <utility>
#include "C_3DPawn.h"
#include "C_Board.h"

// ===== ���O��Ԓ�` =====
using namespace std;

// ===== �ÓI�����o�ϐ� =====
C_RESOURCE_MANAGER*					C_RESOURCE_MANAGER::pInstance;				// �C���X�^���X
std::vector<MESH_DATA*>				C_RESOURCE_MANAGER::Mesh;					// ���b�V�����
std::vector<HIERARCHY_MESH_DATA*>	C_RESOURCE_MANAGER::HierarchyMesh;			// �K�w�\���p���b�V�����
std::vector<TEXTURE_DATA*>			C_RESOURCE_MANAGER::Texture;				// �e�N�X�`�����
std::vector<TEXTURE_DATA*>			C_RESOURCE_MANAGER::FadeTexture;			// �e�N�X�`�����
std::vector<VERTEX_BOARD_DATA*>		C_RESOURCE_MANAGER::VtxBoard;				// �{�[�h���_���
std::vector<VERTEX_BOARD_DATA*>		C_RESOURCE_MANAGER::VtxFadeBoard;			// �{�[�h���_���

//��������������������������������������������
// �R���X�g���N�^
//��������������������������������������������
C_RESOURCE_MANAGER::C_RESOURCE_MANAGER()
{
	pInstance = NEW C_RESOURCE_MANAGER;
}

//��������������������������������������������
// �f�X�g���N�^
//��������������������������������������������
C_RESOURCE_MANAGER::~C_RESOURCE_MANAGER()
{
	SAFE_DELETE(pInstance);
}

//������������������������������������������
// ���f������(�K�w�\������)
//������������������������������������������
HRESULT C_RESOURCE_MANAGER::MakeModel(MESH_DATA &MeshData, CHAR *pszFilename, UINT &uMeshType)
{
	// ��O����
	if (!pszFilename)
		return E_FAIL;

	// ���b�V���̎��
	uMeshType = (UINT)NORMAL_MODEL;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// ���ɐ�������Ă��邩
	if (CheckExisting(pszFilename, MeshData))
		return S_OK;

	// �t�@�C�����Z�b�g
	strcpy(MeshData.szMeshFileName, pszFilename);

	// ���b�V�����Z�b�g
	Mesh.push_back(&MeshData);
	
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, NULL, &Mesh.back()->pD3DXBuffMat, NULL, &Mesh.back()->uNumMat, &Mesh.back()->pD3DXMesh)))
	{
		return E_FAIL;
	}

	// �����擾�̂��߂̉�����
	LPD3DXMESH pMeshWk;
	HRESULT hr = Mesh.back()->pD3DXMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshWk);

	if (SUCCEEDED(hr))
	{
		Mesh.back()->pD3DXMesh->Release();
		Mesh.back()->pD3DXMesh = pMeshWk;
	}
	else
	{
		SAFE_RELEASE(Mesh.back()->pD3DXBuffMat);	// �}�e���A�����
		return false;
	}

	// �����e�[�u���擾
	Mesh.back()->dwAttrNum = 0;
	Mesh.back()->pD3DXMesh->GetAttributeTable(NULL, &Mesh.back()->dwAttrNum);
	Mesh.back()->pAttr = NEW D3DXATTRIBUTERANGE[Mesh.back()->dwAttrNum];
	Mesh.back()->pD3DXMesh->GetAttributeTable(Mesh.back()->pAttr, &Mesh.back()->dwAttrNum);

	// �w��̒��_�t�H�[�}�b�g�ɕϊ�
	DWORD dwFVF = Mesh.back()->pD3DXMesh->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pMeshTmp = Mesh.back()->pD3DXMesh;
		pMeshTmp->CloneMeshFVF(pMeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &Mesh.back()->pD3DXMesh);
		SAFE_RELEASE(pMeshTmp);
		// �@���������ꍇ�͋����I�ɒǉ�
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(Mesh.back()->pD3DXMesh, NULL);
		}
	}

	// ���_���T�C�Y���擾
	Mesh.back()->dwNumVtx = Mesh.back()->pD3DXMesh->GetNumVertices();	// ���_���擾
	Mesh.back()->pVtx = NEW MESH_VTX[Mesh.back()->dwNumVtx];
	LPVOID pWork;

	// ���_�o�b�t�@�A�����b�N
	Mesh.back()->pD3DXMesh->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(Mesh.back()->pVtx, pWork, sizeof(MESH_VTX) * Mesh.back()->dwNumVtx);

	// ���_�o�b�t�@���b�N
	Mesh.back()->pD3DXMesh->UnlockVertexBuffer();

	// �C���f�b�N�X�o�b�t�@����ǂݍ���
	Mesh.back()->dwNumIndx = Mesh.back()->pD3DXMesh->GetNumFaces() * 3;
	Mesh.back()->pIndx = NEW WORD[Mesh.back()->dwNumIndx];

	// �C���f�b�N�X�o�b�t�@���b�N
	Mesh.back()->pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(Mesh.back()->pIndx, pWork, sizeof(WORD) * Mesh.back()->dwNumIndx);

	// �C���f�b�N�X�o�b�t�@�A�����b�N
	Mesh.back()->pD3DXMesh->UnlockIndexBuffer();

	// ���E�{�b�N�X�ƒ��S���W�����߂�
	Mesh.back()->MaxVtx = Mesh.back()->pVtx[0].VtxPos;
	Mesh.back()->MinVtx = Mesh.back()->pVtx[0].VtxPos;
	for (WORD i = 1; i < Mesh.back()->dwNumVtx; i++)
	{
		// �ő�l�擾
		if (Mesh.back()->MaxVtx.x < Mesh.back()->pVtx[i].VtxPos.x)
			Mesh.back()->MaxVtx.x = Mesh.back()->pVtx[i].VtxPos.x;

		if (Mesh.back()->MaxVtx.y < Mesh.back()->pVtx[i].VtxPos.y)
			Mesh.back()->MaxVtx.y = Mesh.back()->pVtx[i].VtxPos.y;

		if (Mesh.back()->MaxVtx.z < Mesh.back()->pVtx[i].VtxPos.z)
			Mesh.back()->MaxVtx.z = Mesh.back()->pVtx[i].VtxPos.z;

		// �ŏ��l�擾
		if (Mesh.back()->MinVtx.x > Mesh.back()->pVtx[i].VtxPos.x)
			Mesh.back()->MinVtx.x = Mesh.back()->pVtx[i].VtxPos.x;

		if (Mesh.back()->MinVtx.y > Mesh.back()->pVtx[i].VtxPos.y)
			Mesh.back()->MinVtx.y = Mesh.back()->pVtx[i].VtxPos.y;

		if (Mesh.back()->MinVtx.z > Mesh.back()->pVtx[i].VtxPos.z)
			Mesh.back()->MinVtx.z = Mesh.back()->pVtx[i].VtxPos.z;
	}

	// ���S���W�A�����蔻��擾
	Mesh.back()->CenterPos = (Mesh.back()->MaxVtx + Mesh.back()->MinVtx) * 0.5f;
	Mesh.back()->CollitionBox = (Mesh.back()->MaxVtx - Mesh.back()->MinVtx) * 0.5f;

	// ���E���擾
	FLOAT dx = Mesh.back()->pVtx[0].VtxPos.x - Mesh.back()->CenterPos.x;
	FLOAT dy = Mesh.back()->pVtx[0].VtxPos.y - Mesh.back()->CenterPos.y;
	FLOAT dz = Mesh.back()->pVtx[0].VtxPos.z - Mesh.back()->CenterPos.z;
	Mesh.back()->fCollisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < Mesh.back()->dwNumVtx; i++)
	{
		dx = Mesh.back()->pVtx[i].VtxPos.x - Mesh.back()->CenterPos.x;
		dy = Mesh.back()->pVtx[i].VtxPos.y - Mesh.back()->CenterPos.y;
		dz = Mesh.back()->pVtx[i].VtxPos.z - Mesh.back()->CenterPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (Mesh.back()->fCollisionRadus < fRadius)
			Mesh.back()->fCollisionRadus = fRadius;
	}

	// ���S���W�����[���h���W�n�ɕϊ�
	//Mesh[pszFilename].CenterPos += PosPawn;

	return S_OK;
}

//��������������������������������������������
// ���_��񐶐�
//��������������������������������������������
HRESULT C_RESOURCE_MANAGER::MakeVertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszName)
{
	// ���ʗp�t�@�C�����Z�b�g
	strcpy(VtxBordData.szName, pszName);

	// �t�F�[�h�p����
	if (VtxBordData.bFade)
		MakeVtxFade(VtxBordData);
	else
		MakeVtx(VtxBordData);

	return S_OK;
}

//������������������������������������������������������������������
// ���_��񐶐�
//������������������������������������������������������������������
bool C_RESOURCE_MANAGER::MakeVtx(VERTEX_BOARD_DATA &VtxBoardData)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// ���_�����Z�b�g
	VtxBoard.push_back(&VtxBoardData);

	// ���_��񐶐�
	switch (VtxBoardData.BoardType)
	{
	case POLYGON_2D:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxBoard.back()->pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y + VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y + VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y - VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxBoard.back()->SizeBoard.x * 0.5f, VtxBoard.back()->PosBoard.y - VtxBoard.back()->SizeBoard.y * 0.5f, 0.0f);

				// rhw�̐ݒ�
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// ���_�f�[�^���A�����b�N����
			VtxBoard.back()->pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxBoard.back()->pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// �@���̐ݒ�
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// ���_�f�[�^���A�����b�N����
		VtxBoard.back()->pD3DVtxBuffBoard->Unlock();
	}
	break;
	case POLYGON_3D:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxBoardData.pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// �@���̐ݒ�
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// ���_�f�[�^���A�����b�N����
		VtxBoard.back()->pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}
}

//������������������������������������������
// �t�F�[�h�p���_��񐶐�
//������������������������������������������
bool C_RESOURCE_MANAGER::MakeVtxFade(VERTEX_BOARD_DATA &VtxBoardData)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// ���_�����Z�b�g
	VtxFadeBoard.push_back(&VtxBoardData);

	// ���_��񐶐�
	switch (VtxBoardData.BoardType)
	{
	case POLYGON_2D:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y + VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y + VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x - VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y - VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.PosBoard.x + VtxFadeBoard.back()->SizeBoard.x * 0.5f, VtxFadeBoard.back()->PosBoard.y - VtxFadeBoard.back()->SizeBoard.y * 0.5f, 0.0f);

				// rhw�̐ݒ�
				pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

				// ���ˌ��̐ݒ�
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// ���_�f�[�^���A�����b�N����
			VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// �@���̐ݒ�
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// ���_�f�[�^���A�����b�N����
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();
	}
	break;
	case POLYGON_3D:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&VtxFadeBoard.back()->pD3DVtxBuffBoard,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		/*
		if (SizeBoard.z == 0.0f)
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, -SizeBoard.y / 2.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, SizeBoard.y / 2.0f, 0.0f);
		}
		else
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		pVtx[2].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x / 2.0f, 0.0f, SizeBoard.z);
		}
		*/


		pVtx[0].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VtxBoardData.SizeBoard.x, -VtxBoardData.SizeBoard.y, 0.0f);
		/*
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
		}
		*/
		// �@���̐ݒ�
		if (VtxBoardData.SizeBoard.z == 0.0f)
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		/*
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		*/
		// ���_�f�[�^���A�����b�N����
		VtxFadeBoard.back()->pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}
}

//������������������������������������������
// �e�N�X�`������
//������������������������������������������
HRESULT C_RESOURCE_MANAGER::CreateTexture(TEXTURE_DATA &TextureData,  CHAR *pszFilename)
{
	// ��O����
	if (!pszFilename)
		return E_FAIL;

	// ���ɐ�������Ă��邩
	if (CheckExisting(pszFilename, TextureData))
		return S_OK;

	// �t�@�C�����Z�b�g
	strcpy(TextureData.szTexFileName, pszFilename);

	Texture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, Texture.back()->szTexFileName, &Texture.back()->pD3DTexture))
		return S_OK;
}


//������������������������������������������
// �K�w�\���p���f���ǂݍ���
//������������������������������������������
HRESULT C_RESOURCE_MANAGER::MakeModelHierarchy(HIERARCHY_MESH_DATA &HierarchyMedhData,CHAR *pszFilename, UINT &uMeshType)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// ���b�V���̎�ޏ�����
	uMeshType = (INT)HIERARCHY_MODEL;

	// ���b�V�����Z�b�g
	strcpy(HierarchyMedhData.szMeshFileName, pszFilename);	// �t�@�C�����Z�b�g

	// ���b�V�����Z�b�g
	HierarchyMesh.push_back(&HierarchyMedhData);

	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(HierarchyMesh.back()->szMeshFileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	HierarchyMesh.back()->Hierarchy.SetDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(HierarchyMesh.back()->szMeshFileName, D3DXMESH_MANAGED, devicePtr, &HierarchyMesh.back()->Hierarchy, NULL, &HierarchyMesh.back()->pFrameRoot, &HierarchyMesh.back()->pAnimCtrl);
	if (FAILED(hr))
		return false;

	// �{�[���ƃt���[���̊֘A�t��
	hr = AllocAllBoneMatrix(HierarchyMesh.back()->pFrameRoot,pszFilename);
	if (FAILED(hr)) return false;

	// �A�j���[�V�����Z�b�g�擾
	HierarchyMesh.back()->uNumAnimSet = 0;
	if (HierarchyMesh.back()->pAnimCtrl)
	{
		HierarchyMesh.back()->uNumAnimSet = HierarchyMesh.back()->pAnimCtrl->GetNumAnimationSets();
		if (HierarchyMesh.back()->uNumAnimSet > 0)
		{
			HierarchyMesh.back()->ppAnimSet = NEW LPD3DXANIMATIONSET[HierarchyMesh.back()->uNumAnimSet];
			for (DWORD u = 0; u < HierarchyMesh.back()->uNumAnimSet; u++)
			{
				HierarchyMesh.back()->pAnimCtrl->GetAnimationSet(u, &HierarchyMesh.back()->ppAnimSet[u]);
			}
		}
	}

	if (HierarchyMesh.back()->pFrameRoot)
	{
		// �}�g���b�N�X�X�V
		SetTime(0.0,pszFilename);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		UpdateFrameMatrices(HierarchyMesh.back()->pFrameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		CalcCollision(HierarchyMesh.back()->pFrameRoot,pszFilename);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	HierarchyMesh.back()->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//������������������������������������������������������������������������������������������
// �t���[���̃}�g���b�N�X���X�V
//������������������������������������������������������������������������������������������
void C_RESOURCE_MANAGER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// �e�̃}�g���b�N�X���|�����킹��
	if (pParentMatrix)
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	// �Z��t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameSibling)
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

	// �q�t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameFirstChild)
	{
		// �������e�ƂȂ�̂ŁA�����̃}�g���b�N�X��n��
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


//��������������������������������������������������������������
// �A�j���[�V�����J�n���Ԑݒ�
//��������������������������������������������������������������
void C_RESOURCE_MANAGER::SetTime(DOUBLE dTime,CHAR *pszFilename)
{
	if (HierarchyMesh.back()->pAnimCtrl == NULL)
		return;

	for (DWORD i = 0; i < HierarchyMesh.back()->pAnimCtrl->GetMaxNumTracks(); ++i)
	{
		HierarchyMesh.back()->pAnimCtrl->SetTrackPosition(i, 0);
	}

	HierarchyMesh.back()->pAnimCtrl->ResetTime();
	HierarchyMesh.back()->pAnimCtrl->AdvanceTime(dTime, NULL);
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT C_RESOURCE_MANAGER::AllocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == NULL)
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(HierarchyMesh.back()->pFrameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == NULL)
			return E_FAIL;
		meshContainerPtr->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT C_RESOURCE_MANAGER::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->meshContainerPtr)
	{
		hr = AllocBoneMatrix(pFrame->meshContainerPtr,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling)
	{
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild)
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild,pszFilename);

	return hr;
}

//������������������������������������������
//  �K�w�\���p ���E��/���E�{�b�N�X�擾
//������������������������������������������
void C_RESOURCE_MANAGER::CalcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename)
{
	D3DXVECTOR3& vMax = HierarchyMesh.back()->CollitionBox;
	D3DXVECTOR3& vMin = HierarchyMesh.back()->CenterPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	HierarchyMesh.back()->fCollisionRadus = -1.0f;
	CalcCollisionFrame(pFrame,pszFilename);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	HierarchyMesh.back()->CollitionBox = vBBox;
	HierarchyMesh.back()->CenterPos = vCenter;
	HierarchyMesh.back()->fCollisionRadus = 0.0f;
	CalcCollisionFrame(pFrame,pszFilename);
}


//��������������������������������������������������������������������
// �t���[�����̒��_���W�̒��o
//��������������������������������������������������������������������
void C_RESOURCE_MANAGER::CalcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->meshContainerPtr;
	while (meshContainerPtr)
	{
		// ���b�V���R���e�i���̒��_���W�̒��o
		CalcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename);
		// ���̃��b�V���R���e�i
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		CalcCollisionFrame(pFrame->pFrameSibling,pszFilename);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		CalcCollisionFrame(pFrame->pFrameFirstChild,pszFilename);
}

//��������������������������������������������������������������������
// ���b�V���R���e�i���̒��_���W�̒��o
//��������������������������������������������������������������������
void C_RESOURCE_MANAGER::CalcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->CombinedTransformationMatrix;
	// ���_���W�̒��o
	LPD3DXMESH pMesh = meshContainerPtr->MeshData.pMesh;
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVtx = pMesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (HierarchyMesh.back()->fCollisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = HierarchyMesh.back()->CollitionBox;
			D3DXVECTOR3& vMin = HierarchyMesh.back()->CenterPos;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - HierarchyMesh.back()->CenterPos));
			if (HierarchyMesh.back()->fCollisionRadus < fRadius)
				HierarchyMesh.back()->fCollisionRadus = fRadius;
		}
	}
	pMesh->UnlockVertexBuffer();
}

//����������������������������������������������������������
// ���b�V����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme,MESH_DATA &MeshData)
{
	// �S���b�V��������
	for (UINT i = 0; i < Mesh.size(); i++)
		if (strcmp(Mesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			MeshData = *Mesh[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �e�N�X�`����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme, TEXTURE_DATA &TextureData)
{
	// �S�e�N�X�`��������
	for (UINT i = 0; i < Texture.size(); i++)
		if (strcmp(Texture[i]->szTexFileName, pszChakNeme) == 0)
		{
			TextureData = *Texture[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �e�N�X�`����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::CheckExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &TextureData)
{
	// �S�e�N�X�`��������
	for (UINT i = 0; i < VtxBoard.size(); i++)
		if (strcmp(VtxBoard[i]->szName, pszChakNeme) == 0)
		{
			TextureData = *VtxBoard[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// ���b�V�����
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyMesh(CHAR *pszChakNeme)
{
	for (INT i = (INT)Mesh.size() - 1; i >= 0; i--)
		if (strcmp(Mesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			SAFE_DELETE(Mesh[i]->pAttr);
			SAFE_DELETE(Mesh[i]->pIndx);
			SAFE_DELETE(Mesh[i]->pVtx);
			Mesh.erase(Mesh.begin() + i);
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �S���b�V�����
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyAllMesh()
{
	// ��O����
	if (Mesh.size() == 0)
		return true;

	for (INT i = 0; i < (INT)Mesh.size(); i++)
	{
		SAFE_DELETE(Mesh[i]->pAttr);
		SAFE_DELETE(Mesh[i]->pIndx);
		SAFE_DELETE(Mesh[i]->pVtx);
		SAFE_DELETE(Mesh[i]->pIndx);
		SAFE_RELEASE(Mesh[i]->pD3DXMesh);
		SAFE_RELEASE(Mesh[i]->pD3DXBuffMat);
	//	Mesh.erase(Mesh.begin() + i);
	}

	Mesh.clear();
	return true;
}

//����������������������������������������������������������
// �K�w�\���p���f�����
//����������������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyHierarchyMesh(CHAR *pszChakNeme)
{
	for (INT i = HierarchyMesh.size() - 1; i >= 0; i--)
		if (strcmp(HierarchyMesh[i]->szMeshFileName, pszChakNeme) == 0)
		{
			
			// �A�j���[�V�������
			if (HierarchyMesh[i]->ppAnimSet)
			{
				for (UINT j = 0; j < HierarchyMesh[i]->uNumAnimSet; j++)
					SAFE_RELEASE(HierarchyMesh[i]->ppAnimSet[j]);
				SAFE_DELETE_ARRAY(HierarchyMesh[i]->ppAnimSet);
			}
			SAFE_RELEASE(HierarchyMesh[i]->pAnimCtrl);

			// ���b�V�����
			if (HierarchyMesh[i]->pFrameRoot)
			{
				D3DXFrameDestroy(HierarchyMesh[i]->pFrameRoot, &HierarchyMesh[i]->Hierarchy);
				HierarchyMesh[i]->pFrameRoot = NULL;
			}
			
			return true;
		}
	return false;
}

//����������������������������������������������������������������
// �S�K�w�\���p���b�V�������
//����������������������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyAllHierarchyMesh()
{
	for (INT i = HierarchyMesh.size() - 1; i >= 0; i--)
	{
		// �A�j���[�V�������
		if (HierarchyMesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < HierarchyMesh[i]->uNumAnimSet; j++)
				SAFE_RELEASE(HierarchyMesh[i]->ppAnimSet[j]);
			SAFE_DELETE_ARRAY(HierarchyMesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(HierarchyMesh[i]->pAnimCtrl);

		// ���b�V�����
		if (HierarchyMesh[i]->pFrameRoot)
		{
			D3DXFrameDestroy(HierarchyMesh[i]->pFrameRoot, &HierarchyMesh[i]->Hierarchy);
			HierarchyMesh[i]->pFrameRoot = NULL;
		}

	}
	HierarchyMesh.clear();
	return true;
}

//������������������������������������������������
// �e�N�X�`�����
//������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyAllTexture()
{
	// ��O����
	if (Texture.size() <= 0)
		return false;

	for (UINT i = 0; i < Texture.size(); i++)
	{
		SAFE_RELEASE(Texture[i]->pD3DTexture);
		Texture.erase(Texture.begin() + i);
	}

	Texture.clear();

	return true;
}

//������������������������������������������������
// ���_�����(�t�F�[�h�p�ȊO)
//������������������������������������������������
bool C_RESOURCE_MANAGER::DestroyVtx()
{
	// ��O����
	if (VtxBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < VtxBoard.size(); i++)
		SAFE_RELEASE(VtxBoard[i]->pD3DVtxBuffBoard);

	VtxBoard.clear();
	
	return true;
}

//��������������������������������������������
// �t�F�[�h�p���_�����
//��������������������������������������������
bool C_RESOURCE_MANAGER::DestroyFadeVtx()
{
	// ��O����
	if (VtxFadeBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < VtxFadeBoard.size(); i++)
		SAFE_RELEASE(VtxFadeBoard[i]->pD3DVtxBuffBoard);
	VtxFadeBoard.clear();

	return true;
}

//����������������������������������������������
// �e�N�X�`���쐬(�t�F�[�h)
//����������������������������������������������
void C_RESOURCE_MANAGER::CreateFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{
	// �t�@�C�����Z�b�g
	strcpy(TextureData.szTexFileName, pszFilename);

	FadeTexture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, FadeTexture.back()->szTexFileName, &FadeTexture.back()->pD3DTexture))
		return;
}

void  C_RESOURCE_MANAGER::CreateNormalTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{

	// ���ɐ�������Ă��邩
	if (CheckExisting(pszFilename, TextureData))
		return;

	// �t�@�C�����Z�b�g
	strcpy(TextureData.szTexFileName, pszFilename);

	Texture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, Texture.back()->szTexFileName, &Texture.back()->pD3DTexture))
		return ;
}