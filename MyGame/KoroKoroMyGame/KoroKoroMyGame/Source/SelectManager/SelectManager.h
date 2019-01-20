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