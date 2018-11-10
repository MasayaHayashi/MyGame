//����������������������������������������������������������������
// Pawn.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Pawn.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Collider/Collider.h"
#include "../Figure/Figure.h"
#include <tchar.h>
#include "../KeyBoard/Keyboard.h"

// ===== �O���[�o���ϐ��錾 =====
#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}

//������������������������������������������
// �R���X�g���N�^
//������������������������������������������
Pawn::Pawn()
{
	// �`��֘A������
	cross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	oldRadRot = 0.0f;

	// �s�񏉊���
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&translateMtx);

	// �e��|�C���^������
	meshData.pVtx			= nullptr;
	meshData.pIndx			= nullptr;
	meshData.pAttr			= nullptr;
	textureData.pD3DTexture = nullptr;
	meshData.pD3DXMesh		= nullptr;
	meshData.pD3DXBuffMat	= nullptr;

	// �ʒu�E�����̏����ݒ�
	pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	accele			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	collisionSize	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	destLanding		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	quatanion	= D3DXQUATERNION(0, 0, 0, 0);
	destQua		= D3DXQUATERNION(0, 0, 0, 0);

	speed = 0.0f;

	hitIndex = -1;

	collisionRadus = 0.0f;
	colorAlpha		= 1.0f;
	maxTrackTime	= 0.0f;

	curSelectAnim	= 0;
	endAnim			= false;
	isHit			= false;
	isGround		= false;

	currentAnim	= 0;
}

//������������������������������������������
// �R���X�g���N�^(���ʗp�Y�����ԍ��w��)
//������������������������������������������
Pawn::Pawn(UINT SetuIndxNum)
{
	// �`��֘A������
//	pD3DXMesh = nullptr;

	// �ʒu�E�����̏����ݒ�
	pos			  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale		  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	move		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot			  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotDest		  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	collisionSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	collisionRadus  = 0.0f;
	colorAlpha		 = 1.0f;
	maxTrackTime	 = 0.0f;

	dwNumIndx		 = 0;
	curSelectAnim	 = 0;
	endAnim		 = false;

	idNumber = SetuIndxNum;
}

//������������������������������������������
// �f�X�g���N�^
//������������������������������������������
Pawn::~Pawn()
{

}

//����������������������������������
// ������
//����������������������������������
void Pawn::initialize()
{

}

//����������������������������������
// �㏈��
//����������������������������������
void Pawn::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//����������������������������������
// �|�[���X�V(���ʏ���)
//����������������������������������
void Pawn::update()
{

}


//����������������������������������
// �`��
//����������������������������������
void Pawn::draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ��O����
	if (!isUsed)
		return;

	// �ړ��A��]�A�g��k���p�s��
	D3DXMATRIX		mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL	*pD3DXMat;
	D3DMATERIAL9	matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	// �g��k��
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	devicePtr->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshData.numMat); nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);

}

//����������������������������������
// �`��
//����������������������������������
void Pawn::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ��O����
	if (!isUsed)
		return;

	// �ړ��A��]�A�g��k���p�s��
	D3DXMATRIX		mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL	*pD3DXMat;
	D3DMATERIAL9	matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	// �g��k��
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);
	
	// ���[���h�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	devicePtr->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
 	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();


	LPD3DXEFFECT pEffect = DirectX3D::getEffect();

	// 
	pEffect->SetTechnique("tecMinimum");

	D3DXMATRIX mtxAll = worldMtx * mtxView * mtxProj;
	pEffect->SetMatrix("mWVP", &mtxAll);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (int nCntMat = 0; nCntMat < (int)meshData.numMat; nCntMat++)
	{

		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	pEffect->EndPass();
	pEffect->End();

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);
	
}

