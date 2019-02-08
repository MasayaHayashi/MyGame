//����������������������������������������������������������������
// StarItem.cpp
// �X�^�[�A�C�e��
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "StarItem.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
StarItem::StarItem()
{

}

//������������������������������������������������
// �R���X�g���N�^
// modelPass:	 ���f���p�X����
// texturePass : �e�N�X�`���p�X����
//������������������������������������������������
StarItem::StarItem(std::string modelPass,std::string texturePass, size_t setNumber,GameObjectType setGameObj,bool setFieldModel)
{
	tagName						= modelPass;
	idNumber					= setNumber;
	modelPasses[tagName]		= passName + modelPass;
	texturePasses[tagName]		= passName + texturePass;
	myGameObjType				= setGameObj;
	isFieldObject				= setFieldModel;
	
	myTransform.isUsed = false;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
StarItem::~StarItem()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void StarItem::initialize()
{
	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, texturePasses[tagName].c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	Collision::registerList(&myTransform, tagName);

	myTransform.scale	 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		 = D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
	myTransform.rotDeg.y = 90.0f;
	myTransform.collisionBox = meshDataObj.collitionBox;
}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void StarItem::finalize()
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
void StarItem::update()
{
	updateExportData();

	if (!myTransform.isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (idNumber == Collision::getHitIndex("star.x"))
	{
		myTransform.isUsed = false;
		GameManager::changeGameType(GameManager::GameType::GetItem);
	}

	myTransform.rotDeg.y += 0.1f;
	D3DXMATRIX matrix;
	D3DXMatrixRotationY(&matrix, D3DXToRadian(myTransform.rotDeg.y));
	D3DXMatrixMultiply(&worldMtx, &worldMtx, &matrix);
}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void StarItem::draw()
{
	if (!myTransform.isUsed)
	{
		return;
	}

	Pawn::draw();
}

//������������������������������������������
// �`��
//������������������������������������������
void StarItem::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	if (!myTransform.isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw(mtxView,mtxProj);
}

//����������������������������������
// �L�[�l�擾
//����������������������������������
const std::string StarItem::getKeyName()
{
	return tagName;
}