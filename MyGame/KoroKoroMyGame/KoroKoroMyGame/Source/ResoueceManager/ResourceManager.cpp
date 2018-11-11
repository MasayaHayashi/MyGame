//����������������������������������������������������������������
// C_ResourceManager.cpp
// ���\�[�X�Ǘ�(���b�V���A�e�N�X�`���Ǘ�)
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "ResourceManager.h"
#include <string>
#include <tchar.h>
#include <utility>
#include "../Pawn/Pawn.h"
#include "../Board/Board.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Pawn/Pawn.h"


// ===== �ÓI�����o�ϐ� =====
ResourceManager*					ResourceManager::pInstance = new ResourceManager;
std::vector <MESH_DATA*>			ResourceManager::mesh;
std::unordered_map<std::string, HIERARCHY_MESH_DATA*>	ResourceManager::hierarchyMesh;
std::vector<TEXTURE_DATA*>			ResourceManager::texture;				// �e�N�X�`�����
std::vector<TEXTURE_DATA*>			ResourceManager::fadeTexture;			// �e�N�X�`�����
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxBoard;				// �{�[�h���_���
std::vector<VERTEX_BOARD_DATA*>		ResourceManager::vtxFadeBoard;			// �{�[�h���_���

#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}

//��������������������������������������������
// �R���X�g���N�^
//��������������������������������������������
ResourceManager::ResourceManager()
{
	
}

//��������������������������������������������
// �f�X�g���N�^
//��������������������������������������������
ResourceManager::~ResourceManager()
{
	SAFE_DELETE(pInstance);
}