//����������������������������������
// �`��
//����������������������������������
void Pawn::draw(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff,DWORD uNumMat)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �ړ��A��]�A�g��k���p�s��
	D3DXMATRIX mtxRot, translateMtx, mtxScale;

	//	D3DXMATRIX mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&worldMtx);
	D3DXMatrixIdentity(&mtxScale);

	devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// �g��k��
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &rotMtx);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	// ���[���h�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	devicePtr->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshData.numMat); nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);

}

//������������������������������������������������������
// ���ݎ��g�������Ă��郏�[���h�s������̂܂܎g���`��
//������������������������������������������������������
void Pawn::drawObjectLocal()
{
	// ��O����
	if (!isUsed)
		return;

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//	D3DXMATRIX mtxRot, translateMtx, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	devicePtr->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)meshData.pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)meshData.numMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshData.pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);
}

//������������������������������������������������������������������������
// �t���[���̕`��
//������������������������������������������������������������������������
void Pawn::drawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;

	while (pMeshContainer)
	{
		// ���b�V���R���e�i�̕`��
		RenderMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		drawFrame(pFrame->pFrameSibling);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		drawFrame(pFrame->pFrameFirstChild);
}

//��������������������������������������������������������������������������������������������������
// ���b�V���R���e�i�̕`��
//��������������������������������������������������������������������������������������������������
void Pawn::RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	if (pMeshContainer->pSkinInfo)
	{
		// �X�L�����b�V�����f��
		LPD3DXBONECOMBINATION pBoneCombi = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		DWORD dwPrevBoneID = UINT_MAX;
		DWORD dwBlendMatrix;
		for (DWORD i = 0; i < pMeshContainer->dwBone; i++)
		{
			dwBlendMatrix = 0;
			for (DWORD k = 0; k < pMeshContainer->dwWeight; ++k)
			{
				if (pBoneCombi[i].BoneId[k] != UINT_MAX)
					dwBlendMatrix = k;
			}
			devicePtr->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrix);
			for (DWORD k = 0; k < pMeshContainer->dwWeight; k++)
			{
				DWORD id = pBoneCombi[i].BoneId[k];
				if (id != UINT_MAX) 
				{
					devicePtr->SetTransform(D3DTS_WORLDMATRIX(k), &(pMeshContainer->pBoneOffsetMatrix[id] * *pMeshContainer->ppBoneMatrix[id]));
				}
			}
			devicePtr->SetMaterial(&pMeshContainer->pMaterials[pBoneCombi[i].AttribId].MatD3D);
			devicePtr->SetTexture(0, pMeshContainer->ppTextures[pBoneCombi[i].AttribId]);
			dwPrevBoneID = pBoneCombi[i].AttribId;

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		return;
	}
	// �X�L���������f��
	devicePtr->SetTransform(D3DTS_WORLD, &pFrame->combinedTransformationMatrix);
	for (DWORD iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; iAttrib++)
	{
		DWORD dwAttrib = pMeshContainer->pAttributeTable[iAttrib].AttribId;
		devicePtr->SetMaterial(&pMeshContainer->pMaterials[dwAttrib].MatD3D);
		devicePtr->SetTexture(0, pMeshContainer->ppTextures[dwAttrib]);
		pMeshContainer->MeshData.pMesh->DrawSubset(dwAttrib);
	}
}

//������������������������������������������������
// ���s�ړ��Z�b�g
//������������������������������������������������
void Pawn::setTranslate(D3DXVECTOR3 pos)
{
	// �ړ��𔽉f
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);

	D3DXMatrixInverse(&mtxInverse, nullptr, &translateMtx);	// �t�s�񐶐�
	mtxOldTranslate = translateMtx;
}

