//����������������������������������������������������������������
// Block.cpp
// �u���b�N�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Block/Block.h"
#include "../ResoueceManager/ResourceManager.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Block::Block()
{

}

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
Block::Block(UINT uSetNumber)
{
	// �u���b�N�̎�ޏ�����
	tagName = "NormalBlock";

	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, ModelPass.c_str());
	strcpy_s(texFileName, TexturePass.c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName,meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.pos = D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2 , 0.0f);

	// �I��ԍ�������
	idNumber = uSetNumber;	// ���ʔԍ�
	uCurrentSelect = 0;		// ���݂̑I��ԍ�

	// �g�p�t���O�X�V
	isUsed = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
Block::~Block()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void Block::initialize()
{

}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void Block::finalize()
{
	ResourceManager::destroyAllMesh();
	ResourceManager::destroyAllTexture();
}

//����������������������������������������������
// �u���b�N�X�V
//����������������������������������������������
void Block::update()
{
	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(mtxWorldPawn, FIELD_BOX_COLOR );

	// �����o���p�f�[�^�X�V
//	updateExportData();
}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void Block::draw()
{
	// ��O����
	if (!isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw();

}

//������������������������������������������
// �`��
//������������������������������������������
void Block::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// ��O����
	if (!isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw(mtxView,mtxProj);

}

//����������������������������������
// �I��ԍ��Z�b�g
//����������������������������������
void Block::SetSelectNum(UINT uSetNumber)
{
	uCurrentSelect = uSetNumber;
}