//������������������������������������������
// ���f������(�K�w�\������)
//������������������������������������������
HRESULT ResourceManager::makeModel(MESH_DATA &meshData, CHAR *pszFilename, MeshObjType &umeshType)
{
	// ��O����
	if (!pszFilename)
		return E_FAIL;

	// ���b�V���̎��
	umeshType = MeshObjType::NormalModel;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// ���ɐ�������Ă��邩
	if (checkExisting(pszFilename, meshData))
		return S_OK;

	// �t�@�C�����Z�b�g
	strcpy_s(meshData.meshFileName, pszFilename);

	// ���b�V�����Z�b�g

	mesh.push_back(&meshData);
	
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &mesh.back()->pD3DXBuffMat, nullptr, &mesh.back()->numMat, &mesh.back()->pD3DXMesh)))
	{
		return E_FAIL;
	}

	// �����擾�̂��߂̉�����
	LPD3DXMESH pmeshWk;
	HRESULT hr = mesh.back()->pD3DXMesh->Optimize(D3DXMESHOPT_ATTRSORT, nullptr, nullptr, nullptr, nullptr, &pmeshWk);

	if (SUCCEEDED(hr))
	{
		mesh.back()->pD3DXMesh->Release();
		mesh.back()->pD3DXMesh = pmeshWk;
	}
	else
	{
		SAFE_RELEASE(mesh.back()->pD3DXBuffMat);	// �}�e���A�����
		return false;
	}

	// �����e�[�u���擾
	mesh.back()->dwAttrNum = 0;
	mesh.back()->pD3DXMesh->GetAttributeTable(nullptr, &mesh.back()->dwAttrNum);
	mesh.back()->pAttr = new D3DXATTRIBUTERANGE[mesh.back()->dwAttrNum];
	mesh.back()->pD3DXMesh->GetAttributeTable(mesh.back()->pAttr, &mesh.back()->dwAttrNum);

	// �w��̒��_�t�H�[�}�b�g�ɕϊ�
	DWORD dwFVF = mesh.back()->pD3DXMesh->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pmeshTmp = mesh.back()->pD3DXMesh;
		pmeshTmp->CloneMeshFVF(pmeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &mesh.back()->pD3DXMesh);
		SAFE_RELEASE(pmeshTmp);
		// �@���������ꍇ�͋����I�ɒǉ�
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(mesh.back()->pD3DXMesh, nullptr);
		}
	}

	// ���_���T�C�Y���擾
	mesh.back()->dwNumVtx = mesh.back()->pD3DXMesh->GetNumVertices();	// ���_���擾
	mesh.back()->pVtx = new MESH_VTX[mesh.back()->dwNumVtx];
	LPVOID pWork;

	// ���_�o�b�t�@�A�����b�N
	mesh.back()->pD3DXMesh->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(mesh.back()->pVtx, pWork, sizeof(MESH_VTX) * mesh.back()->dwNumVtx);

	// ���_�o�b�t�@���b�N
	mesh.back()->pD3DXMesh->UnlockVertexBuffer();

	// �C���f�b�N�X�o�b�t�@����ǂݍ���
	mesh.back()->dwNumIndx = mesh.back()->pD3DXMesh->GetNumFaces() * 3;
	mesh.back()->pIndx = new WORD[mesh.back()->dwNumIndx];

	// �C���f�b�N�X�o�b�t�@���b�N
	mesh.back()->pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(mesh.back()->pIndx, pWork, sizeof(WORD) * mesh.back()->dwNumIndx);

	// �C���f�b�N�X�o�b�t�@�A�����b�N
	mesh.back()->pD3DXMesh->UnlockIndexBuffer();

	// ���E�{�b�N�X�ƒ��S���W�����߂�
	mesh.back()->maxVtx = mesh.back()->pVtx[0].VtxPos;
	mesh.back()->minVtx = mesh.back()->pVtx[0].VtxPos;
	for (WORD i = 1; i < mesh.back()->dwNumVtx; i++)
	{
		// �ő�l�擾
		if (mesh.back()->maxVtx.x < mesh.back()->pVtx[i].VtxPos.x)
			mesh.back()->maxVtx.x = mesh.back()->pVtx[i].VtxPos.x;

		if (mesh.back()->maxVtx.y < mesh.back()->pVtx[i].VtxPos.y)
			mesh.back()->maxVtx.y = mesh.back()->pVtx[i].VtxPos.y;

		if (mesh.back()->maxVtx.z < mesh.back()->pVtx[i].VtxPos.z)
			mesh.back()->maxVtx.z = mesh.back()->pVtx[i].VtxPos.z;

		// �ŏ��l�擾
		if (mesh.back()->minVtx.x > mesh.back()->pVtx[i].VtxPos.x)
			mesh.back()->minVtx.x = mesh.back()->pVtx[i].VtxPos.x;

		if (mesh.back()->minVtx.y > mesh.back()->pVtx[i].VtxPos.y)
			mesh.back()->minVtx.y = mesh.back()->pVtx[i].VtxPos.y;

		if (mesh.back()->minVtx.z > mesh.back()->pVtx[i].VtxPos.z)
			mesh.back()->minVtx.z = mesh.back()->pVtx[i].VtxPos.z;
	}

	// ���S���W�A�����蔻��擾
	mesh.back()->centerPos = (mesh.back()->maxVtx + mesh.back()->minVtx) * 0.5f;
	mesh.back()->collitionBox = (mesh.back()->maxVtx - mesh.back()->minVtx) * 0.5f;

	// ���E���擾
	FLOAT dx = mesh.back()->pVtx[0].VtxPos.x - mesh.back()->centerPos.x;
	FLOAT dy = mesh.back()->pVtx[0].VtxPos.y - mesh.back()->centerPos.y;
	FLOAT dz = mesh.back()->pVtx[0].VtxPos.z - mesh.back()->centerPos.z;
	mesh.back()->collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < mesh.back()->dwNumVtx; i++)
	{
		dx = mesh.back()->pVtx[i].VtxPos.x - mesh.back()->centerPos.x;
		dy = mesh.back()->pVtx[i].VtxPos.y - mesh.back()->centerPos.y;
		dz = mesh.back()->pVtx[i].VtxPos.z - mesh.back()->centerPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (mesh.back()->collisionRadus < fRadius)
			mesh.back()->collisionRadus = fRadius;
	}

	// ���S���W�����[���h���W�n�ɕϊ�
	//mesh[pszFilename].CenterPos += pos;

	return S_OK;
}

//��������������������������������������������
// ���_��񐶐�
//��������������������������������������������
HRESULT ResourceManager::makevertexBoard(VERTEX_BOARD_DATA &VtxBordData, CHAR *pszName)
{
	// ���ʗp�t�@�C�����Z�b�g
	strcpy_s(VtxBordData.name, pszName);

	// �t�F�[�h�p����
	if (VtxBordData.fade)
		makeVtxFade(VtxBordData);
	else
		makeVtx(VtxBordData);

	return S_OK;
}