//������������������������������������������
// ���f������(�K�w�\������)
//������������������������������������������
HRESULT Pawn::makeModel()
{
	// ��O����
	if (!fileName)
		return E_FAIL;
//	if (pD3DXMesh)
//		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &pD3DXBuffMat, nullptr, &uNumMat, &pD3DXMesh)))
	{
		return E_FAIL;
	}

	// �����擾�̂��߂̉�����
	LPD3DXMESH pMeshWk;
	HRESULT hr = pD3DXMesh->Optimize(D3DXMESHOPT_ATTRSORT,nullptr, nullptr, nullptr, nullptr, &pMeshWk);

	if (SUCCEEDED(hr)) 
	{
		pD3DXMesh->Release();
		pD3DXMesh = pMeshWk;
	}
	else
	{
		SAFE_RELEASE(pD3DXBuffMat);
		return false;
	}

	// �����e�[�u���擾
	dwAttrNum = 0;
	pD3DXMesh->GetAttributeTable(nullptr, &dwAttrNum);
	pAttr = new D3DXATTRIBUTERANGE[dwAttrNum];
	pD3DXMesh->GetAttributeTable(pAttr, &dwAttrNum);

	// �w��̒��_�t�H�[�}�b�g�ɕϊ�
	DWORD dwFVF = pD3DXMesh->GetFVF();
	if (dwFVF != FVF_TVERTEX)
	{
		LPD3DXMESH pMeshTmp = pD3DXMesh;
		pMeshTmp->CloneMeshFVF(pMeshTmp->GetOptions(), FVF_TVERTEX, devicePtr, &pD3DXMesh);
		SAFE_RELEASE(pMeshTmp);
		// �@���������ꍇ�͋����I�ɒǉ�
		if ((dwFVF & D3DFVF_NORMAL) == 0)
		{
			D3DXComputeNormals(pD3DXMesh, nullptr);
		}
	}


	// ���_���T�C�Y���擾
	dwNumVtx = pD3DXMesh->GetNumVertices();	// ���_���擾
	pVtx = new MESH_VTX[dwNumVtx];
	LPVOID pWork;

	// ���_�o�b�t�@�A�����b�N
	pD3DXMesh->LockVertexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(pVtx, pWork, sizeof(MESH_VTX) * dwNumVtx);

	// ���_�o�b�t�@���b�N
	pD3DXMesh->UnlockVertexBuffer();

	// �C���f�b�N�X�o�b�t�@����ǂݍ���
	dwNumIndx = pD3DXMesh->GetNumFaces() * 3;
	pIndx = new WORD[dwNumIndx];

	// �C���f�b�N�X�o�b�t�@���b�N
	pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, &pWork);
	CopyMemory(pIndx, pWork, sizeof(WORD) * dwNumIndx);

	// �C���f�b�N�X�o�b�t�@�A�����b�N
	pD3DXMesh->UnlockIndexBuffer();

	// ���E�{�b�N�X�ƒ��S���W�����߂�
	maxVtx = pVtx[0].VtxPos;
	minVtx = pVtx[0].VtxPos;
	for (WORD i = 1; i < dwNumVtx; i++)
	{
		// �ő�l�擾
		if (maxVtx.x < pVtx[i].VtxPos.x)
			maxVtx.x = pVtx[i].VtxPos.x;

		if (maxVtx.y < pVtx[i].VtxPos.y)
			maxVtx.y = pVtx[i].VtxPos.y;

		if (maxVtx.z < pVtx[i].VtxPos.z)
			maxVtx.z = pVtx[i].VtxPos.z;

		// �ŏ��l�擾
		if (minVtx.x > pVtx[i].VtxPos.x)
			minVtx.x = pVtx[i].VtxPos.x;

		if (minVtx.y > pVtx[i].VtxPos.y)
			minVtx.y = pVtx[i].VtxPos.y;

		if (minVtx.z > pVtx[i].VtxPos.z)
			minVtx.z = pVtx[i].VtxPos.z;
	}

	// �ő�A�ŏ��l���X�P�[���ɍ��킹��


	// ���S���W�A�����蔻��擾
	centerPos	 = (maxVtx + minVtx) * 0.5f;
	collitionBox = (maxVtx - minVtx) * 0.5f;

	// ���E���擾
	float dx = pVtx[0].VtxPos.x - centerPos.x;
	float dy = pVtx[0].VtxPos.y - centerPos.y;
	float dz = pVtx[0].VtxPos.z - centerPos.z;
	collisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < dwNumVtx; i++) 
	{
		dx = pVtx[i].VtxPos.x - centerPos.x;
		dy = pVtx[i].VtxPos.y - centerPos.y;
		dz = pVtx[i].VtxPos.z - centerPos.z;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (collisionRadus < fRadius)
			collisionRadus = fRadius;
	}

	// ���S���W�����[���h���W�n�ɕϊ�
	centerPos += pos;

	return S_OK;
}

