//������������������������������������������������������������������������������
// MyHierarchy.cpp
// �A�j���[�V�������b�V���p �K�w�\���������m��/��� �N���X
// Author : MasayaHayashi
//������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MyHierarchy.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=nullptr) { delete[] (p);   (p) = nullptr; } }
#endif
//������������������������������������������������������������������������������
// �t���[������
//������������������������������������������������������������������������������
HRESULT MyHierarchy::CreateFrame(LPCSTR pMesh, LPD3DXFRAME* ppNewFrame)
{
	*ppNewFrame = nullptr;

	// �t���[���p�������m��
	MYFRAME* pFrame = new MYFRAME;
	if (pFrame == nullptr) 
		return E_OUTOFMEMORY;

	ZeroMemory(pFrame, sizeof(MYFRAME));

	// �t���[�����̃R�s�[
	if (pMesh)
	{
		// �t���[��������
		pFrame->Name = new char[lstrlenA(pMesh) + 1];
		if (pFrame->Name == nullptr)
		{
			delete pFrame;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pFrame->Name, pMesh);
	}
	else
	{
		// �t���[�����Ȃ�
		pFrame->Name = nullptr;
	}

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->combinedTransformationMatrix);

	// �m�ۂ����t���[����Ԃ�
	*ppNewFrame = pFrame;
	return S_OK;
}

//������������������������������������������������������������������������������
// ���b�V���R���e�i����
//������������������������������������������������������������������������������
HRESULT MyHierarchy::CreateMeshContainer(LPCSTR					name,
	CONST D3DXMESHDATA*		meshDataPtr,
	CONST D3DXMATERIAL*		materialsPtr,
	CONST D3DXEFFECTINSTANCE*	effectInstancesPtr,
	DWORD						materials,
	CONST DWORD*				adjacencyPtr,
	LPD3DXSKININFO				skinInfoPtr,
	LPD3DXMESHCONTAINER*		ppNewMeshContainer)
{
	// ��肠����nullptr���Z�b�g���Ă���
	*ppNewMeshContainer = nullptr;

	// ����ȃ��b�V���ȊO�̓G���[�Ŗ߂�
	if (meshDataPtr->Type != D3DXMESHTYPE_MESH || meshDataPtr->pMesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	// ���b�V���I�u�W�F�N�g�擾
	LPD3DXMESH meshPtr = meshDataPtr->pMesh;

	// ���b�V���R���e�i�p�������̊m��
	MYMESHCONTAINER* meshContainerPtr = new MYMESHCONTAINER();

	if (meshContainerPtr == nullptr)
		return E_OUTOFMEMORY;

	ZeroMemory(meshContainerPtr, sizeof(MYMESHCONTAINER));

	// ���b�V�����̃R�s�[
	if (name)
	{
		meshContainerPtr->Name = new CHAR[lstrlenA(name) + 1];
		if (meshContainerPtr->Name == nullptr)
		{
			delete meshContainerPtr;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(meshContainerPtr->Name, name);
	}
	else
		meshContainerPtr->Name = nullptr;

	// D3D�f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = nullptr;
	meshPtr->GetDevice(&devicePtr);

	// �|���S��(�O�p�`)���擾
	INT facesAmount = meshPtr->GetNumFaces();

	// �@���L���̃`�F�b�N
	if (meshPtr->GetFVF() & D3DFVF_NORMAL)
	{
		meshContainerPtr->MeshData.pMesh = meshPtr;
		meshPtr->AddRef();
	}
	else
	{
		// �@���������Ȃ��ꍇ�͒ǉ�
		if (FAILED(meshPtr->CloneMeshFVF(meshPtr->GetOptions(), meshPtr->GetFVF() | D3DFVF_NORMAL, devicePtr, &meshContainerPtr->MeshData.pMesh)))
		{
			devicePtr->Release();
			DestroyMeshContainer(meshContainerPtr);
			return E_FAIL;
		}
		meshPtr = meshContainerPtr->MeshData.pMesh;
		D3DXComputeNormals(meshPtr, nullptr);
	}

	// �}�e���A���p�������m��
	meshContainerPtr->NumMaterials = max(1, materials);
	meshContainerPtr->pMaterials = new D3DXMATERIAL[meshContainerPtr->NumMaterials];
	meshContainerPtr->ppTextures = new LPDIRECT3DTEXTURE9[meshContainerPtr->NumMaterials];
	meshContainerPtr->pAdjacency = new DWORD[facesAmount * 3];
	if (meshContainerPtr->pAdjacency == nullptr || meshContainerPtr->ppTextures == nullptr || meshContainerPtr->pMaterials == nullptr)
	{
		DestroyMeshContainer(meshContainerPtr);
		devicePtr->Release();
		return E_OUTOFMEMORY;
	}

	ZeroMemory(meshContainerPtr->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * meshContainerPtr->NumMaterials);
	CopyMemory(meshContainerPtr->pAdjacency, adjacencyPtr, sizeof(DWORD) * facesAmount * 3);

	// �}�e���A���̐ݒ�
	if (materials > 0) 
	{
		// �J�����g�f�B���N�g����ύX
		TCHAR currentDir[_MAX_PATH];
		if (dir[0]) 
		{
			GetCurrentDirectory(_MAX_PATH, currentDir);
			SetCurrentDirectory(dir);
		}

		CopyMemory(meshContainerPtr->pMaterials, materialsPtr, sizeof(D3DXMATERIAL) * materials);
		for (DWORD i = 0; i < materials; ++i) 
		{
			meshContainerPtr->pMaterials[i].MatD3D.Ambient = meshContainerPtr->pMaterials[i].MatD3D.Diffuse;
			if (FAILED(D3DXCreateTextureFromFileA(devicePtr, meshContainerPtr->pMaterials[i].pTextureFilename, &meshContainerPtr->ppTextures[i])))
				meshContainerPtr->ppTextures[i] = nullptr;
		}

		// �J�����g�f�B���N�g�������ɖ߂�
		if (dir[0])
			SetCurrentDirectory(currentDir);
	}
	else 
	{
		ZeroMemory(meshContainerPtr->pMaterials, sizeof(D3DXMATERIAL));
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		meshContainerPtr->pMaterials[0].MatD3D.Ambient = meshContainerPtr->pMaterials[0].MatD3D.Diffuse;
	}

	// �����e�[�u�����擾
	HRESULT hr = meshContainerPtr->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED, meshContainerPtr->MeshData.pMesh->GetFVF(), devicePtr, &meshPtr);
	if (FAILED(hr))
	{
		devicePtr->Release();
		return hr;
	}
	SAFE_RELEASE(meshContainerPtr->MeshData.pMesh);
	meshContainerPtr->MeshData.pMesh = meshPtr;
	hr = meshPtr->GetAttributeTable(nullptr, &meshContainerPtr->NumAttributeGroups);
	if (FAILED(hr)) 
	{
		devicePtr->Release();
		return hr;
	}
	meshContainerPtr->pAttributeTable = new D3DXATTRIBUTERANGE[meshContainerPtr->NumAttributeGroups];
	if (meshContainerPtr->pAttributeTable == nullptr)
	{
		meshContainerPtr->NumAttributeGroups = 0;
		devicePtr->Release();
		return E_OUTOFMEMORY;
	}
	hr = meshPtr->GetAttributeTable(meshContainerPtr->pAttributeTable, nullptr);
	if (FAILED(hr))
	{
		devicePtr->Release();
		return hr;
	}

	// D3D�f�o�C�X���
	devicePtr->Release();

	// �X�L����񂪂��邩
	if (skinInfoPtr)
	{
		// �X�L������ۑ�(�Q�ƃJ�E���^���Z)
		meshContainerPtr->pSkinInfo = skinInfoPtr;
		skinInfoPtr->AddRef();

		// ���[���h�E�}�g���b�N�X�̏ꏊ���m��
		DWORD dwBoneNum = skinInfoPtr->GetNumBones();
		meshContainerPtr->pBoneOffsetMatrix = new D3DXMATRIX[dwBoneNum];
		if (meshContainerPtr->pBoneOffsetMatrix == nullptr)
			return E_OUTOFMEMORY;

		// ���[���h�E�}�g���b�N�X��������
		for (DWORD i = 0; i < dwBoneNum; ++i)
			meshContainerPtr->pBoneOffsetMatrix[i] = *skinInfoPtr->GetBoneOffsetMatrix(i);

		// �X�L���p�ɕϊ�
		hr = skinInfoPtr->ConvertToBlendedMesh(
			meshPtr, 0, meshContainerPtr->pAdjacency,
			nullptr, nullptr, nullptr,
			&meshContainerPtr->dwWeight,
			&meshContainerPtr->dwBone,
			&meshContainerPtr->pBoneBuffer,
			&meshContainerPtr->MeshData.pMesh);

		if (FAILED(hr))
			return hr;
	}

	// �m�ۂ������b�V���R���e�i��Ԃ�
	*ppNewMeshContainer = meshContainerPtr;
	return S_OK;
}

//������������������������������������������������������������������������������
// �t���[���j��
//������������������������������������������������������������������������������
HRESULT MyHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameToFree;
	SAFE_DELETE_ARRAY(pFrame->Name);
	SAFE_DELETE(pFrame);
	return S_OK;
}