//������������������������������������������������������������������
// ���_��񐶐�
//������������������������������������������������������������������
bool ResourceManager::makeVtx(VERTEX_BOARD_DATA &vtxBoardData)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ���_�����Z�b�g
	vtxBoard.push_back(&vtxBoardData);

	// ���_��񐶐�
	switch (vtxBoardData.boardType)
	{
	case  boardType::Polygon2d:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (DirectX3D::VertexSize),	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxBoard.back()->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))										// nullptr�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y + vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxBoard.back()->size.x * 0.5f, vtxBoard.back()->pos.y - vtxBoard.back()->size.y * 0.5f, 0.0f);

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
			vtxBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxBoard.back()->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))										// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// �@���̐ݒ�
		if (vtxBoardData.size.z == 0.0f)
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
		vtxBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;
	case boardType::Polygon3d:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxBoardData.pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))						// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vtxBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		/*
		{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(size.x, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f,size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(size.x,size.y, 0.0f);
		}
		*/
		// �@���̐ݒ�
		if (vtxBoardData.size.z == 0.0f)
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
		vtxBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//������������������������������������������
// �t�F�[�h�p���_��񐶐�
//������������������������������������������
bool ResourceManager::makeVtxFade(VERTEX_BOARD_DATA &vtxBoardData)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ���_�����Z�b�g
	vtxFadeBoard.push_back(&vtxBoardData);

	// ���_��񐶐�
	switch (vtxBoardData.boardType)
	{
	case boardType::Polygon2d:

		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * DirectX3D::VertexSize,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxFadeBoard.back()->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))										// nullptr�ɐݒ�
		{
			return false;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// ���_���W�̐ݒ�
				pVtx[0].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y + vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vtxBoardData.pos.x - vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.pos.x + vtxFadeBoard.back()->size.x * 0.5f, vtxFadeBoard.back()->pos.y - vtxFadeBoard.back()->size.y * 0.5f, 0.0f);

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
			vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
		}
		return true;
	case boardType::Billboard:
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxFadeBoard.back()->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))										// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		
		// �@���̐ݒ�
		if (vtxBoardData.size.z == 0.0f)
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

		// ���_�f�[�^���A�����b�N����
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();
	}
		return true;

	case boardType::Polygon3d:
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * DirectX3D::VertexSize,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxFadeBoard.back()->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			nullptr)))						// nullptr�ɐݒ�
		{
			return false;
		}

		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		vtxFadeBoard.back()->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vtxBoardData.size.x, vtxBoardData.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vtxBoardData.size.x, -vtxBoardData.size.y, 0.0f);

		// �@���̐ݒ�
		if (vtxBoardData.size.z == 0.0f)
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

		// ���_�f�[�^���A�����b�N����
		vtxFadeBoard.back()->pD3DVtxBuff->Unlock();

		return true;
	default:
		break;
	}

	return false;
}

//������������������������������������������
// �e�N�X�`������
//������������������������������������������
HRESULT ResourceManager::createTexture(TEXTURE_DATA &TextureData,  CHAR *pszFilename)
{
	// ��O����
	if (!pszFilename)
		return E_FAIL;

	// ���ɐ�������Ă��邩
	if (checkExisting(pszFilename, TextureData))
		return S_OK;

	// �t�@�C�����Z�b�g
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}


