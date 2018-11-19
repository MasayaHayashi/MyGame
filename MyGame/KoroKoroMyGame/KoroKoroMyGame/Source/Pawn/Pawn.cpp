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
	meshDataObj.vertexPtr			= nullptr;
	meshDataObj.indexPtr			= nullptr;
	meshDataObj.attrPtr				= nullptr;
	textureData.pD3DTexture			= nullptr;
	meshDataObj.meshPtr				= nullptr;
	meshDataObj.materialBufferPtr	= nullptr;

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
//	meshPtr = nullptr;

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
// ������
//����������������������������������
void Pawn::initialize(std::string fileName)
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
	pD3DXMat = (D3DXMATERIAL*)meshDataObj.materialBufferPtr->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshDataObj.numMat); nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshDataObj.meshPtr->DrawSubset(nCntMat);
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
 	pD3DXMat = (D3DXMATERIAL*)meshDataObj.materialBufferPtr->GetBufferPointer();


	LPD3DXEFFECT pEffect = DirectX3D::getEffect();

	// 
	pEffect->SetTechnique("tecMinimum");

	D3DXMATRIX mtxAll = worldMtx * mtxView * mtxProj;
	pEffect->SetMatrix("mWVP", &mtxAll);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshDataObj.numMat); nCntMat++)
	{

		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshDataObj.meshPtr->DrawSubset(nCntMat);
	}

	pEffect->EndPass();
	pEffect->End();

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);
	
}

//����������������������������������
// �`��
//����������������������������������
void Pawn::draw(LPD3DXMESH pMesh, LPDIRECT3DTEXTURE9 pTex, LPD3DXBUFFER pBuff,DWORD numMat)
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
	pD3DXMat = (D3DXMATERIAL*)meshDataObj.materialBufferPtr->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshDataObj.numMat); nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshDataObj.meshPtr->DrawSubset(nCntMat);
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
	pD3DXMat = (D3DXMATERIAL*)meshDataObj.materialBufferPtr->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(meshDataObj.numMat); nCntMat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, textureData.pD3DTexture);

		// �`��
		meshDataObj.meshPtr->DrawSubset(nCntMat);
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
	pMeshContainer->ppBoneMatrix = NEW LPD3DXMATRIX[dwBoneNum];

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
		{
			return hr;
		}
	}
	if (pFrame->pFrameSibling) 
	{
		hr = AllocAllBoneMatrix(pFrame->pFrameSibling);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pFrame->pFrameFirstChild) 
		hr = AllocAllBoneMatrix(pFrame->pFrameFirstChild);

	return hr;
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
	SAFE_RELEASE(pD3DTexture);
	SAFE_RELEASE(materialBufferPtr);
	SAFE_RELEASE(meshPtr);
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

	pD3DXMat = (D3DXMATERIAL*)materialBufferPtr->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < static_cast<INT>(numMat); nCntMat++, pD3DXMat++)
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
		return meshDataObj.collisionRadus;
	case MeshObjType::HierarchyModel:
		return hierarchyMeshData.collisionRadus;
	default:
		break;
	}

	return meshDataObj.collisionRadus;
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
	meshPtr	 = Mesh;
	materialBufferPtr = mat;
	numMat		 = NumMat;
}

//������������������������������������������
// ���b�V���擾
//������������������������������������������
LPD3DXMESH Pawn::getMesh()
{
	return meshDataObj.meshPtr.Get();
}

//������������������������������������������
// �}�e���A���擾
//������������������������������������������
LPD3DXBUFFER Pawn::getMat()
{
	return materialBufferPtr;
}

//������������������������������������������
// �}�e���A�����擾
//������������������������������������������
DWORD Pawn::getMatNum()
{
	return numMat;
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
		return meshDataObj.collitionBox;
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
	return meshDataObj.indexPtr.get();
}


//������������������������������������������
// ���_�A�N�Z�X�p�|�C���^�擾
//������������������������������������������
MESH_VTX* Pawn::getVtxAcess()
{
	//return vertexPtr;
	return meshDataObj.vertexPtr.get();
}

//������������������������������������������
// ���_�̐��擾
//������������������������������������������
DWORD Pawn::getVertexNum()
{
	return meshDataObj.dwNumVtx;

//	return dwNumVtx;
}

//������������������������������������������
// �C���f�b�N�X�o�b�t�@�̐��擾
//������������������������������������������
DWORD Pawn::getIndxNum()
{
//	return dwNumIndx;
	return meshDataObj.dwNumIndx;
}

//������������������������������������������
// �O�p�`�̐��擾
//������������������������������������������
DWORD Pawn::getTriangleNum()
{
//	return dwNumTriangles;
	return meshDataObj.dwNumTriangles;
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
	LPBYTE vertexPtr;
	D3DXVECTOR3 vtx;
	pMesh->LockVertexBuffer(0, (LPVOID*)&vertexPtr);
	for (DWORD i = 0; i < dwVtx; ++i, vertexPtr += dwStride)
	{
		D3DXVec3TransformCoord(&vtx, (LPD3DXVECTOR3)vertexPtr, &matrix);
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
	exportData.posData = pos;		// �ʒu
	exportData.rotData = rot;		// ��]
	exportData.scaleData = scale;	// �g�嗦
	exportData.objType = objType;		// �I�u�W�F�N�g�̎��
	exportData.idNumber = idNumber;		// ���ʔԍ�
	exportData.isUsed = isUsed;		// �g�p�t���O
}

//��������������������������������������������
// �����o���p�f�[�^�Z�b�g
//��������������������������������������������
void Pawn::setExportData(TransformData SetData)
{
	pos = SetData.posData;		// �ʒu
	rot = SetData.rotData;		// ��]
	scale = SetData.scaleData;	// �g�嗦
	objType = SetData.objType;	// �u���b�N�̎��
	idNumber = SetData.idNumber;	// ���ʔԍ�
	isUsed = SetData.isUsed;		// �g�p�t���O
}

//��������������������������������������������
// �����o���p�f�[�^�擾
//��������������������������������������������
TransformData* Pawn::getExportData()
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
//	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(uWorkNum, hierarchyMeshData.ppAnimSet[currentAnim]);
	hierarchyMeshData.animCtrlPtr->SetTrackDesc(uWorkNum, &TD);

	// �V�����A�j���[�V�����Z�b�g���g���b�N0�ɐݒ�
//	hierarchyMeshData.animCtrlPtr->SetTrackAnimationSet(0, hierarchyMeshData.ppAnimSet[uSetNum]);

	currentAnim = uSetNum;
}