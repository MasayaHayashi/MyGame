//����������������������������������������������������������������
// C_ResourceManager.cpp
// ���\�[�X�Ǘ�(���b�V���A�e�N�X�`���Ǘ�)
// Author : Masaya Hayashi
//����������������������������������������������������������������
#define _CRT_SECURE_NO_WARNINGS

// ===== �C���N���[�h�� =====
#include "ResourceManager.h"
#include <string>
#include <tchar.h>
#include <utility>
#include "../MyDelete/MyDelete.h"
#include "../CrtDebug/CrtDebug.h"
#include "../Pawn/Pawn.h"
#include "../Board/Board.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Pawn/Pawn.h"
#include "../Player/Player.h"
#include "../Collision/Collision.h"

// ===== �ÓI�����o�ϐ� =====
std::unique_ptr<ResourceManager>								   ResourceManager::instancePtr = nullptr;
std::vector <MeshData*>											   ResourceManager::meshes;
std::vector<TEXTURE_DATA*>										   ResourceManager::texture;	
std::vector<TEXTURE_DATA*>										   ResourceManager::fadeTexture;
std::vector<VERTEX_BOARD_DATA*>									   ResourceManager::vtxBoard;	
std::vector<VERTEX_BOARD_DATA*>									   ResourceManager::vtxFadeBoard;
std::vector<HIERARCHY_MESH_DATA*>								   ResourceManager::hierarchyMesh;
std::unordered_map<std::string, std::vector<HIERARCHY_MESH_DATA*>>	ResourceManager::resoueceMesh;


#define SAFE_RELEASE(p)		 { if (p) { (p)->Release(); (p)=NULL; }}
#define SAFE_DELETE(p)	     { if( p != nullptr ){ delete p;  p = nullptr; }}
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

}