//������������������������������������������
// �K�w�\���p���f���ǂݍ���
//������������������������������������������
HRESULT ResourceManager::makeModelHierarchy(HIERARCHY_MESH_DATA &setHierarchyMedhData,CHAR *pszFilename,std::string keyName, MeshObjType &umeshType)
{
	// ���b�V���̎�ޏ�����
	umeshType = MeshObjType::HierarchyModel;

	// ���b�V�����Z�b�g
	strcpy_s(setHierarchyMedhData.meshFileName, pszFilename);	// �t�@�C�����Z�b�g

	// ���b�V�����Z�b�g
	std::pair<std::string, HIERARCHY_MESH_DATA*> setPair = std::make_pair(keyName, &setHierarchyMedhData);
//	hierarchyMesh.insert( std::make_pair<std::string, Hie> )
	hierarchyMesh.insert(setPair);

	// hierarchyMesh.push_back(&setHierarchyMedhData);

	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath_s(hierarchyMesh[keyName]->meshFileName, szDir, sizeof(szDir), szDirWk, sizeof(szDirWk), nullptr, 0, nullptr, 0);
	lstrcat(szDir, szDirWk);
	hierarchyMesh[keyName]->hierarchy.setDirectory(szDir);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(hierarchyMesh[keyName]->meshFileName, D3DXMESH_MANAGED, devicePtr, &hierarchyMesh[keyName]->hierarchy, nullptr, &hierarchyMesh[keyName]->frameRoot, &hierarchyMesh[keyName]->animCtrlPtr);
	if (FAILED(hr))
	{
		return false;
	}

	// �{�[���ƃt���[���̊֘A�t��
	hr = allocAllBoneMatrix(hierarchyMesh[keyName]->frameRoot,pszFilename);
	if (FAILED(hr))
	{
		return false;
	}

	// �A�j���[�V�����Z�b�g�擾
	hierarchyMesh[keyName]->numAnimset = 0;
	if (hierarchyMesh[keyName]->animCtrlPtr)
	{
		hierarchyMesh[keyName]->numAnimset = hierarchyMesh[keyName]->animCtrlPtr->GetNumAnimationSets();
		if (hierarchyMesh[keyName]->numAnimset > 0)
		{
			hierarchyMesh[keyName]->ppAnimSet = new LPD3DXANIMATIONSET[hierarchyMesh[keyName]->numAnimset];
			for (DWORD u = 0; u < hierarchyMesh[keyName]->numAnimset; u++)
			{
				hierarchyMesh[keyName]->animCtrlPtr->GetAnimationSet(u, &hierarchyMesh[keyName]->ppAnimSet[u]);
			}
		}
	}

	if (hierarchyMesh[keyName]->frameRoot)
	{
		// �}�g���b�N�X�X�V
		setTime(0.0,pszFilename);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(hierarchyMesh[keyName]->frameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		calcCollision(hierarchyMesh[keyName]->frameRoot,pszFilename);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	hierarchyMesh[keyName]->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//������������������������������������������������������������������������������������������
// �t���[���̃}�g���b�N�X���X�V
//������������������������������������������������������������������������������������������
void ResourceManager::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	// �e�̃}�g���b�N�X���|�����킹��
	if (pParentMatrix)
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	else
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;

	// �Z��t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameSibling)
		updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

	// �q�t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameFirstChild)
	{
		// �������e�ƂȂ�̂ŁA�����̃}�g���b�N�X��n��
		updateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->combinedTransformationMatrix);
	}
}


//��������������������������������������������������������������
// �A�j���[�V�����J�n���Ԑݒ�
//��������������������������������������������������������������
void ResourceManager::setTime(DOUBLE dTime,CHAR *pszFilename,std::string keyName)
{
	if (hierarchyMesh[keyName]->animCtrlPtr == nullptr)
		return;

	for (DWORD i = 0; i < hierarchyMesh[keyName]->animCtrlPtr->GetMaxNumTracks(); ++i)
	{
		hierarchyMesh[keyName]->animCtrlPtr->SetTrackPosition(i, 0);
	}

	hierarchyMesh[keyName]->animCtrlPtr->ResetTime();
	hierarchyMesh[keyName]->animCtrlPtr->AdvanceTime(dTime, nullptr);
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT ResourceManager::allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == nullptr)
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = new LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(hierarchyMesh[keyName]->frameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
			return E_FAIL;
		meshContainerPtr->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT ResourceManager::allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer)
	{
		hr = allocBoneMatrix(pFrame->pMeshContainer,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameSibling,pszFilename);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild)
		hr = allocAllBoneMatrix(pFrame->pFrameFirstChild,pszFilename);

	return hr;
}