//������������������������������������������
// �K�w�\���p���f���ǂݍ���
//������������������������������������������
HRESULT Pawn::makeModelHierarchy()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �f�B���N�g�����o
	TCHAR szDir[_MAX_PATH];
	TCHAR szDirWk[_MAX_DIR];
	_tsplitpath_s(fileName, szDir, sizeof(szDir), szDirWk, sizeof(szDirWk), nullptr, 0, nullptr, 0);
	lstrcat(szDir, szDirWk);
	Hierarchy.setDirectory(szDir);

	// �K�w�\�����b�V���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshHierarchyFromX(fileName, D3DXMESH_MANAGED, devicePtr, &Hierarchy, nullptr, &frameRoot, &animCtrlPtr);
	if (FAILED(hr))
		return false;

	// �{�[���ƃt���[���̊֘A�t��
	hr = AllocAllBoneMatrix(frameRoot);
	if (FAILED(hr)) return false;

	// �A�j���[�V�����Z�b�g�擾
	numAnimset = 0;
	if (animCtrlPtr)
	{
		numAnimset = animCtrlPtr->GetNumAnimationSets();
		if (numAnimset > 0)
		{
			ppAnimSet = new LPD3DXANIMATIONSET[numAnimset];
			for (DWORD u = 0; u < numAnimset; u++)
			{
				animCtrlPtr->GetAnimationSet(u, &ppAnimSet[u]);
			}
		}
	}

	if (frameRoot)
	{
		// �}�g���b�N�X�X�V
		setTime(0.0);
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		updateFrameMatrices(frameRoot, &world);

		// ���E��/���E�{�b�N�X�擾
		calcCollision(frameRoot);
	}

	// �o�ߎ��Ԍv���p�����ݒ�
	dwPrev = timeGetTime();

	return SUCCEEDED(hr);
}


//������������������������������������������������������������������������������������������
// �t���[���̃}�g���b�N�X���X�V
//������������������������������������������������������������������������������������������
void Pawn::updateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
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
void Pawn::setTime(DOUBLE dTime)
{
	if (animCtrlPtr == nullptr) 
		return;

	for (DWORD i = 0; i < animCtrlPtr->GetMaxNumTracks(); ++i) 
	{
		animCtrlPtr->SetTrackPosition(i, 0);
	}

	animCtrlPtr->ResetTime();
	animCtrlPtr->AdvanceTime(dTime, nullptr);
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT Pawn::AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	if (pMeshContainer->pSkinInfo == nullptr)
		return S_OK;	// �X�L����񂪖�����Ή������Ȃ�

	DWORD dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new LPD3DXMATRIX[dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		MYFRAME* pFrame = (MYFRAME*)D3DXFrameFind(frameRoot,pMeshContainer->pSkinInfo->GetBoneName(i));

		if (pFrame == nullptr)
			return E_FAIL;
		pMeshContainer->ppBoneMatrix[i] = &pFrame->combinedTransformationMatrix;
	}
	return S_OK;
}

//����������������������������������������������������������������������������������
// �{�[���p���[���h�E�}�g���b�N�X�̈�m��
//����������������������������������������������������������������������������������
HRESULT Pawn::AllocAllBoneMatrix(LPD3DXFRAME pFrameBase)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	HRESULT hr = S_OK;
	if (pFrame->pMeshContainer) 
	{
		hr = AllocBoneMatrix(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameSibling) 
	{
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}
	if (pFrame->pFrameFirstChild) 
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild);

	return hr;
}

