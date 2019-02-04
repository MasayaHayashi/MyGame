//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// StarItem.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef STAR_ITEM_OBJ
#define STAR_ITEM_OBJ

// ===== インクルード部 =====
#include "../Item/ItemBase.h"
#include <map>

// ===== 定数・マクロ定義 =====

// ===== クラスの前方定義 =====

// ===== 列挙体定義 =====

// ===== 構造体定義 =====

// ===== クラス定義 =====
class StarItem final : public ItemBase
{
public:
	StarItem();
	StarItem(std::string, std::string, size_t,GameObjectType, bool);
	~StarItem();

	void initialize();
	void finalize();
	void update();
	void draw();
	void draw(D3DXMATRIX,D3DXMATRIX);

	const std::string getKeyName();

protected:

private:
};

#endif