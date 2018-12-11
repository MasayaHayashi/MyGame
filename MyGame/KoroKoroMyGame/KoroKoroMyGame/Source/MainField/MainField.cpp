//����������������������������������������������������������������
// C_MainField.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MainField.h"
#include <string>
#include "../Collision/Collision.h"

//��������������������������������������������������
// �R���X�g���N�^
//��������������������������������������������������
MainField::MainField()
{
	strcpy_s(fileName, ModelFilePass.c_str());
	strcpy_s(texFileName, ModelFilePass.c_str());
	myTransform.pos		= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	myTransform.scale	= D3DXVECTOR3(0.7f, 0.1f, 0.5f);

	meshPtr = nullptr;
	isUsed  = true;

	tagName = "field";

	Collision::registerList(&myTransform,"field");

	idNumber = 0;
}

//������������������������������������������������
// �f�X�g���N�^
//������������������������������������������������
MainField::~MainField()
{

}

//��������������������������������������
// �t�B�[���h������
//��������������������������������������
void MainField::initialize()
{
	// X�t�@�C���̓ǂݍ���
	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);
}

//��������������������������������������
// �t�B�[���h�㏈��
//��������������������������������������
void MainField::finalize()
{
	// ���b�V�����
//	ResourceManager::destroyAllMesh();

	// �e�N�X�`�����
//	ResourceManager::destroyAllTexture();
}

//��������������������������������������
// �t�B�[���h�X�V
//��������������������������������������
void MainField::update()
{

}

//��������������������������������������
// �t�B�[���h�`��
//��������������������������������������
void MainField::draw()
{
	Pawn::draw();
}

//��������������������������������������
// ���S���W�擾
//��������������������������������������
D3DXVECTOR3 MainField::getCenterPos() const
{
	return centerPos;
}

//������������������������������������������
// �~��̃t�B�[���h�����蔻��擾
//������������������������������������������
bool MainField::getCollisionCiecle(D3DXVECTOR3 CurrentPos,float Radius) const
{
	if ((CurrentPos.x - centerPos.x) * (CurrentPos.x - centerPos.x) + (CurrentPos.z - centerPos.z) * (CurrentPos.z - centerPos.z) <= ((collisionRadus - Radius) * (collisionRadus - Radius)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

//������������������������������������������
// �}�e���A���̐F�Z�b�g
//������������������������������������������
void MainField::setMaterialcolorEmissive(float fRed, float Green, float Blue, float Alpha)
{
	D3DXMATERIAL *pD3DXMat;

	pD3DXMat = (D3DXMATERIAL*)materialBufferPtr->GetBufferPointer();

	pD3DXMat->MatD3D.Emissive.r = fRed;
	pD3DXMat->MatD3D.Emissive.g = Green;
	pD3DXMat->MatD3D.Emissive.b = Blue;
	pD3DXMat->MatD3D.Emissive.a = Alpha;
}

//������������������������������������������
// �����蔻��p�T�C�Y�擾
//������������������������������������������
FLOAT MainField::getCollisionRadius() const
{
	return collisionRadus;
}