//������������������������������������������
// �e�N�X�`������
//������������������������������������������
HRESULT Pawn::createTexture()
{
	// ��O����
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	if (D3DXCreateTextureFromFile(devicePtr, texFileName, &pD3DTexture))
		return S_OK;

	return S_OK;
}

//������������������������������������������
// ���f���̉��
//������������������������������������������
void Pawn::destroyResorceModel()
{
	ResourceManager::destroyAllMesh();
}

//������������������������������������������
// �e�N�X�`�����
//������������������������������������������
void Pawn::destroyTexture()
{
	SAFE_DELETE(pD3DTexture);
}

//������������������������������������������
// �e�N�X�`�����
//������������������������������������������
void Pawn::destroyResorceTexture()
{
	ResourceManager::destroyAllTexture();
}

//������������������������������������������
// �K�w�\���p���f�����
//������������������������������������������
void Pawn::destroyModelHierarchy()
{
	ResourceManager::destroyAllHierarchymesh();
}

//������������������������������������������
// ���f���̉��
//������������������������������������������
void Pawn::destroyModel()
{
	SAFE_DELETE(pVtx);
	SAFE_DELETE(pIndx);
	SAFE_DELETE(pAttr);

	SAFE_RELEASE(pD3DTexture);
	SAFE_RELEASE(pD3DXBuffMat);
	SAFE_RELEASE(pD3DXMesh);
}

//������������������������������������������
// �ʒu���W�Z�b�g
//������������������������������������������
void Pawn::setPosition(D3DXVECTOR3 SetPos)
{
	pos = SetPos;
}

//������������������������������������������
// �s��ɒ��ڈʒu���W���Z�b�g
//������������������������������������������
void Pawn::setOffset(D3DXVECTOR3 SetOffSet)
{
	worldMtx._41 = SetOffSet.x;
	worldMtx._42 = SetOffSet.y;
	worldMtx._43 = SetOffSet.z;

	colliderPtr->UpdateCollider(worldMtx,Figure::DeaultBoxcolor);
}

//������������������������������������������
// ��]�Z�b�g
//������������������������������������������
void Pawn::setRotation(D3DXVECTOR3 SetRot)
{
	rot = SetRot;
}

//������������������������������������������
// �}�e���A���̐F�Z�b�g
//������������������������������������������
void Pawn::setMaterialcolorEmissive(float fRed,float Green,float Blue,float Alpha)
{
	D3DXMATERIAL *pD3DXMat;

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)uNumMat; nCntMat++, pD3DXMat++)
	{
		pD3DXMat->MatD3D.Emissive.r = fRed;
		pD3DXMat->MatD3D.Emissive.g = Green;
		pD3DXMat->MatD3D.Emissive.b = Blue;
		pD3DXMat->MatD3D.Emissive.a = Alpha;
	}
}

//��������������������������������������������
// ���[���h�s��擾
//��������������������������������������������
D3DXMATRIX* Pawn::getWorldMtx()
{
	return &worldMtx;
}

//��������������������������������������������
// �E�����x�N�g���擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getRightVec()
{
	return D3DXVECTOR3(worldMtx._11, worldMtx._12, worldMtx._13);
}

//��������������������������������������������
// ������x�N�g���擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getUpVec()
{
	return D3DXVECTOR3(worldMtx._21, worldMtx._22, worldMtx._23);
}

//��������������������������������������������
// �O�����x�N�g���擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getForwardVec()
{
	return D3DXVECTOR3(worldMtx._31, worldMtx._32, worldMtx._33);
}

//��������������������������������������������
// �I�t�Z�b�g�x�N�g���擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getOffset()
{
	return D3DXVECTOR3(worldMtx._41, worldMtx._42, worldMtx._43);
}