//��������������������������������������������������������������������������������������������
// ���b�V���R���e�i�j��
//��������������������������������������������������������������������������������������������
HRESULT MyHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER meshContainerPtrToFree)
{
	MYMESHCONTAINER* meshContainerPtr = (MYMESHCONTAINER*)meshContainerPtrToFree;

	SAFE_DELETE_ARRAY(meshContainerPtr->Name);
	SAFE_DELETE_ARRAY(meshContainerPtr->pAdjacency);
	SAFE_DELETE_ARRAY(meshContainerPtr->pMaterials);

	// �S�e�N�X�`���̉��
	if (meshContainerPtr->ppTextures) 
	{
		for (DWORD i = 0; i < meshContainerPtr->NumMaterials; i++) 
			SAFE_RELEASE(meshContainerPtr->ppTextures[i]);
	}

	SAFE_DELETE_ARRAY(meshContainerPtr->ppTextures);

	SAFE_RELEASE(meshContainerPtr->MeshData.pMesh);

	SAFE_DELETE_ARRAY(meshContainerPtr->pAttributeTable);
	meshContainerPtr->NumAttributeGroups = 0;

	SAFE_RELEASE(meshContainerPtr->pBoneBuffer);
	SAFE_DELETE_ARRAY(meshContainerPtr->pBoneOffsetMatrix);
	SAFE_DELETE_ARRAY(meshContainerPtr->ppBoneMatrix);

	SAFE_DELETE(meshContainerPtr);
	return S_OK;
}

//������������������������������������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������������������������������������
MyHierarchy::MyHierarchy()
{
	dir[0] = 0;
}

//������������������������������������������������������������������������������
// X�t�@�C���̂���f�B���N�g����ݒ�
//������������������������������������������������������������������������������
void MyHierarchy::setDirectory(LPCTSTR pszDir)
{
	if (pszDir)
		lstrcpy(dir, pszDir);
	else
		dir[0] = 0;
}