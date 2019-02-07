//����������������������������������������������������������������
// ItemBase.cpp
// �A�C�e�����N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "ItemBase.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//������������������������������������������������
// �R���X�g���N�^
//������������������������������������������������
ItemBase::ItemBase()
{

}

//������������������������������������������������
// �R���X�g���N�^
// modelPass:	 ���f���p�X����
// texturePass : �e�N�X�`���p�X����
//������������������������������������������������
ItemBase::ItemBase(std::string modelPass,std::string texturePass, size_t setNumber,GameObjectType setGameObj,bool setFieldModel)
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
ItemBase::~ItemBase()
{

}

//����������������������������������������������
// �u���b�N������
//����������������������������������������������
void ItemBase::initialize()
{
	// �t�@�C���p�X�ݒ�
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, texturePasses[tagName].c_str());

	// ���f������
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.collisionBox = meshDataObj.collitionBox;

	Collision::registerList(&myTransform, tagName);

	// �ʒu�A�ړ��ʁA�g�嗦������
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		= D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
}

//��������������������������������������������
// �u���b�N�㏈��
//��������������������������������������������
void ItemBase::finalize()
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
void ItemBase::update()
{
	// �R���C�_�[�X�V
//	pCollider->UpdateCollider(mtxWorldPawn, FIELD_BOX_COLOR );
	updateExportData();

	if (!myTransform.isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (myGameObjType == GameObjectType::NormalBlockObj)
	{
		myTransform.rotDeg.y += 0.1f;

		D3DXMATRIX matrix;
		D3DXMatrixRotationY(&matrix, D3DXToRadian(myTransform.rotDeg.y));
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &matrix);
	}

	if (Collision::getTransform(tagName, idNumber)->isHitAABB)
	{
 		GameManager::changeGameType(GameManager::GameType::Miss);
	}

}

//����������������������������������
// �u���b�N�`��
//����������������������������������
void ItemBase::draw()
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
void ItemBase::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// ��O����
	if (!myTransform.isUsed)
	{
		return;
	}

	// �`��
	Pawn::draw(mtxView,mtxProj);
}