//��������������������������������������������
// �ʒu�擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getPosition()
{
	return pos;
}

//��������������������������������������������
// ��]�擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getRotation()
{
	return rot;
}

//��������������������������������������������
// �����蔻��p���a�擾
//��������������������������������������������
FLOAT Pawn::getCollisionRadius()
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		return meshData.collisionRadus;
	case MeshObjType::HierarchyModel:
		return hierarchyMeshData.collisionRadus;
	default:
		break;
	}

	return meshData.collisionRadus;
}

//��������������������������������������������
// �����蔻��p���S���W�擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getCenterPos()
{
	return hierarchyMeshData.centerPos;
}

//��������������������������������������������
// �����蔻��p�ʒu�擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getColliderPos()
{
	return colliderPtr->figurePtr->GetPos();
}

//������������������������������������������
// ���b�V�����Z�b�g
//������������������������������������������
void Pawn::setMesh(LPD3DXMESH Mesh, LPD3DXBUFFER mat, DWORD NumMat)
{
	pD3DXMesh	 = Mesh;
	pD3DXBuffMat = mat;
	uNumMat		 = NumMat;
}

//������������������������������������������
// ���b�V���擾
//������������������������������������������
LPD3DXMESH Pawn::getMesh()
{
	return meshData.pD3DXMesh;
}

//������������������������������������������
// �}�e���A���擾
//������������������������������������������
LPD3DXBUFFER Pawn::getMat()
{
	return pD3DXBuffMat;
}

//������������������������������������������
// �}�e���A�����擾
//������������������������������������������
DWORD Pawn::getMatNum()
{
	return uNumMat;
}

//������������������������������������������
// �g�p���t���O�Z�b�g
//������������������������������������������
void Pawn::setUsedFlg(bool setFlg)
{
	isUsed = setFlg;

	// �R���C�_�[�g�p�t���O�Z�b�g
	if(colliderPtr)
		colliderPtr->setUsedFlg(setFlg);
}

//��������������������������������������������
// �ړ��ʃZ�b�g
//��������������������������������������������
void Pawn::setMoveSpeed(D3DXVECTOR3 Move)
{
	move = Move;
}

//��������������������������������������������
// �ړI���n�ʒu�Z�b�g
//��������������������������������������������
void Pawn::setDestLanding(D3DXVECTOR3 SetLanding)
{
	destLanding = SetLanding;
}

//����������������������������������������������������
// ���������I�u�W�F�N�g�̃C���f�b�N�X���Z�b�g
//����������������������������������������������������
void Pawn::setHitIndex(INT nSetIndex)
{
	hitIndex = nSetIndex;
}

//��������������������������������������������
// �ԍ��Z�b�g
//��������������������������������������������
void Pawn::setNumber(UINT uSetNumber)
{
	idNumber = uSetNumber;
}

//��������������������������������������������
// �ړ��X�s�[�h�擾
//��������������������������������������������
D3DXVECTOR3 Pawn::getMove()
{
	return move;
}

//����������������������������������������������
// ���_���擾
//����������������������������������������������
LPDIRECT3DVERTEXBUFFER9 Pawn::getVtxBuff()
{
	return pD3DVtxBuffPawn;
}

//��������������������������������������������
// �e�N�X�`������
//��������������������������������������������
LPDIRECT3DTEXTURE9 Pawn::getTexture()
{
	return pD3DTexture;
}

//��������������������������������������������
// �g�p�t���O�擾
//��������������������������������������������
BOOL Pawn::getUsedFlg()
{
	return isUsed;
}

