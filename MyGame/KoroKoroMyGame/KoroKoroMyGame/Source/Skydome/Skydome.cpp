//����������������������������������������������������������������
// Skydome.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Skydome.h"
#include "../Camera/camera.h"
#include "../DirectX3D/DirectX3D.h"
#include <string>

// ===== �萔�E�}�N����` =====
#define FILENAME	"data/MODEL/Skydome151009x.x"
#define TEX_FILENAME "data/TEXTURE/ocean.jpg"
#define SCALE (7.0f)

//������������������������������
// �R���X�g���N�^
//������������������������������
Skydome::Skydome()
{
	strcpy_s(fileName, FILENAME);
	strcpy_s(texFileName, TEX_FILENAME);
	scale		 = D3DXVECTOR3(SCALE, SCALE, SCALE);
	pos			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	textureSize  = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
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
	makeModel();
	CreateTexture();
}

//������������������������������
// �X�J�C�h�[���㏈��
//������������������������������
void Skydome::finalize()
{
	// ���f�����
	destroyModel();

	destroyTexture();

}

//������������������������������
// �X�J�C�h�[���X�V
//������������������������������
void Skydome::updateObject()
{
	//C_CAMERA *pCamera = C_CAMERA_MANAGER::getUsedCamera(0);

	//pos = pCamera->getPosCameraP();
}

//������������������������������
// �X�J�C�h�[���`��
//������������������������������
void Skydome::drawObject()
{
	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���C�e�B���O���Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�X�V�𖳌�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&worldMtx);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)uNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pD3DTexture);

		// �`��
		pD3DXMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// ���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//��������������������������������������
// �e�N�X�`������
//��������������������������������������
HRESULT Skydome::CreateTexture()
{
	// ��O����
	if (!texFileName)
		return E_FAIL;

	LPDIRECT3DDEVICE9 pDevice = DirectX3D::getDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, texFileName, &pD3DTexture);

	return S_OK;
}