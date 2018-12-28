//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SelectManager.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef SELECT_MANAGER_H
#define SELECT_MANAGER_H

// ===== インクルード部 =====
#include <array>
#include <unordered_map>
#include "../ResoueceManager/ResourceManager.h"
#include "../DirectX3D/DirectX3D.h"

// ===== クラスの前方宣言 =====

// ===== クラス定義 =====
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