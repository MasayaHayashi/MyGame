//����������������������������������������������������
// SelectManager.h
// Author : Masaya Hayashi
//����������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef SELECT_MANAGER_H
#define SELECT_MANAGER_H

// ===== �C���N���[�h�� =====
#include <array>
#include <unordered_map>
#include "../ResoueceManager/ResourceManager.h"
#include "../DirectX3D/DirectX3D.h"

// ===== �N���X�̑O���錾 =====

// ===== �N���X��` =====
class SelectManager final
{
public:

	SelectManager();
	~SelectManager();

	static constexpr UINT MaxPlayer = 4;

	UINT getModelIgndex(UINT playerIndex);


protected:

private:

	std::array<UINT, MaxPlayer>		currentSelectModels;
	std::unordered_map<UINT, std::string>	changeUintToPass =
	{
		{ 0, ResourceManager::ModelPenchanPass }, 
		{ 1, ResourceManager::ModelPenNoHahaPass },
		{ 2, ResourceManager::ModelChick },
	};
};

#endif