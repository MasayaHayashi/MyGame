//����������������������������������������������������������������
// Goal.cpp
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Goal/Goal.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Goal::Goal()
{

}

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Goal::Goal(std::string keyName, size_t setNumber)
{
	tagName				 = keyName;
	idNumber			 = setNumber;
	modelPasses[keyName] = passName + keyName;

	Collision::registerList(&myTransform, keyName);
	
	isUsed = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Goal::~Goal()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void Goal::initialize()
{
	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, TexturePass.c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		= D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void Goal::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//����������������������������������������������
// �u���b�N�X�V
//����������������������������������������������
void Goal::update()
{
	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(mtxWorldPawn, FIELD_BOX_COLOR );

	// �����o���p�f�[�^�X�V
	updateExportData();
}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void Goal::draw()
{
	if (!isUsed)
	{
		return;
	}

	Pawn::draw();

}

//������������������������������������������
// �`��
//������������������������������������������
void Goal::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	if (!isUsed)
	{
		return;
	}

	Pawn::draw(mtxView,mtxProj);
}

//����������������������������������
// �L�[�l�擾
//����������������������������������
const std::string Goal::getKeyName()
{
	return tagName;
}