//����������������������������������������������������������������
// SpikeBlock.cpp
// �u���b�N�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SpikeBlock.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
SpikeBlock::SpikeBlock()
{

}

//������������������������������������������������
// �R���X�g���N�^
// modelPass:	 ���f���p�X����
// texturePass : �e�N�X�`���p�X����
//������������������������������������������������
SpikeBlock::SpikeBlock(std::string modelPass, std::string texturePass, size_t setNumber, GameObjectType setGameObj, bool setFieldModel)
{
	tagName = modelPass;
	idNumber = setNumber;
	modelPasses[tagName] = passName + modelPass;
	texturePasses[tagName] = passName + texturePass;
	myGameObjType = setGameObj;
	isFieldObject = setFieldModel;

	isUsed = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
SpikeBlock::~SpikeBlock()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void SpikeBlock::initialize()
{
	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.collisionBox = meshDataObj.collitionBox;

	Collision::registerList(&myTransform, tagName);

	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos = D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);

	setDefaultValue();
}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void SpikeBlock::finalize()
{
	ResourceManager::destroyAllMesh();

	modelPasses.clear();
	texturePasses.clear();
	ResourceManager::destroyAllTexture();

	Collision::release(tagName);
}

//����������������������������������������������
// �u���b�N�X�V
//����������������������������������������������
void SpikeBlock::update()
{
	updateExportData();

	if (!isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (MyVector3::getLength(Collision::getTransform("Player", 0)->pos - myTransform.pos) < 15.0f)
	{
		if (myGameObjType == GameObjectType::SpikeObj)
		{
			if (myTransform.pos.y < -7.0f)
			{
				myTransform.pos.y += 0.2f;
			}
		}
	}

	if (Collision::getTransform(tagName, idNumber)->isHitAABB)
	{
		GameManager::changeGameType(GameManager::GameType::Miss);
	}

}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void SpikeBlock::draw()
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
void SpikeBlock::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// ��O����
	if (!isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw(mtxView, mtxProj);
}

//������������������������������������������
// �X�e�[�^�X������
//������������������������������������������
void SpikeBlock::initializeStatus()
{
	Pawn::initializeStatus();
}