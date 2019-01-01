//����������������������������������������������������
// SelectManager.cpp
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SelectManager.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../DirectX3D/DirectX3D.h"

// ===== �ÓI�����o =====
std::vector<std::string>				SelectManager::changeUintToPass;
INT										SelectManager::currentSelectModel;
SelectManager							SelectManager::instance;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SelectManager::SelectManager()
{
	changeUintToPass.push_back(ResourceManager::ModelPenchanPass);
	changeUintToPass.push_back(ResourceManager::ModelPenNoHahaPass);
	changeUintToPass.push_back(ResourceManager::ModelChick);
	changeUintToPass.push_back(ResourceManager::ModelPenchanPass);
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SelectManager::~SelectManager()
{
	
}

//����������������������������������������������
// �C���f�b�N�X�擾
//����������������������������������������������
std::string SelectManager::getModelPass(INT playerIndex)
{
	return changeUintToPass[playerIndex];
}

//��������������������������������������������
// ���ݑI�𒆂̃��f��
//��������������������������������������������
INT SelectManager::getSelect()
{
	return currentSelectModel;
}

//������������������������������������������������
// �Z���N�g�p���Z
//������������������������������������������������
void SelectManager::addSelect()
{
	currentSelectModel ++;

	if (currentSelectModel > MaxPlayer - 1)
	{
		currentSelectModel = MaxPlayer - 1;
	}
}

//������������������������������������������������
// �Z���N�g�p���Z
//������������������������������������������������
void SelectManager::subSelect()
{
	currentSelectModel --;

	if (currentSelectModel < 0)
	{
		currentSelectModel = 0;
	}
}