//������������������������������������������
// ���f������(�K�w�\������)
//������������������������������������������
HRESULT ResourceManager::makeModel(MeshData &meshData, CHAR *pszFilename, MeshObjType &umeshType)
{
	// ��O����
	if (!pszFilename)
	{
		return E_FAIL;
	}

	// ���b�V���̎��
	umeshType = MeshObjType::NormalModel;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// ���ɐ�������Ă��邩
	if (checkExisting(pszFilename, meshData))
	{
		return S_OK;
	}

	// �t�@�C�����Z�b�g
	strcpy_s(meshData.meshFileName, pszFilename);

	// ���b�V�����Z�b�g
	meshes.push_back(&meshData);
	
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pszFilename, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &meshes.back()->materialBufferPtr, nullptr, &meshes.back()->numMat, &meshes.back()->meshPtr)))
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("���f���ǂݍ��݃G���["), MB_ICONERROR);
		return E_FAIL;
	}

	// �����擾�̂��߂̉�����
	LPD3DXMESH pmeshWk;
	HRESULT hr = meshes.back()->meshPtr->Optimize(D3DXMESHOPT_ATTRSORT, nullptr, nullptr, nullptr, nullptr, &pmeshWk);

	if (SUCCEEDED(hr))
	{
		//meshes.back()->meshPtr->Release();
		meshes.back()->meshPtr = pmeshWk;
	}
	else
	{
		//SAFE_RELEASE(meshes.back()->materialBufferPtr);	// �}�e���A�����
		return false;
	}

	// �����e�[�u���擾
	meshes.back()->dwAttrNum = 0;
	meshes.back()->meshPtr->GetAttributeTable(nullptr, &meshes.back()->dwAttrNum);
	meshes.back()->attrPtr = NEW D3DXATTRIBUTERANGE[meshes.back()->dwAttrNum];
	meshes.back()->meshPtr->GetAttributeTable(meshes.back()->attrPtr, &meshes.back()->dwAttrNum);

	// �w��̒��_�t�H�[�}�b�g�ɕϊ�
	DWORD dwFVF = meshes.back()->meshPtr->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pmeshTmp = meshes.back()->meshPtr.Get();
		pmeshTmp->CloneMeshFVF(pmeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &meshes.back()->meshPtr);
		delete pmeshTmp;
		pmeshTmp = nullptr;
	//	SAFE_RELEASE(pmeshTmp);
		// �@���������ꍇ�͋����I�ɒǉ�
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(meshes.back()->meshPtr.Get(), nullptr);
		}
	}

	// ���_���T�C�Y���擾
	meshes.back()->dwNumVtx = meshes.back()->meshPtr->GetNumVertices();	// ���_���擾
	meshes.back()->vertexPtr = NEW MESH_VTX[meshes.back()->dwNumVtx];
	LPVOID pWork;

	// ���_�o�b�t�@�A�����b�N
	meshes.back()->meshPtr->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->vertexPtr, pWork, sizeof(MESH_VTX) * meshes.back()->dwNumVtx);

	// ���_�o�b�t�@���b�N
	meshes.back()->meshPtr->UnlockVertexBuffer();

	// �C���f�b�N�X�o�b�t�@����ǂݍ���
	meshes.back()->dwNumIndx = meshes.back()->meshPtr->GetNumFaces() * 3;
	meshes.back()->indexPtr = NEW WORD[meshes.back()->dwNumIndx];

	// �C���f�b�N�X�o�b�t�@���b�N
	meshes.back()->meshPtr->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(meshes.back()->indexPtr, pWork, sizeof(WORD) * meshes.back()->dwNumIndx);

	// �C���f�b�N�X�o�b�t�@�A�����b�N
	meshes.back()->meshPtr->UnlockIndexBuffer();

	// ���E�{�b�N�X�ƒ��S���W�����߂�
	meshes.back()->maxVtx = meshes.back()->vertexPtr[0].VtxPos;
	meshes.back()->minVtx = meshes.back()->vertexPtr[0].VtxPos;
	for (WORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		// �ő�l�擾
		if (meshes.back()->maxVtx.x < meshes.back()->vertexPtr[i].VtxPos.x)
		{
			meshes.back()->maxVtx.x = meshes.back()->vertexPtr[i].VtxPos.x;
		}

		if (meshes.back()->maxVtx.y < meshes.back()->vertexPtr[i].VtxPos.y)
		{
			meshes.back()->maxVtx.y = meshes.back()->vertexPtr[i].VtxPos.y;
		}

		if (meshes.back()->maxVtx.z < meshes.back()->vertexPtr[i].VtxPos.z)
		{
			meshes.back()->maxVtx.z = meshes.back()->vertexPtr[i].VtxPos.z;
		}

		// �ŏ��l�擾
		if (meshes.back()->minVtx.x > meshes.back()->vertexPtr[i].VtxPos.x)
		{
			meshes.back()->minVtx.x = meshes.back()->vertexPtr[i].VtxPos.x;
		}

		if (meshes.back()->minVtx.y > meshes.back()->vertexPtr[i].VtxPos.y)
		{
			meshes.back()->minVtx.y = meshes.back()->vertexPtr[i].VtxPos.y;
		}
		if (meshes.back()->minVtx.z > meshes.back()->vertexPtr[i].VtxPos.z)
		{
			meshes.back()->minVtx.z = meshes.back()->vertexPtr[i].VtxPos.z;
		}
	}

	// ���S���W�A�����蔻��擾
	meshes.back()->centerPos = (meshes.back()->maxVtx + meshes.back()->minVtx) * 0.5f;
	meshes.back()->collitionBox = (meshes.back()->maxVtx - meshes.back()->minVtx) * 0.5f;

	// ���E���擾
	FLOAT dx = meshes.back()->vertexPtr->VtxPos.x - meshes.back()->centerPos.x;
	FLOAT dy = meshes.back()->vertexPtr->VtxPos.y - meshes.back()->centerPos.y;
	FLOAT dz = meshes.back()->vertexPtr->VtxPos.z - meshes.back()->centerPos.z;
	meshes.back()->collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < meshes.back()->dwNumVtx; i++)
	{
		dx = meshes.back()->vertexPtr[i].VtxPos.x - meshes.back()->centerPos.x;
		dy = meshes.back()->vertexPtr[i].VtxPos.y - meshes.back()->centerPos.y;
		dz = meshes.back()->vertexPtr[i].VtxPos.z - meshes.back()->centerPos.z;
		FLOAT fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (meshes.back()->collisionRadus < fRadius)
		{
			meshes.back()->collisionRadus = fRadius;
		}
	}

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
	{
		makeVtxFade(VtxBordData);
	}
	else
	{
		makeVtx(VtxBordData);
	}

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
		if (FAILED(DirectX3D::getDevice()->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&vtxBoard.back()->pD3DVtxBuff,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))										// NULL�ɐݒ�
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

		// ���_�f�[�^���A�����b�N����
		vtxBoard.back()->pD3DVtxBuff->Unlock();
		return true;
	}
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
	{
		return E_FAIL;
	}

	// ���ɐ�������Ă��邩
	if (checkExisting(pszFilename, TextureData))
	{
		return S_OK;
	}

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
HRESULT ResourceManager::makeModelHierarchy(HIERARCHY_MESH_DATA &setHierarchyMeshData,CHAR *setFilename,std::string keyName, MeshObjType &meshType,UINT index)
{
#if 0
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// ���b�V���̎�ޏ�����
	meshType = MeshObjType::HierarchyModel;

	strcpy_s(setHierarchyMeshData.meshFileName, setFilename);

	hierarchyMesh.push_back(&setHierarchyMeshData);

	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(hierarchyMesh.back()->meshFileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	hierarchyMesh.back()->Hierarchy.setDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(hierarchyMesh.back()->meshFileName, D3DXMESH_MANAGED, pDevice, &hierarchyMesh.back()->Hierarchy, NULL, &hierarchyMesh.back()->pFrameRoot, &hierarchyMesh.back()->pAnimCtrl);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �{�[���ƃt���[���̊֘A�t��
	hr = allocAllBoneMatrix(hierarchyMesh.back()->pFrameRoot, setFilename);
	if (FAILED(hr))
	{
		return false;
	}

	// �A�j���[�V�����Z�b�g�擾
	hierarchyMesh.back()->uNumAnimSet = 0;
	if (hierarchyMesh.back()->pAnimCtrl)
	{
		hierarchyMesh.back()->uNumAnimSet = hierarchyMesh.back()->pAnimCtrl->GetNumAnimationSets();
		if (hierarchyMesh.back()->uNumAnimSet > 0)
		{
			hierarchyMesh.back()->ppAnimSet = NEW LPD3DXANIMATIONSET[hierarchyMesh.back()->uNumAnimSet];
			for (DWORD u = 0; u < hierarchyMesh.back()->uNumAnimSet; u++)
			{
				hierarchyMesh.back()->pAnimCtrl->GetAnimationSet(u, &hierarchyMesh.back()->ppAnimSet[u]);
			}
		}
	}

	if (hierarchyMesh.back()->pFrameRoot)
	{
		// �}�g���b�N�X�X�V
		setTime(0.0, setFilename,keyName,index);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(hierarchyMesh.back()->pFrameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		calcCollision(hierarchyMesh.back()->pFrameRoot, setFilename);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	hierarchyMesh.back()->dwPrev = timeGetTime();

	return SUCCEEDED(hr);
#endif
	return E_FAIL;
}

//������������������������������������������
// �K�w�\���p���f���ǂݍ���
//������������������������������������������
HRESULT ResourceManager::makeModelHierarchyResouce(HIERARCHY_MESH_DATA &setHierarchyMeshData, CHAR *setFilename, std::string keyName, MeshObjType &meshType,UINT index)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// ���b�V���̎�ޏ�����
	meshType = MeshObjType::HierarchyModel;

	resoueceMesh[keyName].push_back(NEW HIERARCHY_MESH_DATA());
	
	strcpy_s(resoueceMesh[keyName][index]->meshFileName, setFilename);

	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath(resoueceMesh[keyName][index]->meshFileName, szDir, szDirWk, NULL, NULL);
	lstrcat(szDir, szDirWk);
	resoueceMesh[keyName][index]->Hierarchy.setDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(resoueceMesh[keyName][index]->meshFileName, D3DXMESH_MANAGED, pDevice, &resoueceMesh[keyName][index]->Hierarchy, NULL, &resoueceMesh[keyName][index]->pFrameRoot, &resoueceMesh[keyName][index]->pAnimCtrl);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �{�[���ƃt���[���̊֘A�t��
	hr = allocAllBoneMatrix(resoueceMesh[keyName][index]->pFrameRoot, setFilename,keyName,index);
	if (FAILED(hr))
	{
		return false;
	}

	// �A�j���[�V�����Z�b�g�擾
	resoueceMesh[keyName][index]->uNumAnimSet = 0;
	if (resoueceMesh[keyName][index]->pAnimCtrl)
	{
		resoueceMesh[keyName][index]->uNumAnimSet = resoueceMesh[keyName][index]->pAnimCtrl->GetNumAnimationSets();
		if (resoueceMesh[keyName][index]->uNumAnimSet > 0)
		{
			resoueceMesh[keyName][index]->ppAnimSet = NEW LPD3DXANIMATIONSET[resoueceMesh[keyName][index]->uNumAnimSet];
			for (DWORD u = 0; u < resoueceMesh[keyName][index]->uNumAnimSet; u++)
			{
				resoueceMesh[keyName][index]->pAnimCtrl->GetAnimationSet(u, &resoueceMesh[keyName][index]->ppAnimSet[u]);
			}
		}
	}

	if (resoueceMesh[keyName][index]->pFrameRoot)
	{
		// �}�g���b�N�X�X�V
		setTime(0.0, setFilename,keyName,index);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(resoueceMesh[keyName][index]->pFrameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		calcCollision(resoueceMesh[keyName][index]->pFrameRoot, setFilename,keyName,index);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	resoueceMesh[keyName][index]->dwPrev = timeGetTime();

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
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	}
	else
	{
		pFrame->combinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	// �Z��t���[��������΃}�g���b�N�X���X�V
	if (pFrame->pFrameSibling)
	{
		updateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
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
void ResourceManager::setTime(DOUBLE dTime,CHAR *pszFilename,std::string keyName,UINT index)
{
	if (resoueceMesh[keyName][index]->pAnimCtrl == nullptr)
	{
		return;
	}

	for (DWORD i = 0; i < resoueceMesh[keyName][index]->pAnimCtrl->GetMaxNumTracks(); ++i)
	{
		resoueceMesh[keyName][index]->pAnimCtrl->SetTrackPosition(i, 0);
	}

	resoueceMesh[keyName][index]->pAnimCtrl->ResetTime();
	resoueceMesh[keyName][index]->pAnimCtrl->AdvanceTime(dTime, nullptr);
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT ResourceManager::allocBoneMatrix(LPD3DXMESHCONTAINER meshContainerPtrBase, CHAR *pszFilename,std::string keyName,UINT index)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrBase;

	if (meshContainerPtr->pSkinInfo == nullptr)
	{
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�
	}

	DWORD dwBoneNum = meshContainerPtr->pSkinInfo->GetNumBones();
	meshContainerPtr->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(resoueceMesh[keyName][index]->pFrameRoot, meshContainerPtr->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
		{
			return E_FAIL;
		}

		meshContainerPtr->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT ResourceManager::allocAllBoneMatrix(LPD3DXFRAME pFrameBase, CHAR *pszFilename,std::string keyName,UINT index)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer)
	{
		hr = allocBoneMatrix(pFrame->pMeshContainer,pszFilename,keyName,index);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameSibling)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameSibling,pszFilename,keyName,index);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameFirstChild)
	{
		hr = allocAllBoneMatrix(pFrame->pFrameFirstChild, pszFilename,keyName,index);
	}

	return hr;
}

//������������������������������������������
//  �K�w�\���p ���E��/���E�{�b�N�X�擾
//������������������������������������������
void ResourceManager::calcCollision(LPD3DXFRAME pFrame,CHAR *pszFilename,std::string keyName,UINT index)
{
	D3DXVECTOR3& vMax = resoueceMesh[keyName][index]->CollitionBox;
	D3DXVECTOR3& vMin = resoueceMesh[keyName][index]->CenterPos;

	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	
	resoueceMesh[keyName][index]->fCollisionRadus = -1.0f;
	calcCollisionFrame(pFrame,pszFilename,keyName,index);
	
	D3DXVECTOR3 vBBox, vCenter;
	vBBox = (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	
	resoueceMesh[keyName][index]->CollitionBox = vBBox;
	resoueceMesh[keyName][index]->CenterPos = vCenter;
	resoueceMesh[keyName][index]->fCollisionRadus = 0.0f;


	calcCollisionFrame(pFrame,pszFilename,keyName,index);
}


//��������������������������������������������������������������������
// �t���[�����̒��_���W�̒��o
//��������������������������������������������������������������������
void ResourceManager::calcCollisionFrame(LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName,UINT index)
{
	LPD3DXMESHCONTAINER meshContainerPtr = pFrame->pMeshContainer;
	while (meshContainerPtr)
	{
		// ���b�V���R���e�i���̒��_���W�̒��o
		calcCollisionMeshContainer(meshContainerPtr, pFrame,pszFilename,keyName,index);
		// ���̃��b�V���R���e�i
		meshContainerPtr = meshContainerPtr->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
	{
		calcCollisionFrame(pFrame->pFrameSibling, pszFilename,keyName,index);
	}
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
	{
		calcCollisionFrame(pFrame->pFrameFirstChild, pszFilename,keyName,index);
	}
}

//��������������������������������������������������������������������
// ���b�V���R���e�i���̒��_���W�̒��o
//��������������������������������������������������������������������
void ResourceManager::calcCollisionMeshContainer(LPD3DXMESHCONTAINER meshContainerPtr, LPD3DXFRAME pFrame, CHAR *pszFilename,std::string keyName,UINT index)
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
		if (resoueceMesh[keyName][index]->fCollisionRadus < 0.0f)
		{
			D3DXVECTOR3& vMax = resoueceMesh[keyName][index]->CollitionBox;
			D3DXVECTOR3& vMin = resoueceMesh[keyName][index]->CenterPos;
			if (vMax.x < vtx.x) { vMax.x = vtx.x; }
			if (vMax.y < vtx.y) { vMax.y = vtx.y; }
			if (vMax.z < vtx.z) { vMax.z = vtx.z; }
			if (vMin.x > vtx.x) { vMin.x = vtx.x; }
			if (vMin.y > vtx.y) { vMin.y = vtx.y; }
			if (vMin.z > vtx.z) { vMin.z = vtx.z; }
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - resoueceMesh[keyName][index]->CenterPos));
			if (resoueceMesh[keyName][index]->fCollisionRadus < fRadius)
			{
				resoueceMesh[keyName][index]->fCollisionRadus = fRadius;
			}
		}
	}
	pmesh->UnlockVertexBuffer();
}

//����������������������������������������������������������
// ���b�V����񂪂��łɂ��邩����
// �����ς݂Ȃ猳�̃f�[�^���g�p
//����������������������������������������������������������
bool ResourceManager::checkExisting(CHAR *pszChakNeme,MeshData &meshDataPtr)
{
	// �S���b�V��������
	for (UINT i = 0; i < meshes.size(); i++)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			meshDataPtr.attrPtr = (meshes[i]->attrPtr);
			meshDataPtr.centerPos = meshes[i]->centerPos;
			meshDataPtr.collisionRadus = meshes[i]->collisionRadus;
			meshDataPtr.collitionBox = meshes[i]->collitionBox;
			meshDataPtr.dwAttrNum = meshes[i]->dwAttrNum;
			meshDataPtr.dwNumIndx = meshes[i]->dwNumIndx;
			meshDataPtr.dwNumTriangles = meshes[i]->dwNumTriangles;
			meshDataPtr.dwNumVtx = meshes[i]->dwNumVtx;
			meshDataPtr.indexPtr = (meshes[i]->indexPtr);
			meshDataPtr.materialBufferPtr = meshes[i]->materialBufferPtr;
			meshDataPtr.maxVtx = meshes[i]->maxVtx;
			strcpy_s(meshDataPtr.meshFileName, meshes[i]->meshFileName);
			meshDataPtr.meshPtr = meshes[i]->meshPtr.Get();
			meshDataPtr.minVtx = meshes[i]->minVtx;
			meshDataPtr.numMat = meshes[i]->numMat;
			meshDataPtr.vertexPtr = (meshes[i]->vertexPtr);


			return true;
		}
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
	{
		if (strcmp(texture[i]->texFileName, pszChakNeme) == 0)
		{
			TextureData = *texture[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
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
	{
		if (strcmp(vtxBoard[i]->name, pszChakNeme) == 0)
		{
			TextureData = *vtxBoard[i];	// �����ς݂Ȃ猳�̃f�[�^���g�p
			return true;
		}
	}
	return false;
}

//����������������������������������������������������������
// ���b�V�����
//����������������������������������������������������������
bool ResourceManager::destroyMesh(CHAR *pszChakNeme)
{
	
	for (INT i = (INT)meshes.size() - 1; i >= 0; i--)
	{
		if (strcmp(meshes[i]->meshFileName, pszChakNeme) == 0)
		{
			delete meshes[i]->attrPtr;
			delete meshes[i]->indexPtr;
			delete meshes[i]->vertexPtr;
			meshes.erase(meshes.begin() + i);
			return true;
		}
	
	}
	
	return false;
}

//����������������������������������������������������������
// �S���b�V�����
//����������������������������������������������������������
bool ResourceManager::destroyAllMesh()
{
	if (meshes.empty())
	{
		return true;
	}

	for (INT i = 0; i < static_cast<INT>(meshes.size()); i++)
	{
		delete meshes[i]->attrPtr;
		meshes[i]->attrPtr = nullptr;

		delete meshes[i]->indexPtr;
		meshes[i]->indexPtr = nullptr;

		delete meshes[i]->vertexPtr;
		meshes[i]->vertexPtr = nullptr;
	}

	meshes.clear();
	
	return true;
}

//����������������������������������������������������������
// �K�w�\���p���f�����
//����������������������������������������������������������
bool ResourceManager::destroyHierarchymesh(CHAR *pszChakNeme,std::string keyName)
{
	
	for (auto mesh : hierarchyMesh)
	{

		// �A�j���[�V�������
		if (hierarchyMesh.back()->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchyMesh.back()->uNumAnimSet; j++)
			{
				hierarchyMesh.back()->ppAnimSet[j]->Release();
				hierarchyMesh.back()->ppAnimSet[j] = nullptr;
			}
		}
		else
		{
			return false;
		}

		hierarchyMesh.back()->pAnimCtrl->Release();
		hierarchyMesh.back()->pAnimCtrl = nullptr;

			// ���b�V�����
		if (hierarchyMesh.back()->pFrameRoot)
		{
			D3DXFrameDestroy(hierarchyMesh.back()->pFrameRoot, &hierarchyMesh.back()->Hierarchy);
			hierarchyMesh.back()->pFrameRoot = nullptr;

			if (hierarchyMesh.empty())
			{
				hierarchyMesh.clear();
			}
			else
			{
				hierarchyMesh.pop_back();
			}

			//return true;
		}
		else
		{
			return false;
		}
	}

	
	return false;
}

//����������������������������������������������������������������
// �S�K�w�\���p���b�V�������
//����������������������������������������������������������������
bool ResourceManager::destroyAllHierarchymesh()
{
	for (INT i = hierarchyMesh.size() - 1; i >= 0; i--)
	{
		// �A�j���[�V�������
		if (hierarchyMesh[i]->ppAnimSet)
		{
			for (UINT j = 0; j < hierarchyMesh[i]->uNumAnimSet; j++)
			{
				SAFE_RELEASE(hierarchyMesh[i]->ppAnimSet[j]);
			}
			SAFE_DELETE_ARRAY(hierarchyMesh[i]->ppAnimSet);
		}
		SAFE_RELEASE(hierarchyMesh[i]->pAnimCtrl);

		// ���b�V�����
		if (hierarchyMesh[i]->pFrameRoot)
		{
			D3DXFrameDestroy(hierarchyMesh[i]->pFrameRoot, &hierarchyMesh[i]->Hierarchy);
 			hierarchyMesh[i]->pFrameRoot = nullptr;
		}

	}
	hierarchyMesh.clear();
	return true;
}

//������������������������������������������������
// ���\�[�X���
//������������������������������������������������
bool ResourceManager::destroyAllResouce(size_t index)
{
	destroyResouceMesh(ModelChick,index);
	destroyResouceMesh(ModelPenchanPass, index);
	destroyResouceMesh(ModelPenNoHahaPass,index);
	
	size_t maxPlayer =Collision::getSize("Player");

	if (index == maxPlayer - 1)
	{
		resoueceMesh[ModelChick].clear();
		resoueceMesh[ModelPenchanPass].clear();
		resoueceMesh[ModelPenNoHahaPass].clear();

		resoueceMesh.clear();
	}

	return true;
}

//������������������������������������������������������������
// ���\�[�X���
//������������������������������������������������������������
void ResourceManager::destroyResouceMesh(std::string keyName,UINT index)
{
	// �A�j���[�V�������
	if (resoueceMesh[keyName][index]->ppAnimSet)
	{
		for (UINT j = 0; j < resoueceMesh[keyName][index]->uNumAnimSet; j++)
		{
			SAFE_RELEASE(resoueceMesh[keyName][index]->ppAnimSet[j]);
		}
		SAFE_DELETE_ARRAY(resoueceMesh[keyName][index]->ppAnimSet);
	}
	SAFE_RELEASE(resoueceMesh[keyName][index]->pAnimCtrl);

	// ���b�V�����
	if (resoueceMesh[keyName][index]->pFrameRoot)
	{
		D3DXFrameDestroy(resoueceMesh[keyName][index]->pFrameRoot, &resoueceMesh[keyName][index]->Hierarchy);
		resoueceMesh[keyName][index]->pFrameRoot = nullptr;
	}

	Mydelete::safeDelete(resoueceMesh[keyName][index]);
	
}

//������������������������������������������������
// �e�N�X�`�����
//������������������������������������������������
bool ResourceManager::destroyAllTexture()
{
	
	// ��O����
	if (texture.size() <= 0)
	{
		return false;
	}

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
	/*
	// ��O����
	if (vtxBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < vtxBoard.size(); i++)
		SAFE_RELEASE(vtxBoard[i]->pD3DVtxBuff);

	vtxBoard.clear();
	
	*/
	return true;
}

//��������������������������������������������
// �t�F�[�h�p���_�����
//��������������������������������������������
bool ResourceManager::destroyFadeVtx()
{
	/*
	// ��O����
	if (vtxFadeBoard.size() <= 0)
		return false;

	// ���_�����
	for (UINT i = 0; i < vtxFadeBoard.size(); i++)
		SAFE_RELEASE(vtxFadeBoard[i]->pD3DVtxBuff);
	vtxFadeBoard.clear();

	*/
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
	{
		return;
	}

	// �t�@�C�����Z�b�g
	strcpy_s(TextureData.texFileName, pszFilename);

	texture.push_back(&TextureData);

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, texture.back()->texFileName, &texture.back()->pD3DTexture))
		return ;
}

//��������������������������������������������
// �C���X�^���X����
//��������������������������������������������
bool ResourceManager::createInstance()
{
	if (instancePtr.get() == nullptr)
	{
		instancePtr.reset(NEW ResourceManager());

		return true;
	}
	else
	{
		return false;
	}
}

//������������������������������������������
// ���f���ύX
//������������������������������������������
void ResourceManager::changeHierarchy(HIERARCHY_MESH_DATA &changeHierarchy,CHAR* keyName,UINT index)
{

	changeHierarchy = *resoueceMesh[keyName][index];
}

//����������������������������������������
// �v���C���[�p����
//����������������������������������������
HRESULT ResourceManager::makeHierarchyResouce(HIERARCHY_MESH_DATA& meshData,std::string setName,CHAR* keyName,UINT index)
{
	return E_FAIL;
}

//����������������������������������������
// �K�w�\���Z�b�g
//����������������������������������������
void ResourceManager::setHierarchy(HIERARCHY_MESH_DATA *meshData,std::string keyName,UINT index)
{
	std::memcpy(meshData, resoueceMesh[keyName][index], sizeof(HIERARCHY_MESH_DATA));
	meshData->CollitionBox = resoueceMesh[keyName][index]->CollitionBox;
}

//����������������������������������������
// �K�w�\���p�A�j���[�V�����f�[�^�擾
//����������������������������������������
LPD3DXANIMATIONSET& ResourceManager::getAnimData(size_t index)
{
	return resoueceMesh["Data/Model/Character/PenChan/PenguinC.x"][0]->ppAnimSet[index];
}