//����������������������������������������������������������������
// Skydome.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Skydome.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include <string>

//������������������������������
// �R���X�g���N�^
//������������������������������
Skydome::Skydome()
{
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());
	scale		 = D3DXVECTOR3(Scale, Scale, Scale);
	pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	textureSize  = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	isUsed = true;
}

//������������������������������
// �f�X�g���N�^
//������������������������������
Skydome::~Skydome()
{

}

//������������������������������
// �X�J�C�h�[��������
//������������������������������
void Skydome::initialize()
{
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//������������������������������
// �X�J�C�h�[���㏈��
//������������������������������
void Skydome::finalize()
{

}

//������������������������������
// �X�J�C�h�[���X�V
//������������������������������
void Skydome::update()
{
	//C_CAMERA *pCamera = C_CAMERA_MANAGER::getUsedCamera(0);

	//pos = pCamera->getPosCameraP();
}

//������������������������������
// �X�J�C�h�[���`��
//������������������������������
void Skydome::draw()
{
	Pawn::draw();

	/*
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���C�e�B���O���Ȃ�
	devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�X�V�𖳌�
	devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&worldMtx);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	devicePtr->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)materialBufferPtr->GetBufferPointer();

	for (int mat = 0; mat < static_cast<INT>(numMat); mat++)
	{
		// �}�e���A���̐ݒ�
		devicePtr->SetMaterial(&pD3DXMat[mat].MatD3D);

		// �e�N�X�`���̐ݒ�
		devicePtr->SetTexture(0, pD3DTexture);

		// �`��
		meshPtr->DrawSubset(mat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	devicePtr->SetMaterial(&matDef);

	// ���ɖ߂�
	devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
	devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	*/
}

//��������������������������������������
// �e�N�X�`������
//��������������������������������������
HRESULT Skydome::createTexture()
{
	// ��O����
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(devicePtr, texFileName, &pD3DTexture);

	return S_OK;
}