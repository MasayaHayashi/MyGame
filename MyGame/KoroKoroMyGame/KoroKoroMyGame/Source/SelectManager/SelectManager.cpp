//����������������������������������������������������
// SelectManager.cpp
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== �C���N���[�h�� =====
#include "SelectManager.h"
#include "../DirectX3D/DirectX3D.h"

// ===== �ÓI�����o =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SelectManager::SelectManager()
{
	for (UINT playerindex = 0; playerindex < MaxPlayer; playerindex ++)
	{
		currentSelectModels[playerindex] = playerindex;
	}
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SelectManager::~SelectManager()
{
	
}

//����������������������������������������������
// �C���f�b�N�X�擾
//��������������������������������������������
UINT SelectManager::getModelIgndex(UINT playerIndex)
{
	if (playerIndex < currentSelectModels.size())
	{
		return -1;
	}

	return currentSelectModels[playerIndex];
}