//������������������������������������������
// �����蔻��{�b�N�X�擾
//������������������������������������������
D3DXVECTOR3 Pawn::getCollisionBox()
{
	switch (meshType)
	{
	case MeshObjType::NormalModel:
		return meshData.collitionBox;
	case MeshObjType::HierarchyModel:
		return hierarchyMeshData.collitionBox;
	default:
		break;
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//������������������������������������������
// �^�O�擾
//������������������������������������������
Pawn::TagType Pawn::getTag()
{
	return tagName;
}

//������������������������������������������
// �����蔻��t���O�Z�b�g
//������������������������������������������
void Pawn::setHitFlg(bool bSet)
{
	isHit = bSet;
}
//����������������������������������������������
// �C���f�b�N�X�o�b�t�@�A�N�Z�X�p�|�C���^�擾
//����������������������������������������������
WORD* Pawn::getIndxAcess()
{
//	return pIndx;
	return meshData.pIndx;
}


//������������������������������������������
// ���_�A�N�Z�X�p�|�C���^�擾
//������������������������������������������
MESH_VTX* Pawn::getVtxAcess()
{
	//return pVtx;
	return meshData.pVtx;
}

//������������������������������������������
// ���_�̐��擾
//������������������������������������������
DWORD Pawn::getVertexNum()
{
	return meshData.dwNumVtx;

//	return dwNumVtx;
}

//������������������������������������������
// �C���f�b�N�X�o�b�t�@�̐��擾
//������������������������������������������
DWORD Pawn::getIndxNum()
{
//	return dwNumIndx;
	return meshData.dwNumIndx;
}

//������������������������������������������
// �O�p�`�̐��擾
//������������������������������������������
DWORD Pawn::getTriangleNum()
{
//	return dwNumTriangles;
	return meshData.dwNumTriangles;
}

//������������������������������������������
// Ray�ƎO�p�`�̂����蔻��p�̒l�Z�b�g
//������������������������������������������
void Pawn::setRayToTriangleInfo(D3DXVECTOR3 SetCross, D3DXVECTOR3 SetNormal)
{
	cross  = SetCross;
	normal = SetNormal;
}

//������������������������������������������
//  �K�w�\���p ���E��/���E�{�b�N�X�擾
//������������������������������������������
void Pawn::calcCollision(LPD3DXFRAME pFrame)
{
	D3DXVECTOR3& vMax = collitionBox;
	D3DXVECTOR3& vMin = centerPos;
	vMax.x = vMax.y = vMax.z = -FLT_MAX;
	vMin.x = vMin.y = vMin.z = FLT_MAX;
	collisionRadus = -1.0f;
	calcCollisionFrame(pFrame);
	D3DXVECTOR3 vBBox, vCenter;
	vBBox	= (vMax - vMin) / 2.0f;
	vCenter = (vMax + vMin) / 2.0f;
	collitionBox	= vBBox;
	centerPos		= vCenter;
	collisionRadus = 0.0f;
	calcCollisionFrame(pFrame);
}

//��������������������������������������
// �t���[�����̒��_���W�̒��o
//��������������������������������������
void Pawn::calcCollisionFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer = pFrame->pMeshContainer;
	while (pMeshContainer) 
	{
		// ���b�V���R���e�i���̒��_���W�̒��o
		calcCollisionMeshContainer(pMeshContainer, pFrame);
		// ���̃��b�V���R���e�i
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	// �Z��t���[��������ΌZ��t���[����`��
	if (pFrame->pFrameSibling)
		calcCollisionFrame(pFrame->pFrameSibling);
	// �q�t���[��������Ύq�t���[����`��
	if (pFrame->pFrameFirstChild)
		calcCollisionFrame(pFrame->pFrameFirstChild);
}

//����������������������������������������������
// ���b�V���R���e�i���̒��_���W�̒��o
//����������������������������������������������
void Pawn::calcCollisionMeshContainer(LPD3DXMESHCONTAINER pMeshContainer, LPD3DXFRAME pFrame)
{
	D3DXMATRIX& matrix = ((MYFRAME*)pFrame)->combinedTransformationMatrix;
	// ���_���W�̒��o
	LPD3DXMESH pMesh = pMeshContainer->MeshData.pMesh;
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVtx = pMesh->GetNumVertices();
	LPBYTE pVtx;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);
	for (DWORD i = 0; i < dwVtx; ++i, pVtx += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)pVtx, &matrix);
		if (collisionRadus < 0.0f) 
		{
			D3DXVECTOR3& vMax = collitionBox;
			D3DXVECTOR3& vMin = centerPos;
			if (vMax.x < vtx.x) vMax.x = vtx.x;
			if (vMax.y < vtx.y) vMax.y = vtx.y;
			if (vMax.z < vtx.z) vMax.z = vtx.z;
			if (vMin.x > vtx.x) vMin.x = vtx.x;
			if (vMin.y > vtx.y) vMin.y = vtx.y;
			if (vMin.z > vtx.z) vMin.z = vtx.z;
		}
		else
		{
			float fRadius = D3DXVec3Length(&(vtx - centerPos));
			if (collisionRadus < fRadius)
				collisionRadus = fRadius;
		}
	}
	pMesh->UnlockVertexBuffer();
}