//������������������������������������������
//  �K�w�\���p ���E��/���E�{�b�N�X�擾
//������������������������������������������
void ResourceManager::calcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename,std::string keyName)
{
	D3DXVECTOR3& vMax = hierarchyMesh[keyName]->collitionBox;
	D3DXVECTOR3& vMin = hierarchyMesh[keyName]->centerPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	hierarchyMesh[keyName]->collisionRadus = -1.0f;
	calcCollisionFrame(pFrame,pszFilename);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	hierarchyMesh[keyName]->collitionBox = vBBox;
	hierarchyMesh[keyName]->centerPos = vCenter;
	hierarchyMesh[keyName]->collisionRadus = 0.0f;
	calcCollisionFrame(pFrame,pszFilename);
}


//��������������������������������������������������������������������
// �t���[�����̒��_���W�̒��o
//��������������������������������������������������������������������
void ResourceManager::calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->pMeshContainer;
	while (meshContainerPtr)
	{
		// ���b�V���R���e�i���̒��_���W�̒��o
		calcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename);
		// ���̃��b�V���R���e�i
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		calcCollisionFrame(pFrame->pFrameSibling,pszFilename);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		calcCollisionFrame(pFrame->pFrameFirstChild,pszFilename);
}

//��������������������������������������������������������������������
// ���b�V���R���e�i���̒��_���W�̒��o
//��������������������������������������������������������������������
void ResourceManager::calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->combinedTransformationMatrix;
	// ���_���W�̒��o
	LPD3DXMESH pmesh = meshContainerPtr->MeshData.pMesh;
	DWORD dwStride = pmesh->GetNumBytesPerVertex();
	DWORD dwVtx = pmesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pmesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (hierarchyMesh[keyName]->collisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = hierarchyMesh[keyName]->collitionBox;
			D3DXVECTOR3& vMin = hierarchyMesh[keyName]->centerPos;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - hierarchyMesh[keyName]->centerPos));
			if (hierarchyMesh[keyName]->collisionRadus < fRadius)
				hierarchyMesh[keyName]->collisionRadus = fRadius;
		}
	}
	pmesh->UnlockVertexBuffer();
}

