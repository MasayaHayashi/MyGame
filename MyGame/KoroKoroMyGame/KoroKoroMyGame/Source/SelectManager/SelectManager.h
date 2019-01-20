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

	static constexpr size_t MaxPlayer = 1;
	static constexpr size_t MaxModel  = 3;

	static std::string SelectManager::getModelPass(INT playerIndex);

	
	static INT getSelect();
	static void addSelect();
	static void subSelect();


protected:

private:
	static INT								currentSelectModel;
	static std::vector<std::string>			changeUintToPass;

	static SelectManager instance;
};

#endif