//������������������������������������������
// �����o���p�f�[�^�X�V
//������������������������������������������
void Pawn::updateExportData()
{
	exportData.PosData = pos;		// �ʒu
	exportData.RotData = rot;		// ��]
	exportData.ScaleData = scale;	// �g�嗦
	exportData.ObjType = objType;		// �I�u�W�F�N�g�̎��
	exportData.uNumber = idNumber;		// ���ʔԍ�
	exportData.isUsed = isUsed;		// �g�p�t���O
}

//��������������������������������������������
// �����o���p�f�[�^�Z�b�g
//��������������������������������������������
void Pawn::setExportData(EXPORT_GAMEOBJ_DATA SetData)
{
	pos = SetData.PosData;		// �ʒu
	rot = SetData.RotData;		// ��]
	scale = SetData.ScaleData;		// �g�嗦
	objType = SetData.ObjType;		// �u���b�N�̎��
	idNumber = SetData.uNumber;		// ���ʔԍ�
	isUsed = SetData.isUsed;			// �g�p�t���O
}

//��������������������������������������������
// �����o���p�f�[�^�擾
//��������������������������������������������
EXPORT_GAMEOBJ_DATA* Pawn::getExportData()
{
	return &exportData;
}

//��������������������������������������������
// �����l���Z�b�g
//��������������������������������������������
void Pawn::setDefaultValue()
{
	defPos			= pos;
	defMove			= move;
	defrot			= rot;
	defScale		= scale;
}

//��������������������������������������������
// �����l�ɃZ�b�g���Ȃ���
//��������������������������������������������
void Pawn::initializeStatus()
{
	pos		= defPos;
	move	= defMove;
	rot		= defrot;
	scale	= defScale;

	accele = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �g��
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mScale);

	// ��]
	D3DXMatrixRotationY(&worldMtx, D3DXToRadian(rot.y));

	// �ړ�
	D3DXMATRIX translateMtx;
	D3DXMatrixTranslation(&translateMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &translateMtx);



}

//��������������������������������������������
// �n�ʏ�t���O�擾
//��������������������������������������������
bool Pawn::getIsGround()
{
	return isGround;
}

//��������������������������������������������
// �n�ʏ�t���O�Z�b�g
//��������������������������������������������
void Pawn::setIsGround(bool bSet)
{
	isGround = bSet;
}

//��������������������������������������������
// �A�j���[�V�����؂�ւ�
//��������������������������������������������
void Pawn::setAnimChange(UINT uSetNum,UINT uWorkNum)
{
	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	hierarchyMeshData.animCtrlPtr->GetTrackDesc(currentAnim, &TD);

	// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
	// �g���b�N�̐ݒ�l���ڍs
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(uWorkNum, hierarchyMeshData.ppAnimSet[currentAnim]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(uWorkNum, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[uSetNum]);

	currentAnim = uSetNum;
}