//����������������������������������������������������������
// ���b�V����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool ResourceManager::checkExisting(CHAR *pszChakNeme,MESH_DATA &meshData)
{
	// �S���b�V��������
	for (UINT i = 0; i < mesh.size(); i++)
		if (strcmp(mesh[i]->meshFileName, pszChakNeme) == 0)
		{
			meshData = *mesh[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �e�N�X�`����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool ResourceManager::checkExisting(CHAR *pszChakNeme, TEXTURE_DATA &TextureData)
{
	// �S�e�N�X�`��������
	for (UINT i = 0; i < texture.size(); i++)
		if (strcmp(texture[i]->texFileName, pszChakNeme) == 0)
		{
			TextureData = *texture[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �e�N�X�`����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool ResourceManager::checkExisting(CHAR *pszChakNeme, VERTEX_BOARD_DATA &TextureData)
{
	// �S�e�N�X�`��������
	for (UINT i = 0; i < vtxBoard.size(); i++)
		if (strcmp(vtxBoard[i]->name, pszChakNeme) == 0)
		{
			TextureData = *vtxBoard[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	return false;
}

//����������������������������������������������������������
// ���b�V�����
//����������������������������������������������������������
bool ResourceManager::destroyMesh(CHAR *pszChakNeme)
{
	for (INT i = (INT)mesh.size() - 1; i >= 0; i--)
		if (strcmp(mesh[i]->meshFileName, pszChakNeme) == 0)
		{
			SAFE_DELETE(mesh[i]->pAttr);
			SAFE_DELETE(mesh[i]->pIndx);
			SAFE_DELETE(mesh[i]->pVtx);
			mesh.erase(mesh.begin() + i);
			return true;
		}
	return false;
}

//����������������������������������������������������������
// �S���b�V�����
//����������������������������������������������������������
bool ResourceManager::destroyAllMesh()
{
	// ��O����
	if (mesh.size() == 0)
		return true;

	for (INT i = 0; i < (INT)mesh.size(); i++)
	{
		SAFE_DELETE(mesh[i]->pAttr);
		SAFE_DELETE(mesh[i]->pIndx);
		SAFE_DELETE(mesh[i]->pVtx);
		SAFE_DELETE(mesh[i]->pIndx);
		SAFE_RELEASE(mesh[i]->pD3DXMesh);
		SAFE_RELEASE(mesh[i]->pD3DXBuffMat);
	//	mesh.erase(mesh.begin() + i);
	}

	mesh.clear();
	return true;
}

//����������������������������������������������������������
// �K�w�\���p���f�����
//����������������������������������������������������������
bool ResourceManager::destroyHierarchymesh(CHAR *pszChakNeme)
{
	for (INT i = hierarchymesh.size() - 1; i >= 0; i--)
		if (strcmp(hierarchymesh[i]->meshFileName, pszChakNeme) == 0)
		{
			
			// �A�j���[�V�������
			if (hierarchymesh[i]->ppAnimSet)
			{
				for (UINT j = 0; j < hierarchymesh[i]->numAnimset; j++)
					SAFE_RELEASE(hierarchymesh[i]->ppAnimSet[j]);
				SAFE_DELETE_ARRAY(hierarchymesh[i]->ppAnimSet);
			}
			SAFE_RELEASE(hierarchymesh[i]->animCtrlPtr);

			// ���b�V�����
			if (hierarchymesh[i]->frameRoot)
			{
				D3DXFrameDestroy(hierarchymesh[i]->frameRoot, &hierarchymesh[i]->hierarchy);
				hierarchymesh[i]->frameRoot = nullptr;
			}
			
			return true;
		}
	return false;
}

//����������������������������������������������������������������
// �S�K�w�\���p���b�V�������
//����������������������������������������������������������������
bool ResourceManager::destroyAllHierarchymesh()
{
	for (INT i = hierarchymesh.size() - 1; i >= 0; i--)
	{
		// �A�j���[�V�������
		if (hierarchymesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchymesh[i]->numAnimset; j++)
				SAFE_RELEASE(hierarchymesh[i]->ppAnimSet[j]);
			SAFE_DELETE_ARRAY(hierarchymesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(hierarchymesh[i]->animCtrlPtr);

		// ���b�V�����
		if (hierarchymesh[i]->frameRoot)
		{
			D3DXFrameDestroy(hierarchymesh[i]->frameRoot, &hierarchymesh[i]->hierarchy);
			hierarchymesh[i]->frameRoot = nullptr;
		}

	}
	hierarchymesh.clear();
	return true;
}

//������������������������������������������������
// �e�N�X�`�����
//������������������������������������������������
bool ResourceManager::destroyAllTexture()
{
	// ��O����
	if (texture.size() <= 0)
		return false;

	for (UINT i = 0; i < texture.size(); i++)
	{
		SAFE_RELEASE(texture[i]->pD3DTexture);
		texture.erase(texture.begin() + i);
	}

	texture.clear();

	return true;
}

//������������������������������������������������
// ���_�����(�t�F�[�h�p�ȊO)
//������������������������������������������������
bool ResourceManager::destroyVtx()
{
	// ��O����
	if (vtxBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < vtxBoard.size(); i++)
		SAFE_RELEASE(vtxBoard[i]->pD3DVtxBuff);

	vtxBoard.clear();
	
	return true;
}

//��������������������������������������������
// �t�F�[�h�p���_�����
//��������������������������������������������
bool ResourceManager::destroyFadeVtx()
{
	// ��O����
	if (vtxFadeBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < vtxFadeBoard.size(); i++)
		SAFE_RELEASE(vtxFadeBoard[i]->pD3DVtxBuff);
	vtxFadeBoard.clear();

	return true;
}

//����������������������������������������������
// �e�N�X�`���쐬(�t�F�[�h)
//����������������������������������������������
void ResourceManager::createFadeTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{
	// �t�@�C�����Z�b�g
	strcpy_s(TextureData.texFileName, pszFilename);

	fadeTexture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, fadeTexture.back()->texFileName, &fadeTexture.back()->pD3DTexture))
		return;
}

void  ResourceManager::createNormalTexture(TEXTURE_DATA& TextureData, CHAR *pszFilename)
{

	// ���ɐ�������Ă��邩
	if (checkExisting(pszFilename, TextureData))
		return;

	// �t�@�C�����Z�b�g
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
		return ;
}